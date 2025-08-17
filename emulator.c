#include "emulator.h"

emulator_t emulator;

int init_app()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("SDL initialization failed: %s\n", SDL_GetError());
		SDL_Quit();
		return 0;
	}

	init_window(&emulator.window_vram, "VRAM", WIN_VRAM_SIZE_X, WIN_VRAM_SIZE_Y);
	init_window(&emulator.window_background, "Background", WIN_BACKGROUND_SIZE_X, WIN_BACKGROUND_SIZE_Y);

	emulator.paused = 0;
	emulator.quit = 0;
	bzero(emulator.current_presses, sizeof(emulator.current_presses));

	return 1;
}

int init_window(window_t* window, char* title, int size_x, int size_y)
{
	window->window = SDL_CreateWindow(
		title,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		size_x * WIN_SCALE,
		size_y * WIN_SCALE,
		SDL_WINDOW_SHOWN
	);
	if (!window->window)
	{
		printf("Window creation failed: %s\n", SDL_GetError());
		SDL_Quit();
		return 0;
	}
	window->renderer = SDL_CreateRenderer(window->window, -1, SDL_RENDERER_ACCELERATED);
	if (!window->renderer)
	{
		printf("SDL initialization failed: %s\n", SDL_GetError());
		SDL_DestroyWindow(window->window);
		SDL_Quit();
		return 0;
	}
	SDL_RenderSetLogicalSize(window->renderer, size_x, size_y);
	SDL_RenderSetIntegerScale(window->renderer, SDL_TRUE);
	window->screen_surface = SDL_CreateRGBSurfaceWithFormat(0, size_x, size_y, 32, SDL_PIXELFORMAT_RGB888);

	return 1;
}

void render_window(window_t* window)
{
	SDL_RenderClear(window->renderer);
	SDL_Texture* render_tex = SDL_CreateTextureFromSurface(window->renderer, window->screen_surface);
	SDL_RenderCopy(window->renderer, render_tex, NULL, NULL);
	SDL_DestroyTexture(render_tex);
	SDL_RenderPresent(window->renderer);
}

void close_app()
{
	SDL_DestroyRenderer(emulator.window_vram.renderer);
	SDL_DestroyWindow(emulator.window_vram.window);

	SDL_DestroyRenderer(emulator.window_background.renderer);
	SDL_DestroyWindow(emulator.window_background.window);

	SDL_Quit();
}

int run_emulator()
{
	while (!emulator.quit)
	{
		long start_time = get_current_time();

		handle_events();

		if (!emulator.paused)
		{
			for (size_t i = 0; i < FRAME_CYCLES; i++)
			{
				timers_tick();
				cpu_tick();
				ppu_tick();
			}
		}

		display_vram();
		display_background();

		long end_time = get_current_time();
		long sleep_time = start_time + FRAME_TIME - end_time;
		if (sleep_time > 0)
			usleep(sleep_time);
		// printf("%ld\n", end_time - start_time);
	}
	return 0;
}

long get_current_time()
{
	struct timespec ts;
	clock_gettime(1, &ts);
	return ts.tv_sec * 1000000l + ts.tv_nsec / 1000l;
}

int get_pixel_code(int tile_id, int x, int y)
{
	u8 bit_index = 7 - x;
	u8 val1 = (memory.video_ram[tile_id * 16 + y * 2] >> bit_index) & 1;
	u8 val2 = (memory.video_ram[tile_id * 16 + y * 2 + 1] >> bit_index) & 1;
	return (val2 << 1) | val1;
}

int get_color(int color_code)
{
	int color;
	switch (color_code) {
		case 0b00:
			color = COLOR_LIGHTER;
			break;
		case 0b01:
			color = COLOR_LIGHT;
			break;
		case 0b10:
			color = COLOR_DARK;
			break;
		case 0b11:
			color = COLOR_DARKER;
			break;
	}
	return color;
}

void display_vram()
{
	size_t tiles_per_row = 16;
	for (size_t y = 0; y < WIN_VRAM_SIZE_Y; y++)
	{
		for (size_t x = 0; x < WIN_VRAM_SIZE_X; x++)
		{
			size_t tile_x = x / 8;
			size_t tile_y = y / 8;
			size_t tile_id = tile_y * tiles_per_row + tile_x;
			u8 color_code = get_pixel_code(tile_id, x % 8, y % 8);
			int color = get_color(color_code);

			((int*)emulator.window_vram.screen_surface->pixels)[y * WIN_VRAM_SIZE_X + x] = color;
		}
	}

	render_window(&emulator.window_vram);
}

void display_background()
{
	u16 start_address = 0x1800;
	size_t tiles_per_row = 32;
	for (size_t y = 0; y < WIN_BACKGROUND_SIZE_Y; y++)
	{
		for (size_t x = 0; x < WIN_BACKGROUND_SIZE_X; x++)
		{
			u8 tile_id = memory.video_ram[start_address + (y / 8) * tiles_per_row + (x / 8)];
			u8 color_code = get_pixel_code(tile_id, x % 8, y % 8);
			int color = get_color(color_code);

			((int*)emulator.window_background.screen_surface->pixels)[y * WIN_BACKGROUND_SIZE_X + x] = color;
		}
	}
	
	render_window(&emulator.window_background);
}

void handle_events()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT || event.window.event == SDL_WINDOWEVENT_CLOSE)
			emulator.quit = 1;
		// on key down
		if (event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.sym == SDLK_UP)
				emulator.current_presses[KEY_UP] = 1;
			if (event.key.keysym.sym == SDLK_DOWN)
				emulator.current_presses[KEY_DOWN] = 1;
			if (event.key.keysym.sym == SDLK_LEFT)
				emulator.current_presses[KEY_LEFT] = 1;
			if (event.key.keysym.sym == SDLK_RIGHT)
				emulator.current_presses[KEY_RIGHT] = 1;
			if (event.key.keysym.sym == SDLK_z)
				emulator.current_presses[KEY_A] = 1;
			if (event.key.keysym.sym == SDLK_x)
				emulator.current_presses[KEY_B] = 1;
			if (event.key.keysym.sym == SDLK_RETURN)
				emulator.current_presses[KEY_START] = 1;
			if (event.key.keysym.sym == SDLK_BACKSPACE)
				emulator.current_presses[KEY_SELECT] = 1;
		}
		// on key up
		if (event.type == SDL_KEYUP)
		{
			if (event.key.keysym.sym == SDLK_UP)
				emulator.current_presses[KEY_UP] = 0;
			if (event.key.keysym.sym == SDLK_DOWN)
				emulator.current_presses[KEY_DOWN] = 0;
			if (event.key.keysym.sym == SDLK_LEFT)
				emulator.current_presses[KEY_LEFT] = 0;
			if (event.key.keysym.sym == SDLK_RIGHT)
				emulator.current_presses[KEY_RIGHT] = 0;
			if (event.key.keysym.sym == SDLK_z)
				emulator.current_presses[KEY_A] = 0;
			if (event.key.keysym.sym == SDLK_x)
				emulator.current_presses[KEY_B] = 0;
			if (event.key.keysym.sym == SDLK_RETURN)
				emulator.current_presses[KEY_START] = 0;
			if (event.key.keysym.sym == SDLK_BACKSPACE)
				emulator.current_presses[KEY_SELECT] = 0;
		}
	}
}


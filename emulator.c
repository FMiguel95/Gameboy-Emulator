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

	emulator.window = SDL_CreateWindow(
		"DMG",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WIN_SIZE_X * WIN_SCALE,
		WIN_SIZE_Y * WIN_SCALE,
		SDL_WINDOW_SHOWN
	);
	if (!emulator.window)
	{
		printf("Window creation failed: %s\n", SDL_GetError());
		SDL_Quit();
		return 0;
	}

	emulator.renderer = SDL_CreateRenderer(emulator.window, -1, SDL_RENDERER_ACCELERATED);
	if (!emulator.renderer)
	{
		printf("SDL initialization failed: %s\n", SDL_GetError());
		SDL_DestroyWindow(emulator.window);
		SDL_Quit();
		return 0;
	}
	SDL_RenderSetLogicalSize(emulator.renderer, WIN_SIZE_X, WIN_SIZE_Y);
	SDL_RenderSetIntegerScale(emulator.renderer, SDL_TRUE);

	emulator.screen_surface = SDL_CreateRGBSurfaceWithFormat(0, WIN_SIZE_X, WIN_SIZE_Y, sizeof(int), SDL_PIXELFORMAT_RGB888);
	emulator.paused = 0;
	emulator.quit = 0;
	bzero(emulator.current_presses, sizeof(emulator.current_presses));

	return 1;
}

void close_app()
{
	SDL_DestroyRenderer(emulator.renderer);
	SDL_DestroyWindow(emulator.window);
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

		long end_time = get_current_time();
		usleep(start_time + FRAME_TIME - end_time);
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

void display_vram()
{
	size_t tiles_per_row = 16;
	for (size_t y = 0; y < WIN_SIZE_Y; y++)
	{
		for (size_t x = 0; x < WIN_SIZE_X; x++)
		{
			size_t tile_x = x / 8;
			size_t tile_y = y / 8;
			size_t tile_index = tile_y * tiles_per_row + tile_x;

			size_t tile_addr = tile_index * 16;
			size_t row_in_tile = y % 8;
			size_t row_addr = tile_addr + (row_in_tile * 2);

			int bit = 7 - (x % 8);
			u8 val1 = (memory.video_ram[row_addr] >> bit) & 1;
			u8 val2 = (memory.video_ram[row_addr + 1] >> bit) & 1;
			u8 color_code = (val2 << 1) | val1;

			int color;
			switch (color_code) {
				case 0b00:
					color = COLOR_DARKER;
					break;
				case 0b01:
					color = COLOR_DARK;
					break;
				case 0b10:
					color = COLOR_LIGHT;
					break;
				case 0b11:
					color = COLOR_LIGHTER;
					break;
			}

			((int*)emulator.screen_surface->pixels)[y * WIN_SIZE_X + x] = color;
		}
	}

	SDL_RenderClear(emulator.renderer);
	SDL_Texture* render_tex = SDL_CreateTextureFromSurface(emulator.renderer, emulator.screen_surface);
	SDL_RenderCopy(emulator.renderer, render_tex, NULL, NULL);
	SDL_DestroyTexture(render_tex);
	SDL_RenderPresent(emulator.renderer);

}

void handle_events()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
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


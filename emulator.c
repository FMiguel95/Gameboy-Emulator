#include "emulator.h"

emulator_t emulator;

int init_app()
{
	// if (SDL_Init(SDL_INIT_VIDEO) != 0)
	// {
	// 	printf("SDL initialization failed: %s\n", SDL_GetError());
	// 	SDL_Quit();
	// 	return 0;
	// }

	// // init_window(&emulator.window_background9800, "Map $9800", WIN_BACKGROUND_SIZE_X, WIN_BACKGROUND_SIZE_Y);
	// // init_window(&emulator.window_background9C00, "Map $9C00", WIN_BACKGROUND_SIZE_X, WIN_BACKGROUND_SIZE_Y);
	// // init_window(&emulator.window_tiles, "Tiles", WIN_VRAM_SIZE_X, WIN_VRAM_SIZE_Y);
	// init_window(&emulator.window_screen, "Game Screen", WIN_SCREEN_SIZE_X, WIN_SCREEN_SIZE_Y);

	emulator.paused = 0;
	emulator.quit = 0;

	return 1;
}

int load_sram()
{
	if (!cartridge.battery)
		return 1;

	int rom_file_name_len = strlen(emulator.rom_file_name);
	strncpy(emulator.save_file_path, "./saves/", 8);
	strncpy(emulator.save_file_path + 8, emulator.rom_file_name, rom_file_name_len);
	strncpy(emulator.save_file_path + 8 + rom_file_name_len, ".sram", 6);
	FILE* file = fopen(emulator.save_file_path, "rb");
	if (!file && errno != ENOENT)
	{
		perror(emulator.save_file_path);
		return 0;
	}

	if (file)
	{
		int size = 0;
		switch (cartridge.ram_size)
		{
		case 0x00:
			size = 0x2000;
			break;
		case 0x02:
			size = 0x2000;
			break;
		case 0x03:
			size = 0x8000;
			break;
		case 0x04:
			size = 0x20000;
			break;
		case 0x05:
			size = 0x10000;
			break;
		}
		fread(cartridge.ram, 1, size, file);
		fclose(file);
	}
	else
		printf("%s: file not found\n", emulator.save_file_path);
	return 1;
}

// int init_window(window_t* window, char* title, int size_x, int size_y)
// {
// 	window->window = SDL_CreateWindow(
// 		title,
// 		SDL_WINDOWPOS_CENTERED,
// 		SDL_WINDOWPOS_CENTERED,
// 		size_x * WIN_SCALE,
// 		size_y * WIN_SCALE,
// 		SDL_WINDOW_SHOWN
// 	);
// 	if (!window->window)
// 	{
// 		printf("Window creation failed: %s\n", SDL_GetError());
// 		SDL_Quit();
// 		return 0;
// 	}
// 	window->renderer = SDL_CreateRenderer(window->window, -1, SDL_RENDERER_ACCELERATED);
// 	if (!window->renderer)
// 	{
// 		printf("SDL initialization failed: %s\n", SDL_GetError());
// 		SDL_DestroyWindow(window->window);
// 		SDL_Quit();
// 		return 0;
// 	}
// 	SDL_RenderSetLogicalSize(window->renderer, size_x, size_y);
// 	SDL_RenderSetIntegerScale(window->renderer, SDL_TRUE);
// 	window->screen_surface = SDL_CreateRGBSurfaceWithFormat(0, size_x, size_y, 32, SDL_PIXELFORMAT_RGB888);

// 	return 1;
// }

// void render_window(window_t* window)
// {
// 	SDL_RenderClear(window->renderer);
// 	SDL_Texture* render_tex = SDL_CreateTextureFromSurface(window->renderer, window->screen_surface);
// 	SDL_RenderCopy(window->renderer, render_tex, NULL, NULL);
// 	SDL_DestroyTexture(render_tex);
// 	SDL_RenderPresent(window->renderer);
// }

void close_app()
{
	if (cartridge.battery)	// dump external ram to save file
	{
		int fd = open(emulator.save_file_path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (fd != -1)
		{
			int size = 0;
			switch (cartridge.ram_size)
			{
			case 0x00:
				size = 0x2000;
				break;
			case 0x02:
				size = 0x2000;
				break;
			case 0x03:
				size = 0x8000;
				break;
			case 0x04:
				size = 0x20000;
				break;
			case 0x05:
				size = 0x10000;
				break;
			}
			write(fd, cartridge.ram, size);
			close(fd);
		}
		else
			perror(emulator.save_file_path);
	}

	// SDL_DestroyRenderer(emulator.window_tiles.renderer);
	// SDL_DestroyWindow(emulator.window_tiles.window);

	// SDL_DestroyRenderer(emulator.window_background9800.renderer);
	// SDL_DestroyWindow(emulator.window_background9800.window);

	// SDL_DestroyRenderer(emulator.window_background9C00.renderer);
	// SDL_DestroyWindow(emulator.window_background9C00.window);

	// SDL_DestroyRenderer(emulator.window_screen.renderer);
	// SDL_DestroyWindow(emulator.window_screen.window);

	// SDL_Quit();
}

int run_emulator()
{
	while (!emulator.quit)
	{
		long start_time = get_current_time();

		// handle_events();

		if (!emulator.paused)
			run_clock(FRAME_CYCLES);

		// display_screen();
		// display_vram();
		// display_background(&emulator.window_background9800, 0x1800);
		// display_background(&emulator.window_background9C00, 0x1C00);

		if (emulator.fforward)
			continue;

		long end_time = get_current_time();
		long sleep_time = start_time + FRAME_TIME - end_time;
		if (sleep_time > 0)
			usleep(sleep_time);
		// printf("%ld\n", end_time - start_time);
	}
	return 0;
}

void run_clock(int n_cycles)
{
	for (size_t i = 0; i < n_cycles; i++)
	{
		timers_tick();
		cpu_tick();
		ppu_tick();
	}
}

long get_current_time()
{
	struct timespec ts;
	clock_gettime(1, &ts);
	return ts.tv_sec * 1000000l + ts.tv_nsec / 1000l;
}

// void draw_line(int pos_x, int pos_y, int dir_x, int dir_y, int length, int color, int* pixels)
// {
// 	for (size_t i = 0; i < length; i++)
// 	{
// 		pixels[(u8)pos_y * 256 + (u8)pos_x] = color;
// 		pos_x += dir_x;
// 		pos_y += dir_y;
// 	}
// }

// void display_vram()
// {
// 	size_t tiles_per_row = 16;
// 	for (size_t y = 0; y < WIN_VRAM_SIZE_Y; y++)
// 	{
// 		for (size_t x = 0; x < WIN_VRAM_SIZE_X; x++)
// 		{
// 			size_t tile_x = x / 8;
// 			size_t tile_y = y / 8;
// 			size_t tile_index = tile_y * tiles_per_row + tile_x;
// 			pixel_code color_code = get_pixel_code(tiles[tile_index], x % 8, y % 8);
// 			pixel_color color = get_color(color_code);

// 			((int*)emulator.window_tiles.screen_surface->pixels)[y * WIN_VRAM_SIZE_X + x] = color;
// 		}
// 	}

// 	render_window(&emulator.window_tiles);
// }

// void display_background(window_t* window, u16 start_address)
// {
// 	size_t tiles_per_row = 32;
// 	for (size_t y = 0; y < WIN_BACKGROUND_SIZE_Y; y++)
// 	{
// 		for (size_t x = 0; x < WIN_BACKGROUND_SIZE_X; x++)
// 		{
// 			u8 tile_id = memory.video_ram[start_address + (y / 8) * tiles_per_row + (x / 8)];
// 			tile t = tiles[convert_tile_index(tile_id)];
// 			pixel_code color_code = get_pixel_code(t, x % 8, y % 8);
// 			pixel_color color = get_color(color_code);

// 			((int*)(window->screen_surface->pixels))[y * WIN_BACKGROUND_SIZE_X + x] = color;
// 		}
// 	}
// 	// draw viewport lines
// 	draw_line(read8(SCX), read8(SCY), 1, 0, WIN_SCREEN_SIZE_X, 0xFF0000, window->screen_surface->pixels);
// 	draw_line(read8(SCX), read8(SCY), 0, 1, WIN_SCREEN_SIZE_Y, 0xFF0000, window->screen_surface->pixels);
// 	draw_line(read8(SCX), read8(SCY) + WIN_SCREEN_SIZE_Y, 1, 0, WIN_SCREEN_SIZE_X, 0xFF0000, window->screen_surface->pixels);
// 	draw_line(read8(SCX) + WIN_SCREEN_SIZE_X, read8(SCY), 0, 1, WIN_SCREEN_SIZE_Y, 0xFF0000, window->screen_surface->pixels);
	
// 	render_window(window);
// }

// void display_screen()
// {
// 	emulator.window_screen.screen_surface->pixels = ppu.pixel_buffer_public;
// 	render_window(&emulator.window_screen);
// }

// void handle_events()
// {
// 	SDL_Event event;

// 	while (SDL_PollEvent(&event))
// 	{
// 		if (event.type == SDL_QUIT || event.window.event == SDL_WINDOWEVENT_CLOSE)
// 			emulator.quit = 1;
// 		// on key down
// 		if (event.type == SDL_KEYDOWN)
// 		{
// 			if (event.key.keysym.sym == SDLK_UP)
// 				press_key(KEY_UP);
// 			if (event.key.keysym.sym == SDLK_DOWN)
// 				press_key(KEY_DOWN);
// 			if (event.key.keysym.sym == SDLK_LEFT)
// 				press_key(KEY_LEFT);
// 			if (event.key.keysym.sym == SDLK_RIGHT)
// 				press_key(KEY_RIGHT);
// 			if (event.key.keysym.sym == SDLK_z)
// 				press_key(KEY_A);
// 			if (event.key.keysym.sym == SDLK_x)
// 				press_key(KEY_B);
// 			if (event.key.keysym.sym == SDLK_RETURN)
// 				press_key(KEY_START);
// 			if (event.key.keysym.sym == SDLK_BACKSPACE)
// 				press_key(KEY_SELECT);

// 			if (event.key.keysym.sym == SDLK_SPACE)
// 				emulator.fforward = 1;
// 			if (event.key.keysym.sym == SDLK_p)
// 				emulator.paused = !emulator.paused;
// 		}
// 		// on key up
// 		if (event.type == SDL_KEYUP)
// 		{
// 			if (event.key.keysym.sym == SDLK_UP)
// 				release_key(KEY_UP);
// 			if (event.key.keysym.sym == SDLK_DOWN)
// 				release_key(KEY_DOWN);
// 			if (event.key.keysym.sym == SDLK_LEFT)
// 				release_key(KEY_LEFT);
// 			if (event.key.keysym.sym == SDLK_RIGHT)
// 				release_key(KEY_RIGHT);
// 			if (event.key.keysym.sym == SDLK_z)
// 				release_key(KEY_A);
// 			if (event.key.keysym.sym == SDLK_x)
// 				release_key(KEY_B);
// 			if (event.key.keysym.sym == SDLK_RETURN)
// 				release_key(KEY_START);
// 			if (event.key.keysym.sym == SDLK_BACKSPACE)
// 				release_key(KEY_SELECT);

// 			if (event.key.keysym.sym == SDLK_SPACE)
// 				emulator.fforward = 0;
// 		}
// 	}
// }


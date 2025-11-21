#ifndef EMULATOR_H
# define EMULATOR_H

# include <time.h>
# include <unistd.h>
# include <fcntl.h>
# include <SDL2/SDL.h>
# include <libgen.h>
# include <errno.h>
# include "cartridge.h"
# include "memory.h"
# include "tile.h"
# include "timers.h"
# include "cpu.h"
# include "ppu.h"
# include "object.h"
# include "joypad.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Surface* screen_surface;
} window_t;

typedef struct
{
	int paused;
	int quit;
	int fforward;
	char* rom_file_name;
	char save_file_path[1024];

	window_t window_screen;
	window_t window_tiles;
	window_t window_background9800;
	window_t window_background9C00;
} emulator_t;
extern emulator_t emulator;

# define FRAME_TIME 16750l // microseconds
# define FRAME_CYCLES 17556 // number of machine cycles in a frame

# define WIN_SCALE 2
# define WIN_VRAM_SIZE_X 128
# define WIN_VRAM_SIZE_Y 192
# define WIN_BACKGROUND_SIZE_X 256
# define WIN_BACKGROUND_SIZE_Y 256
# define WIN_SCREEN_SIZE_X 160
# define WIN_SCREEN_SIZE_Y 144

int init_app();

int load_sram();

int init_window(window_t* window, char* title, int size_x, int size_y);

void render_window(window_t* window);

void close_app();

int run_emulator();

void process_frame();

long get_current_time();

void draw_line(int pos_x, int pos_y, int dir_x, int dir_y, int length, int color, int* pixels);

void display_screen();

void display_vram();

void display_background(window_t* window, u16 start_address);

void handle_events();

#ifdef __cplusplus
}
#endif

#endif

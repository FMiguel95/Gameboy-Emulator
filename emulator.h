#ifndef EMULATOR_H
# define EMULATOR_H

# include <time.h>
# include <unistd.h>
# include <SDL2/SDL.h>
# include "cartridge.h"
# include "memory.h"
# include "timers.h"
# include "cpu.h"
# include "ppu.h"

typedef enum e_Key
{
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_A,
	KEY_B,
	KEY_START,
	KEY_SELECT,
	KEY_COUNT
} keys_e;

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

	int current_presses[8];

	window_t window_vram;
	window_t window_background;
} emulator_t;
extern emulator_t emulator;

# define FRAME_TIME 16750l // microseconds
# define FRAME_CYCLES 17556 // number of machine cycles in a frame

# define WIN_SCALE 2
# define WIN_VRAM_SIZE_X 128
# define WIN_VRAM_SIZE_Y 192
# define WIN_BACKGROUND_SIZE_X 256
# define WIN_BACKGROUND_SIZE_Y 256

# define COLOR_DARKER	0x081820
# define COLOR_DARK		0x346856
# define COLOR_LIGHT	0x88C070
# define COLOR_LIGHTER	0xE0F8D0

int init_app();

int init_window(window_t* window, char* title, int size_x, int size_y);

void render_window(window_t* window);

void close_app();

int run_emulator();

long get_current_time();

int get_pixel_code(int tile_id, int x, int y);

void display_vram();

void display_background();

void handle_events();

#endif

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
	int paused;
	int quit;

	int current_presses[8];

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Surface* screen_surface;
} emulator_t;
extern emulator_t emulator;

# define FRAME_TIME 16750l // microseconds
# define FRAME_CYCLES 17556 // number of machine cycles in a frame

# define WIN_SCALE 3
# define WIN_SIZE_X 128
# define WIN_SIZE_Y 192

# define COLOR_DARKER	0x081820
# define COLOR_DARK		0x346856
# define COLOR_LIGHT	0x88C070
# define COLOR_LIGHTER	0xE0F8D0

int init_app();

void close_app();

int run_emulator();

long get_current_time();

void display_vram();

void handle_events();

#endif

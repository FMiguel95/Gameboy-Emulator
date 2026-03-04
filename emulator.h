#ifndef EMULATOR_H
# define EMULATOR_H

# include <time.h>
# include <unistd.h>
# include <fcntl.h>
# include <SDL2/SDL.h>
# include <libgen.h>
# include <sys/stat.h>
# include <errno.h>
# include "cartridge.h"
# include "memory.h"
# include "tile.h"
# include "timers.h"
# include "cpu.h"
# include "ppu.h"
# include "apu.h"
# include "object.h"
# include "joypad.h"

#ifdef __cplusplus
extern "C" {
#endif

// typedef struct
// {
// 	SDL_Window* window;
// 	SDL_Renderer* renderer;
// 	SDL_Surface* screen_surface;
// } window_t;

typedef struct
{
	int paused;
	int request_cycle;
	int request_scanline;
	int request_frame;
	int quit;
	int fforward;

	char* rom_file_name;
	char rom_file_path[1024];
	char save_file_path[1024];

} emulator_t;
extern emulator_t emulator;

# define FRAME_TIME 16743l // microseconds
# define FRAME_CYCLES 17556 // number of machine cycles in a frame

# define WIN_SCALE 2
# define WIN_VRAM_SIZE_X 128
# define WIN_VRAM_SIZE_Y 192
# define WIN_BACKGROUND_SIZE_X 256
# define WIN_BACKGROUND_SIZE_Y 256
# define WIN_SCREEN_SIZE_X 160
# define WIN_SCREEN_SIZE_Y 144

int init_emu();

int load_rom(const char* path);

int ensure_dir(const char* path);

int load_sram();

void save_sram();

void free_ptr(void* ptr);

void close_rom();

int run_emulator();

void run_clock(int n_cycles);

long get_current_time();

#ifdef __cplusplus
}
#endif

#endif

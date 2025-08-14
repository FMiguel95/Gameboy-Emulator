#ifndef EMULATOR_H
# define EMULATOR_H

# include <time.h>
# include <unistd.h>
# include "cartridge.h"
# include "memory.h"
# include "timers.h"
# include "cpu.h"
# include "ppu.h"

typedef struct
{
	int running;
	int paused;
} emulator;

# define FRAME_TIME 16750l // microseconds
# define FRAME_CYCLES 17556 // number of machine cycles in a frame

int run_emulator();

long get_current_time();

#endif

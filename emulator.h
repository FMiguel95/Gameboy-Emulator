#ifndef EMULATOR_H
# define EMULATOR_H

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

int run_emulator();

#endif

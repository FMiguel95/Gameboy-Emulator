#ifndef EMULATOR_H
# define EMULATOR_H

# include "cartridge.h"
# include "cpu.h"
# include "memory.h"

typedef struct
{
	int running;
	int paused;
} emulator;

int run_emulator();

#endif

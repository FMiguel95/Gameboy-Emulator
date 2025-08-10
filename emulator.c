#include "emulator.h"

int run_emulator()
{
	while (1)
	{
		timers_tick();
		cpu_tick();
		ppu_tick();
	}
	return 0;
}

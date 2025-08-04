#include "emulator.h"

int run_emulator()
{
	init_cpu();
	while (1)
	{
		cpu_tick();
	}
	return 0;
}

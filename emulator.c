#include "emulator.h"

int run_emulator()
{
	while (1)
	{
		cpu_tick();
	}
	return 0;
}

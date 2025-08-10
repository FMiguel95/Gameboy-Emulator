#include "timers.h"

timers_t timers;

int init_timers()
{
	timers = (timers_t){0};
	timers.div = memory.io_registers + 4;
	timers.div_counter = 0;
	timers.tima = memory.io_registers + 5;
	timers.tma = memory.io_registers + 6;
	timers.tac = memory.io_registers + 7;

	return 1;
}

void timers_tick()
{
	if (++timers.div_counter == 0)
		(*timers.div)++;
	
}

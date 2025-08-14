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
	
	if ((*timers.tac & 0b00000100) != 0)	// if tac bit 2, tima is incremented
	{
		timers.tima_counter++;
		u8 clock_speed_code = *timers.tac & 0b00000011;
		if ((clock_speed_code == 0b00 && timers.tima_counter >= 256)
			|| (clock_speed_code == 0b01 && timers.tima_counter >= 4)
			|| (clock_speed_code == 0b10 && timers.tima_counter >= 16)
			|| (clock_speed_code == 0b11 && timers.tima_counter >= 64))
		{
			(*timers.tima)++;
			if (*timers.tima == 0)
			{
				timers.tima_counter = 0;
				*timers.tima = *timers.tma;
				*memory.ie_register |= 0b00000100;
			}
		}
	}
}

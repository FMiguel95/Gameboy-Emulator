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
	timers.div_counter += 4;
	*timers.div = timers.div_counter >> 8;
	
	if (get_flag(*timers.tac, 2))	// if tac bit 2, tima is incremented
	{
		u8 clock_speed_code = *timers.tac & 0b00000011;
		u8 target_bit;
		switch (clock_speed_code)
		{
		case 0b00: target_bit = 9; break;
		case 0b01: target_bit = 3; break;
		case 0b10: target_bit = 5; break;
		case 0b11: target_bit = 7; break;
		}

		u8 prev = ((timers.div_counter - 1) >> target_bit) & 1;
		u8 curr = (timers.div_counter >> target_bit) & 1;
		if (prev == 1 && curr == 0)
		{
			(*timers.tima)++;
			if (*timers.tima == 0)
			{
				*timers.tima = *timers.tma;
				u8 val = read8(IF);
				set_flag(&val, 2, 1);	// request timer interrupt
				write8(IF, val);
			}
		}
	}
}

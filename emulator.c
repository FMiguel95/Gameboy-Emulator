#include "emulator.h"

int run_emulator()
{
	while (1)
	{
		long start_time = get_current_time();

		for (size_t i = 0; i < FRAME_CYCLES; i++)
		{
			timers_tick();
			cpu_tick();
			ppu_tick();
		}

		long end_time = get_current_time();
		usleep(start_time + FRAME_TIME - end_time);
		// printf("%ld\n", end_time - start_time);
	}
	return 0;
}

long get_current_time()
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);


	return ts.tv_sec * 1000000l + ts.tv_nsec / 1000l;
}

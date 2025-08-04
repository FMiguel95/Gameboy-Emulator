#include "emulator.h"

int main(int ac, char** av)
{
	if (ac <= 1)
	{
		printf("Usage: ./emu <rom_path>\n");
		return 1;
	}

	if (!read_rom(av[1]))
		return 1;

	if (!init_memory())
		return 1;

	run_emulator();

	return 0;
}

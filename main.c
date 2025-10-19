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
	if (!init_mbc())
		return 1;
	if (!init_memory())
		return 1;
	emulator.rom_file_name = basename(av[1]);
	if (!load_sram())
		return 1;
	if (!init_tiles())
		return 1;
	if (!init_joypad())
		return 1;
	if (!init_timers())
		return 1;
	if (!init_cpu())
		return 1;
	if (!init_ppu())
		return 1;
	if (!init_app())
	{
		close_app();
		return 1;
	}

	run_emulator();
	close_app();

	return 0;
}

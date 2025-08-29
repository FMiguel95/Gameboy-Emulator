#include "ppu.h"

ppu_t ppu;

int init_ppu()
{
	ppu = (ppu_t){0};
	
	ppu.lcdc = memory.io_registers + 0x40;
	ppu.stat = memory.io_registers + 0x41;
	ppu.scy = memory.io_registers + 0x42;
	ppu.scx = memory.io_registers + 0x43;
	ppu.ly = memory.io_registers + 0x44;
	ppu.lyc = memory.io_registers + 0x45;
	ppu.dma = memory.io_registers + 0x46;
	ppu.bgp = memory.io_registers + 0x47;
	ppu.obp0 = memory.io_registers + 0x48;
	ppu.obp1 = memory.io_registers + 0x49;
	ppu.wy = memory.io_registers + 0x4A;
	ppu.wx = memory.io_registers + 0x4B;

	return 1;
}

void ppu_tick()
{
	for (size_t i = 0; i < 4; i++)
	{
		
	}

	ppu.scanline_cycle++;
	if (ppu.scanline_cycle == 70) // made up h-blank
	{
		u8 IF_val = read8(IF);
		set_flag(&IF_val, Serial, 1);
		write8(IF, IF_val);
	}

	if (ppu.scanline_cycle == 114) // end of scanline
	{
		ppu.scanline_cycle = 0;
		(*ppu.ly)++;
		if (*ppu.ly == 144) // trigger v-blank
		{
			u8 IF_val = read8(IF);
			set_flag(&IF_val, VBlank, 1);
			write8(IF, IF_val);
		}
		else if (*ppu.ly == 154) // end of frame
			*ppu.ly = 0;
	}
}

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
	u8 prev_stat = *ppu.stat;

	for (size_t i = 0; i < 4; i++)
	{
		
	}

	ppu.scanline_cycle++;

	if (*ppu.ly < 144)
	{
		if (ppu.scanline_cycle == 0)
			ppu_set_mode(OAM_scan);
		else if (ppu.scanline_cycle == 20)
			ppu_set_mode(draw);
		else if (ppu.scanline_cycle == 63)
			ppu_set_mode(h_blank);
	}

	if (ppu.scanline_cycle == 114) // end of scanline
	{
		ppu.scanline_cycle = 0;
		(*ppu.ly)++;
		set_flag(ppu.stat, STAT_2, *ppu.lyc == *ppu.ly);
		if (*ppu.ly == 144)
		{
			ppu_set_mode(v_blank);
			// request v-blank interrupt
			u8 IF_val = read8(IF);
			set_flag(&IF_val, VBlank, 1);
			write8(IF, IF_val);
		}
		else if (*ppu.ly == 154) // end of frame
		{
			*ppu.ly = 0;
			ppu_set_mode(OAM_scan);
		}
	}

	if (!is_stat(prev_stat) && is_stat(*ppu.stat))
	{
		u8 IF_val = read8(IF);
		set_flag(&IF_val, LCD, 1);
		write8(IF, IF_val);
	}
}

void ppu_set_mode(ppu_mode mode)
{
	*ppu.stat = (*ppu.stat & 0b11111100) | (mode & 0b00000011);
}

ppu_mode ppu_get_mode()
{
	return *ppu.stat & 0b00000011;
}

int is_stat(u8 stat)
{
	if (get_flag(stat, STAT_6) && get_flag(stat, STAT_2))
		return 1;
	
	if (get_flag(stat, STAT_3) && (stat & 0b11) == h_blank)
		return 1;

	if (get_flag(stat, STAT_4) && (stat & 0b11) == v_blank)
		return 1;

	if (get_flag(stat, STAT_5) && (stat & 0b11) == OAM_scan)
		return 1;
	
	return 0;
}

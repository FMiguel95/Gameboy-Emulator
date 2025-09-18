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

	ppu.object_attributes = (object_t*)(&(memory.oam[0]));

	return 1;
}

void ppu_tick()
{
	static int prev_stat_state;

	ppu.scanline_cycle++;

	if (*ppu.ly < 144)
	{
		if (ppu.scanline_cycle == 0)
		{
			ppu_set_mode(OAM_scan);
			oam_scan();
		}
		else if (ppu.scanline_cycle == 20)
		{
			ppu_set_mode(draw);
			draw_scanline();
		}
		else if (ppu.scanline_cycle == 73)
			ppu_set_mode(h_blank);
	}

	if (ppu.scanline_cycle == 114) // end of scanline
	{
		ppu.scanline_cycle = 0;
		(*ppu.ly)++;
		if (*ppu.ly == 144)
		{
			ppu_set_mode(v_blank);
			// request v-blank interrupt
			u8 IF_val = read8(IF);
			set_flag(&IF_val, VBlank, 1);
			write8(IF, IF_val);
		}
		else if (*ppu.ly == 154) // end of frame
			*ppu.ly = 0;
	}

	set_flag(ppu.stat, STAT_2, *ppu.lyc == *ppu.ly);
	int stat_state = is_stat(*ppu.stat);
	if (!prev_stat_state && stat_state)
	{
		u8 IF_val = read8(IF);
		set_flag(&IF_val, LCD, 1);
		write8(IF, IF_val);
		// printf("STAT irq line: %d\n", *ppu.ly);
	}
	prev_stat_state = stat_state;
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

void oam_scan()
{
	int stored_count = 0;

	int sprite_mode = get_flag(*ppu.lcdc, LCDC_2);
	u8 sprite_height = sprite_mode == 0 ? 8 : 16;
	for (size_t i = 0; i < 40; i++)
	{
		if (stored_count == 10)
			break;
		if (ppu.object_attributes[i].x_pos > 0 &&
			(*ppu.ly + 16) >= ppu.object_attributes[i].y_pos &&
			(*ppu.ly + 16) < (ppu.object_attributes[i].y_pos + sprite_height))
		{
			ppu.scanline_objects[stored_count++] = ppu.object_attributes[i];
		}
	}
}

void draw_scanline()
{
	u8 bg_window_enable = get_flag(*ppu.lcdc, LCDC_0);	// 0 hides both background and window
	u8 object_enable = get_flag(*ppu.lcdc, LCDC_1);		// 0 hides objects
	u8 object_size = get_flag(*ppu.lcdc, LCDC_2);		// 0 makes objects 8x8, 1 makes objects 8x16
	u8 bg_tile_map = get_flag(*ppu.lcdc, LCDC_3);		// 0 background uses tile map at $9C00, 1 uses tile map at $9800
	u8 tile_data_select = get_flag(*ppu.lcdc, LCDC_4);	// 0 uses the 8800 fetch method, 1 uses the 8000
	u8 window_enable = get_flag(*ppu.lcdc, LCDC_5);		// 0 hides the window
	u8 window_tile_map = get_flag(*ppu.lcdc, LCDC_6);	// 0 window uses tile map at $9C00, 1 uses tile map at $9800

	for (size_t i = 0; i < 160; i++)
	{
		int pixel_color = LIGHTER_COLOR;
		if (bg_window_enable)
		{
			// get tile id
			u16 start_address = bg_tile_map ? 0x1C00 : 0x1800;
			int tile_id = memory.video_ram[start_address + 32 * (((*ppu.ly + *ppu.scy) & 0xFF) / 8) + ((i + *ppu.scx) & 0xFF) / 8];
			if (tile_data_select == 0 && tile_id < 128)
				tile_id += 256;
			
			// get tile data
			tile t = tiles[tile_id];
			pixel_code color_code = get_pixel_code(t, (i + *ppu.scx) % 8, (*ppu.ly + *ppu.scy) % 8);
			pixel_color = get_color(color_code);
		}
		ppu.pixel_buffer[*ppu.ly * 160 + i] = pixel_color;
	}
}

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

	ppu.object_attributes = (object_t*)memory.oam;

	return 1;
}

void ppu_tick()
{
	static int prev_stat_state;

	ppu.scanline_cycle++;

	if (*ppu.wy == *ppu.ly)
		ppu.wy_equaled_ly = 1;

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
		ppu.scanline_cycle = -1;
		(*ppu.ly)++;
		if (*ppu.ly == 144)
		{
			ppu_set_mode(v_blank);
			// request v-blank interrupt
			u8 IF_val = read8(IF);
			set_flag(&IF_val, VBlank, 1);
			write8(IF, IF_val);
			// reset window stuff
			ppu.window_line_counter = -1;
			ppu.wy_equaled_ly = 0;
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
	memset(ppu.scanline_objects, 0, sizeof(ppu.scanline_objects));
	int sprite_mode = get_flag(*ppu.lcdc, LCDC_2);
	u8 sprite_height = sprite_mode == 0 ? 8 : 16;
	int stored_count = 0;
	for (size_t i = 0; i < 40; i++)
	{
		// if (*ppu.ly == 5)
		// 	printf("obj %ld y_pos:%d x_pos:%d tile_index:%d attributes:%d\n", i, ppu.object_attributes[i].y_pos, ppu.object_attributes[i].x_pos, ppu.object_attributes[i].tile_index, ppu.object_attributes[i].attributes);
		if (stored_count == 10)
			break;
		if (ppu.object_attributes[i].x_pos > 0 &&
			(*ppu.ly + 16) >= ppu.object_attributes[i].y_pos &&
			(*ppu.ly + 16) < (ppu.object_attributes[i].y_pos + sprite_height))
		{
			ppu.scanline_objects[stored_count++] = ppu.object_attributes[i];
		}
	}
	// if (*ppu.ly == 5)
	// 	printf("...............................\n");
	// if (stored_count > 0)
	// 	printf("ly %d stored:%d\n", *ppu.ly, stored_count);

}

pixel_code get_bg_win_pixel()
{

}

void draw_scanline()
{
	u8 bg_window_enable = get_flag(*ppu.lcdc, LCDC_0);	// 0 hides both background and window
	u8 object_enable = get_flag(*ppu.lcdc, LCDC_1);		// 0 hides objects
	u8 object_size = get_flag(*ppu.lcdc, LCDC_2);		// 0 makes objects 8x8, 1 makes objects 8x16
	u8 bg_tile_map = get_flag(*ppu.lcdc, LCDC_3);		// 0 background uses tile map at $9800, 1 uses tile map at $9C00
	u8 tile_data_select = get_flag(*ppu.lcdc, LCDC_4);	// 0 uses the 8800 fetch method, 1 uses the 8000
	u8 window_enable = get_flag(*ppu.lcdc, LCDC_5);		// 0 hides the window
	u8 window_tile_map = get_flag(*ppu.lcdc, LCDC_6);	// 0 window uses tile map at $9800, 1 uses tile map at $9C00

	int has_window = 0;
	size_t x_pos = 0;

	for (size_t i = 0; i < 160; i++)
	{
		pixel_code palette_code = LIGHTER_CODE;
		pixel_code color_code = LIGHTER_CODE;
		// get object pixel
		for (size_t j = 0; j < 10; j++)
		{
			int sprite_x = ppu.scanline_objects[j].x_pos - 8;
			int sprite_y = ppu.scanline_objects[j].y_pos - 16;
			if (i >= sprite_x && i < sprite_x + 8)
			{
				u8 object_attributes = ppu.scanline_objects[j].attributes;
				tile t = tiles[ppu.scanline_objects[j].tile_index];
				u8 x_pixel = i - sprite_x;
				if (get_flag(object_attributes, 5))
					x_pixel = 7 - x_pixel;
				u8 y_pixel = *ppu.ly - sprite_y;
				if (get_flag(object_attributes, 6))
					y_pixel = 7 - y_pixel;
				palette_code = get_pixel_code(t, x_pixel, y_pixel);
				u8 palette_bit = get_flag(object_attributes, 4);
				u16 palette_address = palette_bit ? OBP1 : OBP0;
				color_code = get_palette_code(palette_code, palette_address);
				break;
			}
		}

		if (bg_window_enable && palette_code == LIGHTER_CODE)
		{
			if (window_enable && ppu.wy_equaled_ly && i >= (int)(*ppu.wx) - 7) // draw from window
			{
				// printf("ly:%d\n", *ppu.ly);
				if (!has_window)
				{
					has_window = 1;
					ppu.window_line_counter++;
					x_pos = 0;
				}
				// get tile id
				u16 start_address = window_tile_map ? 0x1C00 : 0x1800;
				int tile_id = memory.video_ram[start_address + 32 * ((ppu.window_line_counter & 0xFF) / 8) + (x_pos & 0xFF) / 8];
				if (tile_data_select == 0 && tile_id < 128)
					tile_id += 256;
				
				// get tile data
				tile t = tiles[tile_id];
				palette_code = get_pixel_code(t, x_pos % 8, ppu.window_line_counter % 8);
				color_code = get_palette_code(palette_code, BGP);
			}
			else // draw from background
			{
				// get tile id
				u16 start_address = bg_tile_map ? 0x1C00 : 0x1800;
				int tile_id = memory.video_ram[start_address + 32 * (((*ppu.ly + *ppu.scy) & 0xFF) / 8) + ((x_pos + *ppu.scx) & 0xFF) / 8];
				if (tile_data_select == 0 && tile_id < 128)
					tile_id += 256;
				
				// get tile data
				tile t = tiles[tile_id];
				palette_code = get_pixel_code(t, (x_pos + *ppu.scx) % 8, (*ppu.ly + *ppu.scy) % 8);
				color_code = get_palette_code(palette_code, BGP);
			}
		}
		ppu.pixel_buffer[*ppu.ly * 160 + i] = get_color(color_code);
		x_pos++;
	}
}

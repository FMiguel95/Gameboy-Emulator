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

	ppu.pixel_buffer_private = ppu.pixel_buffer1;
	ppu.pixel_buffer_public = ppu.pixel_buffer2;

	return 1;
}

void ppu_tick()
{
	ppu.prev_stat_state = ppu.stat_state;
	if (ppu.booted && get_flag(*ppu.lcdc, LCDC_7) == 0)
	{
		memset(ppu.pixel_buffer_public, 0xFF, 23040 * sizeof(int));
		ppu.booted = 0;
		*ppu.ly = 0;
		ppu.scanline_cycle = 0;
		ppu_set_mode(0);
		ppu.window_line_counter = -1;
		ppu.wy_equaled_ly = 0;
		ppu.line_153_glitch = 0;
		return;
	}
	ppu.stat_state = 0;

	if (*ppu.ly == 153 && ppu.scanline_cycle == 8)	// ly 153 will show as 0 for most of the line
	{
		*ppu.ly = 0;
		ppu.line_153_glitch = 1;
	}

	if ((*ppu.ly < 144) && (*ppu.wy == *ppu.ly))
		ppu.wy_equaled_ly = 1;

	if (*ppu.ly < 144 && !ppu.line_153_glitch)
	{
		if (ppu.scanline_cycle == 0 && ppu.booted)
		{
			ppu.current_mode = OAM_scan;
			oam_scan();
		}
		else if (ppu.scanline_cycle == 80)
		{
			set_mode3_delay();
			ppu.current_mode = draw;
			draw_scanline();
		}
		else if (ppu.scanline_cycle == 252 + ppu.mode3_delay)
			ppu.current_mode = h_blank;
	}

	if (ppu.scanline_cycle == 455) // end of scanline
	{
		ppu.scanline_cycle = -1;
		if (!ppu.line_153_glitch)
			(*ppu.ly)++;
		ppu.line_153_glitch = 0;
		ppu.booted = 1;
		if (*ppu.ly == 144)
		{
			// ppu_set_mode(v_blank);
			ppu.current_mode = v_blank;
			// request v-blank interrupt
			u8 IF_val = read8(IF);
			set_flag(&IF_val, VBlank, 1);
			write8(IF, IF_val);
			// reset window stuff
			ppu.window_line_counter = -1;
			ppu.wy_equaled_ly = 0;

			int* temp;
			temp = ppu.pixel_buffer_private;
			ppu.pixel_buffer_private = ppu.pixel_buffer_public;
			ppu.pixel_buffer_public = temp;

			memset(ppu.pixel_buffer_private, 0, 23040 * sizeof(int)); // screen tearing testing...
		}
		// else if (*ppu.ly == 154) // end of frame
		// 	*ppu.ly = 0;
	}

	ppu_set_mode(ppu.current_mode);
	set_flag(ppu.stat, STAT_2, *ppu.lyc == *ppu.ly);
	ppu.stat_state = is_stat(*ppu.stat);
	if (!ppu.prev_stat_state && ppu.stat_state)
	{
		u8 IF_val = read8(IF);
		set_flag(&IF_val, LCD, 1);
		write8(IF, IF_val);
		// printf("STAT irq line: %d\n", *ppu.ly);
	}
	ppu.scanline_cycle++;
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
	{
		// printf("stat irq : ly==lyc\n");
		return 1;
	}
	
	if (get_flag(stat, STAT_3) && (stat & 0b11) == h_blank)
	{
		// printf("stat irq : h_blank\n");
		return 1;
	}

	if (get_flag(stat, STAT_4) && (stat & 0b11) == v_blank)
	{
		// printf("stat irq : v_blank\n");
		return 1;
	}

	if (get_flag(stat, STAT_5) && (stat & 0b11) == OAM_scan)
	{
		// printf("stat irq : oam_scan\n");
		return 1;
	}
	// printf("no stat irq\n");
	return 0;
}

void set_mode3_delay()
{
	int blocks[20] = {0};

	for (int i = 0; i < 10; i++)
	{
		int x0 = ppu.scanline_objects[i].x_pos - 8;
		int x1 = x0 + 7;

		if (x1 < 0 || x0 >= 160)
			continue;

		if (x0 < 0)
			x0 = 0;
		if (x1 > 159)
			x1 = 159;

		for (int j = x0 / 8; j <= x1 / 8; j++)
			blocks[j] = 1;
	}
	
	int ret = 0;
	for (int i = 0; i < 20; i++)
		ret += (blocks[i] * 6);

	ppu.mode3_delay = ret;
}

void oam_scan()
{
	memset(ppu.scanline_objects, 0, sizeof(ppu.scanline_objects));
	u8 sprite_height = get_flag(*ppu.lcdc, LCDC_2) ? 16 : 8;
	int stored_count = 0;
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

pixel_info get_bg_win_pixel_info(LCD_control* lcdc_flags, size_t i, int* has_window, size_t* x_pos)
{
	pixel_info ret;
	ret.color_code = LIGHTER_CODE;
	ret.palette_index = LIGHTER_CODE;

	if (lcdc_flags->window_enable && ppu.wy_equaled_ly && (int)i >= (int)(*ppu.wx) - 7) // draw from window
	{
		if (!*has_window)
		{
			*has_window = 1;
			ppu.window_line_counter++;
			*x_pos = 0;
			if ((int)(*ppu.wx) - 7 < 0)
				*x_pos = -((int)(*ppu.wx) - 7);
		}
		// get tile id
		u16 start_address = lcdc_flags->window_tile_map ? 0x1C00 : 0x1800;
		int tile_id = memory.video_ram[start_address + 32 * ((ppu.window_line_counter & 0xFF) / 8) + (*x_pos & 0xFF) / 8];
		if (lcdc_flags->tile_data_select == 0 && tile_id < 128)
			tile_id += 256;
		
		// get tile data
		tile t = tiles[tile_id];
		ret.palette_index = get_pixel_code(t, *x_pos % 8, ppu.window_line_counter % 8);
		ret.color_code = get_palette_code(ret.palette_index, BGP);
	}
	else // draw from background
	{
		// get tile id
		u16 start_address = lcdc_flags->bg_tile_map ? 0x1C00 : 0x1800;
		int tile_id = memory.video_ram[start_address + 32 * (((*ppu.ly + *ppu.scy) & 0xFF) / 8) + ((*x_pos + *ppu.scx) & 0xFF) / 8];
		if (lcdc_flags->tile_data_select == 0 && tile_id < 128)
			tile_id += 256;
		
		// get tile data
		tile t = tiles[tile_id];
		ret.palette_index = get_pixel_code(t, (*x_pos + *ppu.scx) % 8, (*ppu.ly + *ppu.scy) % 8);
		ret.color_code = get_palette_code(ret.palette_index, BGP);
	}

	return ret;
}

pixel_info get_object_pixel_info(size_t i, u8 object_size)
{
	pixel_info ret;
	ret.color_code = LIGHTER_CODE;
	ret.palette_index = LIGHTER_CODE;

	u8 selected_x_pos = 0xFF;
	u8 max_height_index = object_size ? 15 : 7;
	for (size_t j = 0; j < 10; j++)
	{
		int sprite_screen_x_pos = (int)(ppu.scanline_objects[j].x_pos) - 8;
		int sprite_screen_y_pos = (int)(ppu.scanline_objects[j].y_pos) - 16;
		if ((int)i >= sprite_screen_x_pos && (int)i < sprite_screen_x_pos + 8)
		{
			u8 object_attributes = ppu.scanline_objects[j].attributes;

			u8 x_pixel = i - sprite_screen_x_pos;
			u8 y_pixel = *ppu.ly - sprite_screen_y_pos;
			if (get_flag(object_attributes, 5))
				x_pixel = 7 - x_pixel;
			if (get_flag(object_attributes, 6))
				y_pixel = max_height_index - y_pixel;

			u8 tile_index = ppu.scanline_objects[j].tile_index;
			if (max_height_index == 15)
			{
				if (y_pixel < 8)
					tile_index &= 0xFE;
				// else
				// 	tile_index |= 0x01;
			}
			tile t = tiles[tile_index];
			
			pixel_code palette_index = get_pixel_code(t, x_pixel, y_pixel);
			if (palette_index == LIGHTER_CODE || selected_x_pos <= ppu.scanline_objects[j].x_pos)
				continue;
			selected_x_pos = ppu.scanline_objects[j].x_pos;
			ret.palette_index = get_pixel_code(t, x_pixel, y_pixel);
			u8 palette_bit = get_flag(object_attributes, 4);
			u16 palette_address = palette_bit ? OBP1 : OBP0;
			ret.color_code = get_palette_code(ret.palette_index, palette_address);
			ret.object_attributes = object_attributes;
		}
	}
	return ret;
}

void draw_scanline()
{
	LCD_control lcdc_flags;
	lcdc_flags.bg_window_enable = get_flag(*ppu.lcdc, LCDC_0);	// 0 hides both background and window
	lcdc_flags.object_enable = get_flag(*ppu.lcdc, LCDC_1);		// 0 hides objects
	lcdc_flags.object_size = get_flag(*ppu.lcdc, LCDC_2);		// 0 makes objects 8x8, 1 makes objects 8x16
	lcdc_flags.bg_tile_map = get_flag(*ppu.lcdc, LCDC_3);		// 0 background uses tile map at $9800, 1 uses tile map at $9C00
	lcdc_flags.tile_data_select = get_flag(*ppu.lcdc, LCDC_4);	// 0 uses the 8800 fetch method, 1 uses the 8000
	lcdc_flags.window_enable = get_flag(*ppu.lcdc, LCDC_5);		// 0 hides the window
	lcdc_flags.window_tile_map = get_flag(*ppu.lcdc, LCDC_6);	// 0 window uses tile map at $9800, 1 uses tile map at $9C00

	int has_window = 0;
	size_t x_pos = 0;

	for (size_t i = 0; i < 160; i++)
	{
		pixel_info object_pixel_info = get_object_pixel_info(i, lcdc_flags.object_size);
		pixel_info bg_win_pixel_info = get_bg_win_pixel_info(&lcdc_flags, i, &has_window, &x_pos);

		pixel_code final_pixel_code = LIGHTER_CODE;

		if (lcdc_flags.object_enable && object_pixel_info.palette_index != LIGHTER_CODE) // if objects are enabled and pixel isn't transparent
		{
			// if bg is enabled and priority is set and color isn't 0 draw bg instead
			if (lcdc_flags.bg_window_enable && get_flag(object_pixel_info.object_attributes, 7) == 1 && bg_win_pixel_info.palette_index != LIGHTER_CODE)
				final_pixel_code = bg_win_pixel_info.color_code;
			else // draw object
				final_pixel_code = object_pixel_info.color_code;
		}
		else if (lcdc_flags.bg_window_enable) // just draw background if it's enabled
			final_pixel_code = bg_win_pixel_info.color_code;

		ppu.pixel_buffer_private[*ppu.ly * 160 + i] = get_color(final_pixel_code);
		x_pos++;
	}
}

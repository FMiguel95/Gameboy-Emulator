#include "tile.h"

tile tiles[0x300];

int init_tiles()
{
	for (size_t i = 0; i < 0x180; i++)
	{
		tiles[i] = memory.video_ram + i * 16;
	}
	for (size_t i = 0x180; i < 0x300; i++)
	{
		tiles[i] = memory.video_ram + 0x2000 + (i - 0x180) * 16;
	}
	return 1;
}

int convert_tile_index(u8 index)
{
	int converted_index = index;
	if (get_flag(*ppu.lcdc, LCDC_4) == 0 && index < 128)
		converted_index += 256;
	return converted_index;
}

pixel_index get_pixel_code(tile t, int x, int y)
{
	u8 bit_index = 7 - x;
	u8 val1 = (*(t + y * 2) >> bit_index) & 1;
	u8 val2 = (*(t + y * 2 + 1) >> bit_index) & 1;
	return (val2 << 1) | val1;
}

palette_code get_palette_code(pixel_index code, u16 data_address)
{
	u8 palette_data = read8_absolute(data_address);
	return (palette_data >> (code * 2)) & 0b11;
}

pixel_color get_color(palette_code code)
{
	switch (code) {
		case palette_code_0:
			return LIGHTER_COLOR;
		case palette_code_1:
			return LIGHT_COLOR;
		case palette_code_2:
			return DARK_COLOR;
		case palette_code_3:
			return DARKER_COLOR;
		default:
			printf("invalid color code\n");
			return LIGHTER_COLOR;
	}
}

int rgb555_to_rgb888(u16 rgb555)
{
	int r = rgb555 << 3 & 0b11111000;
	int g = rgb555 >> 2 & 0b11111000;
	int b = rgb555 >> 7 & 0b11111000;

	return ((r << 16) | (g << 8) | (b << 0));
}

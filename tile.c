#include "tile.h"

tile tiles[0x180];

int init_tiles()
{
	for (size_t i = 0; i < 0x180; i++)
	{
		tiles[i] = memory.video_ram + i * 16;
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

pixel_code get_pixel_code(tile t, int x, int y)
{
	u8 bit_index = 7 - x;
	u8 val1 = (*(t + y * 2) >> bit_index) & 1;
	u8 val2 = (*(t + y * 2 + 1) >> bit_index) & 1;
	return (val2 << 1) | val1;
}

pixel_color get_color(pixel_code code)
{
	switch (code) {
		case LIGHTER_CODE:
			return LIGHTER_COLOR;
		case LIGHT_CODE:
			return LIGHT_COLOR;
		case DARK_CODE:
			return DARK_COLOR;
		case DARKER_CODE:
			return DARKER_COLOR;
		default:
			printf("invalid color code\n");
			return LIGHTER_COLOR;
	}
}

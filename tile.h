#ifndef TILE_H
# define TILE_H

# include "types.h"
# include "memory.h"
# include "ppu.h"

typedef u8* tile;

extern tile tiles[0x180];

typedef enum {
	LIGHTER_CODE = 0b00,
	LIGHT_CODE = 0b01,
	DARK_CODE = 0b10,
	DARKER_CODE = 0b11
} pixel_code;

typedef enum {
	LIGHTER_COLOR = 0xE0F8D0,
	LIGHT_COLOR = 0x88C070,
	DARK_COLOR = 0x346856,
	DARKER_COLOR = 0x081820
} pixel_color;

int init_tiles();

int convert_tile_index(u8 index);

pixel_code get_pixel_code(tile t, int x, int y);

pixel_code get_palette_code(pixel_code code, u16 data_address);

pixel_color get_color(pixel_code code);

#endif

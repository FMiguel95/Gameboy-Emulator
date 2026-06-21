#ifndef TILE_H
# define TILE_H

# include "types.h"
# include "memory.h"
# include "ppu.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef u8* tile;

extern tile tiles[0x300];

typedef enum {
	pixel_index_0 = 0,
	pixel_index_1 = 1,
	pixel_index_2 = 2,
	pixel_index_3 = 3
} pixel_index;

typedef enum {
	palette_code_0 = 0,
	palette_code_1 = 1,
	palette_code_2 = 2,
	palette_code_3 = 3
} palette_code;

typedef enum {
	LIGHTER_COLOR = 0xE0F8D0,
	LIGHT_COLOR = 0x88C070,
	DARK_COLOR = 0x346856,
	DARKER_COLOR = 0x081820
} pixel_color;

typedef enum {
	tile_background,
	tile_window,
	tile_object
} tile_type;

typedef struct {
	tile_type tile_type;
	pixel_index palette_index;
	palette_code color_code;
	u8 tile_attributes;
} pixel_info;

int init_tiles();

int convert_tile_index(u8 index);

pixel_index get_pixel_code(tile t, int x, int y);

palette_code get_palette_code(pixel_index code, u16 data_address);

pixel_color get_color(palette_code code);

int rgb555_to_rgb888(u16 rgb555);

#ifdef __cplusplus
}
#endif

#endif

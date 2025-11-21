#ifndef PPU_H
# define PPU_H

# include "types.h"
# include "tile.h"
# include "object.h"
# include "memory.h"
# include "cpu.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	u8* lcdc;	// LCD Control
	u8* stat;	// LCD status
	u8* scy;	// Scroll Y
	u8* scx;	// Scroll X
	u8* ly;		// LCD Y coordinate (R)
	u8* lyc;	// LY compare
	u8* dma;	// DMA transfer and start
	u8* bgp;	// BG palette
	u8* obp0;	// Object palette 0
	u8* obp1;	// Object palette 1
	u8* wy;		// Window Y position
	u8* wx;		// Window X position

	object_t* object_attributes;
	object_t scanline_objects[10];

	int scanline_cycle;
	int window_line_counter;
	int wy_equaled_ly;
	int line_153_glitch;

	int pixel_buffer1[23040];	// 160 * 144
	int pixel_buffer2[23040];
	int* pixel_buffer_public;
	int* pixel_buffer_private;
} ppu_t;

extern ppu_t ppu;

typedef enum {
	LCDC_0,	// BG and Window enable/priority
	LCDC_1,	// OBJ enable
	LCDC_2,	// OBJ size
	LCDC_3,	// BG tile map area
	LCDC_4,	// BG and Window tile data area
	LCDC_5,	// Window enable
	LCDC_6,	// Window tile map area
	LCDC_7	// LCD enable
} LCDC_bit;

typedef struct {
	u8 bg_window_enable;
	u8 object_enable;
	u8 object_size;
	u8 bg_tile_map;
	u8 tile_data_select;
	u8 window_enable;
	u8 window_tile_map;
} LCD_control;

typedef enum {
	STAT_0,	// PPU mode R
	STAT_1,	// PPU mode R
	STAT_2,	// LYC == LY R
	STAT_3,	// Mode 0 int select R/W
	STAT_4,	// Mode 1 int select R/W
	STAT_5,	// Mode 2 int select R/W
	STAT_6,	// LYC int select R/W
} STAT_bit;

typedef enum {
	h_blank	 = 0b00,
	v_blank	 = 0b01,
	OAM_scan = 0b10,
	draw	 = 0b11
} ppu_mode;

int init_ppu();

// mode 2 -> OAM scan		20 cycles				VRAM
// mode 3 -> drawing pixels	43-72.25 cycles			None
// mode 0 -> h blank		21.75-51 cycles			VRAM, OAM
// ...
// mode 1 -> v blank		114 * 10 = 1140 cycles	VRAM, OAM
//							114 * 154 = 17556 cycles per frame
void ppu_tick();

void ppu_set_mode(ppu_mode mode);

ppu_mode ppu_get_mode();

int is_stat(u8 stat);

void oam_scan();

void draw_scanline();

#ifdef __cplusplus
}
#endif

#endif

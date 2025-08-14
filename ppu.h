#ifndef PPU_H
# define PPU_H

# include "types.h"
# include "memory.h"

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



	int buffer[23040];	// 160x144 screen
} ppu_t;

extern ppu_t ppu;

int init_ppu();

// mode 2 -> OAM scan		20 cycles				VRAM
// mode 3 -> drawing pixels	43-72.25 cycles			None
// mode 0 -> h blank		21.75-51 cycles			VRAM, OAM
// ...
// mode 1 -> v blank		114 * 10 = 1140 cycles	VRAM, OAM
void ppu_tick();

#endif

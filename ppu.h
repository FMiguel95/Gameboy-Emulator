#ifndef PPU_H
# define PPU_H

# include "types.h"
# include "memory.h"

typedef enum {
	mode1,
	mode2,
	mode3,
	mode4
} ppu_mode;

typedef struct {
	ppu_mode current_mode;
} ppu_t;

extern ppu_t ppu;

int init_ppu();

void ppu_tick();

#endif

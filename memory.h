#ifndef MEMORY_H
# define MEMORY_H

# include "types.h"

typedef struct {
	u8 rom_bank0[0x4000];		// 0000 - 3FFF
	u8 rom_bank1[0x4000];		// 4000 - 7FFF
	u8 video_ram[0x2000];		// 8000 - 9FFF
	u8 external_ram[0x2000];	// A000 - BFFF
	u8 work_ram[0x2000];		// C000 - DFFF
	u8 echo_ram[0x1E00];		// E000 - FDFF
	u8 oam[0xA0];				// FE00 - FE9F
	u8 unused[0x60];			// FEA0 - FEFF
	u8 io_registers[0x7F];		// FF00 - FF7F
	u8 high_ram[0x7F];			// FF80 - FFFE
	u8 ie_register[0x1];		// FFFF
} memory_t;

static memory_t memory;

void write8(u16 address, u8 val);

void write16(u16 address, u16 val);

u8 read8(u16 address);

u16 read16(u16 address);

#endif

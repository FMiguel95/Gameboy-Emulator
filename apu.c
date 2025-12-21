#include "apu.h"

apu_t apu;

int init_apu()
{
	apu = (apu_t){0};
	
	apu.nr10 = memory.io_registers + 0x10;
	apu.nr11 = memory.io_registers + 0x11;
	apu.nr12 = memory.io_registers + 0x12;
	apu.nr13 = memory.io_registers + 0x13;
	apu.nr14 = memory.io_registers + 0x14;

	apu.nr21 = memory.io_registers + 0x16;
	apu.nr22 = memory.io_registers + 0x17;
	apu.nr23 = memory.io_registers + 0x18;
	apu.nr24 = memory.io_registers + 0x19;

	apu.nr30 = memory.io_registers + 0x1A;
	apu.nr31 = memory.io_registers + 0x1B;
	apu.nr32 = memory.io_registers + 0x1C;
	apu.nr33 = memory.io_registers + 0x1D;
	apu.nr34 = memory.io_registers + 0x1E;
	
	apu.nr41 = memory.io_registers + 0x20;
	apu.nr42 = memory.io_registers + 0x21;
	apu.nr43 = memory.io_registers + 0x22;
	apu.nr44 = memory.io_registers + 0x23;

	apu.nr50 = memory.io_registers + 0x24;
	apu.nr51 = memory.io_registers + 0x25;
	apu.nr52 = memory.io_registers + 0x26;

	apu.wave_ram = memory.io_registers + 0x30;

	return 1;
}

void apu_tick()
{
	int target_bit = 4;
	if (((*timers.div >> target_bit) & 1) == 0 && ((timers.div_prev >> target_bit) & 1) == 1)
		apu.div_apu++;

	// if (apu.div_apu % 8 == 0)
	// 	// Envelope sweep
	// if (apu.div_apu % 2 == 0)
	// 	// Sound length
	// if (apu.div_apu % 4 == 0)
	// 	// CH1 freq sweep
}

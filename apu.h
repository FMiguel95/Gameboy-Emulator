#ifndef CPU_H
# define CPU_H

# include "types.h"
# include "memory.h"

#ifdef __cplusplus
extern "C" {
#endif

// channels output a value from $0 to $F
// DACs convert these from -1 to 1
// all 4 values added together the result ranges from -4 to 4
// the output is put through a high-pass filter

typedef struct {
	// Global control registers
	u8* nr52;		// Audio master control
	u8* nr51;		// Sound panning
	u8* nr50;		// Master volume & VIN panning

	// Sound Channel 1 — Pulse with period sweep
	u8* nr10;		// Channel 1 sweep
	u8* nr11;		// Channel 1 length timer & duty cycle
	u8* nr12;		// Channel 1 volume & envelope
	u8* nr13;		// Channel 1 period low [write-only]
	u8* nr14;		// Channel 1 period high & control

	// Sound Channel 2 — Pulse
	u8* nr21;		// Channel 2 length timer & duty cycle
	u8* nr22;		// Channel 2 volume & envelope
	u8* nr23;		// Channel 2 period low [write-only]
	u8* nr24;		// Channel 2 period high & control

	// Sound Channel 3 — Wave output
	u8* nr30;		// Channel 3 DAC enable
	u8* nr31;		// Channel 3 length timer [write-only]
	u8* nr32;		// Channel 3 output level
	u8* nr33;		// Channel 3 period low [write-only]
	u8* nr34;		// Channel 3 period high & control
	u8* wave_ram;	// Wave pattern RAM

	// Sound Channel 4 — Noise
	u8* nr41;		// Channel 4 length timer [write-only]
	u8* nr42;		// Channel 4 volume & envelope
	u8* nr43;		// Channel 4 frequency & randomness
	u8* nr44;		// Channel 4 control

	u8 div_apu;

} apu_t;

extern apu_t apu;

int duty_cycles[4][8] = {
	{ 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 1, 0, 0, 0 },
	{ 1, 0, 0, 0, 0, 0, 0, 1 }
};

int init_ppu();

void apu_tick();

#ifdef __cplusplus
}
#endif

#endif

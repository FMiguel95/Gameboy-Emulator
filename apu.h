#ifndef APU_H
# define APU_H

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
	int ch1_request_trigger;
	int ch1_length_timer;
	int ch1_period_divider;
	int ch1_duty_pos;
	int ch1_current_volume;
	int ch1_envelope_timer;

	// Sound Channel 2 — Pulse
	u8* nr21;		// Channel 2 length timer & duty cycle
	u8* nr22;		// Channel 2 volume & envelope
	u8* nr23;		// Channel 2 period low [write-only]
	u8* nr24;		// Channel 2 period high & control
	int ch2_request_trigger;
	int ch2_length_timer;
	int ch2_period_divider;
	int ch2_duty_pos;
	int ch2_current_volume;
	int ch2_envelope_timer;

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
	int div_apu_ticked;

} apu_t;

extern apu_t apu;

typedef enum {
	NR_0,	// 
	NR_1,	// 
	NR_2,	// 
	NR_3,	// 
	NR_4,	// 
	NR_5,	// 
	NR_6,	// 
	NR_7,	// 
} NR_bit;

typedef enum {
	NR52_0,	// CH1 on?
	NR52_1,	// CH2 on?
	NR52_2,	// CH3 on?
	NR52_3,	// CH4 on?
	NR52_4,	// unused
	NR52_5,	// unused
	NR52_6,	// unused
	NR52_7,	// Audio on/off
} NR52_bit;

typedef enum {
	NR51_0,	// CH1 right
	NR51_1,	// CH2 right
	NR51_2,	// CH3 right
	NR51_3,	// CH4 right
	NR51_4,	// CH1 left
	NR51_5,	// CH2 left
	NR51_6,	// CH3 left
	NR51_7,	// CH4 left
} NR51_bit;

typedef enum {
	NR50_0,	// Right volume
	NR50_1,	// Right volume
	NR50_2,	// Right volume
	NR50_3,	// VIN right
	NR50_4,	// Left volume
	NR50_5,	// Left volume
	NR50_6,	// Left volume
	NR50_7,	// VIN left
} NR50_bit;

typedef enum {
	NR11_0,	// Initial length timer
	NR11_1,	// Initial length timer
	NR11_2,	// Initial length timer
	NR11_3,	// Initial length timer
	NR11_4,	// Initial length timer
	NR11_5,	// Initial length timer
	NR11_6,	// Wave duty
	NR11_7,	// Wave duty
} NR11_bit;

typedef enum {
	NR12_0,	// Sweep pace
	NR12_1,	// Sweep pace
	NR12_2,	// Sweep pace
	NR12_3,	// Env dir
	NR12_4,	// Initial volume
	NR12_5,	// Initial volume
	NR12_6,	// Initial volume
	NR12_7,	// Initial volume
} NR12_bit;

typedef enum {
	NR13_0,	// Period low
	NR13_1,	// Period low
	NR13_2,	// Period low
	NR13_3,	// Period low
	NR13_4,	// Period low
	NR13_5,	// Period low
	NR13_6,	// Period low
	NR13_7,	// Period low
} NR13_bit;

typedef enum {
	NR14_0,	// Period high
	NR14_1,	// Period high
	NR14_2,	// Period high
	NR14_3,	// unused
	NR14_4,	// unused
	NR14_5,	// unused
	NR14_6,	// Length enable
	NR14_7,	// Trigger
} NR14_bit;

typedef enum {
	NR21_0,	// Initial length timer
	NR21_1,	// Initial length timer
	NR21_2,	// Initial length timer
	NR21_3,	// Initial length timer
	NR21_4,	// Initial length timer
	NR21_5,	// Initial length timer
	NR21_6,	// Wave duty
	NR21_7,	// Wave duty
} NR21_bit;

typedef enum {
	NR22_0,	// Sweep pace
	NR22_1,	// Sweep pace
	NR22_2,	// Sweep pace
	NR22_3,	// Env dir
	NR22_4,	// Initial volume
	NR22_5,	// Initial volume
	NR22_6,	// Initial volume
	NR22_7,	// Initial volume
} NR22_bit;

typedef enum {
	NR23_0,	// Period low
	NR23_1,	// Period low
	NR23_2,	// Period low
	NR23_3,	// Period low
	NR23_4,	// Period low
	NR23_5,	// Period low
	NR23_6,	// Period low
	NR23_7,	// Period low
} NR23_bit;

typedef enum {
	NR24_0,	// Period high
	NR24_1,	// Period high
	NR24_2,	// Period high
	NR24_3,	// unused
	NR24_4,	// unused
	NR24_5,	// unused
	NR24_6,	// Length enable
	NR24_7,	// Trigger
} NR24_bit;

int init_apu();

void apu_tick();

#ifdef __cplusplus
}
#endif

#endif

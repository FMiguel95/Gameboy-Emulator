#ifndef APU_H
# define APU_H

# include "types.h"
# include "memory.h"

#ifdef __cplusplus
extern "C" {
#endif

# define SAMPLE_RATE 48000
# define SAMPLE_BUFFER_SIZE 4096

typedef struct {
	s8 buffer[SAMPLE_BUFFER_SIZE * 2];
	int head_index;
	int tail_index;
} ring_buffer;

void buffer_reset(ring_buffer* rb);

void buffer_push(ring_buffer* rb, s8 val);

int buffer_size(const ring_buffer* rb);

// channels output a value from $0 to $F
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
	int ch1_sweep_pace;
	int ch1_sweep_pace_timer;

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
	u8* wave_ram;	// Wave pattern RAM (16 bytes)
	int ch3_request_trigger;
	int ch3_length_timer;
	int ch3_period_divider;
	int ch3_duty_pos;

	// Sound Channel 4 — Noise
	u8* nr41;		// Channel 4 length timer [write-only]
	u8* nr42;		// Channel 4 volume & envelope
	u8* nr43;		// Channel 4 frequency & randomness
	u8* nr44;		// Channel 4 control
	int ch4_request_trigger;
	int ch4_length_timer;
	int ch4_current_volume;
	int ch4_envelope_timer;
	u16 ch4_shift_register;
	int ch4_shift_timer;

	u8 div_apu;
	int div_apu_ticked;

	ring_buffer rb;
	long sample_timer; // values are in one hundred thousandths of a second

	int sound_enable_global;
	int sound_enable_ch1;
	int sound_enable_ch2;
	int sound_enable_ch3;
	int sound_enable_ch4;

} apu_t;

extern apu_t apu;

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

typedef enum {
	NR30_0,	// unused
	NR30_1,	// unused
	NR30_2,	// unused
	NR30_3,	// unused
	NR30_4,	// unused
	NR30_5,	// unused
	NR30_6,	// unused
	NR30_7,	// DAC on/off
} NR30_bit;

typedef enum {
	NR31_0,	// Initial length timer
	NR31_1,	// Initial length timer
	NR31_2,	// Initial length timer
	NR31_3,	// Initial length timer
	NR31_4,	// Initial length timer
	NR31_5,	// Initial length timer
	NR31_6,	// Initial length timer
	NR31_7,	// Initial length timer
} NR31_bit;

typedef enum {
	NR32_0,	// unused
	NR32_1,	// unused
	NR32_2,	// unused
	NR32_3,	// unused
	NR32_4,	// unused
	NR32_5,	// Output level
	NR32_6,	// Output level
	NR32_7,	// unused
} NR32_bit;

typedef enum {
	NR33_0,	// Period low
	NR33_1,	// Period low
	NR33_2,	// Period low
	NR33_3,	// Period low
	NR33_4,	// Period low
	NR33_5,	// Period low
	NR33_6,	// Period low
	NR33_7,	// Period low
} NR33_bit;

typedef enum {
	NR34_0,	// Period high
	NR34_1,	// Period high
	NR34_2,	// Period high
	NR34_3,	// unused
	NR34_4,	// unused
	NR34_5,	// unused
	NR34_6,	// Length enable
	NR34_7,	// Trigger
} NR34_bit;

typedef enum {
	NR41_0,	// Initial length timer
	NR41_1,	// Initial length timer
	NR41_2,	// Initial length timer
	NR41_3,	// Initial length timer
	NR41_4,	// Initial length timer
	NR41_5,	// Initial length timer
	NR41_6,	// unused
	NR41_7,	// unused
} NR41_bit;

typedef enum {
	NR42_0,	// Sweep pace
	NR42_1,	// Sweep pace
	NR42_2,	// Sweep pace
	NR42_3,	// Env dir
	NR42_4,	// Initial volume
	NR42_5,	// Initial volume
	NR42_6,	// Initial volume
	NR42_7,	// Initial volume
} NR42_bit;

typedef enum {
	NR43_0,	// Clock divider
	NR43_1,	// Clock divider
	NR43_2,	// Clock divider
	NR43_3,	// LFSR width
	NR43_4,	// Clock shift
	NR43_5,	// Clock shift
	NR43_6,	// Clock shift
	NR43_7,	// Clock shift
} NR43_bit;

typedef enum {
	NR44_0,	// unused
	NR44_1,	// unused
	NR44_2,	// unused
	NR44_3,	// unused
	NR44_4,	// unused
	NR44_5,	// unused
	NR44_6,	// Length enable
	NR44_7,	// Trigger
} NR44_bit;

int init_apu();

void apu_tick();

#ifdef __cplusplus
}
#endif

#endif

#include "apu.h"

apu_t apu;

static int duty_cycles[4][8] = {
	{ 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 1, 1, 1, 1, 0, 0, 0 },
	{ 1, 0, 0, 0, 0, 0, 0, 1 }
};

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

	buffer_reset(&apu.rb);
	apu.sample_timer = 0;

	return 1;
}

int ch1_digital_output()
{
	if (!get_flag(*apu.nr52, NR52_0))
		return 0;
	
	int wave_duty = *apu.nr21 >> 6;
	int val = duty_cycles[wave_duty][apu.ch1_duty_pos] * apu.ch1_current_volume;
	return val;
}

int ch2_digital_output()
{
	if (!get_flag(*apu.nr52, NR52_1))
		return 0;
	
	int wave_duty = *apu.nr21 >> 6;
	int val = duty_cycles[wave_duty][apu.ch2_duty_pos] * apu.ch2_current_volume;
	return val;
}

void ch1_tick()
{
	if (apu.ch1_request_trigger) // trigger channel
	{
		apu.ch1_request_trigger = 0;
		// enable ch1
		set_flag(apu.nr52, NR52_0, 1);
		// reset length timer if expired
		if (apu.ch1_length_timer > 0)
			apu.ch1_length_timer = 64 - (*apu.nr11 & 0b111111);
		// set period divider to the contents of nr13 and nr14
		apu.ch1_period_divider = ((int)(*apu.nr14 & 0b111) << 8) | *apu.nr13;
		// reset envelop timer
		apu.ch1_envelope_timer = *apu.nr12 & 0b111;
		// volume is set to contents of nr12 initial volume
		apu.ch1_current_volume = (*apu.nr12 >> 4) & 0b1111;
	}

	if (*apu.nr12 & 0b11111000 == 0) // disable channel if initial volume and env is set to 0
		set_flag(apu.nr52, NR52_0, 0);

	if (!get_flag(*apu.nr52, NR52_0))
		return;

	apu.ch1_period_divider++;
	if (apu.ch1_period_divider > 0x7FF)
	{
		apu.ch1_period_divider = ((int)(*apu.nr14 & 0b111) << 8) | *apu.nr13;
		apu.ch1_duty_pos = (apu.ch1_duty_pos + 1) & 7;
	}

	if (!apu.div_apu_ticked)
		return;

	if (apu.div_apu % 2 == 0) // 256 Hz sound length
	{
		if (apu.ch1_length_timer > 0 && get_flag(*apu.nr14, NR14_6))
		{
			apu.ch1_length_timer--;
			if (apu.ch1_length_timer == 0) // channel disables itself after length timer expires
			{
				set_flag(apu.nr52, NR52_0, 0);
				return;
			}
		}
	}

	int ch1_sweep_pace = *apu.nr12 & 0b111;
	if (apu.div_apu % 8 == 0 && ch1_sweep_pace > 0) // 64 Hz envelope sweep
	{
		apu.ch1_envelope_timer--;
		if (apu.ch1_envelope_timer == 0)
		{
			apu.ch1_envelope_timer = ch1_sweep_pace;
			int dir = get_flag(*apu.nr12, NR12_3) ? 1 : -1;
			apu.ch1_current_volume += dir;
			if (apu.ch1_current_volume < 0x0)
				apu.ch1_current_volume = 0x0;
			else if (apu.ch1_current_volume > 0xF)
				apu.ch1_current_volume = 0xF;
		}
	}
}

void ch2_tick()
{
	if (apu.ch2_request_trigger) // trigger channel
	{
		apu.ch2_request_trigger = 0;
		// enable ch2
		set_flag(apu.nr52, NR52_1, 1);
		// reset length timer if expired
		if (apu.ch2_length_timer > 0)
			apu.ch2_length_timer = 64 - (*apu.nr21 & 0b111111);
		// set period divider to the contents of NR23 and NR24
		apu.ch2_period_divider = ((int)(*apu.nr24 & 0b111) << 8) | *apu.nr23;
		// reset envelop timer
		apu.ch2_envelope_timer = *apu.nr22 & 0b111;
		// volume is set to contents of NR22 initial volume
		apu.ch2_current_volume = (*apu.nr22 >> 4) & 0b1111;
	}

	if (*apu.nr22 & 0b11111000 == 0) // disable channel if initial volume and env is set to 0
		set_flag(apu.nr52, NR52_1, 0);

	if (!get_flag(*apu.nr52, NR52_1))
		return;

	apu.ch2_period_divider++;
	if (apu.ch2_period_divider > 0x7FF)
	{
		apu.ch2_period_divider = ((int)(*apu.nr24 & 0b111) << 8) | *apu.nr23;
		apu.ch2_duty_pos = (apu.ch2_duty_pos + 1) & 7;
	}

	if (!apu.div_apu_ticked)
		return;

	if (apu.div_apu % 2 == 0) // 256 Hz sound length
	{
		if (apu.ch2_length_timer > 0 && get_flag(*apu.nr24, NR24_6))
		{
			apu.ch2_length_timer--;
			if (apu.ch2_length_timer == 0) // channel disables itself after length timer expires
			{
				set_flag(apu.nr52, NR52_1, 0);
				return;
			}
		}
	}

	int ch2_sweep_pace = *apu.nr22 & 0b111;
	if (apu.div_apu % 8 == 0 && ch2_sweep_pace > 0) // 64 Hz envelope sweep
	{
		apu.ch2_envelope_timer--;
		if (apu.ch2_envelope_timer == 0)
		{
			apu.ch2_envelope_timer = ch2_sweep_pace;
			int dir = get_flag(*apu.nr22, NR22_3) ? 1 : -1;
			apu.ch2_current_volume += dir;
			if (apu.ch2_current_volume < 0x0)
				apu.ch2_current_volume = 0x0;
			else if (apu.ch2_current_volume > 0xF)
				apu.ch2_current_volume = 0xF;
		}
	}
}

u8 ch3_digital_output()
{
	return 0;
}

u8 ch4_digital_output()
{
	return 0;
}

void push_sample()
{
	// apu.sample_buffer[apu.sample_iterator++] = (ch1_digital_output() / 2 + ch2_digital_output() / 2) * 8;
	// // apu.sample_buffer[apu.sample_iterator++] = 0;
	// if (apu.sample_iterator == SAMPLE_BUFFER_SIZE)
	// {
	// 	apu.sample_iterator = 0;
	// 	// printf("0\n");
	// }
	u8 sample = (ch1_digital_output() + ch2_digital_output()) * 4;
	buffer_push(&apu.rb, sample);
}

void apu_tick()
{
	apu.sample_timer += SAMPLE_RATE;
	if (apu.sample_timer >= 1048576)
	{
		push_sample();
		apu.sample_timer -= 1048576;
	}

	apu.div_apu_ticked = 0;
	int target_bit = 4; // 5 when double speed mode
	if (((*timers.div >> target_bit) & 1) == 0 && ((timers.div_prev >> target_bit) & 1) == 1)
	{
		apu.div_apu++; // 512 Hz frequency
		apu.div_apu_ticked = 1;
	}

	if (get_flag(*apu.nr52, NR52_7) == 0)
		return;

	ch1_tick();
	ch2_tick();
	// if (apu.div_apu % 8 == 0) // 64 Hz
	// 	// Envelope sweep
	// if (apu.div_apu % 2 == 0) // 256 Hz
	// 	// Sound length
	// if (apu.div_apu % 4 == 0) // 128 Hz
	// 	// CH1 freq sweep

}

void buffer_reset(ring_buffer* rb)
{
	memset(rb->buffer, 0, SAMPLE_BUFFER_SIZE * 2);
	rb->head_index = 0;
	rb->tail_index = 0;
}

void buffer_push(ring_buffer* rb, u8 val)
{
	rb->tail_index = (SAMPLE_BUFFER_SIZE - 1) & (rb->tail_index + 1);
	rb->buffer[rb->tail_index] = val;
	rb->buffer[SAMPLE_BUFFER_SIZE + rb->tail_index] = val;
	if (rb->tail_index == rb->head_index)
		rb->head_index = (SAMPLE_BUFFER_SIZE - 1) & (rb->head_index + 1);
}

int buffer_size(const ring_buffer* rb)
{
	return (rb->tail_index - rb->head_index) & (SAMPLE_BUFFER_SIZE - 1);
}

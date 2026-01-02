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

	return 1;
}

u8 ch1_digital_output()
{
	return 0;
}

u8 ch2_digital_output()
{
	return 0;
}

void ch2_tick()
{
	if (!get_flag(*apu.nr24, 7))
		return;
	if (apu.ch2_request_trigger) // trigger channel
	{
		apu.ch2_request_trigger = 0;
		apu.ch2_length_timer = 64 - (*apu.nr21 & 0b11111);
		apu.ch2_period_divider = ((int)(*apu.nr24 & 0b111) << 8) | *apu.nr23;
		apu.ch2_envelope_timer = *apu.nr22 & 0b111;
		apu.ch2_current_volume = (*apu.nr22 >> 4) & 0b1111;
	}

	apu.ch2_period_divider++;
	if (apu.ch2_period_divider > 0x7FF)
	{
		apu.ch2_period_divider = ((int)(*apu.nr24 & 0b111) << 8) | *apu.nr23;
		apu.ch2_duty_pos = (apu.ch2_duty_pos + 1) & 7;
	}


	if (apu.div_apu % 2 == 0) // 256 Hz sound length
	{
		if (apu.ch2_length_timer > 0 && get_flag(*apu.nr24, 6))
		{
			apu.ch2_length_timer--;
			if (apu.ch2_length_timer == 0)
			{
				set_flag(apu.nr24, 7, 0);
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
			int dir = get_flag(*apu.nr22, 3) ? 1 : -1;
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

void apu_tick()
{
	int target_bit = 4; // 5 when double speed mode
	if (((*timers.div >> target_bit) & 1) == 0 && ((timers.div_prev >> target_bit) & 1) == 1)
		apu.div_apu++; // 512 Hz frequency

	if (get_flag(*apu.nr52, 7) == 0)
		return;

	// if (apu.div_apu % 8 == 0) // 64 Hz
	// 	// Envelope sweep
	// if (apu.div_apu % 2 == 0) // 256 Hz
	// 	// Sound length
	// if (apu.div_apu % 4 == 0) // 128 Hz
	// 	// CH1 freq sweep
}

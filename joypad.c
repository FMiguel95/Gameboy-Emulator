#include "joypad.h"

joypad_t joypad;

int init_joypad()
{
	joypad = (joypad_t){0};
	joypad.joyp = memory.io_registers + 0x0;
	*joypad.joyp = 0b00111111;
	return 1;
}

u8 read_joypad()
{
	u8 result = *joypad.joyp | 0b00001111;

	// set_flag(&result, 7, 1);
	// set_flag(&result, 6, 1);

	if (!get_flag(*joypad.joyp, 5))
	{
		if (joypad.keys_pressed[KEY_START])
			set_flag(&result, 3, 0);
		if (joypad.keys_pressed[KEY_SELECT])
			set_flag(&result, 2, 0);
		if (joypad.keys_pressed[KEY_B])
			set_flag(&result, 1, 0);
		if (joypad.keys_pressed[KEY_A])
			set_flag(&result, 0, 0);
	}

	if (!get_flag(*joypad.joyp, 4))
	{
		if (joypad.keys_pressed[KEY_DOWN])
			set_flag(&result, 3, 0);
		if (joypad.keys_pressed[KEY_UP])
			set_flag(&result, 2, 0);
		if (joypad.keys_pressed[KEY_LEFT])
			set_flag(&result, 1, 0);
		if (joypad.keys_pressed[KEY_RIGHT])
			set_flag(&result, 0, 0);
	}

	*joypad.joyp = result;
	return result;
}

void write_joypad(u8 val)
{
	*joypad.joyp = (val & 0b11110000);
}

void press_key(keys_e key)
{
	if (joypad.keys_pressed[key] == 0)	// call joypad interrupt
	{
		u8 val = read8(IF);
		set_flag(&val, 4, 1);
		write8(IF, val);
	}
	joypad.keys_pressed[key] = 1;
}

void release_key(keys_e key)
{
	joypad.keys_pressed[key] = 0;
}

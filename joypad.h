#ifndef JOYPAD_H
# define JOYPAD_H

# include "types.h"
# include "memory.h"


typedef enum e_Key
{
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_A,
	KEY_B,
	KEY_START,
	KEY_SELECT,
	KEY_COUNT
} keys_e;

typedef struct {
	u8 p1;
	int keys_pressed[KEY_COUNT];
} joypad_t;

extern joypad_t joypad;

int init_joypad();

u8 read_joypad();

void press_key(keys_e key);

void release_key(keys_e key);

#endif

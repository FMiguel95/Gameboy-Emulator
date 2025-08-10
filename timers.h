#ifndef TIMERS_H
# define TIMERS_H

# include "types.h"
# include "memory.h"

typedef struct {
	u8* div;	// Divider register
	u8 div_counter;

	u8* tima;	// Timer counter
	

	u8* tma;	// Timer modulo
	

	u8* tac;	// Timer control
	

} timers_t;

extern timers_t timers;

int init_timers();

void timers_tick();

#endif

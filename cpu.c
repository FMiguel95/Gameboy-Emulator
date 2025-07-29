#include "cpu.h"

void cpu_tick()
{
	static void (*instruction_to_execute)();

	if (cpu.instruction_cycles_remain == 0)
		instruction_to_execute = instruction_set[cpu.reg16_PC++];

	instruction_to_execute();
}

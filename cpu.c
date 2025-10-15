#include "cpu.h"
cpu_t cpu;

int init_cpu()
{
	cpu = (cpu_t){0};
	cpu.reg8_A = ((u8*)(&cpu.reg16_AF)) + 1;
	cpu.reg8_F = (u8*)(&cpu.reg16_AF);
	cpu.reg8_B = ((u8*)(&cpu.reg16_BC)) + 1;
	cpu.reg8_C = (u8*)(&cpu.reg16_BC);
	cpu.reg8_D = ((u8*)(&cpu.reg16_DE)) + 1;
	cpu.reg8_E = (u8*)(&cpu.reg16_DE);
	cpu.reg8_H = ((u8*)(&cpu.reg16_HL)) + 1;
	cpu.reg8_L = (u8*)(&cpu.reg16_HL);
	cpu.reg8_S = ((u8*)(&cpu.reg16_SP)) + 1;
	cpu.reg8_P = (u8*)(&cpu.reg16_SP);

	// skip_boot_rom();

	return 1;
}

void skip_boot_rom()
{
	*cpu.reg8_A = 0x01;
	*cpu.reg8_F = 0xB0;
	*cpu.reg8_B = 0x00;
	*cpu.reg8_C = 0x13;
	*cpu.reg8_D = 0x00;
	*cpu.reg8_E = 0xD8;
	*cpu.reg8_H = 0x01;
	*cpu.reg8_L = 0x4D;
	cpu.reg16_SP = 0xFFFE;
	cpu.reg16_PC = 0x0100;
}

void cpu_log()
{
	// printf("A:%.2X F:%.2X B:%.2X C:%.2X D:%.2X E:%.2X H:%.2X L:%.2X SP:%.4X PC:%.4X PCMEM:%.2X,%.2X,%.2X,%.2X\n",
	// 	*cpu.reg8_A, *cpu.reg8_F, *cpu.reg8_B, *cpu.reg8_C, *cpu.reg8_D, *cpu.reg8_E, *cpu.reg8_H, *cpu.reg8_L, cpu.reg16_SP, cpu.reg16_PC,
	// 	read8(cpu.reg16_PC), read8(cpu.reg16_PC+1), read8(cpu.reg16_PC+2), read8(cpu.reg16_PC+3));

	printf("A:%3d F:%3d B:%3d C:%3d D:%3d E:%3d H:%3d L:%3d SP:%5d PC:%5d PCMEM:%5d,%5d,%5d,%5d, IE:%3d\n",
		*cpu.reg8_A, *cpu.reg8_F, *cpu.reg8_B, *cpu.reg8_C, *cpu.reg8_D, *cpu.reg8_E, *cpu.reg8_H, *cpu.reg8_L, cpu.reg16_SP, cpu.reg16_PC,
		read8(cpu.reg16_PC), read8(cpu.reg16_PC+1), read8(cpu.reg16_PC+2), read8(cpu.reg16_PC+3), *memory.ie_register);
}

void cpu_tick()
{
	// memory_t* mem = &memory;
	// cpu_t* ccpu = &cpu;

	if (cpu.halt_mode)
	{
		if (read8(IE) & read8(IF))
			cpu.halt_mode = 0;
		else
			return;
	}

	if (cpu.instruction_cycles_remain == 0)
	{
		// cpu_print_status();
		if (!cpu.prefix_instruction)
		{
			// cpu_log();
			u8 opcode = read8(cpu.reg16_PC++);
			cpu.instruction_to_execute = instruction_set[opcode];
			interrupt_handler();	// interrupt servicing happens after fetching the next opcode
		}
		else
		{
			cpu.instruction_to_execute = CB_set[read8(cpu.reg16_PC++)];
			cpu.prefix_instruction = 0;
		}

		if (cpu.IME_set_request == 1)	// EI instruction delay
			cpu.IME = 1;
		if (cpu.IME_set_request)
			cpu.IME_set_request--;
	}
	cpu.instruction_to_execute();

	*cpu.reg8_F &= 0b11110000;	// lower 4 bits of F can't be written
}

void interrupt_handler()
{
	interrupt_flag f = interrupt_pending();	// interrupt servicing happens after fetching the next opcode
	if (f == NoInterrupt)
		return;
	
	cpu.IME = 0;
	u8 IF_val = read8(IF);
	set_flag(&IF_val, f, 0);
	write8(IF, IF_val);
	
	switch (f)
	{
	case VBlank:
		cpu.instruction_to_execute = call_interrupt_VBlank;
		break;
		
	case LCD:
		cpu.instruction_to_execute = call_interrupt_LCD;
		break;
		
	case Timer:
		cpu.instruction_to_execute = call_interrupt_Timer;
		break;
		
	case Serial:
		cpu.instruction_to_execute = call_interrupt_Serial;
		break;
		
	case Joypad:
		cpu.instruction_to_execute = call_interrupt_Joypad;
		break;
	}
}

interrupt_flag interrupt_pending()
{
	if (!cpu.IME)
		return NoInterrupt;
	
	for (size_t i = 0; i < 8; i++)
	{
		if (get_flag(read8(IF), i) && get_flag(read8(IE), i))
			return i;
	}
	return NoInterrupt;
}

void call_interrupt(interrupt_flag flag)
{
	// printf("interrupt:%d\n", flag);
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 5;
	
	switch (cpu.instruction_cycles_remain)
	{
	case 5:
		cpu.reg16_PC--;
		break;

	case 4:
		cpu.reg16_SP--;
		break;

	case 3:
		write8(cpu.reg16_SP--, (u8)(cpu.reg16_PC >> 8));
		break;
	
	case 2:
		write8(cpu.reg16_SP, (u8)(cpu.reg16_PC & 0x00FF));
		break;

	case 1:
		switch (flag)
		{
		case VBlank:
			cpu.reg16_PC = 0x40;
			break;
			
		case LCD:
			cpu.reg16_PC = 0x48;
			break;
			
		case Timer:
			cpu.reg16_PC = 0x50;
			break;
			
		case Serial:
			cpu.reg16_PC = 0x58;
			break;
			
		case Joypad:
			cpu.reg16_PC = 0x60;
			break;
		}
		break;
	}
	cpu.instruction_cycles_remain--;
}
void call_interrupt_VBlank() { call_interrupt(VBlank); }
void call_interrupt_LCD() { call_interrupt(LCD); }
void call_interrupt_Timer() { call_interrupt(Timer); }
void call_interrupt_Serial() { call_interrupt(Serial); }
void call_interrupt_Joypad() { call_interrupt(Joypad); }

void cpu_print_status()
{
	printf("BC: 0x%.4X\n", cpu.reg16_BC);
	printf("DE: 0x%.4X\n", cpu.reg16_DE);
	printf("HL: 0x%.4X\n", cpu.reg16_HL);
	printf("A: 0x%.2X\n", *cpu.reg8_A);
	printf("flag z: %u\n", get_flag(*cpu.reg8_F, flag_z));
	printf("flag n: %u\n", get_flag(*cpu.reg8_F, flag_n));
	printf("flag h: %u\n", get_flag(*cpu.reg8_F, flag_h));
	printf("flag c: %u\n", get_flag(*cpu.reg8_F, flag_c));
	printf("PC: 0x%.4X\n", cpu.reg16_PC);
	printf("SP: 0x%.4X\n", cpu.reg16_SP);
	if (cpu.prefix_instruction)
		printf(">> CB %.2X\n", read8(cpu.reg16_PC));
	else
		printf(">> %s\n", opcode_decode[read8(cpu.reg16_PC)]);
	getchar();
}

int check_condition(condition cc)
{
	switch (cc)
	{
	case Z:
		return get_flag(*cpu.reg8_F, flag_z) == 1;
		break;
	
	case NZ:
		return get_flag(*cpu.reg8_F, flag_z) == 0;
		break;

	case C:
		return get_flag(*cpu.reg8_F, flag_c) == 1;
		break;

	case NC:
		return get_flag(*cpu.reg8_F, flag_c) == 0;
		break;
	}

	return 0;
}

void NOP() // 1 1
{
}
void opcode00() { NOP(); }

void HALT()
{
	cpu.halt_mode = 1;
	// printf("HALT\n");
}
void opcode76() { HALT(); }

void STOP() // 2 2
{
	printf("STOP\n");
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 2;
	
	switch (cpu.instruction_cycles_remain)
	{
	case 2:
		break;

	case 1:
		write8(DIV, 0); // reset DIV
		u8 _ = read8(cpu.reg16_PC++);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcode10() { STOP(); }

void LD_r8r8(u8* r1, u8* r2) // 1 1
{
	*r1 = *r2;
	// if (r1 == cpu.reg8_B && r2 == cpu.reg8_B)
	// {
	// 	cpu_log();
	// }
}
void opcode40() { LD_r8r8(cpu.reg8_B, cpu.reg8_B); }
void opcode41() { LD_r8r8(cpu.reg8_B, cpu.reg8_C); }
void opcode42() { LD_r8r8(cpu.reg8_B, cpu.reg8_D); }
void opcode43() { LD_r8r8(cpu.reg8_B, cpu.reg8_E); }
void opcode44() { LD_r8r8(cpu.reg8_B, cpu.reg8_H); }
void opcode45() { LD_r8r8(cpu.reg8_B, cpu.reg8_L); }
void opcode47() { LD_r8r8(cpu.reg8_B, cpu.reg8_A); }
void opcode48() { LD_r8r8(cpu.reg8_C, cpu.reg8_B); }
void opcode49() { LD_r8r8(cpu.reg8_C, cpu.reg8_C); }
void opcode4A() { LD_r8r8(cpu.reg8_C, cpu.reg8_D); }
void opcode4B() { LD_r8r8(cpu.reg8_C, cpu.reg8_E); }
void opcode4C() { LD_r8r8(cpu.reg8_C, cpu.reg8_H); }
void opcode4D() { LD_r8r8(cpu.reg8_C, cpu.reg8_L); }
void opcode4F() { LD_r8r8(cpu.reg8_C, cpu.reg8_A); }
void opcode50() { LD_r8r8(cpu.reg8_D, cpu.reg8_B); }
void opcode51() { LD_r8r8(cpu.reg8_D, cpu.reg8_C); }
void opcode52() { LD_r8r8(cpu.reg8_D, cpu.reg8_D); }
void opcode53() { LD_r8r8(cpu.reg8_D, cpu.reg8_E); }
void opcode54() { LD_r8r8(cpu.reg8_D, cpu.reg8_H); }
void opcode55() { LD_r8r8(cpu.reg8_D, cpu.reg8_L); }
void opcode57() { LD_r8r8(cpu.reg8_D, cpu.reg8_A); }
void opcode58() { LD_r8r8(cpu.reg8_E, cpu.reg8_B); }
void opcode59() { LD_r8r8(cpu.reg8_E, cpu.reg8_C); }
void opcode5A() { LD_r8r8(cpu.reg8_E, cpu.reg8_D); }
void opcode5B() { LD_r8r8(cpu.reg8_E, cpu.reg8_E); }
void opcode5C() { LD_r8r8(cpu.reg8_E, cpu.reg8_H); }
void opcode5D() { LD_r8r8(cpu.reg8_E, cpu.reg8_L); }
void opcode5F() { LD_r8r8(cpu.reg8_E, cpu.reg8_A); }
void opcode60() { LD_r8r8(cpu.reg8_H, cpu.reg8_B); }
void opcode61() { LD_r8r8(cpu.reg8_H, cpu.reg8_C); }
void opcode62() { LD_r8r8(cpu.reg8_H, cpu.reg8_D); }
void opcode63() { LD_r8r8(cpu.reg8_H, cpu.reg8_E); }
void opcode64() { LD_r8r8(cpu.reg8_H, cpu.reg8_H); }
void opcode65() { LD_r8r8(cpu.reg8_H, cpu.reg8_L); }
void opcode67() { LD_r8r8(cpu.reg8_H, cpu.reg8_A); }
void opcode68() { LD_r8r8(cpu.reg8_L, cpu.reg8_B); }
void opcode69() { LD_r8r8(cpu.reg8_L, cpu.reg8_C); }
void opcode6A() { LD_r8r8(cpu.reg8_L, cpu.reg8_D); }
void opcode6B() { LD_r8r8(cpu.reg8_L, cpu.reg8_E); }
void opcode6C() { LD_r8r8(cpu.reg8_L, cpu.reg8_H); }
void opcode6D() { LD_r8r8(cpu.reg8_L, cpu.reg8_L); }
void opcode6F() { LD_r8r8(cpu.reg8_L, cpu.reg8_A); }
void opcode78() { LD_r8r8(cpu.reg8_A, cpu.reg8_B); }
void opcode79() { LD_r8r8(cpu.reg8_A, cpu.reg8_C); }
void opcode7A() { LD_r8r8(cpu.reg8_A, cpu.reg8_D); }
void opcode7B() { LD_r8r8(cpu.reg8_A, cpu.reg8_E); }
void opcode7C() { LD_r8r8(cpu.reg8_A, cpu.reg8_H); }
void opcode7D() { LD_r8r8(cpu.reg8_A, cpu.reg8_L); }
void opcode7F() { LD_r8r8(cpu.reg8_A, cpu.reg8_A); }

void LD_r8n8(u8* r) // 2 2
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 2;
	
	static u8 n;
	switch (cpu.instruction_cycles_remain)
	{
	case 2:
		break;

	case 1:
		n = read8(cpu.reg16_PC++);
		*r = n;
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcode06() { LD_r8n8(cpu.reg8_B); }
void opcode0E() { LD_r8n8(cpu.reg8_C); }
void opcode16() { LD_r8n8(cpu.reg8_D); }
void opcode1E() { LD_r8n8(cpu.reg8_E); }
void opcode26() { LD_r8n8(cpu.reg8_H); }
void opcode2E() { LD_r8n8(cpu.reg8_L); }
void opcode3E() { LD_r8n8(cpu.reg8_A); }

void LD_r16n16(u16* r) // 3 3
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 3;
	
	static u16 n;
	switch (cpu.instruction_cycles_remain)
	{
	case 3:
		break;
	
	case 2:
		n = read8(cpu.reg16_PC++);
		break;
	
	case 1:
		n |= (u16)read8(cpu.reg16_PC++) << 8;
		*r = n;
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcode01() { LD_r16n16(&cpu.reg16_BC); }
void opcode11() { LD_r16n16(&cpu.reg16_DE); }
void opcode21() { LD_r16n16(&cpu.reg16_HL); }
void opcode31() { LD_r16n16(&cpu.reg16_SP); }

void LD_HLr8(u8* r) // 2 1
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 2;

	switch (cpu.instruction_cycles_remain)
	{
	case 2:
		break;

	case 1:
		write8(cpu.reg16_HL, *r);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcode70() { LD_HLr8(cpu.reg8_B); }
void opcode71() { LD_HLr8(cpu.reg8_C); }
void opcode72() { LD_HLr8(cpu.reg8_D); }
void opcode73() { LD_HLr8(cpu.reg8_E); }
void opcode74() { LD_HLr8(cpu.reg8_H); }
void opcode75() { LD_HLr8(cpu.reg8_L); }
void opcode77() { LD_HLr8(cpu.reg8_A); }

void LD_HLn8() // 3 2
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 3;

	static u8 n;
	switch (cpu.instruction_cycles_remain)
	{
	case 3:
		break;

	case 2:
		n = read8(cpu.reg16_PC++);
		break;

	case 1:
		write8(cpu.reg16_HL, n);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcode36() { LD_HLn8(); }

void LD_r8HL(u8* r) // 2 1
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 2;

	switch (cpu.instruction_cycles_remain)
	{
	case 2:
		break;

	case 1:
		*r = read8(cpu.reg16_HL);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcode46() { LD_r8HL(cpu.reg8_B); }
void opcode4E() { LD_r8HL(cpu.reg8_C); }
void opcode56() { LD_r8HL(cpu.reg8_D); }
void opcode5E() { LD_r8HL(cpu.reg8_E); }
void opcode66() { LD_r8HL(cpu.reg8_H); }
void opcode6E() { LD_r8HL(cpu.reg8_L); }
void opcode7E() { LD_r8HL(cpu.reg8_A); }

void LD_r16A(u16* r) // 2 1
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 2;

	switch (cpu.instruction_cycles_remain)
	{
	case 2:
		break;

	case 1:
		write8(*r, *cpu.reg8_A);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcode02() { LD_r16A(&cpu.reg16_BC); }
void opcode12() { LD_r16A(&cpu.reg16_DE); }

void LD_n16A() // 4 3
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 4;

	static u16 address;
	switch (cpu.instruction_cycles_remain)
	{
	case 4:
		break;

	case 3:
		address = read8(cpu.reg16_PC++);
		break;

	case 2:
		address |= ((u16)read8(cpu.reg16_PC++) << 8);
		break;

	case 1:
		write8(address, *cpu.reg8_A);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeEA() { LD_n16A(); }

void LDH_n8A() // 3 2
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 3;

	static u8 address;
	switch (cpu.instruction_cycles_remain)
	{
	case 3:
		break;

	case 2:
		address = read8(cpu.reg16_PC++);
		break;

	case 1:
		write8(0xFF00 + address, *cpu.reg8_A);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeE0() { LDH_n8A(); }

void LDH_CA() // 2 1
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 2;

	switch (cpu.instruction_cycles_remain)
	{
	case 2:
		break;

	case 1:
		write8(0xFF00 + *cpu.reg8_C, *cpu.reg8_A);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeE2() { LDH_CA(); }

void LD_Ar16(u16* r) // 2 1
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 2;

	switch (cpu.instruction_cycles_remain)
	{
	case 2:
		break;

	case 1:
		*cpu.reg8_A = read8(*r);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcode0A() { LD_Ar16(&cpu.reg16_BC); }
void opcode1A() { LD_Ar16(&cpu.reg16_DE); }

void LD_An16() // 4 3
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 4;

	static u16 address;
	switch (cpu.instruction_cycles_remain)
	{
	case 4:
		break;

	case 3:
		address = read8(cpu.reg16_PC++);
		break;

	case 2:
		address |= ((u16)read8(cpu.reg16_PC++) << 8);
		break;

	case 1:
		*cpu.reg8_A = read8(address);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeFA() { LD_An16(); }

void LDH_An8() // 3 2
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 3;

	static u8 address;
	switch (cpu.instruction_cycles_remain)
	{
	case 3:
		break;

	case 2:
		address = read8(cpu.reg16_PC++);
		break;

	case 1:
		*cpu.reg8_A = read8(0xFF00 + address);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeF0() { LDH_An8(); }

void LDH_AC() // 2 1
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 2;

	switch (cpu.instruction_cycles_remain)
	{
	case 2:
		break;

	case 1:
		*cpu.reg8_A = read8(0xFF00 + *cpu.reg8_C);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeF2() { LDH_AC(); }

void LD_HLIA() // 2 1
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 2;

	switch (cpu.instruction_cycles_remain)
	{
	case 2:
		break;

	case 1:
		write8(cpu.reg16_HL, *cpu.reg8_A);
		cpu.reg16_HL = cpu.reg16_HL + 1;
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcode22() { LD_HLIA(); }

void LD_HLDA() // 2 1
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 2;

	switch (cpu.instruction_cycles_remain)
	{
	case 2:
		break;

	case 1:
		write8(cpu.reg16_HL, *cpu.reg8_A);
		cpu.reg16_HL = cpu.reg16_HL - 1;
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcode32() { LD_HLDA(); }

void LD_AHLD() // 2 1
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 2;

	switch (cpu.instruction_cycles_remain)
	{
	case 2:
		break;

	case 1:
		*cpu.reg8_A = read8(cpu.reg16_HL);
		cpu.reg16_HL = cpu.reg16_HL - 1;
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcode3A() { LD_AHLD(); }

void LD_AHLI() // 2 1
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 2;

	switch (cpu.instruction_cycles_remain)
	{
	case 2:
		break;

	case 1:
		*cpu.reg8_A = read8(cpu.reg16_HL);
		cpu.reg16_HL = cpu.reg16_HL + 1;
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcode2A() { LD_AHLI(); }

void LD_n16SP() // 5 3
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 5;

	static u16 address;
	switch (cpu.instruction_cycles_remain)
	{
	case 5:
		break;

	case 4:
		address = read8(cpu.reg16_PC++);
		break;

	case 3:
		address |= ((u16)read8(cpu.reg16_PC++) << 8);
		break;

	case 2:
		write8(address, (u8)cpu.reg16_SP);
		break;

	case 1:
		write8(address + 1, (u8)(cpu.reg16_SP >> 8));
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcode08() { LD_n16SP(); }

void LD_SPHL() // 2 1
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 2;

	switch (cpu.instruction_cycles_remain)
	{
	case 2:
		*cpu.reg8_P = *cpu.reg8_L;
		break;

	case 1:
		// cpu.reg16_SP = cpu.reg16_HL;
		*cpu.reg8_S = *cpu.reg8_H;
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeF9() { LD_SPHL(); }

void LD_SPHLe8() // 3 2
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 3;

	static u16 res;
	switch (cpu.instruction_cycles_remain)
	{
	case 3:
		break;
	
	case 2:
		s8 n = read8(cpu.reg16_PC++);
		set_flag(cpu.reg8_F, flag_h, ((cpu.reg16_SP & 0xF) + (n & 0xF)) > 0xF);
		set_flag(cpu.reg8_F, flag_c, ((cpu.reg16_SP & 0xFF) + (n & 0xFF)) > 0xFF);
		res = cpu.reg16_SP + n;
		set_flag(cpu.reg8_F, flag_z, 0);
		set_flag(cpu.reg8_F, flag_n, 0);
		break;

	case 1:
		cpu.reg16_HL = res;
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeF8() { LD_SPHLe8(); }

void ADC_Ar8(u8* r) // 1 1
{
	set_flag(cpu.reg8_F, flag_h, ((*cpu.reg8_A & 0xF) + (*r & 0xF) + get_flag(*cpu.reg8_F, flag_c)) > 0xF);
	u16 res = *cpu.reg8_A + *r + get_flag(*cpu.reg8_F, flag_c);
	*cpu.reg8_A = (u8)res;
	set_flag(cpu.reg8_F, flag_z, *cpu.reg8_A == 0);
	set_flag(cpu.reg8_F, flag_n, 0);
	set_flag(cpu.reg8_F, flag_c, res > 0xFF);
}
void opcode88() { ADC_Ar8(cpu.reg8_B); }
void opcode89() { ADC_Ar8(cpu.reg8_C); }
void opcode8A() { ADC_Ar8(cpu.reg8_D); }
void opcode8B() { ADC_Ar8(cpu.reg8_E); }
void opcode8C() { ADC_Ar8(cpu.reg8_H); }
void opcode8D() { ADC_Ar8(cpu.reg8_L); }
void opcode8F() { ADC_Ar8(cpu.reg8_A); }

void ADC_AHL() // 2 1
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 2;

	switch (cpu.instruction_cycles_remain)
	{
	case 2:
		break;

	case 1:
		u8 val = read8(cpu.reg16_HL);
		set_flag(cpu.reg8_F, flag_h, ((*cpu.reg8_A & 0xF) + (val & 0xF) + get_flag(*cpu.reg8_F, flag_c)) > 0xF);
		u16 res = *cpu.reg8_A + val + get_flag(*cpu.reg8_F, flag_c);
		*cpu.reg8_A = (u8)res;
		set_flag(cpu.reg8_F, flag_z, *cpu.reg8_A == 0);
		set_flag(cpu.reg8_F, flag_n, 0);
		set_flag(cpu.reg8_F, flag_c, res > 0xFF);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcode8E() { ADC_AHL(); }

void ADC_An8() // 2 2
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 2;

	switch (cpu.instruction_cycles_remain)
	{
	case 2:
		break;

	case 1:
		u8 val = read8(cpu.reg16_PC++);
		set_flag(cpu.reg8_F, flag_h, ((*cpu.reg8_A & 0xF) + (val & 0xF) + get_flag(*cpu.reg8_F, flag_c)) > 0xF);
		u16 res = *cpu.reg8_A + val + get_flag(*cpu.reg8_F, flag_c);
		*cpu.reg8_A = (u8)res;
		set_flag(cpu.reg8_F, flag_z, *cpu.reg8_A == 0);
		set_flag(cpu.reg8_F, flag_n, 0);
		set_flag(cpu.reg8_F, flag_c, res > 0xFF);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeCE() { ADC_An8(); }

void ADD_Ar8(u8* r) // 1 1
{
	set_flag(cpu.reg8_F, flag_h, ((*cpu.reg8_A & 0xF) + (*r & 0xF)) > 0xF);
	u16 res = *cpu.reg8_A + *r;
	*cpu.reg8_A = (u8)res;
	set_flag(cpu.reg8_F, flag_z, *cpu.reg8_A == 0);
	set_flag(cpu.reg8_F, flag_n, 0);
	set_flag(cpu.reg8_F, flag_c, res > 0xFF);
}
void opcode80() { ADD_Ar8(cpu.reg8_B); }
void opcode81() { ADD_Ar8(cpu.reg8_C); }
void opcode82() { ADD_Ar8(cpu.reg8_D); }
void opcode83() { ADD_Ar8(cpu.reg8_E); }
void opcode84() { ADD_Ar8(cpu.reg8_H); }
void opcode85() { ADD_Ar8(cpu.reg8_L); }
void opcode87() { ADD_Ar8(cpu.reg8_A); }

void ADD_AHL() // 2 1
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 2;

	switch (cpu.instruction_cycles_remain)
	{
	case 2:
		break;

	case 1:
		u8 val = read8(cpu.reg16_HL);
		set_flag(cpu.reg8_F, flag_h, ((*cpu.reg8_A & 0xF) + (val & 0xF)) > 0xF);
		u16 res = *cpu.reg8_A + val;
		*cpu.reg8_A = (u8)res;
		set_flag(cpu.reg8_F, flag_z, *cpu.reg8_A == 0);
		set_flag(cpu.reg8_F, flag_n, 0);
		set_flag(cpu.reg8_F, flag_c, res > 0xFF);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcode86() { ADD_AHL(); }

void ADD_An8() // 2 2
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 2;

	switch (cpu.instruction_cycles_remain)
	{
	case 2:
		break;

	case 1:
		u8 val = read8(cpu.reg16_PC++);
		set_flag(cpu.reg8_F, flag_h, ((*cpu.reg8_A & 0xF) + (val & 0xF)) > 0xF);
		u16 res = *cpu.reg8_A + val;
		*cpu.reg8_A = (u8)res;
		set_flag(cpu.reg8_F, flag_z, *cpu.reg8_A == 0);
		set_flag(cpu.reg8_F, flag_n, 0);
		set_flag(cpu.reg8_F, flag_c, res > 0xFF);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeC6() { ADD_An8(); }

void ADD_HLr16(u8* h, u8* l) // 2 1
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 2;

	switch (cpu.instruction_cycles_remain)
	{
	case 2:
		break;

	case 1:
		u16 res = *cpu.reg8_L + *l;
		*cpu.reg8_L = (u8)res;
		set_flag(cpu.reg8_F, flag_c, res > 0xFF);
		set_flag(cpu.reg8_F, flag_h, ((*cpu.reg8_H & 0xF) + (*h & 0xF)) + get_flag(*cpu.reg8_F, flag_c) > 0xF);
		res = *cpu.reg8_H + *h + get_flag(*cpu.reg8_F, flag_c);
		*cpu.reg8_H = (u8)res;
		set_flag(cpu.reg8_F, flag_n, 0);
		set_flag(cpu.reg8_F, flag_c, res > 0xFF);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcode09() { ADD_HLr16(cpu.reg8_B, cpu.reg8_C); }
void opcode19() { ADD_HLr16(cpu.reg8_D, cpu.reg8_E); }
void opcode29() { ADD_HLr16(cpu.reg8_H, cpu.reg8_L); }
void opcode39() { ADD_HLr16(cpu.reg8_S, cpu.reg8_P); }

void ADD_SPe8() // 4 2
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 4;

	static s8 val;
	switch (cpu.instruction_cycles_remain)
	{
	case 4:
		break;

	case 3:
		val = (s8)read8(cpu.reg16_PC++);
		break;

	case 2:
		set_flag(cpu.reg8_F, flag_h, ((cpu.reg16_SP & 0xF) + (val & 0xF)) > 0xF);
		u16 res = (cpu.reg16_SP & 0xFF) + (val & 0xFF);
		set_flag(cpu.reg8_F, flag_c, res > 0xFF);
		set_flag(cpu.reg8_F, flag_z, 0);
		set_flag(cpu.reg8_F, flag_n, 0);
		break;

	case 1:
		cpu.reg16_SP += val;
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeE8() { ADD_SPe8(); }

void SBC_Ar8(u8* r) // 1 1
{
	u8 carry = get_flag(*cpu.reg8_F, flag_c);
	set_flag(cpu.reg8_F, flag_h, (*cpu.reg8_A & 0xF) < ((*r & 0xF) + carry));
	set_flag(cpu.reg8_F, flag_c, (int)*cpu.reg8_A - (int)*r - (int)carry < 0);
	*cpu.reg8_A = *cpu.reg8_A - *r - carry;
	set_flag(cpu.reg8_F, flag_z, *cpu.reg8_A == 0);
	set_flag(cpu.reg8_F, flag_n, 1);
}
void opcode98() { SBC_Ar8(cpu.reg8_B); }
void opcode99() { SBC_Ar8(cpu.reg8_C); }
void opcode9A() { SBC_Ar8(cpu.reg8_D); }
void opcode9B() { SBC_Ar8(cpu.reg8_E); }
void opcode9C() { SBC_Ar8(cpu.reg8_H); }
void opcode9D() { SBC_Ar8(cpu.reg8_L); }
void opcode9F() { SBC_Ar8(cpu.reg8_A); }

void SBC_AHL() // 2 1
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 2;

	switch (cpu.instruction_cycles_remain)
	{
	case 2:
		break;

	case 1:
		u8 val = read8(cpu.reg16_HL);
		u8 carry = get_flag(*cpu.reg8_F, flag_c);
		set_flag(cpu.reg8_F, flag_h, (*cpu.reg8_A & 0xF) < ((val & 0xF) + carry));
		set_flag(cpu.reg8_F, flag_c, (int)*cpu.reg8_A - (int)val - (int)carry < 0);
		*cpu.reg8_A = *cpu.reg8_A - val - carry;
		set_flag(cpu.reg8_F, flag_z, *cpu.reg8_A == 0);
		set_flag(cpu.reg8_F, flag_n, 1);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcode9E() { SBC_AHL(); }

void SBC_An8() // 2 2
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 2;

	switch (cpu.instruction_cycles_remain)
	{
	case 2:
		break;

	case 1:
		u8 val = read8(cpu.reg16_PC++);
		u8 carry = get_flag(*cpu.reg8_F, flag_c);
		set_flag(cpu.reg8_F, flag_h, (*cpu.reg8_A & 0xF) < ((val & 0xF) + carry));
		set_flag(cpu.reg8_F, flag_c, (int)*cpu.reg8_A - (int)val - (int)carry < 0);
		*cpu.reg8_A = *cpu.reg8_A - val - carry;
		set_flag(cpu.reg8_F, flag_z, *cpu.reg8_A == 0);
		set_flag(cpu.reg8_F, flag_n, 1);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeDE() { SBC_An8(); }

void SUB_Ar8(u8* r) // 1 1
{
	set_flag(cpu.reg8_F, flag_h, (*cpu.reg8_A & 0xF) < ((*r & 0xF)));
	set_flag(cpu.reg8_F, flag_c, (int)*cpu.reg8_A - (int)*r < 0);
	*cpu.reg8_A = *cpu.reg8_A - *r;
	set_flag(cpu.reg8_F, flag_z, *cpu.reg8_A == 0);
	set_flag(cpu.reg8_F, flag_n, 1);
}
void opcode90() { SUB_Ar8(cpu.reg8_B); }
void opcode91() { SUB_Ar8(cpu.reg8_C); }
void opcode92() { SUB_Ar8(cpu.reg8_D); }
void opcode93() { SUB_Ar8(cpu.reg8_E); }
void opcode94() { SUB_Ar8(cpu.reg8_H); }
void opcode95() { SUB_Ar8(cpu.reg8_L); }
void opcode97() { SUB_Ar8(cpu.reg8_A); }

void SUB_AHL() // 2 1
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 2;

	switch (cpu.instruction_cycles_remain)
	{
	case 2:
		break;

	case 1:
		u8 val = read8(cpu.reg16_HL);
		set_flag(cpu.reg8_F, flag_h, (*cpu.reg8_A & 0xF) < ((val & 0xF)));
		set_flag(cpu.reg8_F, flag_c, (int)*cpu.reg8_A - (int)val < 0);
		*cpu.reg8_A = *cpu.reg8_A - val;
		set_flag(cpu.reg8_F, flag_z, *cpu.reg8_A == 0);
		set_flag(cpu.reg8_F, flag_n, 1);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcode96() { SUB_AHL(); }

void SUB_An8() // 2 2
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 2;

	switch (cpu.instruction_cycles_remain)
	{
	case 2:
		break;

	case 1:
		u8 val = read8(cpu.reg16_PC++);
		set_flag(cpu.reg8_F, flag_h, (*cpu.reg8_A & 0xF) < ((val & 0xF)));
		set_flag(cpu.reg8_F, flag_c, (int)*cpu.reg8_A - (int)val < 0);
		*cpu.reg8_A = *cpu.reg8_A - val;
		set_flag(cpu.reg8_F, flag_z, *cpu.reg8_A == 0);
		set_flag(cpu.reg8_F, flag_n, 1);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeD6() { SUB_An8(); }

void CP_Ar8(u8* r) // 1 1
{
	set_flag(cpu.reg8_F, flag_h, (*cpu.reg8_A & 0xF) < ((*r & 0xF)));
	set_flag(cpu.reg8_F, flag_c, (int)*cpu.reg8_A - (int)*r < 0);
	set_flag(cpu.reg8_F, flag_z, *cpu.reg8_A == *r);
	set_flag(cpu.reg8_F, flag_n, 1);
}
void opcodeB8() { CP_Ar8(cpu.reg8_B); }
void opcodeB9() { CP_Ar8(cpu.reg8_C); }
void opcodeBA() { CP_Ar8(cpu.reg8_D); }
void opcodeBB() { CP_Ar8(cpu.reg8_E); }
void opcodeBC() { CP_Ar8(cpu.reg8_H); }
void opcodeBD() { CP_Ar8(cpu.reg8_L); }
void opcodeBF() { CP_Ar8(cpu.reg8_A); }

void CP_AHL() // 2 1
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 2;

	switch (cpu.instruction_cycles_remain)
	{
	case 2:
		break;

	case 1:
		u8 val = read8(cpu.reg16_HL);
		set_flag(cpu.reg8_F, flag_h, (*cpu.reg8_A & 0xF) < ((val & 0xF)));
		set_flag(cpu.reg8_F, flag_c, (int)*cpu.reg8_A - (int)val < 0);
		set_flag(cpu.reg8_F, flag_z, *cpu.reg8_A == val);
		set_flag(cpu.reg8_F, flag_n, 1);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeBE() { CP_AHL(); }

void CP_An8() // 2 2
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 2;

	switch (cpu.instruction_cycles_remain)
	{
	case 2:
		break;

	case 1:
		u8 val = read8(cpu.reg16_PC++);
		set_flag(cpu.reg8_F, flag_h, (*cpu.reg8_A & 0xF) < ((val & 0xF)));
		set_flag(cpu.reg8_F, flag_c, (int)*cpu.reg8_A - (int)val < 0);
		set_flag(cpu.reg8_F, flag_z, *cpu.reg8_A == val);
		set_flag(cpu.reg8_F, flag_n, 1);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeFE() { CP_An8(); }

void INC_r8(u8* r) // 1 1
{
	(*r)++;
	set_flag(cpu.reg8_F, flag_z, *r == 0);
	set_flag(cpu.reg8_F, flag_n, 0);
	set_flag(cpu.reg8_F, flag_h, (*r & 0xF) == 0);
}
void opcode04() { INC_r8(cpu.reg8_B); }
void opcode0C() { INC_r8(cpu.reg8_C); }
void opcode14() { INC_r8(cpu.reg8_D); }
void opcode1C() { INC_r8(cpu.reg8_E); }
void opcode24() { INC_r8(cpu.reg8_H); }
void opcode2C() { INC_r8(cpu.reg8_L); }
void opcode3C() { INC_r8(cpu.reg8_A); }

void INC_HL() // 3 1
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 3;

	static u8 val;
	switch (cpu.instruction_cycles_remain)
	{
	case 3:
		break;

	case 2:
		val = read8(cpu.reg16_HL);
		val++;
		break;

	case 1:
		write8(cpu.reg16_HL, val);
		set_flag(cpu.reg8_F, flag_z, val == 0);
		set_flag(cpu.reg8_F, flag_n, 0);
		set_flag(cpu.reg8_F, flag_h, (val & 0xF) == 0);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcode34() { INC_HL(); }

void INC_r16(u16* r) // 2 1
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 2;

	switch (cpu.instruction_cycles_remain)
	{
	case 2:
		break;

	case 1:
		(*r)++;
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcode03() { INC_r16(&cpu.reg16_BC); }
void opcode13() { INC_r16(&cpu.reg16_DE); }
void opcode23() { INC_r16(&cpu.reg16_HL); }
void opcode33() { INC_r16(&cpu.reg16_SP); }

void DEC_r8(u8* r) // 1 1
{
	(*r)--;
	set_flag(cpu.reg8_F, flag_z, *r == 0);
	set_flag(cpu.reg8_F, flag_n, 1);
	set_flag(cpu.reg8_F, flag_h, (*r & 0xF) == 0xF);
}
void opcode05() { DEC_r8(cpu.reg8_B); }
void opcode0D() { DEC_r8(cpu.reg8_C); }
void opcode15() { DEC_r8(cpu.reg8_D); }
void opcode1D() { DEC_r8(cpu.reg8_E); }
void opcode25() { DEC_r8(cpu.reg8_H); }
void opcode2D() { DEC_r8(cpu.reg8_L); }
void opcode3D() { DEC_r8(cpu.reg8_A); }

void DEC_HL() // 3 1
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 3;

	static u8 val;
	switch (cpu.instruction_cycles_remain)
	{
	case 3:
		break;

	case 2:
		val = read8(cpu.reg16_HL);
		val--;
		break;

	case 1:
		write8(cpu.reg16_HL, val);
		set_flag(cpu.reg8_F, flag_z, val == 0);
		set_flag(cpu.reg8_F, flag_n, 1);
		set_flag(cpu.reg8_F, flag_h, (val & 0xF) == 0xF);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcode35() { DEC_HL(); }

void DEC_r16(u16* r) // 2 1
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 2;

	switch (cpu.instruction_cycles_remain)
	{
	case 2:
		break;

	case 1:
		(*r)--;
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcode0B() { DEC_r16(&cpu.reg16_BC); }
void opcode1B() { DEC_r16(&cpu.reg16_DE); }
void opcode2B() { DEC_r16(&cpu.reg16_HL); }
void opcode3B() { DEC_r16(&cpu.reg16_SP); }

void AND_Ar8(u8* r) // 1 1
{
	*cpu.reg8_A = *cpu.reg8_A & *r;
	set_flag(cpu.reg8_F, flag_z, *cpu.reg8_A == 0);
	set_flag(cpu.reg8_F, flag_n, 0);
	set_flag(cpu.reg8_F, flag_h, 1);
	set_flag(cpu.reg8_F, flag_c, 0);
}
void opcodeA0() { AND_Ar8(cpu.reg8_B); }
void opcodeA1() { AND_Ar8(cpu.reg8_C); }
void opcodeA2() { AND_Ar8(cpu.reg8_D); }
void opcodeA3() { AND_Ar8(cpu.reg8_E); }
void opcodeA4() { AND_Ar8(cpu.reg8_H); }
void opcodeA5() { AND_Ar8(cpu.reg8_L); }
void opcodeA7() { AND_Ar8(cpu.reg8_A); }

void AND_AHL() // 2 1
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 2;

	switch (cpu.instruction_cycles_remain)
	{
	case 2:
		break;

	case 1:
		*cpu.reg8_A = *cpu.reg8_A & read8(cpu.reg16_HL);
		set_flag(cpu.reg8_F, flag_z, *cpu.reg8_A == 0);
		set_flag(cpu.reg8_F, flag_n, 0);
		set_flag(cpu.reg8_F, flag_h, 1);
		set_flag(cpu.reg8_F, flag_c, 0);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeA6() { AND_AHL(); }

void AND_An8() // 2 2
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 2;

	switch (cpu.instruction_cycles_remain)
	{
	case 2:
		break;

	case 1:
		*cpu.reg8_A = *cpu.reg8_A & read8(cpu.reg16_PC++);
		set_flag(cpu.reg8_F, flag_z, *cpu.reg8_A == 0);
		set_flag(cpu.reg8_F, flag_n, 0);
		set_flag(cpu.reg8_F, flag_h, 1);
		set_flag(cpu.reg8_F, flag_c, 0);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeE6() { AND_An8(); }

void CPL() // 1 1
{
	*cpu.reg8_A = ~*cpu.reg8_A;
	set_flag(cpu.reg8_F, flag_n, 1);
	set_flag(cpu.reg8_F, flag_h, 1);
}
void opcode2F() { CPL(); }

void OR_Ar8(u8* r) // 1 1
{
	*cpu.reg8_A = *cpu.reg8_A | *r;
	set_flag(cpu.reg8_F, flag_z, *cpu.reg8_A == 0);
	set_flag(cpu.reg8_F, flag_n, 0);
	set_flag(cpu.reg8_F, flag_h, 0);
	set_flag(cpu.reg8_F, flag_c, 0);
}
void opcodeB0() { OR_Ar8(cpu.reg8_B); }
void opcodeB1() { OR_Ar8(cpu.reg8_C); }
void opcodeB2() { OR_Ar8(cpu.reg8_D); }
void opcodeB3() { OR_Ar8(cpu.reg8_E); }
void opcodeB4() { OR_Ar8(cpu.reg8_H); }
void opcodeB5() { OR_Ar8(cpu.reg8_L); }
void opcodeB7() { OR_Ar8(cpu.reg8_A); }

void OR_AHL() // 2 1
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 2;

	switch (cpu.instruction_cycles_remain)
	{
	case 2:
		break;

	case 1:
		*cpu.reg8_A = *cpu.reg8_A | read8(cpu.reg16_HL);
		set_flag(cpu.reg8_F, flag_z, *cpu.reg8_A == 0);
		set_flag(cpu.reg8_F, flag_n, 0);
		set_flag(cpu.reg8_F, flag_h, 0);
		set_flag(cpu.reg8_F, flag_c, 0);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeB6() { OR_AHL(); }

void OR_An8() // 2 2
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 2;

	switch (cpu.instruction_cycles_remain)
	{
	case 2:
		break;

	case 1:
		*cpu.reg8_A = *cpu.reg8_A | read8(cpu.reg16_PC++);
		set_flag(cpu.reg8_F, flag_z, *cpu.reg8_A == 0);
		set_flag(cpu.reg8_F, flag_n, 0);
		set_flag(cpu.reg8_F, flag_h, 0);
		set_flag(cpu.reg8_F, flag_c, 0);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeF6() { OR_An8(); }

void XOR_Ar8(u8* r) // 1 1
{
	*cpu.reg8_A = *cpu.reg8_A ^ *r;
	set_flag(cpu.reg8_F, flag_z, *cpu.reg8_A == 0);
	set_flag(cpu.reg8_F, flag_n, 0);
	set_flag(cpu.reg8_F, flag_h, 0);
	set_flag(cpu.reg8_F, flag_c, 0);
}
void opcodeA8() { XOR_Ar8(cpu.reg8_B); }
void opcodeA9() { XOR_Ar8(cpu.reg8_C); }
void opcodeAA() { XOR_Ar8(cpu.reg8_D); }
void opcodeAB() { XOR_Ar8(cpu.reg8_E); }
void opcodeAC() { XOR_Ar8(cpu.reg8_H); }
void opcodeAD() { XOR_Ar8(cpu.reg8_L); }
void opcodeAF() { XOR_Ar8(cpu.reg8_A); }

void XOR_AHL() // 2 1
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 2;

	switch (cpu.instruction_cycles_remain)
	{
	case 2:
		break;

	case 1:
		*cpu.reg8_A = *cpu.reg8_A ^ read8(cpu.reg16_HL);
		set_flag(cpu.reg8_F, flag_z, *cpu.reg8_A == 0);
		set_flag(cpu.reg8_F, flag_n, 0);
		set_flag(cpu.reg8_F, flag_h, 0);
		set_flag(cpu.reg8_F, flag_c, 0);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeAE() { XOR_AHL(); }

void XOR_An8() // 2 2
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 2;

	switch (cpu.instruction_cycles_remain)
	{
	case 2:
		break;

	case 1:
		*cpu.reg8_A = *cpu.reg8_A ^ read8(cpu.reg16_PC++);
		set_flag(cpu.reg8_F, flag_z, *cpu.reg8_A == 0);
		set_flag(cpu.reg8_F, flag_n, 0);
		set_flag(cpu.reg8_F, flag_h, 0);
		set_flag(cpu.reg8_F, flag_c, 0);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeEE() { XOR_An8(); }

void CCF() // 1 1
{
	set_flag(cpu.reg8_F, flag_n, 0);
	set_flag(cpu.reg8_F, flag_h, 0);
	set_flag(cpu.reg8_F, flag_c, !get_flag(*cpu.reg8_F, flag_c));
}
void opcode3F() { CCF(); }

void SCF() // 1 1
{
	set_flag(cpu.reg8_F, flag_n, 0);
	set_flag(cpu.reg8_F, flag_h, 0);
	set_flag(cpu.reg8_F, flag_c, 1);
}
void opcode37() { SCF(); }

void EI() // 1 1
{
	cpu.IME_set_request = 2;
}
void opcodeFB() { EI(); }

void DI() // 1 1
{
	cpu.IME = 0;
	cpu.IME_set_request = 0;
}
void opcodeF3() { DI(); }

void POP_r16(u8* h, u8* l) // 3 2
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 3;

	switch (cpu.instruction_cycles_remain)
	{
	case 3:
		break;

	case 2:
		*l = read8(cpu.reg16_SP++);
		break;

	case 1:
		*h = read8(cpu.reg16_SP++);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeC1() { POP_r16(cpu.reg8_B, cpu.reg8_C); }
void opcodeD1() { POP_r16(cpu.reg8_D, cpu.reg8_E); }
void opcodeE1() { POP_r16(cpu.reg8_H, cpu.reg8_L); }
void opcodeF1() { POP_r16(cpu.reg8_A, cpu.reg8_F); }

void PUSH_r16(u8* h, u8* l) // 4 2
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 4;

	switch (cpu.instruction_cycles_remain)
	{
	case 4:
		break;

	case 3:
		cpu.reg16_SP--;
		break;
	
	case 2:
		write8(cpu.reg16_SP--, *h);
		break;

	case 1:
		write8(cpu.reg16_SP, *l);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeC5() { PUSH_r16(cpu.reg8_B, cpu.reg8_C); }
void opcodeD5() { PUSH_r16(cpu.reg8_D, cpu.reg8_E); }
void opcodeE5() { PUSH_r16(cpu.reg8_H, cpu.reg8_L); }
void opcodeF5() { PUSH_r16(cpu.reg8_A, cpu.reg8_F); }

void JP_HL() // 1 1
{
	cpu.reg16_PC = cpu.reg16_HL;
}
void opcodeE9() { JP_HL(); }

void JP_n16() // 4 3
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 4;

	static u16 n;
	switch (cpu.instruction_cycles_remain)
	{
	case 4:
		break;

	case 3:
		n = read8(cpu.reg16_PC++);
		break;
	
	case 2:
		n |= (u16)read8(cpu.reg16_PC++) << 8;
		cpu.reg16_PC = n;
		break;

	case 1:
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeC3() { JP_n16(); }

void JP_cce8(condition cc) // 4 3
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 4;

	static u16 n;
	switch (cpu.instruction_cycles_remain)
	{
	case 4:
		break;

	case 3:
		n = read8(cpu.reg16_PC++);
		break;
	
	case 2:
		n |= (u16)read8(cpu.reg16_PC++) << 8;
		if (!check_condition(cc))
			cpu.instruction_cycles_remain = 1;
		break;

	case 1:
		cpu.reg16_PC = n;
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeC2() { JP_cce8(NZ); }
void opcodeCA() { JP_cce8(Z); }
void opcodeD2() { JP_cce8(NC); }
void opcodeDA() { JP_cce8(C); }

void JR_n16() // 3 2
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 3;

	static s8 offset;
	switch (cpu.instruction_cycles_remain)
	{
	case 3:
		break;
	
	case 2:
		offset = (s8)read8(cpu.reg16_PC++);
		break;

	case 1:
		cpu.reg16_PC += offset;
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcode18() { JR_n16(); }

void JR_cce8(condition cc) // 3 2
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 3;

	static s8 offset;
	switch (cpu.instruction_cycles_remain)
	{

	case 3:
		break;
	
	case 2:
		offset = read8(cpu.reg16_PC++);
		if (!check_condition(cc))
			cpu.instruction_cycles_remain = 1;
		break;

	case 1:
		cpu.reg16_PC += offset;
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcode20() { JR_cce8(NZ); }
void opcode28() { JR_cce8(Z); }
void opcode30() { JR_cce8(NC); }
void opcode38() { JR_cce8(C); }

void CALL_n16() // 6 3
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 6;

	static u16 address;
	switch (cpu.instruction_cycles_remain)
	{
	case 6:
		break;

	case 5:
		address = read8(cpu.reg16_PC++);
		break;

	case 4:
		address |= ((u16)read8(cpu.reg16_PC++) << 8);
		break;

	case 3:
		cpu.reg16_SP--;
		break;
	
	case 2:
		write8(cpu.reg16_SP--, (u8)(cpu.reg16_PC >> 8));
		break;
	
	case 1:
		write8(cpu.reg16_SP, (u8)(cpu.reg16_PC & 0x00FF));
		cpu.reg16_PC = address;
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeCD() { CALL_n16(); }

void CALL_ccn16(condition cc) // 6 3
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 6;

	static u16 address;
	switch (cpu.instruction_cycles_remain)
	{
	case 6:
		break;

	case 5:
		address = read8(cpu.reg16_PC++);
		break;

	case 4:
		address |= ((u16)read8(cpu.reg16_PC++) << 8);
		break;
		
	case 3:
	if (!check_condition(cc))
		cpu.instruction_cycles_remain = 1;
		break;
	
	case 2:
		write8(--cpu.reg16_SP, (u8)(cpu.reg16_PC >> 8));
		break;
	
	case 1:
		write8(--cpu.reg16_SP, (u8)(cpu.reg16_PC & 0x00FF));
		cpu.reg16_PC = address;
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeC4() { CALL_ccn16(NZ); }
void opcodeCC() { CALL_ccn16(Z); }
void opcodeD4() { CALL_ccn16(NC); }
void opcodeDC() { CALL_ccn16(C); }

void RET() // 4 1
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 4;

	static u16 address;
	switch (cpu.instruction_cycles_remain)
	{
	case 4:
		break;

	case 3:
		address = read8(cpu.reg16_SP++);
		break;

	case 2:
		address |= ((u16)read8(cpu.reg16_SP++) << 8);
		break;

	case 1:
		cpu.reg16_PC = address;
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeC9() { RET(); }

void RET_cc(condition cc) // 5 1
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 5;

	static u16 address;
	switch (cpu.instruction_cycles_remain)
	{
	case 5:
		break;

	case 4:
		if (!check_condition(cc))
			cpu.instruction_cycles_remain = 1;
		break;

	case 3:
		address = read8(cpu.reg16_SP++);
		break;

	case 2:
		address |= ((u16)read8(cpu.reg16_SP++) << 8);
		break;

	case 1:
		cpu.reg16_PC = address;
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeC0() { RET_cc(NZ); }
void opcodeC8() { RET_cc(Z); }
void opcodeD0() { RET_cc(NC); }
void opcodeD8() { RET_cc(C); }

void RETI() // 4 1
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 4;

	static u16 address;
	switch (cpu.instruction_cycles_remain)
	{
	case 4:
		break;

	case 3:
		cpu.IME = 1;
		address = read8(cpu.reg16_SP++);
		break;

	case 2:
		address |= ((u16)read8(cpu.reg16_SP++) << 8);
		break;

	case 1:
		cpu.reg16_PC = address;
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeD9() { RETI(); }

void RST(u8 vec) // 4 1
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 4;

	switch (cpu.instruction_cycles_remain)
	{
	case 4:
		break;

	case 3:
		write8(--cpu.reg16_SP, (u8)(cpu.reg16_PC >> 8));
		break;
	
	case 2:
		write8(--cpu.reg16_SP, (u8)(cpu.reg16_PC & 0x00FF));
		break;

	case 1:
		cpu.reg16_PC = vec;
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeC7() { RST(0x00); }
void opcodeCF() { RST(0x08); }
void opcodeD7() { RST(0x10); }
void opcodeDF() { RST(0x18); }
void opcodeE7() { RST(0x20); }
void opcodeEF() { RST(0x28); }
void opcodeF7() { RST(0x30); }
void opcodeFF() { RST(0x38); }

void DDA() // 1 1
{
	if (!get_flag(*cpu.reg8_F, flag_n))
	{
		if (get_flag(*cpu.reg8_F, flag_c) || *cpu.reg8_A > 0x99)
		{
			(*cpu.reg8_A) += 0x60;
			set_flag(cpu.reg8_F, flag_c, 1);
		}
		if (get_flag(*cpu.reg8_F, flag_h) || (*cpu.reg8_A & 0x0f) > 0x09)
			(*cpu.reg8_A) += 0x6;
	}
	else
	{
		if (get_flag(*cpu.reg8_F, flag_c))
			(*cpu.reg8_A) -= 0x60;
		if (get_flag(*cpu.reg8_F, flag_h))
			(*cpu.reg8_A) -= 0x6;
	}
	set_flag(cpu.reg8_F, flag_z, *cpu.reg8_A == 0);
	set_flag(cpu.reg8_F, flag_h, 0);
}
void opcode27() { DDA(); }

void RLCA() // 1 1
{
	u8 bit7 = (*cpu.reg8_A & 0b10000000) >> 7;
	*cpu.reg8_A = (*cpu.reg8_A << 1) | bit7;
	set_flag(cpu.reg8_F, flag_z, 0);
	set_flag(cpu.reg8_F, flag_n, 0);
	set_flag(cpu.reg8_F, flag_h, 0);
	set_flag(cpu.reg8_F, flag_c, bit7);
}
void opcode07() { RLCA(); }

void RRCA() // 1 1
{
	u8 bit0 = *cpu.reg8_A & 0b00000001;
	*cpu.reg8_A = (bit0 << 7) | (*cpu.reg8_A >> 1);
	set_flag(cpu.reg8_F, flag_z, 0);
	set_flag(cpu.reg8_F, flag_n, 0);
	set_flag(cpu.reg8_F, flag_h, 0);
	set_flag(cpu.reg8_F, flag_c, bit0);
}
void opcode0F() { RRCA(); }

void RLA() // 1 1
{
	u8 bit7 = (*cpu.reg8_A & 0b10000000) >> 7;
	*cpu.reg8_A = (*cpu.reg8_A << 1) | get_flag(*cpu.reg8_F, flag_c);
	set_flag(cpu.reg8_F, flag_z, 0);
	set_flag(cpu.reg8_F, flag_n, 0);
	set_flag(cpu.reg8_F, flag_h, 0);
	set_flag(cpu.reg8_F, flag_c, bit7);
}
void opcode17() { RLA(); }

void RRA() // 1 1
{
	u8 bit0 = *cpu.reg8_A & 0b00000001;
	*cpu.reg8_A = (get_flag(*cpu.reg8_F, flag_c) << 7) | (*cpu.reg8_A >> 1);
	set_flag(cpu.reg8_F, flag_z, 0);
	set_flag(cpu.reg8_F, flag_n, 0);
	set_flag(cpu.reg8_F, flag_h, 0);
	set_flag(cpu.reg8_F, flag_c, bit0);
}
void opcode1F() { RRA(); }

void PREFIX() // 1 1
{
	cpu.prefix_instruction = 1;
}
void opcodeCB() { PREFIX(); }

void hole(char* str)
{
	printf("opcode hole %s\n", str);
	// while (1)
	// 	sleep(1000);
}
void opcodeD3() { hole("D3"); }
void opcodeDB() { hole("DB"); }
void opcodeDD() { hole("DD"); }
void opcodeE3() { hole("E3"); }
void opcodeE4() { hole("E4"); }
void opcodeEB() { hole("EB"); }
void opcodeEC() { hole("EC"); }
void opcodeED() { hole("ED"); }
void opcodeF4() { hole("F4"); }
void opcodeFC() { hole("FC"); }
void opcodeFD() { hole("FD"); }

void BIT_u3r8(u8 bit, u8* r) // 2 2
{
	set_flag(cpu.reg8_F, flag_z, ((*r >> bit) & 1) == 0);
	set_flag(cpu.reg8_F, flag_n, 0);
	set_flag(cpu.reg8_F, flag_h, 1);
}
void opcodeCB40() { BIT_u3r8(0, cpu.reg8_B); }
void opcodeCB41() { BIT_u3r8(0, cpu.reg8_C); }
void opcodeCB42() { BIT_u3r8(0, cpu.reg8_D); }
void opcodeCB43() { BIT_u3r8(0, cpu.reg8_E); }
void opcodeCB44() { BIT_u3r8(0, cpu.reg8_H); }
void opcodeCB45() { BIT_u3r8(0, cpu.reg8_L); }
void opcodeCB47() { BIT_u3r8(0, cpu.reg8_A); }
void opcodeCB48() { BIT_u3r8(1, cpu.reg8_B); }
void opcodeCB49() { BIT_u3r8(1, cpu.reg8_C); }
void opcodeCB4A() { BIT_u3r8(1, cpu.reg8_D); }
void opcodeCB4B() { BIT_u3r8(1, cpu.reg8_E); }
void opcodeCB4C() { BIT_u3r8(1, cpu.reg8_H); }
void opcodeCB4D() { BIT_u3r8(1, cpu.reg8_L); }
void opcodeCB4F() { BIT_u3r8(1, cpu.reg8_A); }
void opcodeCB50() { BIT_u3r8(2, cpu.reg8_B); }
void opcodeCB51() { BIT_u3r8(2, cpu.reg8_C); }
void opcodeCB52() { BIT_u3r8(2, cpu.reg8_D); }
void opcodeCB53() { BIT_u3r8(2, cpu.reg8_E); }
void opcodeCB54() { BIT_u3r8(2, cpu.reg8_H); }
void opcodeCB55() { BIT_u3r8(2, cpu.reg8_L); }
void opcodeCB57() { BIT_u3r8(2, cpu.reg8_A); }
void opcodeCB58() { BIT_u3r8(3, cpu.reg8_B); }
void opcodeCB59() { BIT_u3r8(3, cpu.reg8_C); }
void opcodeCB5A() { BIT_u3r8(3, cpu.reg8_D); }
void opcodeCB5B() { BIT_u3r8(3, cpu.reg8_E); }
void opcodeCB5C() { BIT_u3r8(3, cpu.reg8_H); }
void opcodeCB5D() { BIT_u3r8(3, cpu.reg8_L); }
void opcodeCB5F() { BIT_u3r8(3, cpu.reg8_A); }
void opcodeCB60() { BIT_u3r8(4, cpu.reg8_B); }
void opcodeCB61() { BIT_u3r8(4, cpu.reg8_C); }
void opcodeCB62() { BIT_u3r8(4, cpu.reg8_D); }
void opcodeCB63() { BIT_u3r8(4, cpu.reg8_E); }
void opcodeCB64() { BIT_u3r8(4, cpu.reg8_H); }
void opcodeCB65() { BIT_u3r8(4, cpu.reg8_L); }
void opcodeCB67() { BIT_u3r8(4, cpu.reg8_A); }
void opcodeCB68() { BIT_u3r8(5, cpu.reg8_B); }
void opcodeCB69() { BIT_u3r8(5, cpu.reg8_C); }
void opcodeCB6A() { BIT_u3r8(5, cpu.reg8_D); }
void opcodeCB6B() { BIT_u3r8(5, cpu.reg8_E); }
void opcodeCB6C() { BIT_u3r8(5, cpu.reg8_H); }
void opcodeCB6D() { BIT_u3r8(5, cpu.reg8_L); }
void opcodeCB6F() { BIT_u3r8(5, cpu.reg8_A); }
void opcodeCB70() { BIT_u3r8(6, cpu.reg8_B); }
void opcodeCB71() { BIT_u3r8(6, cpu.reg8_C); }
void opcodeCB72() { BIT_u3r8(6, cpu.reg8_D); }
void opcodeCB73() { BIT_u3r8(6, cpu.reg8_E); }
void opcodeCB74() { BIT_u3r8(6, cpu.reg8_H); }
void opcodeCB75() { BIT_u3r8(6, cpu.reg8_L); }
void opcodeCB77() { BIT_u3r8(6, cpu.reg8_A); }
void opcodeCB78() { BIT_u3r8(7, cpu.reg8_B); }
void opcodeCB79() { BIT_u3r8(7, cpu.reg8_C); }
void opcodeCB7A() { BIT_u3r8(7, cpu.reg8_D); }
void opcodeCB7B() { BIT_u3r8(7, cpu.reg8_E); }
void opcodeCB7C() { BIT_u3r8(7, cpu.reg8_H); }
void opcodeCB7D() { BIT_u3r8(7, cpu.reg8_L); }
void opcodeCB7F() { BIT_u3r8(7, cpu.reg8_A); }

void BIT_u3HL(u8 bit) // 3 2
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 2;

	static u8 val;
	switch (cpu.instruction_cycles_remain)
	{
	case 2:
		break;
	
	case 1:
		val = read8(cpu.reg16_HL);
		set_flag(cpu.reg8_F, flag_z, ((val >> bit) & 1) == 0);
		set_flag(cpu.reg8_F, flag_n, 0);
		set_flag(cpu.reg8_F, flag_h, 1);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeCB46() { BIT_u3HL(0); }
void opcodeCB4E() { BIT_u3HL(1); }
void opcodeCB56() { BIT_u3HL(2); }
void opcodeCB5E() { BIT_u3HL(3); }
void opcodeCB66() { BIT_u3HL(4); }
void opcodeCB6E() { BIT_u3HL(5); }
void opcodeCB76() { BIT_u3HL(6); }
void opcodeCB7E() { BIT_u3HL(7); }

void RES_u3r8(u8 bit, u8* r) // 2 2
{
	u8 mask = 1 << bit;
	*r = *r & ~mask;
}
void opcodeCB80() { RES_u3r8(0, cpu.reg8_B); }
void opcodeCB81() { RES_u3r8(0, cpu.reg8_C); }
void opcodeCB82() { RES_u3r8(0, cpu.reg8_D); }
void opcodeCB83() { RES_u3r8(0, cpu.reg8_E); }
void opcodeCB84() { RES_u3r8(0, cpu.reg8_H); }
void opcodeCB85() { RES_u3r8(0, cpu.reg8_L); }
void opcodeCB87() { RES_u3r8(0, cpu.reg8_A); }
void opcodeCB88() { RES_u3r8(1, cpu.reg8_B); }
void opcodeCB89() { RES_u3r8(1, cpu.reg8_C); }
void opcodeCB8A() { RES_u3r8(1, cpu.reg8_D); }
void opcodeCB8B() { RES_u3r8(1, cpu.reg8_E); }
void opcodeCB8C() { RES_u3r8(1, cpu.reg8_H); }
void opcodeCB8D() { RES_u3r8(1, cpu.reg8_L); }
void opcodeCB8F() { RES_u3r8(1, cpu.reg8_A); }
void opcodeCB90() { RES_u3r8(2, cpu.reg8_B); }
void opcodeCB91() { RES_u3r8(2, cpu.reg8_C); }
void opcodeCB92() { RES_u3r8(2, cpu.reg8_D); }
void opcodeCB93() { RES_u3r8(2, cpu.reg8_E); }
void opcodeCB94() { RES_u3r8(2, cpu.reg8_H); }
void opcodeCB95() { RES_u3r8(2, cpu.reg8_L); }
void opcodeCB97() { RES_u3r8(2, cpu.reg8_A); }
void opcodeCB98() { RES_u3r8(3, cpu.reg8_B); }
void opcodeCB99() { RES_u3r8(3, cpu.reg8_C); }
void opcodeCB9A() { RES_u3r8(3, cpu.reg8_D); }
void opcodeCB9B() { RES_u3r8(3, cpu.reg8_E); }
void opcodeCB9C() { RES_u3r8(3, cpu.reg8_H); }
void opcodeCB9D() { RES_u3r8(3, cpu.reg8_L); }
void opcodeCB9F() { RES_u3r8(3, cpu.reg8_A); }
void opcodeCBA0() { RES_u3r8(4, cpu.reg8_B); }
void opcodeCBA1() { RES_u3r8(4, cpu.reg8_C); }
void opcodeCBA2() { RES_u3r8(4, cpu.reg8_D); }
void opcodeCBA3() { RES_u3r8(4, cpu.reg8_E); }
void opcodeCBA4() { RES_u3r8(4, cpu.reg8_H); }
void opcodeCBA5() { RES_u3r8(4, cpu.reg8_L); }
void opcodeCBA7() { RES_u3r8(4, cpu.reg8_A); }
void opcodeCBA8() { RES_u3r8(5, cpu.reg8_B); }
void opcodeCBA9() { RES_u3r8(5, cpu.reg8_C); }
void opcodeCBAA() { RES_u3r8(5, cpu.reg8_D); }
void opcodeCBAB() { RES_u3r8(5, cpu.reg8_E); }
void opcodeCBAC() { RES_u3r8(5, cpu.reg8_H); }
void opcodeCBAD() { RES_u3r8(5, cpu.reg8_L); }
void opcodeCBAF() { RES_u3r8(5, cpu.reg8_A); }
void opcodeCBB0() { RES_u3r8(6, cpu.reg8_B); }
void opcodeCBB1() { RES_u3r8(6, cpu.reg8_C); }
void opcodeCBB2() { RES_u3r8(6, cpu.reg8_D); }
void opcodeCBB3() { RES_u3r8(6, cpu.reg8_E); }
void opcodeCBB4() { RES_u3r8(6, cpu.reg8_H); }
void opcodeCBB5() { RES_u3r8(6, cpu.reg8_L); }
void opcodeCBB7() { RES_u3r8(6, cpu.reg8_A); }
void opcodeCBB8() { RES_u3r8(7, cpu.reg8_B); }
void opcodeCBB9() { RES_u3r8(7, cpu.reg8_C); }
void opcodeCBBA() { RES_u3r8(7, cpu.reg8_D); }
void opcodeCBBB() { RES_u3r8(7, cpu.reg8_E); }
void opcodeCBBC() { RES_u3r8(7, cpu.reg8_H); }
void opcodeCBBD() { RES_u3r8(7, cpu.reg8_L); }
void opcodeCBBF() { RES_u3r8(7, cpu.reg8_A); }

void RES_u3HL(u8 bit) // 4 2
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 3;

	static u8 val;
	switch (cpu.instruction_cycles_remain)
	{
	case 3:
		break;
	
	case 2:
		val = read8(cpu.reg16_HL);
		u8 mask = 1 << bit;
		val= val & ~mask;
		break;

	case 1:
		write8(cpu.reg16_HL, val);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeCB86() { RES_u3HL(0); }
void opcodeCB8E() { RES_u3HL(1); }
void opcodeCB96() { RES_u3HL(2); }
void opcodeCB9E() { RES_u3HL(3); }
void opcodeCBA6() { RES_u3HL(4); }
void opcodeCBAE() { RES_u3HL(5); }
void opcodeCBB6() { RES_u3HL(6); }
void opcodeCBBE() { RES_u3HL(7); }

void SET_u3r8(u8 bit, u8* r) // 2 2
{
	u8 mask = 1 << bit;
	*r = *r | mask;
}
void opcodeCBC0() { SET_u3r8(0, cpu.reg8_B); }
void opcodeCBC1() { SET_u3r8(0, cpu.reg8_C); }
void opcodeCBC2() { SET_u3r8(0, cpu.reg8_D); }
void opcodeCBC3() { SET_u3r8(0, cpu.reg8_E); }
void opcodeCBC4() { SET_u3r8(0, cpu.reg8_H); }
void opcodeCBC5() { SET_u3r8(0, cpu.reg8_L); }
void opcodeCBC7() { SET_u3r8(0, cpu.reg8_A); }
void opcodeCBC8() { SET_u3r8(1, cpu.reg8_B); }
void opcodeCBC9() { SET_u3r8(1, cpu.reg8_C); }
void opcodeCBCA() { SET_u3r8(1, cpu.reg8_D); }
void opcodeCBCB() { SET_u3r8(1, cpu.reg8_E); }
void opcodeCBCC() { SET_u3r8(1, cpu.reg8_H); }
void opcodeCBCD() { SET_u3r8(1, cpu.reg8_L); }
void opcodeCBCF() { SET_u3r8(1, cpu.reg8_A); }
void opcodeCBD0() { SET_u3r8(2, cpu.reg8_B); }
void opcodeCBD1() { SET_u3r8(2, cpu.reg8_C); }
void opcodeCBD2() { SET_u3r8(2, cpu.reg8_D); }
void opcodeCBD3() { SET_u3r8(2, cpu.reg8_E); }
void opcodeCBD4() { SET_u3r8(2, cpu.reg8_H); }
void opcodeCBD5() { SET_u3r8(2, cpu.reg8_L); }
void opcodeCBD7() { SET_u3r8(2, cpu.reg8_A); }
void opcodeCBD8() { SET_u3r8(3, cpu.reg8_B); }
void opcodeCBD9() { SET_u3r8(3, cpu.reg8_C); }
void opcodeCBDA() { SET_u3r8(3, cpu.reg8_D); }
void opcodeCBDB() { SET_u3r8(3, cpu.reg8_E); }
void opcodeCBDC() { SET_u3r8(3, cpu.reg8_H); }
void opcodeCBDD() { SET_u3r8(3, cpu.reg8_L); }
void opcodeCBDF() { SET_u3r8(3, cpu.reg8_A); }
void opcodeCBE0() { SET_u3r8(4, cpu.reg8_B); }
void opcodeCBE1() { SET_u3r8(4, cpu.reg8_C); }
void opcodeCBE2() { SET_u3r8(4, cpu.reg8_D); }
void opcodeCBE3() { SET_u3r8(4, cpu.reg8_E); }
void opcodeCBE4() { SET_u3r8(4, cpu.reg8_H); }
void opcodeCBE5() { SET_u3r8(4, cpu.reg8_L); }
void opcodeCBE7() { SET_u3r8(4, cpu.reg8_A); }
void opcodeCBE8() { SET_u3r8(5, cpu.reg8_B); }
void opcodeCBE9() { SET_u3r8(5, cpu.reg8_C); }
void opcodeCBEA() { SET_u3r8(5, cpu.reg8_D); }
void opcodeCBEB() { SET_u3r8(5, cpu.reg8_E); }
void opcodeCBEC() { SET_u3r8(5, cpu.reg8_H); }
void opcodeCBED() { SET_u3r8(5, cpu.reg8_L); }
void opcodeCBEF() { SET_u3r8(5, cpu.reg8_A); }
void opcodeCBF0() { SET_u3r8(6, cpu.reg8_B); }
void opcodeCBF1() { SET_u3r8(6, cpu.reg8_C); }
void opcodeCBF2() { SET_u3r8(6, cpu.reg8_D); }
void opcodeCBF3() { SET_u3r8(6, cpu.reg8_E); }
void opcodeCBF4() { SET_u3r8(6, cpu.reg8_H); }
void opcodeCBF5() { SET_u3r8(6, cpu.reg8_L); }
void opcodeCBF7() { SET_u3r8(6, cpu.reg8_A); }
void opcodeCBF8() { SET_u3r8(7, cpu.reg8_B); }
void opcodeCBF9() { SET_u3r8(7, cpu.reg8_C); }
void opcodeCBFA() { SET_u3r8(7, cpu.reg8_D); }
void opcodeCBFB() { SET_u3r8(7, cpu.reg8_E); }
void opcodeCBFC() { SET_u3r8(7, cpu.reg8_H); }
void opcodeCBFD() { SET_u3r8(7, cpu.reg8_L); }
void opcodeCBFF() { SET_u3r8(7, cpu.reg8_A); }

void SET_u3HL(u8 bit) // 4 2
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 3;

	static u8 val;
	switch (cpu.instruction_cycles_remain)
	{
	case 3:
		break;
	
	case 2:
		val = read8(cpu.reg16_HL);
		u8 mask = 1 << bit;
		val = val | mask;
		break;

	case 1:
		write8(cpu.reg16_HL, val);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeCBC6() { SET_u3HL(0); }
void opcodeCBCE() { SET_u3HL(1); }
void opcodeCBD6() { SET_u3HL(2); }
void opcodeCBDE() { SET_u3HL(3); }
void opcodeCBE6() { SET_u3HL(4); }
void opcodeCBEE() { SET_u3HL(5); }
void opcodeCBF6() { SET_u3HL(6); }
void opcodeCBFE() { SET_u3HL(7); }

void RL_r8(u8* r) // 2 2
{
	u8 bit7 = (*r & 0b10000000) >> 7;
	*r = (*r << 1) | get_flag(*cpu.reg8_F, flag_c);
	set_flag(cpu.reg8_F, flag_z, *r == 0);
	set_flag(cpu.reg8_F, flag_n, 0);
	set_flag(cpu.reg8_F, flag_h, 0);
	set_flag(cpu.reg8_F, flag_c, bit7);
}
void opcodeCB10() { RL_r8(cpu.reg8_B); }
void opcodeCB11() { RL_r8(cpu.reg8_C); }
void opcodeCB12() { RL_r8(cpu.reg8_D); }
void opcodeCB13() { RL_r8(cpu.reg8_E); }
void opcodeCB14() { RL_r8(cpu.reg8_H); }
void opcodeCB15() { RL_r8(cpu.reg8_L); }
void opcodeCB17() { RL_r8(cpu.reg8_A); }

void RL_HL() // 4 2
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 3;

	static u8 val;
	switch (cpu.instruction_cycles_remain)
	{
	case 3:
		break;
	
	case 2:
		val = read8(cpu.reg16_HL);
		u8 bit7 = (val & 0b10000000) >> 7;
		val = (val << 1) | get_flag(*cpu.reg8_F, flag_c);
		set_flag(cpu.reg8_F, flag_z, val == 0);
		set_flag(cpu.reg8_F, flag_n, 0);
		set_flag(cpu.reg8_F, flag_h, 0);
		set_flag(cpu.reg8_F, flag_c, bit7);
		break;

	case 1:
		write8(cpu.reg16_HL, val);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeCB16() { RL_HL(); }

void RR_r8(u8* r) // 2 2
{
	u8 bit0 = *r & 0b00000001;
	*r = (get_flag(*cpu.reg8_F, flag_c) << 7) | (*r >> 1);
	set_flag(cpu.reg8_F, flag_z, *r == 0);
	set_flag(cpu.reg8_F, flag_n, 0);
	set_flag(cpu.reg8_F, flag_h, 0);
	set_flag(cpu.reg8_F, flag_c, bit0);
}
void opcodeCB18() { RR_r8(cpu.reg8_B); }
void opcodeCB19() { RR_r8(cpu.reg8_C); }
void opcodeCB1A() { RR_r8(cpu.reg8_D); }
void opcodeCB1B() { RR_r8(cpu.reg8_E); }
void opcodeCB1C() { RR_r8(cpu.reg8_H); }
void opcodeCB1D() { RR_r8(cpu.reg8_L); }
void opcodeCB1F() { RR_r8(cpu.reg8_A); }

void RR_HL() // 4 2
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 3;

	static u8 val;
	switch (cpu.instruction_cycles_remain)
	{
	case 3:
		break;
	
	case 2:
		val = read8(cpu.reg16_HL);
		u8 bit0 = val & 0b00000001;
		val = (get_flag(*cpu.reg8_F, flag_c) << 7) | (val >> 1);
		set_flag(cpu.reg8_F, flag_z, val == 0);
		set_flag(cpu.reg8_F, flag_n, 0);
		set_flag(cpu.reg8_F, flag_h, 0);
		set_flag(cpu.reg8_F, flag_c, bit0);
		break;

	case 1:
		write8(cpu.reg16_HL, val);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeCB1E() { RR_HL(); }

void RLC_r8(u8* r) // 1 1
{
	u8 bit7 = (*r & 0b10000000) >> 7;
	*r = (*r << 1) | bit7;
	set_flag(cpu.reg8_F, flag_z, *r == 0);
	set_flag(cpu.reg8_F, flag_n, 0);
	set_flag(cpu.reg8_F, flag_h, 0);
	set_flag(cpu.reg8_F, flag_c, bit7);
}
void opcodeCB00() { RLC_r8(cpu.reg8_B); }
void opcodeCB01() { RLC_r8(cpu.reg8_C); }
void opcodeCB02() { RLC_r8(cpu.reg8_D); }
void opcodeCB03() { RLC_r8(cpu.reg8_E); }
void opcodeCB04() { RLC_r8(cpu.reg8_H); }
void opcodeCB05() { RLC_r8(cpu.reg8_L); }
void opcodeCB07() { RLC_r8(cpu.reg8_A); }

void RLC_HL() // 4 2
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 3;

	static u8 val;
	switch (cpu.instruction_cycles_remain)
	{
	case 3:
		break;
	
	case 2:
		val = read8(cpu.reg16_HL);
		u8 bit7 = (val & 0b10000000) >> 7;
		val = (val << 1) | bit7;
		set_flag(cpu.reg8_F, flag_z, val == 0);
		set_flag(cpu.reg8_F, flag_n, 0);
		set_flag(cpu.reg8_F, flag_h, 0);
		set_flag(cpu.reg8_F, flag_c, bit7);
		break;

	case 1:
		write8(cpu.reg16_HL, val);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeCB06() { RLC_HL(); }

void RRC_r8(u8* r) // 2 2
{
	u8 bit0 = *r & 0b00000001;
	*r = (bit0 << 7) | (*r >> 1);
	set_flag(cpu.reg8_F, flag_z, *r == 0);
	set_flag(cpu.reg8_F, flag_n, 0);
	set_flag(cpu.reg8_F, flag_h, 0);
	set_flag(cpu.reg8_F, flag_c, bit0);
}
void opcodeCB08() { RRC_r8(cpu.reg8_B); }
void opcodeCB09() { RRC_r8(cpu.reg8_C); }
void opcodeCB0A() { RRC_r8(cpu.reg8_D); }
void opcodeCB0B() { RRC_r8(cpu.reg8_E); }
void opcodeCB0C() { RRC_r8(cpu.reg8_H); }
void opcodeCB0D() { RRC_r8(cpu.reg8_L); }
void opcodeCB0F() { RRC_r8(cpu.reg8_A); }

void RRC_HL() // 4 2
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 3;

	static u8 val;
	switch (cpu.instruction_cycles_remain)
	{
	case 3:
		break;
	
	case 2:
		val = read8(cpu.reg16_HL);
		u8 bit0 = val & 0b00000001;
		val = (bit0 << 7) | (val >> 1);
		set_flag(cpu.reg8_F, flag_z, val == 0);
		set_flag(cpu.reg8_F, flag_n, 0);
		set_flag(cpu.reg8_F, flag_h, 0);
		set_flag(cpu.reg8_F, flag_c, bit0);
		break;

	case 1:
		write8(cpu.reg16_HL, val);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeCB0E() { RRC_HL(); }

void SLA_r8(u8* r) // 2 2
{
	u8 bit7 = (*r & 0b10000000) >> 7;
	*r = *r << 1;
	set_flag(cpu.reg8_F, flag_z, *r == 0);
	set_flag(cpu.reg8_F, flag_n, 0);
	set_flag(cpu.reg8_F, flag_h, 0);
	set_flag(cpu.reg8_F, flag_c, bit7);
}
void opcodeCB20() { SLA_r8(cpu.reg8_B); }
void opcodeCB21() { SLA_r8(cpu.reg8_C); }
void opcodeCB22() { SLA_r8(cpu.reg8_D); }
void opcodeCB23() { SLA_r8(cpu.reg8_E); }
void opcodeCB24() { SLA_r8(cpu.reg8_H); }
void opcodeCB25() { SLA_r8(cpu.reg8_L); }
void opcodeCB27() { SLA_r8(cpu.reg8_A); }

void SLA_HL() // 4 2
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 3;

	static u8 val;
	switch (cpu.instruction_cycles_remain)
	{
	case 3:
		break;
	
	case 2:
		val = read8(cpu.reg16_HL);
		u8 bit7 = (val & 0b10000000) >> 7;
		val = val << 1;
		set_flag(cpu.reg8_F, flag_z, val == 0);
		set_flag(cpu.reg8_F, flag_n, 0);
		set_flag(cpu.reg8_F, flag_h, 0);
		set_flag(cpu.reg8_F, flag_c, bit7);
		break;

	case 1:
		write8(cpu.reg16_HL, val);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeCB26() { SLA_HL(); }

void SRA_r8(u8* r) // 2 2
{
	u8 bit0 = *r & 0b00000001;
	*r = (*r & 0b10000000) | (*r >> 1);
	set_flag(cpu.reg8_F, flag_z, *r == 0);
	set_flag(cpu.reg8_F, flag_n, 0);
	set_flag(cpu.reg8_F, flag_h, 0);
	set_flag(cpu.reg8_F, flag_c, bit0);
}
void opcodeCB28() { SRA_r8(cpu.reg8_B); }
void opcodeCB29() { SRA_r8(cpu.reg8_C); }
void opcodeCB2A() { SRA_r8(cpu.reg8_D); }
void opcodeCB2B() { SRA_r8(cpu.reg8_E); }
void opcodeCB2C() { SRA_r8(cpu.reg8_H); }
void opcodeCB2D() { SRA_r8(cpu.reg8_L); }
void opcodeCB2F() { SRA_r8(cpu.reg8_A); }

void SRA_HL() // 4 2
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 3;

	static u8 val;
	switch (cpu.instruction_cycles_remain)
	{
	case 3:
		break;
	
	case 2:
		val = read8(cpu.reg16_HL);
		u8 bit0 = val & 0b00000001;
		val = (val & 0b10000000) | val >> 1;
		set_flag(cpu.reg8_F, flag_z, val == 0);
		set_flag(cpu.reg8_F, flag_n, 0);
		set_flag(cpu.reg8_F, flag_h, 0);
		set_flag(cpu.reg8_F, flag_c, bit0);
		break;

	case 1:
		write8(cpu.reg16_HL, val);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeCB2E() { SRA_HL(); }

void SRL_r8(u8* r) // 2 2
{
	u8 bit0 = *r & 0b00000001;
	*r = *r >> 1;
	set_flag(cpu.reg8_F, flag_z, *r == 0);
	set_flag(cpu.reg8_F, flag_n, 0);
	set_flag(cpu.reg8_F, flag_h, 0);
	set_flag(cpu.reg8_F, flag_c, bit0);
}
void opcodeCB38() { SRL_r8(cpu.reg8_B); }
void opcodeCB39() { SRL_r8(cpu.reg8_C); }
void opcodeCB3A() { SRL_r8(cpu.reg8_D); }
void opcodeCB3B() { SRL_r8(cpu.reg8_E); }
void opcodeCB3C() { SRL_r8(cpu.reg8_H); }
void opcodeCB3D() { SRL_r8(cpu.reg8_L); }
void opcodeCB3F() { SRL_r8(cpu.reg8_A); }

void SRL_HL() // 4 2
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 3;

	static u8 val;
	switch (cpu.instruction_cycles_remain)
	{
	case 3:
		break;
	
	case 2:
		val = read8(cpu.reg16_HL);
		u8 bit0 = val & 0b00000001;
		val = val >> 1;
		set_flag(cpu.reg8_F, flag_z, val == 0);
		set_flag(cpu.reg8_F, flag_n, 0);
		set_flag(cpu.reg8_F, flag_h, 0);
		set_flag(cpu.reg8_F, flag_c, bit0);
		break;

	case 1:
		write8(cpu.reg16_HL, val);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeCB3E() { SRL_HL(); }

void SWAP_r8(u8* r)
{
	*r = (*r << 4) | (*r >> 4);
	set_flag(cpu.reg8_F, flag_z, *r == 0);
	set_flag(cpu.reg8_F, flag_n, 0);
	set_flag(cpu.reg8_F, flag_h, 0);
	set_flag(cpu.reg8_F, flag_c, 0);
}
void opcodeCB30() { SWAP_r8(cpu.reg8_B); }
void opcodeCB31() { SWAP_r8(cpu.reg8_C); }
void opcodeCB32() { SWAP_r8(cpu.reg8_D); }
void opcodeCB33() { SWAP_r8(cpu.reg8_E); }
void opcodeCB34() { SWAP_r8(cpu.reg8_H); }
void opcodeCB35() { SWAP_r8(cpu.reg8_L); }
void opcodeCB37() { SWAP_r8(cpu.reg8_A); }

void SWAP_HL() // 4 2
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 3;

	static u8 val;
	switch (cpu.instruction_cycles_remain)
	{
	case 3:
		break;

	case 2:
		val = read8(cpu.reg16_HL);
		val = (val << 4) | (val >> 4);
		break;

	case 1:
		write8(cpu.reg16_HL, val);
		set_flag(cpu.reg8_F, flag_z, val == 0);
		set_flag(cpu.reg8_F, flag_n, 0);
		set_flag(cpu.reg8_F, flag_h, 0);
		set_flag(cpu.reg8_F, flag_c, 0);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeCB36() { SWAP_HL(); }

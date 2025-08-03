#include "cpu.h"

u8 get_flag(f_flag bit)
{
	return ((*cpu.reg8_F >> bit) & 1);
}

void set_flag(f_flag bit, u8 val)
{
	*cpu.reg8_F |= val << bit;
}

int check_condition(condition cc)
{
	switch (cc)
	{
	case Z:
		return get_flag(flag_z) == 1;
		break;
	
	case NZ:
		return get_flag(flag_z) == 0;
		break;

	case C:
		return get_flag(flag_c) == 1;
		break;

	case NC:
		return get_flag(flag_c) == 0;
		break;
	}

	return 0;
}

void NOP() // 1 1
{
}
void opcode00() { NOP(); }

void LD_r8r8(u8* r1, u8* r2) // 1 1
{
	*r1 = *r2;
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
void opcode01() { LD_r16n16((u16*)cpu.reg16_BC); }
void opcode11() { LD_r16n16((u16*)cpu.reg16_DE); }
void opcode21() { LD_r16n16((u16*)cpu.reg16_HL); }
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
		write8(*((u16*)cpu.reg16_HL), *r);
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
		write8(*((u16*)cpu.reg16_HL), n);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcode36() { LD_HLn8(); }

void LD_r8HL(u8* r) // 2 1
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 2;

	static u8 address;
	switch (cpu.instruction_cycles_remain)
	{
	case 2:
		break;

	case 1:
		*r = read8(*((u16*)cpu.reg16_HL));
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
void opcode02() { LD_r16A((u16*)cpu.reg16_BC); }
void opcode12() { LD_r16A((u16*)cpu.reg16_DE); }

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
void opcode0A() { LD_Ar16((u16*)cpu.reg16_BC); }
void opcode1A() { LD_Ar16((u16*)cpu.reg16_DE); }

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
		write8(*((u16*)cpu.reg16_HL), *cpu.reg8_A);
		*((u16*)cpu.reg16_HL) = *((u16*)cpu.reg16_HL) + 1;
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
		write8(*((u16*)cpu.reg16_HL), *cpu.reg8_A);
		*((u16*)cpu.reg16_HL) = *((u16*)cpu.reg16_HL) - 1;
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
		*cpu.reg8_A = read8(*((u16*)cpu.reg16_HL));
		*((u16*)cpu.reg16_HL) = *((u16*)cpu.reg16_HL) - 1;
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
		*cpu.reg8_A = read8(*((u16*)cpu.reg16_HL));
		*((u16*)cpu.reg16_HL) = *((u16*)cpu.reg16_HL) + 1;
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
		break;

	case 1:
		cpu.reg16_SP = *((u16*)cpu.reg16_HL);
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
		set_flag(flag_h, ((cpu.reg16_SP & 0xF) + (n & 0xF)) > 0xF);
		set_flag(flag_c, ((cpu.reg16_SP & 0xFF) + n) > 0xF);
		res = cpu.reg16_SP + n;
		set_flag(flag_z, 0);
		set_flag(flag_n, 0);
		break;

	case 1:
		*((u16*)cpu.reg16_HL) = res;
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeF8() { LD_SPHLe8(); }

void ADC_Ar8(u8* r) // 1 1
{
	set_flag(flag_h, ((*cpu.reg8_A & 0xF) + (*r & 0xF) + get_flag(flag_c)) > 0xF);
	u16 res = *cpu.reg8_A + *r + get_flag(flag_c);
	*cpu.reg8_A = (u8)res;
	set_flag(flag_z, *cpu.reg8_A == 0);
	set_flag(flag_n, 0);
	set_flag(flag_c, res > 0xFF);
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
		u8 val = read8(*((u16*)cpu.reg16_HL));
		set_flag(flag_h, ((*cpu.reg8_A & 0xF) + (val & 0xF) + get_flag(flag_c)) > 0xF);
		u16 res = *cpu.reg8_A + val + get_flag(flag_c);
		*cpu.reg8_A = (u8)res;
		set_flag(flag_z, *cpu.reg8_A == 0);
		set_flag(flag_n, 0);
		set_flag(flag_c, res > 0xFF);
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
		set_flag(flag_h, ((*cpu.reg8_A & 0xF) + (val & 0xF) + get_flag(flag_c)) > 0xF);
		u16 res = *cpu.reg8_A + val + get_flag(flag_c);
		*cpu.reg8_A = (u8)res;
		set_flag(flag_z, *cpu.reg8_A == 0);
		set_flag(flag_n, 0);
		set_flag(flag_c, res > 0xFF);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeCE() { ADC_An8(); }

void ADD_Ar8(u8* r) // 1 1
{
	set_flag(flag_h, ((*cpu.reg8_A & 0xF) + (*r & 0xF)) > 0xF);
	u16 res = *cpu.reg8_A + *r;
	*cpu.reg8_A = (u8)res;
	set_flag(flag_z, *cpu.reg8_A == 0);
	set_flag(flag_n, 0);
	set_flag(flag_c, res > 0xFF);
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
		u8 val = read8(*((u16*)cpu.reg16_HL));
		set_flag(flag_h, ((*cpu.reg8_A & 0xF) + (val & 0xF)) > 0xF);
		u16 res = *cpu.reg8_A + val;
		*cpu.reg8_A = (u8)res;
		set_flag(flag_z, *cpu.reg8_A == 0);
		set_flag(flag_n, 0);
		set_flag(flag_c, res > 0xFF);
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
		set_flag(flag_h, ((*cpu.reg8_A & 0xF) + (val & 0xF)) > 0xF);
		u16 res = *cpu.reg8_A + val;
		*cpu.reg8_A = (u8)res;
		set_flag(flag_z, *cpu.reg8_A == 0);
		set_flag(flag_n, 0);
		set_flag(flag_c, res > 0xFF);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeC6() { ADD_An8(); }

void ADD_HLr16(u8* r) // 2 1
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 2;

	switch (cpu.instruction_cycles_remain)
	{
	case 2:
		break;

	case 1:
		u16 res = *cpu.reg8_L + r[1];
		*cpu.reg8_L = (u8)res;
		set_flag(flag_c, res > 0xFF);
		set_flag(flag_h, ((*cpu.reg8_H & 0xF) + (r[0] & 0xF)) + get_flag(flag_c) > 0xF);
		res = *cpu.reg8_H + r[0] + get_flag(flag_c);
		*cpu.reg8_H = (u8)res;
		set_flag(flag_n, 0);
		set_flag(flag_c, res > 0xFF);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcode09() { ADD_HLr16(cpu.reg16_BC); }
void opcode19() { ADD_HLr16(cpu.reg16_DE); }
void opcode29() { ADD_HLr16(cpu.reg16_HL); }
void opcode39() { ADD_HLr16((u8*)&cpu.reg16_SP); }

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
		set_flag(flag_h, ((cpu.reg16_SP & 0xF) + (val & 0xF)) > 0xF);
		u16 res = (cpu.reg16_SP & 0xFF) + val;
		set_flag(flag_c, res > 0xFF);
		set_flag(flag_z, 0);
		set_flag(flag_n, 0);
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
	u8 carry = get_flag(flag_c);
	set_flag(flag_h, (*cpu.reg8_A & 0xF) < ((*r & 0xF) + carry));
	set_flag(flag_c, (int)*cpu.reg8_A - (int)*r - (int)carry < 0);
	*cpu.reg8_A = *cpu.reg8_A - *r - carry;
	set_flag(flag_z, *cpu.reg8_A == 0 ? 1 : 0);
	set_flag(flag_n, 1);
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
		u8 val = read8(*((u16*)cpu.reg16_HL));
		u8 carry = get_flag(flag_c);
		set_flag(flag_h, (*cpu.reg8_A & 0xF) < ((val & 0xF) + carry));
		set_flag(flag_c, (int)*cpu.reg8_A - (int)val - (int)carry < 0);
		*cpu.reg8_A = *cpu.reg8_A - val - carry;
		set_flag(flag_z, *cpu.reg8_A == 0 ? 1 : 0);
		set_flag(flag_n, 1);
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
		u8 carry = get_flag(flag_c);
		set_flag(flag_h, (*cpu.reg8_A & 0xF) < ((val & 0xF) + carry));
		set_flag(flag_c, (int)*cpu.reg8_A - (int)val - (int)carry < 0);
		*cpu.reg8_A = *cpu.reg8_A - val - carry;
		set_flag(flag_z, *cpu.reg8_A == 0 ? 1 : 0);
		set_flag(flag_n, 1);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeDE() { SBC_An8(); }

void SUB_Ar8(u8* r) // 1 1
{
	set_flag(flag_h, (*cpu.reg8_A & 0xF) < ((*r & 0xF)));
	set_flag(flag_c, (int)*cpu.reg8_A - (int)*r < 0);
	*cpu.reg8_A = *cpu.reg8_A - *r;
	set_flag(flag_z, *cpu.reg8_A == 0 ? 1 : 0);
	set_flag(flag_n, 1);
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
		u8 val = read8(*((u16*)cpu.reg16_HL));
		set_flag(flag_h, (*cpu.reg8_A & 0xF) < ((val & 0xF)));
		set_flag(flag_c, (int)*cpu.reg8_A - (int)val < 0);
		*cpu.reg8_A = *cpu.reg8_A - val;
		set_flag(flag_z, *cpu.reg8_A == 0 ? 1 : 0);
		set_flag(flag_n, 1);
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
		set_flag(flag_h, (*cpu.reg8_A & 0xF) < ((val & 0xF)));
		set_flag(flag_c, (int)*cpu.reg8_A - (int)val < 0);
		*cpu.reg8_A = *cpu.reg8_A - val;
		set_flag(flag_z, *cpu.reg8_A == 0 ? 1 : 0);
		set_flag(flag_n, 1);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeD6() { SUB_An8(); }

void CP_Ar8(u8* r) // 1 1
{
	set_flag(flag_h, (*cpu.reg8_A & 0xF) < ((*r & 0xF)));
	set_flag(flag_c, (int)*cpu.reg8_A - (int)*r < 0);
	set_flag(flag_z, *cpu.reg8_A == *r ? 1 : 0);
	set_flag(flag_n, 1);
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
		u8 val = read8(*((u16*)cpu.reg16_HL));
		set_flag(flag_h, (*cpu.reg8_A & 0xF) < ((val & 0xF)));
		set_flag(flag_c, (int)*cpu.reg8_A - (int)val < 0);
		set_flag(flag_z, *cpu.reg8_A == val);
		set_flag(flag_n, 1);
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
		set_flag(flag_h, (*cpu.reg8_A & 0xF) < ((val & 0xF)));
		set_flag(flag_c, (int)*cpu.reg8_A - (int)val < 0);
		set_flag(flag_z, *cpu.reg8_A == val);
		set_flag(flag_n, 1);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeFE() { CP_An8(); }

void INC_r8(u8* r) // 1 1
{
	(*r)++;
	set_flag(flag_z, *r == 0);
	set_flag(flag_n, 0);
	set_flag(flag_h, (*r & 0xF) == 0);
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
		val = read8(*((u16*)cpu.reg16_HL));
		val++;
		break;

	case 1:
		u8 val = read8(cpu.reg16_PC++);
		set_flag(flag_z, val == 0);
		set_flag(flag_n, 0);
		set_flag(flag_h, (val & 0xF) == 0);
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
void opcode03() { INC_r16((u16*)cpu.reg16_BC); }
void opcode13() { INC_r16((u16*)cpu.reg16_DE); }
void opcode23() { INC_r16((u16*)cpu.reg16_HL); }
void opcode33() { INC_r16(&cpu.reg16_SP); }

void DEC_r8(u8* r) // 1 1
{
	(*r)--;
	set_flag(flag_z, *r == 0);
	set_flag(flag_n, 1);
	set_flag(flag_h, (*r & 0xF) == 0xF);
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
		val = read8(*((u16*)cpu.reg16_HL));
		val--;
		break;

	case 1:
		u8 val = read8(cpu.reg16_PC++);
		set_flag(flag_z, val == 0);
		set_flag(flag_n, 1);
		set_flag(flag_h, (val & 0xF) == 0xF);
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
void opcode0B() { DEC_r16((u16*)cpu.reg16_BC); }
void opcode1B() { DEC_r16((u16*)cpu.reg16_DE); }
void opcode2B() { DEC_r16((u16*)cpu.reg16_HL); }
void opcode3B() { DEC_r16(&cpu.reg16_SP); }

void AND_Ar8(u8* r) // 1 1
{
	*cpu.reg8_A = *cpu.reg8_A & *r;
	set_flag(flag_z, *cpu.reg8_A == 0 ? 1 : 0);
	set_flag(flag_n, 0);
	set_flag(flag_h, 1);
	set_flag(flag_c, 0);
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
		*cpu.reg8_A = *cpu.reg8_A & read8(*((u16*)cpu.reg16_HL));
		set_flag(flag_z, *cpu.reg8_A == 0 ? 1 : 0);
		set_flag(flag_n, 0);
		set_flag(flag_h, 1);
		set_flag(flag_c, 0);
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
		set_flag(flag_z, *cpu.reg8_A == 0 ? 1 : 0);
		set_flag(flag_n, 0);
		set_flag(flag_h, 1);
		set_flag(flag_c, 0);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeE6() { AND_An8(); }

void CPL() // 1 1
{
	*cpu.reg8_A = ~*cpu.reg8_A;
	set_flag(flag_n, 1);
	set_flag(flag_h, 1);
}
void opcode2F() { CPL(); }

void OR_Ar8(u8* r) // 1 1
{
	*cpu.reg8_A = *cpu.reg8_A | *r;
	set_flag(flag_z, *cpu.reg8_A == 0 ? 1 : 0);
	set_flag(flag_n, 0);
	set_flag(flag_h, 0);
	set_flag(flag_c, 0);
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
		*cpu.reg8_A = *cpu.reg8_A | read8(*((u16*)cpu.reg16_HL));
		set_flag(flag_z, *cpu.reg8_A == 0 ? 1 : 0);
		set_flag(flag_n, 0);
		set_flag(flag_h, 0);
		set_flag(flag_c, 0);
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
		set_flag(flag_z, *cpu.reg8_A == 0 ? 1 : 0);
		set_flag(flag_n, 0);
		set_flag(flag_h, 0);
		set_flag(flag_c, 0);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeF6() { OR_An8(); }

void XOR_Ar8(u8* r) // 1 1
{
	*cpu.reg8_A = *cpu.reg8_A ^ *r;
	set_flag(flag_z, *cpu.reg8_A == 0 ? 1 : 0);
	set_flag(flag_n, 0);
	set_flag(flag_h, 0);
	set_flag(flag_c, 0);
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
		*cpu.reg8_A = *cpu.reg8_A ^ read8(*((u16*)cpu.reg16_HL));
		set_flag(flag_z, *cpu.reg8_A == 0 ? 1 : 0);
		set_flag(flag_n, 0);
		set_flag(flag_h, 0);
		set_flag(flag_c, 0);
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
		set_flag(flag_z, *cpu.reg8_A == 0 ? 1 : 0);
		set_flag(flag_n, 0);
		set_flag(flag_h, 0);
		set_flag(flag_c, 0);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeEE() { XOR_An8(); }

void CCF() // 1 1
{
	set_flag(flag_n, 0);
	set_flag(flag_h, 0);
	set_flag(flag_c, !get_flag(flag_c));
}
void opcode3F() { CCF(); }

void SCF() // 1 1
{
	set_flag(flag_n, 0);
	set_flag(flag_h, 0);
	set_flag(flag_c, 1);
}
void opcode37() { SCF(); }

void EI() // 1 1
{
	cpu.IME = 1;
}
void opcodeFB() { EI(); }

void DI() // 1 1
{
	cpu.IME = 0;
}
void opcodeF3() { DI(); }

void POP_r16(u8* r) // 3 2
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 3;

	switch (cpu.instruction_cycles_remain)
	{
	case 3:
		break;

	case 2:
		r[1] = read8(cpu.reg16_SP++);
		break;

	case 1:
		r[0] = read8(cpu.reg16_SP++);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeC1() { POP_r16(cpu.reg16_BC); }
void opcodeD1() { POP_r16(cpu.reg16_DE); }
void opcodeE1() { POP_r16(cpu.reg16_HL); }
void opcodeF1() { POP_r16(cpu.reg16_AF); }

void PUSH_r16(u8* r) // 4 2
{
	if (!cpu.instruction_cycles_remain)
		cpu.instruction_cycles_remain = 4;

	switch (cpu.instruction_cycles_remain)
	{
	case 4:
		break;

	case 3:
		write8(--cpu.reg16_SP, r[0]);
		break;

	case 2:
		break;

	case 1:
		write8(--cpu.reg16_SP, r[1]);
		break;
	}
	cpu.instruction_cycles_remain--;
}
void opcodeC5() { PUSH_r16(cpu.reg16_BC); }
void opcodeD5() { PUSH_r16(cpu.reg16_DE); }
void opcodeE5() { PUSH_r16(cpu.reg16_HL); }
void opcodeF5() { PUSH_r16(cpu.reg16_AF); }

void JP_HL() // 1 1
{
	cpu.reg16_PC = *((u16*)cpu.reg16_HL);
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
		break;

	case 1:
		cpu.reg16_PC = n;
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
		cpu.instruction_cycles_remain = 4;

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
		write8(--cpu.reg16_SP, (u8)(cpu.reg16_PC & 0x00FF));
		break;

	case 2:
		write8(--cpu.reg16_SP, (u8)(cpu.reg16_PC >> 8));
		break;

	case 1:
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
		if (!check_condition(cc))
			cpu.instruction_cycles_remain = 1;
		break;

	case 3:
		write8(--cpu.reg16_SP, (u8)(cpu.reg16_PC & 0x00FF));
		break;

	case 2:
		write8(--cpu.reg16_SP, (u8)(cpu.reg16_PC >> 8));
		break;

	case 1:
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

void DDA()
{
	u8 adjustment = 0;
	int set_c = 0;

	if (get_flag(flag_n))
	{
		if (get_flag(flag_h))
			adjustment += 0x6;
		if (get_flag(flag_c))
			adjustment += 0x60;
		(*cpu.reg8_A) -= adjustment;
	}
	else
	{
		if (get_flag(flag_h) || (*cpu.reg8_A & 0xF) > 0x9)
			adjustment += 0x6;
		if (get_flag(flag_c) || *cpu.reg8_A > 0x99)
		{
			adjustment += 0x60;
			set_c = 1;
		}
		(*cpu.reg8_A) += adjustment;
	}
	set_flag(flag_z, *cpu.reg8_A == 0);
	set_flag(flag_h, 0);
	set_flag(flag_c, set_c);
}
void opcode27() { DDA(); }

void RLCA() // 1 1
{
	u8 bit7 = (*cpu.reg8_A & 0b10000000) >> 7;
	*cpu.reg8_A = (*cpu.reg8_A << 1) | bit7;
	set_flag(flag_z, 0);
	set_flag(flag_n, 0);
	set_flag(flag_h, 0);
	set_flag(flag_c, bit7);
}
void opcode07() { RLCA(); }

void RRCA() // 1 1
{
	u8 bit0 = *cpu.reg8_A & 0b00000001;
	*cpu.reg8_A = (bit0 << 7) | (*cpu.reg8_A >> 1);
	set_flag(flag_z, 0);
	set_flag(flag_n, 0);
	set_flag(flag_h, 0);
	set_flag(flag_c, bit0);
}
void opcode0F() { RRCA(); }

void RLA() // 1 1
{
	u8 bit7 = (*cpu.reg8_A & 0b10000000) >> 7;
	*cpu.reg8_A = (*cpu.reg8_A << 1) | get_flag(flag_c);
	set_flag(flag_z, 0);
	set_flag(flag_n, 0);
	set_flag(flag_h, 0);
	set_flag(flag_c, bit7);
}
void opcode17() { RLA(); }

void RRA() // 1 1
{
	u8 bit0 = *cpu.reg8_A & 0b00000001;
	*cpu.reg8_A = (get_flag(flag_c) << 7) | (*cpu.reg8_A >> 1);
	set_flag(flag_z, 0);
	set_flag(flag_n, 0);
	set_flag(flag_h, 0);
	set_flag(flag_c, bit0);
}
void opcode1F() { RRA(); }

void cpu_tick()
{
	static void (*instruction_to_execute)();

	if (cpu.instruction_cycles_remain == 0)
		instruction_to_execute = instruction_set[cpu.reg16_PC++];

	instruction_to_execute();
}

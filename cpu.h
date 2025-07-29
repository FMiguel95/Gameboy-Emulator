#ifndef CPU_H
# define CPU_H

# include "types.h"
# include "memory.h"

typedef struct {
	u8 reg16_AF[2];
	u8* reg8_A;
	u8* reg8_F;

	u8 reg16_BC[2];
	u8* reg8_B;
	u8* reg8_C;

	u8 reg16_DE[2];
	u8* reg8_D;
	u8* reg8_E;

	u8 reg16_HL[2];
	u8* reg8_H;
	u8* reg8_L;

	u16 reg16_SP;

	u16 reg16_PC;

	u8 instruction_cycles_remain;

} cpu_t;

static cpu_t cpu;

typedef enum {
	flag_c = 4,
	flag_h = 5,
	flag_n = 6,
	flag_z = 7
} f_flag;

u8 get_flag(f_flag bit)
{
	return ((*cpu.reg8_F >> bit) & 1);
}

void set_flag(f_flag bit, u8 val)
{
	*cpu.reg8_F |= val << bit;
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
		n = n << 8;
		break;
	
	case 1:
		n |= (u16)read8(cpu.reg16_PC++);
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
		address = address << 8;
		break;

	case 2:
		address |= (u16)read8(cpu.reg16_PC++);
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
void opcode1A() { LD_Ar16((u16*)cpu.reg16_BC); }

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
		address = address << 8;
		break;

	case 2:
		address |= (u16)read8(cpu.reg16_PC++);
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

	s8 val;
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
void opcode33() { INC_r16(cpu.reg16_SP); }

////////////////////////
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
void opcode3B() { DEC_r16(cpu.reg16_SP); }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

static void (*instruction_set[0x100])() = {
	[0x00] = opcode00,
	[0x01] = opcode01,
	// [0x02] = opcode02,
	[0x03] = opcode03,
	[0x04] = opcode04,
	[0x05] = opcode05,
	[0x06] = opcode06,
	// [0x07] = opcode07,
	// [0x08] = opcode08,
	[0x09] = opcode09,
	// [0x0A] = opcode0A,
	[0x0B] = opcode0B,
	[0x0C] = opcode0C,
	[0x0D] = opcode0D,
	[0x0E] = opcode0E,
	// [0x0F] = opcode0F,
	// [0x10] = opcode10,
	[0x11] = opcode11,
	// [0x12] = opcode12,
	[0x13] = opcode13,
	[0x14] = opcode14,
	[0x15] = opcode15,
	[0x16] = opcode16,
	// [0x17] = opcode17,
	// [0x18] = opcode18,
	[0x19] = opcode19,
	// [0x1A] = opcode1A,
	[0x1B] = opcode1B,
	[0x1C] = opcode1C,
	[0x1D] = opcode1D,
	[0x1E] = opcode1E,
	// [0x1F] = opcode1F,
	// [0x20] = opcode20,
	[0x21] = opcode21,
	[0x22] = opcode22,
	[0x23] = opcode23,
	[0x24] = opcode24,
	[0x25] = opcode25,
	// [0x26] = opcode26,
	[0x26] = opcode26,
	// [0x27] = opcode27,
	// [0x28] = opcode28,
	[0x29] = opcode29,
	[0x2A] = opcode2A,
	[0x2B] = opcode2B,
	[0x2C] = opcode2C,
	[0x2D] = opcode2D,
	[0x2E] = opcode2E,
	// [0x2F] = opcode2F,
	// [0x30] = opcode30,
	[0x31] = opcode31,
	[0x32] = opcode32,
	[0x33] = opcode33,
	[0x34] = opcode34,
	[0x35] = opcode35,
	[0x36] = opcode36,
	// [0x37] = opcode37,
	// [0x38] = opcode38,
	[0x39] = opcode39,
	[0x3A] = opcode3A,
	[0x3B] = opcode3B,
	[0x3C] = opcode3C,
	[0x3D] = opcode3D,
	[0x3E] = opcode3E,
	// [0x3F] = opcode3F,
	[0x40] = opcode40,
	[0x41] = opcode41,
	[0x42] = opcode42,
	[0x43] = opcode43,
	[0x44] = opcode44,
	[0x45] = opcode45,
	[0x46] = opcode46,
	[0x47] = opcode47,
	[0x48] = opcode48,
	[0x49] = opcode49,
	[0x4A] = opcode4A,
	[0x4B] = opcode4B,
	[0x4C] = opcode4C,
	[0x4D] = opcode4D,
	[0x4E] = opcode4E,
	[0x4F] = opcode4F,
	[0x50] = opcode50,
	[0x51] = opcode51,
	[0x52] = opcode52,
	[0x53] = opcode53,
	[0x54] = opcode54,
	[0x55] = opcode55,
	[0x56] = opcode56,
	[0x57] = opcode57,
	[0x58] = opcode58,
	[0x59] = opcode59,
	[0x5A] = opcode5A,
	[0x5B] = opcode5B,
	[0x5C] = opcode5C,
	[0x5D] = opcode5D,
	[0x5E] = opcode5E,
	[0x5F] = opcode5F,
	[0x60] = opcode60,
	[0x61] = opcode61,
	[0x62] = opcode62,
	[0x63] = opcode63,
	[0x64] = opcode64,
	[0x65] = opcode65,
	[0x66] = opcode66,
	[0x67] = opcode67,
	[0x68] = opcode68,
	[0x69] = opcode69,
	[0x6A] = opcode6A,
	[0x6B] = opcode6B,
	[0x6C] = opcode6C,
	[0x6D] = opcode6D,
	[0x6E] = opcode6E,
	[0x6F] = opcode6F,
	[0x70] = opcode70,
	[0x71] = opcode71,
	[0x72] = opcode72,
	[0x73] = opcode73,
	[0x74] = opcode74,
	[0x75] = opcode75,
	// [0x76] = opcode76,
	[0x77] = opcode77,
	[0x78] = opcode78,
	[0x79] = opcode79,
	[0x7A] = opcode7A,
	[0x7B] = opcode7B,
	[0x7C] = opcode7C,
	[0x7D] = opcode7D,
	[0x7E] = opcode7E,
	[0x7F] = opcode7F,
	[0x80] = opcode80,
	[0x81] = opcode81,
	[0x82] = opcode82,
	[0x83] = opcode83,
	[0x84] = opcode84,
	[0x85] = opcode85,
	[0x86] = opcode86,
	[0x87] = opcode87,
	[0x88] = opcode88,
	[0x89] = opcode89,
	[0x8A] = opcode8A,
	[0x8B] = opcode8B,
	[0x8C] = opcode8C,
	[0x8D] = opcode8D,
	[0x8E] = opcode8E,
	[0x8F] = opcode8F,
	[0x90] = opcode90,
	[0x91] = opcode91,
	[0x92] = opcode92,
	[0x93] = opcode93,
	[0x94] = opcode94,
	[0x95] = opcode95,
	[0x96] = opcode96,
	[0x97] = opcode97,
	[0x98] = opcode98,
	[0x99] = opcode99,
	[0x9A] = opcode9A,
	[0x9B] = opcode9B,
	[0x9C] = opcode9C,
	[0x9D] = opcode9D,
	[0x9E] = opcode9E,
	[0x9F] = opcode9F,
	[0xA0] = opcodeA0,
	[0xA1] = opcodeA1,
	[0xA2] = opcodeA2,
	[0xA3] = opcodeA3,
	[0xA4] = opcodeA4,
	[0xA5] = opcodeA5,
	[0xA6] = opcodeA6,
	[0xA7] = opcodeA7,
	// [0xA8] = opcodeA8,
	// [0xA9] = opcodeA9,
	// [0xAA] = opcodeAA,
	// [0xAB] = opcodeAB,
	// [0xAC] = opcodeAC,
	// [0xAD] = opcodeAD,
	// [0xAE] = opcodeAE,
	// [0xAF] = opcodeAF,
	// [0xB0] = opcodeB0,
	// [0xB1] = opcodeB1,
	// [0xB2] = opcodeB2,
	// [0xB3] = opcodeB3,
	// [0xB4] = opcodeB4,
	// [0xB5] = opcodeB5,
	// [0xB6] = opcodeB6,
	// [0xB7] = opcodeB7,
	[0xB8] = opcodeB8,
	[0xB9] = opcodeB9,
	[0xBA] = opcodeBA,
	[0xBB] = opcodeBB,
	[0xBC] = opcodeBC,
	[0xBD] = opcodeBD,
	[0xBE] = opcodeBE,
	[0xBF] = opcodeBF,
	// [0xC0] = opcodeC0,
	// [0xC1] = opcodeC1,
	// [0xC2] = opcodeC2,
	// [0xC3] = opcodeC3,
	// [0xC4] = opcodeC4,
	// [0xC5] = opcodeC5,
	[0xC6] = opcodeC6,
	// [0xC7] = opcodeC7,
	// [0xC8] = opcodeC8,
	// [0xC9] = opcodeC9,
	// [0xCA] = opcodeCA,
	// [0xCB] = opcodeCB,
	// [0xCC] = opcodeCC,
	// [0xCD] = opcodeCD,
	[0xCE] = opcodeCE,
	// [0xCF] = opcodeCF,
	// [0xD0] = opcodeD0,
	// [0xD1] = opcodeD1,
	// [0xD2] = opcodeD2,
	[0xD3] = 0,
	// [0xD4] = opcodeD4,
	// [0xD5] = opcodeD5,
	[0xD6] = opcodeD6,
	// [0xD7] = opcodeD7,
	// [0xD8] = opcodeD8,
	// [0xD9] = opcodeD9,
	// [0xDA] = opcodeDA,
	[0xDB] = 0,
	// [0xDC] = opcodeDC,
	[0xDD] = 0,
	[0xDE] = opcodeDE,
	// [0xDF] = opcodeDF,
	[0xE0] = opcodeE0,
	// [0xE1] = opcodeE1,
	[0xE2] = opcodeE2,
	[0xE3] = 0,
	[0xE4] = 0,
	// [0xE5] = opcodeE5,
	[0xE6] = opcodeE6,
	// [0xE7] = opcodeE7,
	[0xE8] = opcodeE8,
	// [0xE9] = opcodeE9,
	[0xEA] = opcodeEA,
	[0xEB] = 0,
	[0xEC] = 0,
	[0xED] = 0,
	// [0xEE] = opcodeEE,
	// [0xEF] = opcodeEF,
	[0xF0] = opcodeF0,
	// [0xF1] = opcodeF1,
	[0xF2] = opcodeF2,
	// [0xF3] = opcodeF3,
	[0xF4] = 0,
	// [0xF5] = opcodeF5,
	// [0xF6] = opcodeF6,
	// [0xF7] = opcodeF7,
	// [0xF8] = opcodeF8,
	// [0xF9] = opcodeF9,
	[0xFA] = opcodeFA,
	// [0xFB] = opcodeFB,
	[0xFC] = 0,
	[0xFD] = 0,
	[0xFE] = opcodeFE,
	// [0xFF] = opcodeFF
};

void cpu_tick();

#endif

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

	u8 IME;

} cpu_t;

static cpu_t cpu;

typedef enum {
	flag_c = 4,
	flag_h = 5,
	flag_n = 6,
	flag_z = 7
} f_flag;
u8 get_flag(f_flag bit);
void set_flag(f_flag bit, u8 val);

typedef enum {
	Z,
	NZ,
	C,
	NC
} condition;
int check_condition(condition cc);

void NOP();
void opcode00();

void LD_r8r8(u8* r1, u8* r2);
void opcode40();
void opcode41();
void opcode42();
void opcode43();
void opcode44();
void opcode45();
void opcode47();
void opcode48();
void opcode49();
void opcode4A();
void opcode4B();
void opcode4C();
void opcode4D();
void opcode4F();
void opcode50();
void opcode51();
void opcode52();
void opcode53();
void opcode54();
void opcode55();
void opcode57();
void opcode58();
void opcode59();
void opcode5A();
void opcode5B();
void opcode5C();
void opcode5D();
void opcode5F();
void opcode60();
void opcode61();
void opcode62();
void opcode63();
void opcode64();
void opcode65();
void opcode67();
void opcode68();
void opcode69();
void opcode6A();
void opcode6B();
void opcode6C();
void opcode6D();
void opcode6F();
void opcode78();
void opcode79();
void opcode7A();
void opcode7B();
void opcode7C();
void opcode7D();
void opcode7F();

void LD_r8n8(u8* r);
void opcode06();
void opcode0E();
void opcode16();
void opcode1E();
void opcode26();
void opcode2E();
void opcode3E();

void LD_r16n16(u16* r);
void opcode01();
void opcode11();
void opcode21();
void opcode31();

void LD_HLr8(u8* r);
void opcode70();
void opcode71();
void opcode72();
void opcode73();
void opcode74();
void opcode75();
void opcode77();

void LD_HLn8();
void opcode36();

void LD_r8HL(u8* r);
void opcode46();
void opcode4E();
void opcode56();
void opcode5E();
void opcode66();
void opcode6E();
void opcode7E();

void LD_r16A(u16* r);
void opcode02();
void opcode12();

void LD_n16A();
void opcodeEA();

void LDH_n8A();
void opcodeE0();

void LDH_CA();
void opcodeE2();

void LD_Ar16(u16* r);
void opcode0A();
void opcode1A();

void LD_An16();
void opcodeFA();

void LDH_An8();
void opcodeF0();

void LDH_AC();
void opcodeF2();

void LD_HLIA();
void opcode22();

void LD_HLDA();
void opcode32();

void LD_AHLD();
void opcode3A();

void LD_AHLI();
void opcode2A();

void LD_n16SP();
void opcode08();

void LD_SPHLe8();
void opcodeF8();

void ADC_Ar8(u8* r);
void opcode88();
void opcode89();
void opcode8A();
void opcode8B();
void opcode8C();
void opcode8D();
void opcode8F();

void ADC_AHL();
void opcode8E();

void ADC_An8();
void opcodeCE();

void ADD_Ar8(u8* r);
void opcode80();
void opcode81();
void opcode82();
void opcode83();
void opcode84();
void opcode85();
void opcode87();

void ADD_AHL();
void opcode86();

void ADD_An8();
void opcodeC6();

void ADD_HLr16(u8* r);
void opcode09();
void opcode19();
void opcode29();
void opcode39();

void ADD_SPe8();
void opcodeE8();

void SBC_Ar8(u8* r);
void opcode98();
void opcode99();
void opcode9A();
void opcode9B();
void opcode9C();
void opcode9D();
void opcode9F();

void SBC_AHL();
void opcode9E();

void SBC_An8();
void opcodeDE();

void SUB_Ar8(u8* r);
void opcode90();
void opcode91();
void opcode92();
void opcode93();
void opcode94();
void opcode95();
void opcode97();

void SUB_AHL();
void opcode96();

void SUB_An8();
void opcodeD6();

void CP_Ar8(u8* r);
void opcodeB8();
void opcodeB9();
void opcodeBA();
void opcodeBB();
void opcodeBC();
void opcodeBD();
void opcodeBF();

void CP_AHL();
void opcodeBE();

void CP_An8();
void opcodeFE();

void INC_r8(u8* r);
void opcode04();
void opcode0C();
void opcode14();
void opcode1C();
void opcode24();
void opcode2C();
void opcode3C();

void INC_HL();
void opcode34();

void INC_r16(u16* r);
void opcode03();
void opcode13();
void opcode23();
void opcode33();

void DEC_r8(u8* r);
void opcode05();
void opcode0D();
void opcode15();
void opcode1D();
void opcode25();
void opcode2D();
void opcode3D();

void DEC_HL();
void opcode35();

void DEC_r16(u16* r);
void opcode0B();
void opcode1B();
void opcode2B();
void opcode3B();

void AND_Ar8(u8* r);
void opcodeA0();
void opcodeA1();
void opcodeA2();
void opcodeA3();
void opcodeA4();
void opcodeA5();
void opcodeA7();

void AND_AHL();
void opcodeA6();

void AND_An8();
void opcodeE6();

void CPL();
void opcode2F();

void OR_Ar8(u8* r);
void opcodeB0();
void opcodeB1();
void opcodeB2();
void opcodeB3();
void opcodeB4();
void opcodeB5();
void opcodeB7();

void OR_AHL();
void opcodeB6();

void OR_An8();
void opcodeF6();

void XOR_Ar8(u8* r);
void opcodeA8();
void opcodeA9();
void opcodeAA();
void opcodeAB();
void opcodeAC();
void opcodeAD();
void opcodeAF();

void XOR_AHL();
void opcodeAE();

void XOR_An8();
void opcodeEE();

void CCF();
void opcode3F();

void SCF();
void opcode37();

void EI();
void opcodeFB();

void DI();
void opcodeF3();

void POP_r16(u8* r);
void opcodeC1();
void opcodeD1();
void opcodeE1();
void opcodeF1();

void PUSH_r16(u8* r);
void opcodeC5();
void opcodeD5();
void opcodeE5();
void opcodeF5();

void JP_HL();
void opcodeE9();

void JP_n16();
void opcodeC3();

void JP_cce8(condition cc);
void opcodeC2();
void opcodeCA();
void opcodeD2();
void opcodeDA();

void JR_n16();
void opcode18();

void JR_cce8(condition cc);
void opcode20();
void opcode28();
void opcode30();
void opcode38();

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
		address = read8(cpu.reg16_SP++);
		break;

	case 2:
		address |= ((u16)read8(cpu.reg16_SP++) << 8);
		break;

	case 1:
		cpu.reg16_PC = address;
		cpu.IME = 0;
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

static void (*instruction_set[0x100])() = {
	[0x00] = opcode00,
	[0x01] = opcode01,
	// [0x02] = opcode02,
	[0x03] = opcode03,
	[0x04] = opcode04,
	[0x05] = opcode05,
	[0x06] = opcode06,
	// [0x07] = opcode07,
	[0x08] = opcode08,
	[0x09] = opcode09,
	[0x0A] = opcode0A,
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
	[0x18] = opcode18,
	[0x19] = opcode19,
	[0x1A] = opcode1A,
	[0x1B] = opcode1B,
	[0x1C] = opcode1C,
	[0x1D] = opcode1D,
	[0x1E] = opcode1E,
	// [0x1F] = opcode1F,
	[0x20] = opcode20,
	[0x21] = opcode21,
	[0x22] = opcode22,
	[0x23] = opcode23,
	[0x24] = opcode24,
	[0x25] = opcode25,
	// [0x26] = opcode26,
	[0x26] = opcode26,
	// [0x27] = opcode27,
	[0x28] = opcode28,
	[0x29] = opcode29,
	[0x2A] = opcode2A,
	[0x2B] = opcode2B,
	[0x2C] = opcode2C,
	[0x2D] = opcode2D,
	[0x2E] = opcode2E,
	[0x2F] = opcode2F,
	[0x30] = opcode30,
	[0x31] = opcode31,
	[0x32] = opcode32,
	[0x33] = opcode33,
	[0x34] = opcode34,
	[0x35] = opcode35,
	[0x36] = opcode36,
	[0x37] = opcode37,
	[0x38] = opcode38,
	[0x39] = opcode39,
	[0x3A] = opcode3A,
	[0x3B] = opcode3B,
	[0x3C] = opcode3C,
	[0x3D] = opcode3D,
	[0x3E] = opcode3E,
	[0x3F] = opcode3F,
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
	[0xA8] = opcodeA8,
	[0xA9] = opcodeA9,
	[0xAA] = opcodeAA,
	[0xAB] = opcodeAB,
	[0xAC] = opcodeAC,
	[0xAD] = opcodeAD,
	[0xAE] = opcodeAE,
	[0xAF] = opcodeAF,
	[0xB0] = opcodeB0,
	[0xB1] = opcodeB1,
	[0xB2] = opcodeB2,
	[0xB3] = opcodeB3,
	[0xB4] = opcodeB4,
	[0xB5] = opcodeB5,
	[0xB6] = opcodeB6,
	[0xB7] = opcodeB7,
	[0xB8] = opcodeB8,
	[0xB9] = opcodeB9,
	[0xBA] = opcodeBA,
	[0xBB] = opcodeBB,
	[0xBC] = opcodeBC,
	[0xBD] = opcodeBD,
	[0xBE] = opcodeBE,
	[0xBF] = opcodeBF,
	[0xC0] = opcodeC0,
	[0xC1] = opcodeC1,
	[0xC2] = opcodeC2,
	[0xC3] = opcodeC3,
	[0xC4] = opcodeC4,
	[0xC5] = opcodeC5,
	[0xC6] = opcodeC6,
	[0xC7] = opcodeC7,
	[0xC8] = opcodeC8,
	[0xC9] = opcodeC9,
	[0xCA] = opcodeCA,
	// [0xCB] = opcodeCB,
	[0xCC] = opcodeCC,
	[0xCD] = opcodeCD,
	[0xCE] = opcodeCE,
	[0xCF] = opcodeCF,
	[0xD0] = opcodeD0,
	[0xD1] = opcodeD1,
	[0xD2] = opcodeD2,
	[0xD3] = 0,
	[0xD4] = opcodeD4,
	[0xD5] = opcodeD5,
	[0xD6] = opcodeD6,
	[0xD7] = opcodeD7,
	[0xD8] = opcodeD8,
	[0xD9] = opcodeD9,
	[0xDA] = opcodeDA,
	[0xDB] = 0,
	[0xDC] = opcodeDC,
	[0xDD] = 0,
	[0xDE] = opcodeDE,
	[0xDF] = opcodeDF,
	[0xE0] = opcodeE0,
	[0xE1] = opcodeE1,
	[0xE2] = opcodeE2,
	[0xE3] = 0,
	[0xE4] = 0,
	[0xE5] = opcodeE5,
	[0xE6] = opcodeE6,
	[0xE7] = opcodeE7,
	[0xE8] = opcodeE8,
	[0xE9] = opcodeE9,
	[0xEA] = opcodeEA,
	[0xEB] = 0,
	[0xEC] = 0,
	[0xED] = 0,
	[0xEE] = opcodeEE,
	[0xEF] = opcodeEF,
	[0xF0] = opcodeF0,
	[0xF1] = opcodeF1,
	[0xF2] = opcodeF2,
	[0xF3] = opcodeF3,
	[0xF4] = 0,
	[0xF5] = opcodeF5,
	[0xF6] = opcodeF6,
	[0xF7] = opcodeF7,
	[0xF8] = opcodeF8,
	// [0xF9] = opcodeF9,
	[0xFA] = opcodeFA,
	[0xFB] = opcodeFB,
	[0xFC] = 0,
	[0xFD] = 0,
	[0xFE] = opcodeFE,
	[0xFF] = opcodeFF
};

void cpu_tick();

#endif

// 01 OK
// 02
// 03 OK
// 04 OK
// 05 OK
// 06 OK
// 07 OK
// 08 OK
// 09 OK
// 10 OK
// 11 OK
#ifndef CPU_H
# define CPU_H

# include "types.h"
# include "memory.h"
# include <unistd.h>
# include <stdio.h>

typedef struct {
	u16 reg16_AF;
	u8* reg8_A;
	u8* reg8_F;

	u16 reg16_BC;
	u8* reg8_B;
	u8* reg8_C;

	u16 reg16_DE;
	u8* reg8_D;
	u8* reg8_E;

	u16 reg16_HL;
	u8* reg8_H;
	u8* reg8_L;

	u16 reg16_SP;
	u8* reg8_S;
	u8* reg8_P;

	u16 reg16_PC;

	u8 IME;
	u8 IME_set_request;

	void (*instruction_to_execute)();
	u8 prefix_instruction;
	u8 instruction_cycles_remain;

} cpu_t;

extern cpu_t cpu;

int init_cpu();

void skip_boot_rom();

void cpu_tick();

void cpu_log();
void cpu_print_status();

typedef enum {
	flag_c = 4,
	flag_h = 5,
	flag_n = 6,
	flag_z = 7
} f_flag;

typedef enum {
	Z,
	NZ,
	C,
	NC
} condition;
int check_condition(condition cc);

typedef enum {
	VBlank = 0,
	LCD = 1,
	Timer = 2,
	Serial = 3,
	Joypad = 4,
	NoInterrupt = 0xFF
} interrupt_flag;


interrupt_flag interrupt_pending();
void interrupt_handler();
void call_interrupt(interrupt_flag flag);
void call_interrupt_VBlank();
void call_interrupt_LCD();
void call_interrupt_Timer();
void call_interrupt_Serial();
void call_interrupt_Joypad();

void NOP();
void opcode00();

void HALT();
void opcode76();

void STOP();
void opcode10();

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

void LD_SPHL();
void opcodeF9();

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

void ADD_HLr16(u8* h, u8* l);
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

void POP_r16(u8* h, u8* l);
void opcodeC1();
void opcodeD1();
void opcodeE1();
void opcodeF1();

void PUSH_r16(u8* h, u8* l);
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

void CALL_n16();
void opcodeCD();

void CALL_ccn16(condition cc);
void opcodeC4();
void opcodeCC();
void opcodeD4();
void opcodeDC();

void RET();
void opcodeC9();

void RET_cc(condition cc);
void opcodeC0();
void opcodeC8();
void opcodeD0();
void opcodeD8();

void RETI();
void opcodeD9();

void RST(u8 vec);
void opcodeC7();
void opcodeCF();
void opcodeD7();
void opcodeDF();
void opcodeE7();
void opcodeEF();
void opcodeF7();
void opcodeFF();

void DDA();
void opcode27();

void RLCA();
void opcode07();

void RRCA();
void opcode0F();

void RLA();
void opcode17();

void RRA();
void opcode1F();

void PREFIX();
void opcodeCB();

void hole(char* str);
void opcodeD3();
void opcodeDB();
void opcodeDD();
void opcodeE3();
void opcodeE4();
void opcodeEB();
void opcodeEC();
void opcodeED();
void opcodeF4();
void opcodeFC();
void opcodeFD();

static void (*instruction_set[0x100])() = {
	[0x00] = opcode00,
	[0x01] = opcode01,
	[0x02] = opcode02,
	[0x03] = opcode03,
	[0x04] = opcode04,
	[0x05] = opcode05,
	[0x06] = opcode06,
	[0x07] = opcode07,
	[0x08] = opcode08,
	[0x09] = opcode09,
	[0x0A] = opcode0A,
	[0x0B] = opcode0B,
	[0x0C] = opcode0C,
	[0x0D] = opcode0D,
	[0x0E] = opcode0E,
	[0x0F] = opcode0F,
	[0x10] = opcode10,
	[0x11] = opcode11,
	[0x12] = opcode12,
	[0x13] = opcode13,
	[0x14] = opcode14,
	[0x15] = opcode15,
	[0x16] = opcode16,
	[0x17] = opcode17,
	[0x18] = opcode18,
	[0x19] = opcode19,
	[0x1A] = opcode1A,
	[0x1B] = opcode1B,
	[0x1C] = opcode1C,
	[0x1D] = opcode1D,
	[0x1E] = opcode1E,
	[0x1F] = opcode1F,
	[0x20] = opcode20,
	[0x21] = opcode21,
	[0x22] = opcode22,
	[0x23] = opcode23,
	[0x24] = opcode24,
	[0x25] = opcode25,
	[0x26] = opcode26,
	[0x27] = opcode27,
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
	[0x76] = opcode76,
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
	[0xCB] = opcodeCB,
	[0xCC] = opcodeCC,
	[0xCD] = opcodeCD,
	[0xCE] = opcodeCE,
	[0xCF] = opcodeCF,
	[0xD0] = opcodeD0,
	[0xD1] = opcodeD1,
	[0xD2] = opcodeD2,
	[0xD3] = opcodeD3,
	[0xD4] = opcodeD4,
	[0xD5] = opcodeD5,
	[0xD6] = opcodeD6,
	[0xD7] = opcodeD7,
	[0xD8] = opcodeD8,
	[0xD9] = opcodeD9,
	[0xDA] = opcodeDA,
	[0xDB] = opcodeDB,
	[0xDC] = opcodeDC,
	[0xDD] = opcodeDD,
	[0xDE] = opcodeDE,
	[0xDF] = opcodeDF,
	[0xE0] = opcodeE0,
	[0xE1] = opcodeE1,
	[0xE2] = opcodeE2,
	[0xE3] = opcodeE3,
	[0xE4] = opcodeE4,
	[0xE5] = opcodeE5,
	[0xE6] = opcodeE6,
	[0xE7] = opcodeE7,
	[0xE8] = opcodeE8,
	[0xE9] = opcodeE9,
	[0xEA] = opcodeEA,
	[0xEB] = opcodeEB,
	[0xEC] = opcodeEC,
	[0xED] = opcodeED,
	[0xEE] = opcodeEE,
	[0xEF] = opcodeEF,
	[0xF0] = opcodeF0,
	[0xF1] = opcodeF1,
	[0xF2] = opcodeF2,
	[0xF3] = opcodeF3,
	[0xF4] = opcodeF4,
	[0xF5] = opcodeF5,
	[0xF6] = opcodeF6,
	[0xF7] = opcodeF7,
	[0xF8] = opcodeF8,
	[0xF9] = opcodeF9,
	[0xFA] = opcodeFA,
	[0xFB] = opcodeFB,
	[0xFC] = opcodeFC,
	[0xFD] = opcodeFD,
	[0xFE] = opcodeFE,
	[0xFF] = opcodeFF
};

void BIT_u3r8(u8 bit, u8* r);
void opcodeCB40();
void opcodeCB41();
void opcodeCB42();
void opcodeCB43();
void opcodeCB44();
void opcodeCB45();
void opcodeCB47();
void opcodeCB48();
void opcodeCB49();
void opcodeCB4A();
void opcodeCB4B();
void opcodeCB4C();
void opcodeCB4D();
void opcodeCB4F();
void opcodeCB50();
void opcodeCB51();
void opcodeCB52();
void opcodeCB53();
void opcodeCB54();
void opcodeCB55();
void opcodeCB57();
void opcodeCB58();
void opcodeCB59();
void opcodeCB5A();
void opcodeCB5B();
void opcodeCB5C();
void opcodeCB5D();
void opcodeCB5F();
void opcodeCB60();
void opcodeCB61();
void opcodeCB62();
void opcodeCB63();
void opcodeCB64();
void opcodeCB65();
void opcodeCB67();
void opcodeCB68();
void opcodeCB69();
void opcodeCB6A();
void opcodeCB6B();
void opcodeCB6C();
void opcodeCB6D();
void opcodeCB6F();
void opcodeCB70();
void opcodeCB71();
void opcodeCB72();
void opcodeCB73();
void opcodeCB74();
void opcodeCB75();
void opcodeCB77();
void opcodeCB78();
void opcodeCB79();
void opcodeCB7A();
void opcodeCB7B();
void opcodeCB7C();
void opcodeCB7D();
void opcodeCB7F();

void BIT_u3HL(u8 bit);
void opcodeCB46();
void opcodeCB4E();
void opcodeCB56();
void opcodeCB5E();
void opcodeCB66();
void opcodeCB6E();
void opcodeCB76();
void opcodeCB7E();

void RES_u3r8(u8 bit, u8* r);
void opcodeCB80();
void opcodeCB81();
void opcodeCB82();
void opcodeCB83();
void opcodeCB84();
void opcodeCB85();
void opcodeCB87();
void opcodeCB88();
void opcodeCB89();
void opcodeCB8A();
void opcodeCB8B();
void opcodeCB8C();
void opcodeCB8D();
void opcodeCB8F();
void opcodeCB90();
void opcodeCB91();
void opcodeCB92();
void opcodeCB93();
void opcodeCB94();
void opcodeCB95();
void opcodeCB97();
void opcodeCB98();
void opcodeCB99();
void opcodeCB9A();
void opcodeCB9B();
void opcodeCB9C();
void opcodeCB9D();
void opcodeCB9F();
void opcodeCBA0();
void opcodeCBA1();
void opcodeCBA2();
void opcodeCBA3();
void opcodeCBA4();
void opcodeCBA5();
void opcodeCBA7();
void opcodeCBA8();
void opcodeCBA9();
void opcodeCBAA();
void opcodeCBAB();
void opcodeCBAC();
void opcodeCBAD();
void opcodeCBAF();
void opcodeCBB0();
void opcodeCBB1();
void opcodeCBB2();
void opcodeCBB3();
void opcodeCBB4();
void opcodeCBB5();
void opcodeCBB7();
void opcodeCBB8();
void opcodeCBB9();
void opcodeCBBA();
void opcodeCBBB();
void opcodeCBBC();
void opcodeCBBD();
void opcodeCBBF();

void RES_u3HL(u8 bit);
void opcodeCB86();
void opcodeCB8E();
void opcodeCB96();
void opcodeCB9E();
void opcodeCBA6();
void opcodeCBAE();
void opcodeCBB6();
void opcodeCBBE();

void SET_u3r8(u8 bit, u8* r);
void opcodeCBC0();
void opcodeCBC1();
void opcodeCBC2();
void opcodeCBC3();
void opcodeCBC4();
void opcodeCBC5();
void opcodeCBC7();
void opcodeCBC8();
void opcodeCBC9();
void opcodeCBCA();
void opcodeCBCB();
void opcodeCBCC();
void opcodeCBCD();
void opcodeCBCF();
void opcodeCBD0();
void opcodeCBD1();
void opcodeCBD2();
void opcodeCBD3();
void opcodeCBD4();
void opcodeCBD5();
void opcodeCBD7();
void opcodeCBD8();
void opcodeCBD9();
void opcodeCBDA();
void opcodeCBDB();
void opcodeCBDC();
void opcodeCBDD();
void opcodeCBDF();
void opcodeCBE0();
void opcodeCBE1();
void opcodeCBE2();
void opcodeCBE3();
void opcodeCBE4();
void opcodeCBE5();
void opcodeCBE7();
void opcodeCBE8();
void opcodeCBE9();
void opcodeCBEA();
void opcodeCBEB();
void opcodeCBEC();
void opcodeCBED();
void opcodeCBEF();
void opcodeCBF0();
void opcodeCBF1();
void opcodeCBF2();
void opcodeCBF3();
void opcodeCBF4();
void opcodeCBF5();
void opcodeCBF7();
void opcodeCBF8();
void opcodeCBF9();
void opcodeCBFA();
void opcodeCBFB();
void opcodeCBFC();
void opcodeCBFD();
void opcodeCBFF();

void SET_u3HL(u8 bit);
void opcodeCBC6();
void opcodeCBCE();
void opcodeCBD6();
void opcodeCBDE();
void opcodeCBE6();
void opcodeCBEE();
void opcodeCBF6();
void opcodeCBFE();

void RL_r8(u8* r);
void opcodeCB10();
void opcodeCB11();
void opcodeCB12();
void opcodeCB13();
void opcodeCB14();
void opcodeCB15();
void opcodeCB17();

void RL_HL();
void opcodeCB16();

void RR_r8(u8* r);
void opcodeCB18();
void opcodeCB19();
void opcodeCB1A();
void opcodeCB1B();
void opcodeCB1C();
void opcodeCB1D();
void opcodeCB1F();

void RR_HL();
void opcodeCB1E();

void RLC_r8(u8* r);
void opcodeCB00();
void opcodeCB01();
void opcodeCB02();
void opcodeCB03();
void opcodeCB04();
void opcodeCB05();
void opcodeCB07();

void RLC_HL();
void opcodeCB06();

void RRC_r8(u8* r);
void opcodeCB08();
void opcodeCB09();
void opcodeCB0A();
void opcodeCB0B();
void opcodeCB0C();
void opcodeCB0D();
void opcodeCB0F();

void RRC_HL();
void opcodeCB0E();

void SLA_r8(u8* r);
void opcodeCB20();
void opcodeCB21();
void opcodeCB22();
void opcodeCB23();
void opcodeCB24();
void opcodeCB25();
void opcodeCB27();

void SLA_HL();
void opcodeCB26();

void SRA_r8(u8* r);
void opcodeCB28();
void opcodeCB29();
void opcodeCB2A();
void opcodeCB2B();
void opcodeCB2C();
void opcodeCB2D();
void opcodeCB2F();

void SRA_HL();
void opcodeCB2E();

void SRL_r8(u8* r);
void opcodeCB38();
void opcodeCB39();
void opcodeCB3A();
void opcodeCB3B();
void opcodeCB3C();
void opcodeCB3D();
void opcodeCB3F();

void SRL_HL();
void opcodeCB3E();

void SWAP_r8(u8* r);
void opcodeCB30();
void opcodeCB31();
void opcodeCB32();
void opcodeCB33();
void opcodeCB34();
void opcodeCB35();
void opcodeCB37();

void SWAP_HL();
void opcodeCB36();

static void (*CB_set[0x100])() = {
	[0x00] = opcodeCB00,
	[0x01] = opcodeCB01,
	[0x02] = opcodeCB02,
	[0x03] = opcodeCB03,
	[0x04] = opcodeCB04,
	[0x05] = opcodeCB05,
	[0x06] = opcodeCB06,
	[0x07] = opcodeCB07,
	[0x08] = opcodeCB08,
	[0x09] = opcodeCB09,
	[0x0A] = opcodeCB0A,
	[0x0B] = opcodeCB0B,
	[0x0C] = opcodeCB0C,
	[0x0D] = opcodeCB0D,
	[0x0E] = opcodeCB0E,
	[0x0F] = opcodeCB0F,
	[0x10] = opcodeCB10,
	[0x11] = opcodeCB11,
	[0x12] = opcodeCB12,
	[0x13] = opcodeCB13,
	[0x14] = opcodeCB14,
	[0x15] = opcodeCB15,
	[0x16] = opcodeCB16,
	[0x17] = opcodeCB17,
	[0x18] = opcodeCB18,
	[0x19] = opcodeCB19,
	[0x1A] = opcodeCB1A,
	[0x1B] = opcodeCB1B,
	[0x1C] = opcodeCB1C,
	[0x1D] = opcodeCB1D,
	[0x1E] = opcodeCB1E,
	[0x1F] = opcodeCB1F,
	[0x20] = opcodeCB20,
	[0x21] = opcodeCB21,
	[0x22] = opcodeCB22,
	[0x23] = opcodeCB23,
	[0x24] = opcodeCB24,
	[0x25] = opcodeCB25,
	[0x26] = opcodeCB26,
	[0x27] = opcodeCB27,
	[0x28] = opcodeCB28,
	[0x29] = opcodeCB29,
	[0x2A] = opcodeCB2A,
	[0x2B] = opcodeCB2B,
	[0x2C] = opcodeCB2C,
	[0x2D] = opcodeCB2D,
	[0x2E] = opcodeCB2E,
	[0x2F] = opcodeCB2F,
	[0x30] = opcodeCB30,
	[0x31] = opcodeCB31,
	[0x32] = opcodeCB32,
	[0x33] = opcodeCB33,
	[0x34] = opcodeCB34,
	[0x35] = opcodeCB35,
	[0x36] = opcodeCB36,
	[0x37] = opcodeCB37,
	[0x38] = opcodeCB38,
	[0x39] = opcodeCB39,
	[0x3A] = opcodeCB3A,
	[0x3B] = opcodeCB3B,
	[0x3C] = opcodeCB3C,
	[0x3D] = opcodeCB3D,
	[0x3E] = opcodeCB3E,
	[0x3F] = opcodeCB3F,
	[0x40] = opcodeCB40,
	[0x41] = opcodeCB41,
	[0x42] = opcodeCB42,
	[0x43] = opcodeCB43,
	[0x44] = opcodeCB44,
	[0x45] = opcodeCB45,
	[0x46] = opcodeCB46,
	[0x47] = opcodeCB47,
	[0x48] = opcodeCB48,
	[0x49] = opcodeCB49,
	[0x4A] = opcodeCB4A,
	[0x4B] = opcodeCB4B,
	[0x4C] = opcodeCB4C,
	[0x4D] = opcodeCB4D,
	[0x4E] = opcodeCB4E,
	[0x4F] = opcodeCB4F,
	[0x50] = opcodeCB50,
	[0x51] = opcodeCB51,
	[0x52] = opcodeCB52,
	[0x53] = opcodeCB53,
	[0x54] = opcodeCB54,
	[0x55] = opcodeCB55,
	[0x56] = opcodeCB56,
	[0x57] = opcodeCB57,
	[0x58] = opcodeCB58,
	[0x59] = opcodeCB59,
	[0x5A] = opcodeCB5A,
	[0x5B] = opcodeCB5B,
	[0x5C] = opcodeCB5C,
	[0x5D] = opcodeCB5D,
	[0x5E] = opcodeCB5E,
	[0x5F] = opcodeCB5F,
	[0x60] = opcodeCB60,
	[0x61] = opcodeCB61,
	[0x62] = opcodeCB62,
	[0x63] = opcodeCB63,
	[0x64] = opcodeCB64,
	[0x65] = opcodeCB65,
	[0x66] = opcodeCB66,
	[0x67] = opcodeCB67,
	[0x68] = opcodeCB68,
	[0x69] = opcodeCB69,
	[0x6A] = opcodeCB6A,
	[0x6B] = opcodeCB6B,
	[0x6C] = opcodeCB6C,
	[0x6D] = opcodeCB6D,
	[0x6E] = opcodeCB6E,
	[0x6F] = opcodeCB6F,
	[0x70] = opcodeCB70,
	[0x71] = opcodeCB71,
	[0x72] = opcodeCB72,
	[0x73] = opcodeCB73,
	[0x74] = opcodeCB74,
	[0x75] = opcodeCB75,
	[0x76] = opcodeCB76,
	[0x77] = opcodeCB77,
	[0x78] = opcodeCB78,
	[0x79] = opcodeCB79,
	[0x7A] = opcodeCB7A,
	[0x7B] = opcodeCB7B,
	[0x7C] = opcodeCB7C,
	[0x7D] = opcodeCB7D,
	[0x7E] = opcodeCB7E,
	[0x7F] = opcodeCB7F,
	[0x80] = opcodeCB80,
	[0x81] = opcodeCB81,
	[0x82] = opcodeCB82,
	[0x83] = opcodeCB83,
	[0x84] = opcodeCB84,
	[0x85] = opcodeCB85,
	[0x86] = opcodeCB86,
	[0x87] = opcodeCB87,
	[0x88] = opcodeCB88,
	[0x89] = opcodeCB89,
	[0x8A] = opcodeCB8A,
	[0x8B] = opcodeCB8B,
	[0x8C] = opcodeCB8C,
	[0x8D] = opcodeCB8D,
	[0x8E] = opcodeCB8E,
	[0x8F] = opcodeCB8F,
	[0x90] = opcodeCB90,
	[0x91] = opcodeCB91,
	[0x92] = opcodeCB92,
	[0x93] = opcodeCB93,
	[0x94] = opcodeCB94,
	[0x95] = opcodeCB95,
	[0x96] = opcodeCB96,
	[0x97] = opcodeCB97,
	[0x98] = opcodeCB98,
	[0x99] = opcodeCB99,
	[0x9A] = opcodeCB9A,
	[0x9B] = opcodeCB9B,
	[0x9C] = opcodeCB9C,
	[0x9D] = opcodeCB9D,
	[0x9E] = opcodeCB9E,
	[0x9F] = opcodeCB9F,
	[0xA0] = opcodeCBA0,
	[0xA1] = opcodeCBA1,
	[0xA2] = opcodeCBA2,
	[0xA3] = opcodeCBA3,
	[0xA4] = opcodeCBA4,
	[0xA5] = opcodeCBA5,
	[0xA6] = opcodeCBA6,
	[0xA7] = opcodeCBA7,
	[0xA8] = opcodeCBA8,
	[0xA9] = opcodeCBA9,
	[0xAA] = opcodeCBAA,
	[0xAB] = opcodeCBAB,
	[0xAC] = opcodeCBAC,
	[0xAD] = opcodeCBAD,
	[0xAE] = opcodeCBAE,
	[0xAF] = opcodeCBAF,
	[0xB0] = opcodeCBB0,
	[0xB1] = opcodeCBB1,
	[0xB2] = opcodeCBB2,
	[0xB3] = opcodeCBB3,
	[0xB4] = opcodeCBB4,
	[0xB5] = opcodeCBB5,
	[0xB6] = opcodeCBB6,
	[0xB7] = opcodeCBB7,
	[0xB8] = opcodeCBB8,
	[0xB9] = opcodeCBB9,
	[0xBA] = opcodeCBBA,
	[0xBB] = opcodeCBBB,
	[0xBC] = opcodeCBBC,
	[0xBD] = opcodeCBBD,
	[0xBE] = opcodeCBBE,
	[0xBF] = opcodeCBBF,
	[0xC0] = opcodeCBC0,
	[0xC1] = opcodeCBC1,
	[0xC2] = opcodeCBC2,
	[0xC3] = opcodeCBC3,
	[0xC4] = opcodeCBC4,
	[0xC5] = opcodeCBC5,
	[0xC6] = opcodeCBC6,
	[0xC7] = opcodeCBC7,
	[0xC8] = opcodeCBC8,
	[0xC9] = opcodeCBC9,
	[0xCA] = opcodeCBCA,
	[0xCB] = opcodeCBCB,
	[0xCC] = opcodeCBCC,
	[0xCD] = opcodeCBCD,
	[0xCE] = opcodeCBCE,
	[0xCF] = opcodeCBCF,
	[0xD0] = opcodeCBD0,
	[0xD1] = opcodeCBD1,
	[0xD2] = opcodeCBD2,
	[0xD3] = opcodeCBD3,
	[0xD4] = opcodeCBD4,
	[0xD5] = opcodeCBD5,
	[0xD6] = opcodeCBD6,
	[0xD7] = opcodeCBD7,
	[0xD8] = opcodeCBD8,
	[0xD9] = opcodeCBD9,
	[0xDA] = opcodeCBDA,
	[0xDB] = opcodeCBDB,
	[0xDC] = opcodeCBDC,
	[0xDD] = opcodeCBDD,
	[0xDE] = opcodeCBDE,
	[0xDF] = opcodeCBDF,
	[0xE0] = opcodeCBE0,
	[0xE1] = opcodeCBE1,
	[0xE2] = opcodeCBE2,
	[0xE3] = opcodeCBE3,
	[0xE4] = opcodeCBE4,
	[0xE5] = opcodeCBE5,
	[0xE6] = opcodeCBE6,
	[0xE7] = opcodeCBE7,
	[0xE8] = opcodeCBE8,
	[0xE9] = opcodeCBE9,
	[0xEA] = opcodeCBEA,
	[0xEB] = opcodeCBEB,
	[0xEC] = opcodeCBEC,
	[0xED] = opcodeCBED,
	[0xEE] = opcodeCBEE,
	[0xEF] = opcodeCBEF,
	[0xF0] = opcodeCBF0,
	[0xF1] = opcodeCBF1,
	[0xF2] = opcodeCBF2,
	[0xF3] = opcodeCBF3,
	[0xF4] = opcodeCBF4,
	[0xF5] = opcodeCBF5,
	[0xF6] = opcodeCBF6,
	[0xF7] = opcodeCBF7,
	[0xF8] = opcodeCBF8,
	[0xF9] = opcodeCBF9,
	[0xFA] = opcodeCBFA,
	[0xFB] = opcodeCBFB,
	[0xFC] = opcodeCBFC,
	[0xFD] = opcodeCBFD,
	[0xFE] = opcodeCBFE,
	[0xFF] = opcodeCBFF
};

static const char* opcode_decode[0x100] = {
	[0x00] = "NOP",
	[0x01] = "LD BC, n16",
	[0x02] = "LD [BC], A",
	[0x03] = "INC BC",
	[0x04] = "INC B",
	[0x05] = "DEC B",
	[0x06] = "LD B, n8",
	[0x07] = "RLCA",
	[0x08] = "LD [a16], SP",
	[0x09] = "ADD HL, BC",
	[0x0A] = "LD A, [BC]",
	[0x0B] = "DEC BC",
	[0x0C] = "INC C",
	[0x0D] = "DEC C",
	[0x0E] = "LD C, n8",
	[0x0F] = "RRCA",
	[0x10] = "STOP n8",
	[0x11] = "LD DE, n16",
	[0x12] = "LD [DE], A",
	[0x13] = "INC DE",
	[0x14] = "INC D",
	[0x15] = "DEC D",
	[0x16] = "LD D, n8",
	[0x17] = "RLA",
	[0x18] = "JR e8",
	[0x19] = "ADD HL, DE",
	[0x1A] = "LD A, [DE]",
	[0x1B] = "DEC DE",
	[0x1C] = "INC E",
	[0x1D] = "DEC E",
	[0x1E] = "LD E, n8",
	[0x1F] = "RRA",
	[0x20] = "JR NZ, e8",
	[0x21] = "LD HL, n16",
	[0x22] = "LD [HL+], A",
	[0x23] = "INC HL",
	[0x24] = "INC H",
	[0x25] = "DEC H",
	[0x26] = "LD H, n8",
	[0x27] = "DAA",
	[0x28] = "JR Z, e8",
	[0x29] = "ADD HL, HL",
	[0x2A] = "LD A, [HL+]",
	[0x2B] = "DEC HL",
	[0x2C] = "INC L",
	[0x2D] = "DEC L",
	[0x2E] = "LD L, n8",
	[0x2F] = "CPL",
	[0x30] = "JR NC, e8",
	[0x31] = "LD SP, n16",
	[0x32] = "LD [HL-], A",
	[0x33] = "INC SP",
	[0x34] = "INC [HL]",
	[0x35] = "DEC [HL]",
	[0x36] = "LD [HL], n8",
	[0x37] = "SCF",
	[0x38] = "JR C, e8",
	[0x39] = "ADD HL, SP",
	[0x3A] = "LD A, [HL-]",
	[0x3B] = "DEC SP",
	[0x3C] = "INC A",
	[0x3D] = "DEC A",
	[0x3E] = "LD A, n8",
	[0x3F] = "CCF",
	[0x40] = "LD B, B",
	[0x41] = "LD B, C",
	[0x42] = "LD B, D",
	[0x43] = "LD B, E",
	[0x44] = "LD B, H",
	[0x45] = "LD B, L",
	[0x46] = "LD B, [HL]",
	[0x47] = "LD B, A",
	[0x48] = "LD C, B",
	[0x49] = "LD C, C",
	[0x4A] = "LD C, D",
	[0x4B] = "LD C, E",
	[0x4C] = "LD C, H",
	[0x4D] = "LD C, L",
	[0x4E] = "LD C, [HL]",
	[0x4F] = "LD C, A",
	[0x50] = "LD D, B",
	[0x51] = "LD D, C",
	[0x52] = "LD D, D",
	[0x53] = "LD D, E",
	[0x54] = "LD D, H",
	[0x55] = "LD D, L",
	[0x56] = "LD D, [HL]",
	[0x57] = "LD D, A",
	[0x58] = "LD E, B",
	[0x59] = "LD E, C",
	[0x5A] = "LD E, D",
	[0x5B] = "LD E, E",
	[0x5C] = "LD E, H",
	[0x5D] = "LD E, L",
	[0x5E] = "LD E, [HL]",
	[0x5F] = "LD E, A",
	[0x60] = "LD H, B",
	[0x61] = "LD H, C",
	[0x62] = "LD H, D",
	[0x63] = "LD H, E",
	[0x64] = "LD H, H",
	[0x65] = "LD H, L",
	[0x66] = "LD H, [HL]",
	[0x67] = "LD H, A",
	[0x68] = "LD L, B",
	[0x69] = "LD L, C",
	[0x6A] = "LD L, D",
	[0x6B] = "LD L, E",
	[0x6C] = "LD L, H",
	[0x6D] = "LD L, L",
	[0x6E] = "LD L, [HL]",
	[0x6F] = "LD L, A",
	[0x70] = "LD [HL], B",
	[0x71] = "LD [HL], C",
	[0x72] = "LD [HL], D",
	[0x73] = "LD [HL], E",
	[0x74] = "LD [HL], H",
	[0x75] = "LD [HL], L",
	[0x76] = "HALT",
	[0x77] = "LD [HL], A",
	[0x78] = "LD A, B",
	[0x79] = "LD A, C",
	[0x7A] = "LD A, D",
	[0x7B] = "LD A, E",
	[0x7C] = "LD A, H",
	[0x7D] = "LD A, L",
	[0x7E] = "LD A, [HL]",
	[0x7F] = "LD A, A",
	[0x80] = "ADD A, B",
	[0x81] = "ADD A, C",
	[0x82] = "ADD A, D",
	[0x83] = "ADD A, E",
	[0x84] = "ADD A, H",
	[0x85] = "ADD A, L",
	[0x86] = "ADD A, [HL]",
	[0x87] = "ADD A, A",
	[0x88] = "ADC A, B",
	[0x89] = "ADC A, C",
	[0x8A] = "ADC A, D",
	[0x8B] = "ADC A, E",
	[0x8C] = "ADC A, H",
	[0x8D] = "ADC A, L",
	[0x8E] = "ADC A, [HL]",
	[0x8F] = "ADC A, A",
	[0x90] = "SUB A, B",
	[0x91] = "SUB A, C",
	[0x92] = "SUB A, D",
	[0x93] = "SUB A, E",
	[0x94] = "SUB A, H",
	[0x95] = "SUB A, L",
	[0x96] = "SUB A, [HL]",
	[0x97] = "SUB A, A",
	[0x98] = "SBC A, B",
	[0x99] = "SBC A, C",
	[0x9A] = "SBC A, D",
	[0x9B] = "SBC A, E",
	[0x9C] = "SBC A, H",
	[0x9D] = "SBC A, L",
	[0x9E] = "SBC A, [HL]",
	[0x9F] = "SBC A, A",
	[0xA0] = "AND A, B",
	[0xA1] = "AND A, C",
	[0xA2] = "AND A, D",
	[0xA3] = "AND A, E",
	[0xA4] = "AND A, H",
	[0xA5] = "AND A, L",
	[0xA6] = "AND A, [HL]",
	[0xA7] = "AND A, A",
	[0xA8] = "XOR A, B",
	[0xA9] = "XOR A, C",
	[0xAA] = "XOR A, D",
	[0xAB] = "XOR A, E",
	[0xAC] = "XOR A, H",
	[0xAD] = "XOR A, L",
	[0xAE] = "XOR A, [HL]",
	[0xAF] = "XOR A, A",
	[0xB0] = "OR A, B",
	[0xB1] = "OR A, C",
	[0xB2] = "OR A, D",
	[0xB3] = "OR A, E",
	[0xB4] = "OR A, H",
	[0xB5] = "OR A, L",
	[0xB6] = "OR A, [HL]",
	[0xB7] = "OR A, A",
	[0xB8] = "CP A, B",
	[0xB9] = "CP A, C",
	[0xBA] = "CP A, D",
	[0xBB] = "CP A, E",
	[0xBC] = "CP A, H",
	[0xBD] = "CP A, L",
	[0xBE] = "CP A, [HL]",
	[0xBF] = "CP A, A",
	[0xC0] = "RET NZ",
	[0xC1] = "POP BC",
	[0xC2] = "JP NZ, a16",
	[0xC3] = "JP a16",
	[0xC4] = "CALL NZ, a16",
	[0xC5] = "PUSH BC",
	[0xC6] = "ADD A, n8",
	[0xC7] = "RST $00",
	[0xC8] = "RET Z",
	[0xC9] = "RET",
	[0xCA] = "JP Z, a16",
	[0xCB] = "PREFIX",
	[0xCC] = "CALL Z, a16",
	[0xCD] = "CALL a16",
	[0xCE] = "ADC A, n8",
	[0xCF] = "RST $08",
	[0xD0] = "RET NC",
	[0xD1] = "POP DE",
	[0xD2] = "JP NC, a16",
	[0xD3] = "hardlock",
	[0xD4] = "CALL NC, a16",
	[0xD5] = "PUSH DE",
	[0xD6] = "SUB A, n8",
	[0xD7] = "RST $10",
	[0xD8] = "RET C",
	[0xD9] = "RETI",
	[0xDA] = "JP C, a16",
	[0xDB] = "hardlock",
	[0xDC] = "CALL C, a16",
	[0xDD] = "hardlock",
	[0xDE] = "SBC A, n8",
	[0xDF] = "RST $18",
	[0xE0] = "LDH [a8], A",
	[0xE1] = "POP HL",
	[0xE2] = "LDH [C], A",
	[0xE3] = "hardlock",
	[0xE4] = "hardlock",
	[0xE5] = "PUSH HL",
	[0xE6] = "AND A, n8",
	[0xE7] = "RST $20",
	[0xE8] = "ADD SP, e8",
	[0xE9] = "JP HL",
	[0xEA] = "LD [a16], A",
	[0xEB] = "hardlock",
	[0xEC] = "hardlock",
	[0xED] = "hardlock",
	[0xEE] = "XOR A, n8",
	[0xEF] = "RST $28",
	[0xF0] = "LDH A, [a8]",
	[0xF1] = "POP AF",
	[0xF2] = "LDH A, [C]",
	[0xF3] = "DI",
	[0xF4] = "hardlock",
	[0xF5] = "PUSH AF",
	[0xF6] = "OR A, n8",
	[0xF7] = "RST $30",
	[0xF8] = "LD HL, SP + e8",
	[0xF9] = "LD SP, HL",
	[0xFA] = "LD A, [a16]",
	[0xFB] = "EI",
	[0xFC] = "hardlock",
	[0xFD] = "hardlock",
	[0xFE] = "CP A, n8",
	[0xFF] = "RST $38"
};

#endif

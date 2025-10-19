#ifndef CARTRIDGE_H
# define CARTRIDGE_H

# include "types.h"
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

typedef struct {
	u8 selected_rom1_bank;
	u8 selected_rom2_bank;
	u8 selected_ram_bank;
	u8 rom_bank_number_mask;
	int ram_enable;

	void (*write_mbc)(u16, u8);
} mbc_t;

mbc_t* no_mbc();

typedef struct {
	u8 selected_rom1_bank;
	u8 selected_rom2_bank;
	u8 selected_ram_bank;
	u8 rom_bank_number_mask;
	int ram_enable;

	void (*write_mbc)(u16, u8);

	u8 reg_rom_bank_number;		// lower 5 bits
	u8 reg_rom_ram_bank_number;	// 2 bits
	u8 reg_rom_ram_mode_select;	// 1 bit

} mbc1_t;

mbc_t* new_mbc1();

typedef struct {
	u8 selected_rom1_bank;
	u8 selected_rom2_bank;
	u8 selected_ram_bank;
	u8 rom_bank_number_mask;
	int ram_enable;

	void (*write_mbc)(u16, u8);

	u8 reg_ram_enable_rom_bank_number;	// 8 bits

} mbc2_t;

mbc_t* new_mbc2();

typedef struct {
	u8 selected_rom1_bank;
	u8 selected_rom2_bank;
	u8 selected_ram_bank;
	u8 rom_bank_number_mask;
	int ram_enable;	// also timer enable

	void (*write_mbc)(u16, u8);

	u8 reg_rtc;
	u8 reg_rom_bank_number;
	u8 reg_ram_bank_number_rtc_register_select;
	u8 reg_latch_clock_data;

} mbc3_t;

mbc_t* new_mbc3();

typedef struct {

} mbc5_t;

typedef struct {
	u8* rom;
	u8* ram;
	mbc_t* mbc;
	int battery;

	// entry point				// 0100 - 0103
	u8 nintendo_logo[48];		// 0104 - 0133
	u8 title[16];				// 0134 - 0143
	u8 manufactor_code[4];		// 013F - 0142
	u8 cgb_flag;				// 0143
	u8 licensee_code_new[2];	// 0144 - 0145
	u8 sgb_flag;				// 0146
	u8 cartridge_type;			// 0147
	u8 rom_size;				// 0148
	u8 ram_size;				// 0149
	u8 destination_code;		// 014A
	u8 licensee_code_old;		// 014B
	u8 rom_version_number;		// 014C
	u8 header_checksum;			// 014D
	u16 global_checksum;		// 014E-014F
} cartridge_t;

extern cartridge_t cartridge;

void write_mbc0(u16 address, u8 val);

void write_mbc1(u16 address, u8 val);

void write_mbc2(u16 address, u8 val);

void write_mbc3(u16 address, u8 val);

void update_banks_mbc1();

void update_banks_mbc3();

static const char* cartridge_types[0x100] = {
	[0x00] = "ROM ONLY",
	[0x01] = "MBC1",
	[0x02] = "MBC1+RAM",
	[0x03] = "MBC1+RAM+BATTERY",
	[0x05] = "MBC2",
	[0x06] = "MBC2+BATTERY",
	[0x08] = "ROM+RAM",
	[0x09] = "ROM+RAM+BATTERY",
	[0x0B] = "MMM01",
	[0x0C] = "MMM01+RAM",
	[0x0D] = "MMM01+RAM+BATTERY",
	[0x0F] = "MBC3+TIMER+BATTERY",
	[0x10] = "MBC3+TIMER+RAM+BATTERY",
	[0x11] = "MBC3",
	[0x12] = "MBC3+RAM",
	[0x13] = "MBC3+RAM+BATTERY",
	[0x19] = "MBC5",
	[0x1A] = "MBC5+RAM",
	[0x1B] = "MBC5+RAM+BATTERY",
	[0x1C] = "MBC5+RUMBLE",
	[0x1D] = "MBC5+RUMBLE+RAM",
	[0x1E] = "MBC5+RUMBLE+RAM+BATTERY",
	[0x20] = "MBC6",
	[0x22] = "MBC7+SENSOR+RUMBLE+RAM+BATTERY",
	[0xFC] = "POCKET CAMERA",
	[0xFD] = "BANDAI TAMA5",
	[0xFE] = "HuC3",
	[0xFF] = "HuC1+RAM+BATTERY"
};

static const char* rom_sizes[0x100] = {
	[0x00] = "32 KiB",
	[0x01] = "64 KiB",
	[0x02] = "128 KiB",
	[0x03] = "256 KiB",
	[0x04] = "512 KiB",
	[0x05] = "1 MiB",
	[0x06] = "2 MiB",
	[0x07] = "4 MiB",
	[0x08] = "8 MiB"
};

static const char* ram_sizes[0x100] = {
	[0x00] = "0",
	[0x02] = "8 KiB",
	[0x03] = "32 KiB",
	[0x04] = "128 KiB",
	[0x05] = "64 KiB"
};

static const char* destination_codes[0x100] = {
	[0x00] = "Japan",
	[0x01] = "Overseas"
};

static const char* old_licensee_codes[0x100] = {
	[0x00] = "None",
	[0x01] = "Nintendo",
	[0x08] = "Capcom",
	[0x09] = "HOT-B",
	[0x0A] = "Jaleco",
	[0x0B] = "Coconuts Japan",
	[0x0C] = "Elite Systems",
	[0x13] = "EA (Electronic Arts)",
	[0x18] = "Hudson Soft",
	[0x19] = "ITC Entertainment",
	[0x1A] = "Yanoman",
	[0x1D] = "Japan Clary",
	[0x1F] = "Virgin Games Ltd.",
	[0x24] = "PCM Complete",
	[0x25] = "San-X",
	[0x28] = "Kemco",
	[0x29] = "SETA Corporation",
	[0x30] = "Infogrames",
	[0x31] = "Nintendo",
	[0x32] = "Bandai",
	[0x33] = 0,
	[0x34] = "Konami",
	[0x35] = "HectorSoft",
	[0x38] = "Capcom",
	[0x39] = "Banpresto",
	[0x3C] = "Entertainment Interactive",
	[0x3E] = "Gremlin",
	[0x41] = "Ubi Soft",
	[0x42] = "Atlus",
	[0x44] = "Malibu Interactive",
	[0x46] = "Angel",
	[0x47] = "Spectrum HoloByte",
	[0x49] = "Irem",
	[0x4A] = "Virgin Games Ltd.",
	[0x4D] = "Malibu Interactive",
	[0x4F] = "U.S. Gold",
	[0x50] = "Absolute",
	[0x51] = "Acclaim Entertainment",
	[0x52] = "Activision",
	[0x53] = "Sammy USA Corporation",
	[0x54] = "GameTek",
	[0x55] = "Park Place",
	[0x56] = "LJN",
	[0x57] = "Matchbox",
	[0x59] = "Milton Bradley Company",
	[0x5A] = "Mindscape",
	[0x5B] = "Romstar",
	[0x5C] = "Naxat Soft",
	[0x5D] = "Tradewest",
	[0x60] = "Titus Interactive",
	[0x61] = "Virgin Games Ltd.",
	[0x67] = "Ocean Software",
	[0x69] = "EA (Electronic Arts)",
	[0x6E] = "Elite Systems",
	[0x6F] = "Electro Brain",
	[0x70] = "Infogrames",
	[0x71] = "Interplay Entertainment",
	[0x72] = "Broderbund",
	[0x73] = "Sculptured Software",
	[0x75] = "The Sales Curve Limited",
	[0x78] = "THQ",
	[0x79] = "Accolade",
	[0x7A] = "Triffix Entertainment",
	[0x7C] = "MicroProse",
	[0x7F] = "Kemco",
	[0x80] = "Misawa Entertainment",
	[0x83] = "LOZC G.",
	[0x86] = "Tokuma Shoten",
	[0x8B] = "Bullet-Proof Software",
	[0x8C] = "Vic Tokai Corp.",
	[0x8E] = "Ape Inc.",
	[0x8F] = "I'Max",
	[0x91] = "Chunsoft Co.",
	[0x92] = "Video System",
	[0x93] = "Tsubaraya Productions",
	[0x95] = "Varie",
	[0x96] = "Yonezawa/S'Pal",
	[0x97] = "Kemco",
	[0x99] = "Arc",
	[0x9A] = "Nihon Bussan",
	[0x9B] = "Tecmo",
	[0x9C] = "Imagineer",
	[0x9E] = "Banpresto",
	[0x9F] = "Nova",
	[0xA1] = "Hori Electric",
	[0xA2] = "Bandai",
	[0xA4] = "Konami",
	[0xA6] = "Kawada",
	[0xA7] = "Takara",
	[0xA9] = "Technos Japan",
	[0xAA] = "Broderbund",
	[0xAC] = "Toei Animation",
	[0xAD] = "Toho",
	[0xAF] = "Namco",
	[0xB0] = "Acclaim Entertainment",
	[0xB1] = "ASCII Corporation",
	[0xB2] = "Bandai",
	[0xB4] = "Square Enix",
	[0xB6] = "HAL Laboratory",
	[0xB7] = "SNK",
	[0xB9] = "Pony Canyon",
	[0xBA] = "Culture Brain",
	[0xBB] = "Sunsoft",
	[0xBD] = "Sony Imagesoft",
	[0xBF] = "Sammy Corporation",
	[0xC0] = "Taito",
	[0xC2] = "Kemco",
	[0xC3] = "Square",
	[0xC4] = "Tokuma Shoten",
	[0xC5] = "Data East",
	[0xC6] = "Tonkin House",
	[0xC8] = "Koei",
	[0xC9] = "UFL",
	[0xCA] = "Ultra Games",
	[0xCB] = "VAP, Inc.",
	[0xCC] = "Use Corporation",
	[0xCD] = "Meldac",
	[0xCE] = "Pony Canyon",
	[0xCF] = "Angel",
	[0xD0] = "Taito",
	[0xD1] = "SOFEL (Software Engineering Lab)",
	[0xD2] = "Quest",
	[0xD3] = "Sigma Enterprises",
	[0xD4] = "ASK Kodansha Co.",
	[0xD6] = "Naxat Soft",
	[0xD7] = "Copya System",
	[0xD9] = "Banpresto",
	[0xDA] = "Tomy",
	[0xDB] = "LJN",
	[0xDD] = "Nippon Computer Systems",
	[0xDE] = "Human Ent.",
	[0xDF] = "Altron",
	[0xE0] = "Jaleco",
	[0xE1] = "Towa Chiki",
	[0xE2] = "Yutaka",
	[0xE3] = "Varie",
	[0xE5] = "Epoch",
	[0xE7] = "Athena",
	[0xE8] = "Asmik Ace Entertainment",
	[0xE9] = "Natsume",
	[0xEA] = "King Records",
	[0xEB] = "Atlus",
	[0xEC] = "Epic/Sony Records",
	[0xEE] = "IGS",
	[0xF0] = "A Wave",
	[0xF3] = "Extreme Entertainment",
	[0xFF] = "LJN"
};

int read_rom(const char* path);

int init_mbc();

#endif

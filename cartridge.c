#include "cartridge.h"
cartridge_t cartridge;

const char* cartridge_types[0x100];
const char* rom_sizes[0x100];
const char* ram_sizes[0x100];
const char* destination_codes[0x100];
const char* old_licensee_codes[0x100];

void init_labels()
{
	cartridge_types[0x00] = "ROM ONLY";
	cartridge_types[0x01] = "MBC1";
	cartridge_types[0x02] = "MBC1+RAM";
	cartridge_types[0x03] = "MBC1+RAM+BATTERY";
	cartridge_types[0x05] = "MBC2";
	cartridge_types[0x06] = "MBC2+BATTERY";
	cartridge_types[0x08] = "ROM+RAM";
	cartridge_types[0x09] = "ROM+RAM+BATTERY";
	cartridge_types[0x0B] = "MMM01";
	cartridge_types[0x0C] = "MMM01+RAM";
	cartridge_types[0x0D] = "MMM01+RAM+BATTERY";
	cartridge_types[0x0F] = "MBC3+TIMER+BATTERY";
	cartridge_types[0x10] = "MBC3+TIMER+RAM+BATTERY";
	cartridge_types[0x11] = "MBC3";
	cartridge_types[0x12] = "MBC3+RAM";
	cartridge_types[0x13] = "MBC3+RAM+BATTERY";
	cartridge_types[0x19] = "MBC5";
	cartridge_types[0x1A] = "MBC5+RAM";
	cartridge_types[0x1B] = "MBC5+RAM+BATTERY";
	cartridge_types[0x1C] = "MBC5+RUMBLE";
	cartridge_types[0x1D] = "MBC5+RUMBLE+RAM";
	cartridge_types[0x1E] = "MBC5+RUMBLE+RAM+BATTERY";
	cartridge_types[0x20] = "MBC6";
	cartridge_types[0x22] = "MBC7+SENSOR+RUMBLE+RAM+BATTERY";
	cartridge_types[0xFC] = "POCKET CAMERA";
	cartridge_types[0xFD] = "BANDAI TAMA5";
	cartridge_types[0xFE] = "HuC3";
	cartridge_types[0xFF] = "HuC1+RAM+BATTERY";

	rom_sizes[0x00] = "32 KiB";
	rom_sizes[0x01] = "64 KiB";
	rom_sizes[0x02] = "128 KiB";
	rom_sizes[0x03] = "256 KiB";
	rom_sizes[0x04] = "512 KiB";
	rom_sizes[0x05] = "1 MiB";
	rom_sizes[0x06] = "2 MiB";
	rom_sizes[0x07] = "4 MiB";
	rom_sizes[0x08] = "8 MiB";

	ram_sizes[0x00] = "0";
	ram_sizes[0x02] = "8 KiB";
	ram_sizes[0x03] = "32 KiB";
	ram_sizes[0x04] = "128 KiB";
	ram_sizes[0x05] = "64 KiB";

	destination_codes[0x100];
	destination_codes[0x00] = "Japan";
	destination_codes[0x01] = "Overseas";

	old_licensee_codes[0x00] = "None";
	old_licensee_codes[0x01] = "Nintendo";
	old_licensee_codes[0x08] = "Capcom";
	old_licensee_codes[0x09] = "HOT-B";
	old_licensee_codes[0x0A] = "Jaleco";
	old_licensee_codes[0x0B] = "Coconuts Japan";
	old_licensee_codes[0x0C] = "Elite Systems";
	old_licensee_codes[0x13] = "EA (Electronic Arts)";
	old_licensee_codes[0x18] = "Hudson Soft";
	old_licensee_codes[0x19] = "ITC Entertainment";
	old_licensee_codes[0x1A] = "Yanoman";
	old_licensee_codes[0x1D] = "Japan Clary";
	old_licensee_codes[0x1F] = "Virgin Games Ltd.";
	old_licensee_codes[0x24] = "PCM Complete";
	old_licensee_codes[0x25] = "San-X";
	old_licensee_codes[0x28] = "Kemco";
	old_licensee_codes[0x29] = "SETA Corporation";
	old_licensee_codes[0x30] = "Infogrames";
	old_licensee_codes[0x31] = "Nintendo";
	old_licensee_codes[0x32] = "Bandai";
	old_licensee_codes[0x33] = 0;
	old_licensee_codes[0x34] = "Konami";
	old_licensee_codes[0x35] = "HectorSoft";
	old_licensee_codes[0x38] = "Capcom";
	old_licensee_codes[0x39] = "Banpresto";
	old_licensee_codes[0x3C] = "Entertainment Interactive";
	old_licensee_codes[0x3E] = "Gremlin";
	old_licensee_codes[0x41] = "Ubi Soft";
	old_licensee_codes[0x42] = "Atlus";
	old_licensee_codes[0x44] = "Malibu Interactive";
	old_licensee_codes[0x46] = "Angel";
	old_licensee_codes[0x47] = "Spectrum HoloByte";
	old_licensee_codes[0x49] = "Irem";
	old_licensee_codes[0x4A] = "Virgin Games Ltd.";
	old_licensee_codes[0x4D] = "Malibu Interactive";
	old_licensee_codes[0x4F] = "U.S. Gold";
	old_licensee_codes[0x50] = "Absolute";
	old_licensee_codes[0x51] = "Acclaim Entertainment";
	old_licensee_codes[0x52] = "Activision";
	old_licensee_codes[0x53] = "Sammy USA Corporation";
	old_licensee_codes[0x54] = "GameTek";
	old_licensee_codes[0x55] = "Park Place";
	old_licensee_codes[0x56] = "LJN";
	old_licensee_codes[0x57] = "Matchbox";
	old_licensee_codes[0x59] = "Milton Bradley Company";
	old_licensee_codes[0x5A] = "Mindscape";
	old_licensee_codes[0x5B] = "Romstar";
	old_licensee_codes[0x5C] = "Naxat Soft";
	old_licensee_codes[0x5D] = "Tradewest";
	old_licensee_codes[0x60] = "Titus Interactive";
	old_licensee_codes[0x61] = "Virgin Games Ltd.";
	old_licensee_codes[0x67] = "Ocean Software";
	old_licensee_codes[0x69] = "EA (Electronic Arts)";
	old_licensee_codes[0x6E] = "Elite Systems";
	old_licensee_codes[0x6F] = "Electro Brain";
	old_licensee_codes[0x70] = "Infogrames";
	old_licensee_codes[0x71] = "Interplay Entertainment";
	old_licensee_codes[0x72] = "Broderbund";
	old_licensee_codes[0x73] = "Sculptured Software";
	old_licensee_codes[0x75] = "The Sales Curve Limited";
	old_licensee_codes[0x78] = "THQ";
	old_licensee_codes[0x79] = "Accolade";
	old_licensee_codes[0x7A] = "Triffix Entertainment";
	old_licensee_codes[0x7C] = "MicroProse";
	old_licensee_codes[0x7F] = "Kemco";
	old_licensee_codes[0x80] = "Misawa Entertainment";
	old_licensee_codes[0x83] = "LOZC G.";
	old_licensee_codes[0x86] = "Tokuma Shoten";
	old_licensee_codes[0x8B] = "Bullet-Proof Software";
	old_licensee_codes[0x8C] = "Vic Tokai Corp.";
	old_licensee_codes[0x8E] = "Ape Inc.";
	old_licensee_codes[0x8F] = "I'Max";
	old_licensee_codes[0x91] = "Chunsoft Co.";
	old_licensee_codes[0x92] = "Video System";
	old_licensee_codes[0x93] = "Tsubaraya Productions";
	old_licensee_codes[0x95] = "Varie";
	old_licensee_codes[0x96] = "Yonezawa/S'Pal";
	old_licensee_codes[0x97] = "Kemco";
	old_licensee_codes[0x99] = "Arc";
	old_licensee_codes[0x9A] = "Nihon Bussan";
	old_licensee_codes[0x9B] = "Tecmo";
	old_licensee_codes[0x9C] = "Imagineer";
	old_licensee_codes[0x9E] = "Banpresto";
	old_licensee_codes[0x9F] = "Nova";
	old_licensee_codes[0xA1] = "Hori Electric";
	old_licensee_codes[0xA2] = "Bandai";
	old_licensee_codes[0xA4] = "Konami";
	old_licensee_codes[0xA6] = "Kawada";
	old_licensee_codes[0xA7] = "Takara";
	old_licensee_codes[0xA9] = "Technos Japan";
	old_licensee_codes[0xAA] = "Broderbund";
	old_licensee_codes[0xAC] = "Toei Animation";
	old_licensee_codes[0xAD] = "Toho";
	old_licensee_codes[0xAF] = "Namco";
	old_licensee_codes[0xB0] = "Acclaim Entertainment";
	old_licensee_codes[0xB1] = "ASCII Corporation";
	old_licensee_codes[0xB2] = "Bandai";
	old_licensee_codes[0xB4] = "Square Enix";
	old_licensee_codes[0xB6] = "HAL Laboratory";
	old_licensee_codes[0xB7] = "SNK";
	old_licensee_codes[0xB9] = "Pony Canyon";
	old_licensee_codes[0xBA] = "Culture Brain";
	old_licensee_codes[0xBB] = "Sunsoft";
	old_licensee_codes[0xBD] = "Sony Imagesoft";
	old_licensee_codes[0xBF] = "Sammy Corporation";
	old_licensee_codes[0xC0] = "Taito";
	old_licensee_codes[0xC2] = "Kemco";
	old_licensee_codes[0xC3] = "Square";
	old_licensee_codes[0xC4] = "Tokuma Shoten";
	old_licensee_codes[0xC5] = "Data East";
	old_licensee_codes[0xC6] = "Tonkin House";
	old_licensee_codes[0xC8] = "Koei";
	old_licensee_codes[0xC9] = "UFL";
	old_licensee_codes[0xCA] = "Ultra Games";
	old_licensee_codes[0xCB] = "VAP, Inc.";
	old_licensee_codes[0xCC] = "Use Corporation";
	old_licensee_codes[0xCD] = "Meldac";
	old_licensee_codes[0xCE] = "Pony Canyon";
	old_licensee_codes[0xCF] = "Angel";
	old_licensee_codes[0xD0] = "Taito";
	old_licensee_codes[0xD1] = "SOFEL (Software Engineering Lab)";
	old_licensee_codes[0xD2] = "Quest";
	old_licensee_codes[0xD3] = "Sigma Enterprises";
	old_licensee_codes[0xD4] = "ASK Kodansha Co.";
	old_licensee_codes[0xD6] = "Naxat Soft";
	old_licensee_codes[0xD7] = "Copya System";
	old_licensee_codes[0xD9] = "Banpresto";
	old_licensee_codes[0xDA] = "Tomy";
	old_licensee_codes[0xDB] = "LJN";
	old_licensee_codes[0xDD] = "Nippon Computer Systems";
	old_licensee_codes[0xDE] = "Human Ent.";
	old_licensee_codes[0xDF] = "Altron";
	old_licensee_codes[0xE0] = "Jaleco";
	old_licensee_codes[0xE1] = "Towa Chiki";
	old_licensee_codes[0xE2] = "Yutaka";
	old_licensee_codes[0xE3] = "Varie";
	old_licensee_codes[0xE5] = "Epoch";
	old_licensee_codes[0xE7] = "Athena";
	old_licensee_codes[0xE8] = "Asmik Ace Entertainment";
	old_licensee_codes[0xE9] = "Natsume";
	old_licensee_codes[0xEA] = "King Records";
	old_licensee_codes[0xEB] = "Atlus";
	old_licensee_codes[0xEC] = "Epic/Sony Records";
	old_licensee_codes[0xEE] = "IGS";
	old_licensee_codes[0xF0] = "A Wave";
	old_licensee_codes[0xF3] = "Extreme Entertainment";
	old_licensee_codes[0xFF] = "LJN";
}

int read_rom(const char* path)
{
	init_labels();
	FILE* file = fopen(path, "rb");
	if (!file)
	{
		perror(path);
		return 0;
	}

	fseek(file, 0l, SEEK_END);
	long rom_length = ftell(file);
	rewind(file);

	u8* rom = malloc(rom_length);
	if (!rom)
	{
		fprintf(stderr, "Fatal: failed to allocate %lu bytes\n", rom_length);
		return 0;
	}

	fread(rom, 1, rom_length, file);
	fclose(file);

	memcpy(cartridge.nintendo_logo, rom + 0x0104, 48);
	memcpy(cartridge.title, rom + 0x0134, 16);
	memcpy(cartridge.manufactor_code, rom + 0x013F, 4);
	cartridge.cgb_flag = rom[0x0143];
	memcpy(cartridge.licensee_code_new, rom + 0x0144, 2);
	cartridge.sgb_flag = rom[0x0146];
	cartridge.cartridge_type = rom[0x0147];
	cartridge.rom_size = rom[0x0148];
	cartridge.ram_size = rom[0x0149];
	cartridge.destination_code = rom[0x014A];
	cartridge.licensee_code_old = rom[0x014B];
	cartridge.rom_version_number = rom[0x014C];
	cartridge.header_checksum = rom[0x014D];
	cartridge.global_checksum = rom[0x014E] << 8 | rom[0x014F];

	u8 calculated_checksum = 0;
	for (u16 address = 0x0134; address <= 0x014C; address++)
		calculated_checksum = calculated_checksum - rom[address] - 1;

	u16 calculated_global_checksum = 0;
	for (int i = 0; i < 0x014E; i++) 
		calculated_global_checksum += rom[i];
	for (int i = 0x0150; i < rom_length; i++) 
		calculated_global_checksum += rom[i];

	cartridge.rom = rom;
	switch (cartridge.ram_size)
	{
	case 0x00:
		cartridge.ram = NULL;
		break;
	case 0x02:
		cartridge.ram = calloc(0x2000, 1);
		break;
	case 0x03:
		cartridge.ram = calloc(0x8000, 1);
		break;
	case 0x04:
		cartridge.ram = calloc(0x20000, 1);
		break;
	case 0x05:
		cartridge.ram = calloc(0x10000, 1);
		break;
	
	default:
		break;
	}

	switch (cartridge.cartridge_type)
	{
	case 0x03:	// MBC1+RAM+BATTERY
	case 0x06:	// MBC2+BATTERY
	case 0x09:	// ROM+RAM+BATTERY
	case 0x0D:	// MMM01+RAM+BATTERY
	case 0x0F:	// MBC3+TIMER+BATTERY
	case 0x10:	// MBC3+TIMER+RAM+BATTERY
	case 0x13:	// MBC3+RAM+BATTERY
	case 0x1B:	// MBC5+RAM+BATTERY
	case 0x1E:	// MBC5+RUMBLE+RAM+BATTERY
	case 0x22:	// MBC7+SENSOR+RUMBLE+RAM+BATTERY
	case 0xFF:	// HuC1+RAM+BATTERY
		cartridge.battery = 1;
	}

	printf("title: %.16s\n", cartridge.title);
	// printf("manufactor_code: %s\n", cartridge.manufactor_code);
	// printf("cgb_flag: %u\n", cartridge.cgb_flag);
	// printf("licensee_code_new: %s\n", cartridge.licensee_code_new);
	// printf("sgb_flag: %u\n", cartridge.sgb_flag);
	printf("cartridge_type: %s\n", cartridge_types[cartridge.cartridge_type]);
	printf("rom_size: %s\n", rom_sizes[cartridge.rom_size]);
	printf("ram_size: %s\n", ram_sizes[cartridge.ram_size]);
	// printf("destination_code: %s\n", destination_codes[cartridge.destination_code]);
	// printf("licensee_code_old: %s\n", old_licensee_codes[cartridge.licensee_code_old]);
	// printf("rom_version_number: %u\n", cartridge.rom_version_number);
	// printf("header_checksum: %u\n", cartridge.header_checksum);
	// printf("calculated_checksum: %u\n", calculated_checksum);
	// printf("global_checksum: %u\n", cartridge.global_checksum);
	// printf("calculated_global_checksum: %u\n", calculated_global_checksum);

	return 1;
}

mbc_t* no_mbc()
{
	mbc_t* mbc = malloc(sizeof(mbc_t));
	mbc->ram_enable = 0;
	mbc->rom_bank_number_mask = 1;
	mbc->selected_rom1_bank = 0;
	mbc->selected_rom2_bank = 1;
	mbc->selected_ram_bank = 0;
	mbc->write_mbc = &write_mbc0;
	return mbc;
}

mbc_t* new_mbc1()
{
	mbc1_t* mbc = malloc(sizeof(mbc1_t));
	if (cartridge.ram_size == 0x00)
		cartridge.ram = calloc(0x2000, sizeof(u8));

	mbc->ram_enable = 0;
	mbc->reg_rom_bank_number = 1;
	mbc->reg_rom_ram_bank_number = 0;
	mbc->reg_rom_ram_mode_select = 0;
	mbc->rom_bank_number_mask = 1;
	for (size_t i = 0; i < cartridge.rom_size; i++)
	{
		mbc->rom_bank_number_mask <<= 1;
		mbc->rom_bank_number_mask++;
	}
	mbc->selected_rom1_bank = 0;
	mbc->selected_rom2_bank = 1;
	mbc->selected_ram_bank = 0;
	mbc->write_mbc = &write_mbc1;

	return (mbc_t*)mbc;
}

mbc_t* new_mbc2()
{
	mbc2_t* mbc = malloc(sizeof(mbc2_t));

	mbc->ram_enable = 0;
	mbc->reg_ram_enable_rom_bank_number = 0;
	cartridge.ram = calloc(0x200, sizeof(u8));
	mbc->rom_bank_number_mask = 1;
	for (size_t i = 0; i < cartridge.rom_size; i++)
	{
		mbc->rom_bank_number_mask <<= 1;
		mbc->rom_bank_number_mask++;
	}
	mbc->selected_rom1_bank = 0;
	mbc->selected_rom2_bank = 1;
	mbc->selected_ram_bank = 0;
	mbc->write_mbc = &write_mbc2;

	return (mbc_t*)mbc;
}

mbc_t* new_mbc3()
{
	mbc3_t* mbc = malloc(sizeof(mbc3_t));

	mbc->ram_enable = 0;
	mbc->reg_rtc = 0;
	mbc->reg_rom_bank_number = 1;
	mbc->reg_ram_bank_number_rtc_register_select = 0;
	mbc->reg_latch_clock_data = 0;
	mbc->rom_bank_number_mask = 1;
	for (size_t i = 0; i < cartridge.rom_size; i++)
	{
		mbc->rom_bank_number_mask <<= 1;
		mbc->rom_bank_number_mask++;
	}
	mbc->selected_rom1_bank = 0;
	mbc->selected_rom2_bank = 1;
	mbc->selected_ram_bank = 0;
	mbc->write_mbc = &write_mbc3;

	return (mbc_t*)mbc;
}

mbc_t* new_mbc5()
{
	mbc5_t* mbc = malloc(sizeof(mbc5_t));

	mbc->ram_enable = 0;
	mbc->reg_rom_bank_number = 1;
	mbc->reg_rom_bank_number2 = 0;
	mbc->rom_bank_number_mask = 1;
	for (size_t i = 0; i < cartridge.rom_size; i++)
	{
		mbc->rom_bank_number_mask <<= 1;
		mbc->rom_bank_number_mask++;
	}
	mbc->selected_rom1_bank = 0;
	mbc->selected_rom2_bank = 1;
	mbc->selected_ram_bank = 0;
	mbc->write_mbc = &write_mbc5;

	return (mbc_t*)mbc;
}

void write_mbc0(u16 address, u8 val)
{
	if (address >= 0xA000 && address < 0xC000)
	{
		if (cartridge.mbc->ram_enable)
			(cartridge.ram + 0x2000 * cartridge.mbc->selected_ram_bank)[address - 0xA000] = val;
		return;
	}
	return;
}

void write_mbc1(u16 address, u8 val)
{
	mbc1_t* mbc = (mbc1_t*)cartridge.mbc;

	if (address < 0x2000) // RAM Enable
		mbc->ram_enable = (val & 0b1111) == 0xA;
	else if (address < 0x4000) // ROM Bank Number
		mbc->reg_rom_bank_number = val & 0b11111;
	else if (address < 0x6000) // RAM Bank Number - or - Upper Bits of ROM Bank Number
		mbc->reg_rom_ram_bank_number = val & 0b11;
	else if (address < 0x8000) // ROM/RAM Mode Select
		mbc->reg_rom_ram_mode_select = val & 1;
	else if (address < 0xC000)
	{
		if (cartridge.mbc->ram_enable)
			(cartridge.ram + 0x2000 * cartridge.mbc->selected_ram_bank)[address - 0xA000] = val;
		return;
	}

	update_banks_mbc1();
}

void update_banks_mbc1()
{
	mbc1_t* mbc = (mbc1_t*)cartridge.mbc;

	// rom bank
	mbc->selected_rom1_bank = 0;
	mbc->selected_rom2_bank = mbc->reg_rom_bank_number;
	if (mbc->selected_rom2_bank == 0)
		mbc->selected_rom2_bank = 1;
	mbc->selected_rom2_bank = (mbc->reg_rom_ram_bank_number << 5) | mbc->selected_rom2_bank;
	if (mbc->reg_rom_ram_mode_select == 1)
		mbc->selected_rom1_bank = mbc->reg_rom_ram_bank_number << 5;
	mbc->selected_rom1_bank &= mbc->rom_bank_number_mask;
	mbc->selected_rom2_bank &= mbc->rom_bank_number_mask;

	// ram bank
	mbc->selected_ram_bank = 0;
	if (cartridge.ram_size > 0x02 && mbc->reg_rom_ram_mode_select == 1)
		mbc->selected_ram_bank = mbc->reg_rom_ram_bank_number;
}

void write_mbc2(u16 address, u8 val)
{
	mbc2_t* mbc = (mbc2_t*)cartridge.mbc;

	if (address < 0x4000) // ROM Bank Number
		mbc->reg_ram_enable_rom_bank_number = val;
	else if (address < 0x8000) // unused bits
		return;
	else if (address >= 0xA000 && address < 0xC000) // normal ram write
	{
		if (mbc->ram_enable)
			cartridge.ram[(address - 0xA000) & 0b111111111] = (val & 0b1111);
		return;
	}

	int bit8 = (address >> 8) & 1;
	
	if (!bit8) // enable/disable RAM
		mbc->ram_enable = (val & 0b1111) == 0xA;
	else // select rom bank
	{
		mbc->selected_rom2_bank = (val & 0b1111);
		if (mbc->selected_rom2_bank == 0)
			mbc->selected_rom2_bank = 1;
		mbc->selected_rom2_bank &= mbc->rom_bank_number_mask;
	}
}

void write_mbc3(u16 address, u8 val)
{
	mbc3_t* mbc = (mbc3_t*)cartridge.mbc;

	if (address < 0x2000) // RAM and Timer Enable
		mbc->ram_enable = (val & 0b1111) == 0xA;
	else if (address < 0x4000) // ROM Bank Number
		mbc->reg_rom_bank_number = val & 0b1111111;
	else if (address < 0x6000) // RAM Bank Number - or - RTC Register Select
		mbc->reg_ram_bank_number_rtc_register_select = val & 0b1111;
	else if (address < 0x8000) // Latch Clock Data
		mbc->reg_latch_clock_data = val;
	else if (address < 0xC000)
	{
		if (cartridge.mbc->ram_enable)
			(cartridge.ram + 0x2000 * cartridge.mbc->selected_ram_bank)[address - 0xA000] = val;
		return;
	}

	update_banks_mbc3();
}

void update_banks_mbc3()
{
	mbc3_t* mbc = (mbc3_t*)cartridge.mbc;

	// rom bank
	mbc->selected_rom1_bank = 0;
	mbc->selected_rom2_bank = mbc->reg_rom_bank_number;
	if (mbc->selected_rom2_bank == 0)
		mbc->selected_rom2_bank = 1;
	mbc->selected_rom2_bank &= mbc->rom_bank_number_mask;

	// ram bank
	mbc->selected_ram_bank = 0;
	if (cartridge.ram_size > 0x02)
		mbc->selected_ram_bank = mbc->reg_ram_bank_number_rtc_register_select & 0b11;
}

void write_mbc5(u16 address, u8 val)
{
	mbc5_t* mbc = (mbc5_t*)cartridge.mbc;

	if (address < 0x2000) // RAM Enable
		mbc->ram_enable = (val & 0b1111) == 0xA;
	else if (address < 0x3000) // ROM Bank Number 8 bits
		mbc->reg_rom_bank_number = val;
	else if (address < 0x4000) // ROM Bank Number 1 bit
		mbc->reg_rom_bank_number2 = val & 1;
	else if (address < 0x6000) // RAM Bank Number
		mbc->reg_ram_bank_number = val & 0b1111;
	else if (address >= 0xA000 && address < 0xC000) // normal ram write
	{
		if (cartridge.mbc->ram_enable)
			(cartridge.ram + 0x2000 * cartridge.mbc->selected_ram_bank)[address - 0xA000] = val;
		return;
	}

	update_banks_mbc5();
}

void update_banks_mbc5()
{
	mbc5_t* mbc = (mbc5_t*)cartridge.mbc;

	// rom bank
	mbc->selected_rom1_bank = 0;
	mbc->selected_rom2_bank = ((mbc->reg_rom_bank_number2 << 8) | mbc->reg_rom_bank_number) & mbc->rom_bank_number_mask;

	// ram bank
	mbc->selected_ram_bank = 0;
	if (cartridge.ram_size > 0x02)
		mbc->selected_ram_bank = mbc->reg_ram_bank_number & 0b11;
}

int init_mbc()
{
	if (cartridge.rom_size > 0x08)
	{
		printf("Invalid rom size byte in header (%X)\n", cartridge.rom_size);
		return 0;
	}

	switch (cartridge.cartridge_type)
	{
	case 0x00:	// ROM ONLY
		cartridge.mbc = no_mbc();
		break;
	case 0x01:	// MBC1
	case 0x02:	// MBC1+RAM
	case 0x03:	// MBC1+RAM+BATTERY
		cartridge.mbc = new_mbc1();
		break;
	case 0x05:	// MBC2
	case 0x06:	// MBC2+BATTERY
		cartridge.mbc = new_mbc2();
		break;
	case 0x0F:	// MBC3+TIMER+BATTERY
	case 0x10:	// MBC3+TIMER+RAM+BATTERY
	case 0x11:	// MBC3
	case 0x12:	// MBC3+RAM
	case 0x13:	// MBC3+RAM+BATTERY
		cartridge.mbc = new_mbc3();
		break;
	case 0x19:	// MBC5
	case 0x1A:	// MBC5+RAM
	case 0x1B:	// MBC5+RAM+BATTERY
	case 0x1C:	// MBC5+RUMBLE
	case 0x1D:	// MBC5+RUMBLE+RAM
	case 0x1E:	// MBC5+RUMBLE+RAM+BATTERY
		cartridge.mbc = new_mbc5();
		break;
	default:
		cartridge.mbc = new_mbc5();
		break;
	}
	return 1;
}

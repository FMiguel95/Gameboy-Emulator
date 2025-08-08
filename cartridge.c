#include "cartridge.h"
cartridge_t cartridge;

int read_rom(const char* path)
{
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

	printf("title: %.16s\n", cartridge.title);
	printf("manufactor_code: %s\n", cartridge.manufactor_code);
	printf("cgb_flag: %u\n", cartridge.cgb_flag);
	printf("licensee_code_new: %s\n", cartridge.licensee_code_new);
	printf("sgb_flag: %u\n", cartridge.sgb_flag);
	printf("cartridge_type: %s\n", cartridge_types[cartridge.cartridge_type]);
	printf("rom_size: %s\n", rom_sizes[cartridge.rom_size]);
	printf("ram_size: %s\n", ram_sizes[cartridge.ram_size]);
	printf("destination_code: %s\n", destination_codes[cartridge.destination_code]);
	printf("licensee_code_old: %s\n", old_licensee_codes[cartridge.licensee_code_old]);
	printf("rom_version_number: %u\n", cartridge.rom_version_number);
	printf("header_checksum: %u\n", cartridge.header_checksum);
	printf("calculated_checksum: %u\n", calculated_checksum);
	printf("global_checksum: %u\n", cartridge.global_checksum);
	printf("calculated_global_checksum: %u\n", calculated_global_checksum);

	return 1;
}

#include "memory.h"
memory_t memory;

void write8(u16 address, u8 val)
{
	if (address == DMA)
		return oam_dma_transfer(val);

	if (address == DIV)	// writing any value to DIV resets it to $00
		val = 0;

	if (address == JOYP)
		return write_joypad(val);

	if (address < 0x4000)
		write_mbc1(address, val);
	else if (address < 0x8000)
		write_mbc1(address, val);
	else if (address < 0xA000)
		memory.video_ram[address - 0x8000] = val;
	else if (address < 0xC000 && cartridge.ram)
		(cartridge.ram + 0x2000 * cartridge.mbc.selected_ram_bank)[address - 0xA000] = val;
	else if (address < 0xE000)
		memory.work_ram[address - 0xC000] = val;
	else if (address < 0xFE00)
		memory.echo_ram[address - 0xE000] = val;
	else if (address < 0xFEA0)
	{
		// u8 ppu_mode = read8(STAT) & 0b11;
		// if (ppu_mode == 0 || ppu_mode == 1)
			memory.oam[address - 0xFE00] = val;
	}
	else if (address < 0xFF00)
		memory.unused[address - 0xFEA0] = val;
	else if (address < 0xFF80)
		memory.io_registers[address - 0xFF00] = val;
	else if (address < 0xFFFF)
		memory.high_ram[address - 0xFF80] = val;
	else
		memory.ie_register[address - 0xFFFF] = val;

	if (address == SC)
		putchar(read8(SB));
}

void write8_absolute(u16 address, u8 val)
{
	if (address < 0x4000)
		memory.rom_bank0[address] = val;
	else if (address < 0x8000)
		memory.rom_bank1[address - 0x4000] = val;
	else if (address < 0xA000)
		memory.video_ram[address - 0x8000] = val;
	else if (address < 0xC000 && cartridge.ram)
		(cartridge.ram + 0x2000 * cartridge.mbc.selected_ram_bank)[address - 0xA000] = val;
	else if (address < 0xE000)
		memory.work_ram[address - 0xC000] = val;
	else if (address < 0xFE00)
		memory.echo_ram[address - 0xE000] = val;
	else if (address < 0xFEA0)
		memory.oam[address - 0xFE00] = val;
	else if (address < 0xFF00)
		memory.unused[address - 0xFEA0] = val;
	else if (address < 0xFF80)
		memory.io_registers[address - 0xFF00] = val;
	else if (address < 0xFFFF)
		memory.high_ram[address - 0xFF80] = val;
	else
		memory.ie_register[address - 0xFFFF] = val;
}

void write16(u16 address, u16 val)
{
	write8(address, val & 0xFF);
	write8(address + 1, val >> 8);
}

u8 read8(u16 address)
{
	if (address == JOYP)
		return read_joypad();

	if (!memory.io_registers[0x50] && address < 0x0100)
		return boot_rom[address];
	else if (address < 0x4000)
		return memory.rom_bank0[address];
	else if (address < 0x8000)
		return (cartridge.rom + 0x4000 * cartridge.mbc.selected_rom_bank)[address - 0x4000];
	else if (address < 0xA000)
		return memory.video_ram[address - 0x8000];
	else if (address < 0xC000)
		return (cartridge.ram + 0x2000 * cartridge.mbc.selected_ram_bank)[address - 0xA000];
	else if (address < 0xE000)
		return memory.work_ram[address - 0xC000];
	else if (address < 0xFE00)
		return memory.echo_ram[address - 0xE000];
	else if (address < 0xFEA0)
	{
		u8 ppu_mode = read8(STAT) & 0b11;
		if (ppu_mode == 0 || ppu_mode == 1)
			return memory.oam[address - 0xFE00];
		else
			return 0xFF;
	}
	else if (address < 0xFF00)
		return memory.unused[address - 0xFEA0];
	else if (address < 0xFF80)
		return memory.io_registers[address - 0xFF00];
	else if (address < 0xFFFF)
		return memory.high_ram[address - 0xFF80];
	else
		return memory.ie_register[address - 0xFFFF];
}

u8 read8_absolute(u16 address)
{
	if (!memory.io_registers[0x50] && address < 0x0100)
		return boot_rom[address];
	else if (address < 0x4000)
		return memory.rom_bank0[address];
	else if (address < 0x8000)
		return (cartridge.rom + 0x4000 * cartridge.mbc.selected_rom_bank)[address - 0x4000];
	else if (address < 0xA000)
		return memory.video_ram[address - 0x8000];
	else if (address < 0xC000)
		return (cartridge.ram + 0x2000 * cartridge.mbc.selected_ram_bank)[address - 0xA000];
	else if (address < 0xE000)
		return memory.work_ram[address - 0xC000];
	else if (address < 0xFE00)
		return memory.echo_ram[address - 0xE000];
	else if (address < 0xFEA0)
		return memory.oam[address - 0xFE00];
	else if (address < 0xFF00)
		return memory.unused[address - 0xFEA0];
	else if (address < 0xFF80)
		return memory.io_registers[address - 0xFF00];
	else if (address < 0xFFFF)
		return memory.high_ram[address - 0xFF80];
	else
		return memory.ie_register[address - 0xFFFF];
}

u16 read16(u16 address)
{
	u8 low_byte = read8(address);
	u8 high_byte = read8(address + 1);
	return ((u16)high_byte << 8) | low_byte;
}

void oam_dma_transfer(u8 location)
{
	// printf("oam_dma_transfer\n");
	u16 address_start = location * 0x100;

	for (size_t i = 0; i < 0xA0; i++)
	{
		u8 val = read8_absolute(address_start + i);
		memory.oam[i] = val;
	}
}

u8 get_flag(u8 byte, u8 bit)
{
	return ((byte >> bit) & 1);
}

void set_flag(u8* byte, u8 bit, u8 val)
{
	if (val)
		*byte |= (1 << bit);
	else
		*byte &= ~(1 << bit);
}

int init_memory()
{
	// if (cartridge.cartridge_type != 0x00)
	// {
	// 	printf("Unsupported cartridge type: %s\n", cartridge_types[cartridge.cartridge_type]);
	// 	return 0;
	// }
	// if (cartridge.rom_size != 0x00)
	// {
	// 	printf("Unsupported ROM size: %s\n", rom_sizes[cartridge.rom_size]);
	// 	return 0;
	// }

	memory = (memory_t){0};
	// memcpy(memory.rom_bank0, cartridge.rom, 0x4000);
	// memcpy(memory.rom_bank1, cartridge.rom + 0x4000, 0x4000);
	memory.rom_bank0 = cartridge.rom;
	memory.rom_bank1 = cartridge.rom + 0x4000;

	// write8(LY, 0x90);

	return 1;
}

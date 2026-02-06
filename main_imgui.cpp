#include "./imgui/imgui.h"
#include "./imgui/imgui_impl_sdl3.h"
#include "./imgui/imgui_impl_sdlrenderer3.h"
#include <SDL3/SDL.h>
// #include <gtk/gtk.h>
#include <stdio.h>
#include "emulator.h"

void reset()
{
	save_sram();
	close_rom();
	load_rom(emulator.rom_file_path);
}

void imgui_menubar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Load ROM...")) {}
			if (ImGui::MenuItem("Save State", nullptr, false, false)) {}
			if (ImGui::MenuItem("Load State", nullptr, false, false)) {}
			if (ImGui::MenuItem("Exit", "Esc")) { emulator.quit = 1; }
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Emulation"))
		{
			if (ImGui::MenuItem("Pause", "P"))
			{
				emulator.paused = !emulator.paused;
			}
			if (ImGui::MenuItem("Reset", "R"))
			{
				reset();
			}
			if (ImGui::MenuItem("Stop"))
			{

			}
			ImGui::Separator();
			if (ImGui::MenuItem("Step Cycle", "B"))
			{
				emulator.request_cycle = 1;
			}
			if (ImGui::MenuItem("Step Scanline", "N"))
			{
				emulator.request_scanline = 1;
			}
			if (ImGui::MenuItem("Step Frame", "M"))
			{
				emulator.request_frame = 1;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void imgui_cpu()
{
	if (ImGui::Begin("CPU"))
	{
		ImGui::Text("$%04X: %s", cpu.reg16_PC - 1, decode_opcode(cpu.loaded_opcode));
		ImGui::Separator();
		ImGui::BeginTable("cpu registers", 2);
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::Text("A:%02X", *cpu.reg8_A);
		ImGui::SameLine();
		ImGui::Text("F:%02X", *cpu.reg8_F);
		ImGui::Text("B:%02X", *cpu.reg8_B);
		ImGui::SameLine();
		ImGui::Text("C:%02X", *cpu.reg8_C);
		ImGui::Text("D:%02X", *cpu.reg8_D);
		ImGui::SameLine();
		ImGui::Text("E:%02X", *cpu.reg8_E);
		ImGui::Text("H:%02X", *cpu.reg8_H);
		ImGui::SameLine();
		ImGui::Text("L:%02X", *cpu.reg8_L);

		ImGui::TableSetColumnIndex(1);
		// ImGui::Separator();
		ImGui::Text("SP: %04X", cpu.reg16_SP);
		ImGui::Text("PC: %04X", cpu.reg16_PC);
		ImGui::Text("Z N H C");
		ImGui::Text("%d %d %d %d", get_flag(*cpu.reg8_F, flag_z), get_flag(*cpu.reg8_F, flag_n), get_flag(*cpu.reg8_F, flag_h), get_flag(*cpu.reg8_F, flag_c));
		ImGui::EndTable();

		ImGui::Separator();
		ImGui::Checkbox("IME", (bool*)&cpu.IME);
		
		u8 reg_IE = read8_absolute(IE);
		u8 reg_IF = read8_absolute(IF);
		bool ie0 = get_flag(reg_IE, VBlank);
		bool ie1 = get_flag(reg_IE, LCD);
		bool ie2 = get_flag(reg_IE, Timer);
		bool ie3 = get_flag(reg_IE, Serial);
		bool ie4 = get_flag(reg_IE, Joypad);
		bool if0 = get_flag(reg_IF, VBlank);
		bool if1 = get_flag(reg_IF, LCD);
		bool if2 = get_flag(reg_IF, Timer);
		bool if3 = get_flag(reg_IF, Serial);
		bool if4 = get_flag(reg_IF, Joypad);

		ImGui::BeginTable("interrupts", 3);
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		// ImGui::Text("IME");
		ImGui::TableNextColumn();
		ImGui::Text("IE");
		ImGui::TableNextColumn();
		ImGui::Text("IF");
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("VBlank");
		ImGui::TableNextColumn();
		ImGui::Checkbox("##ie0", &ie0);
		ImGui::TableNextColumn();
		ImGui::Checkbox("##if0", &if0);
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("LCD");
		ImGui::TableNextColumn();
		ImGui::Checkbox("##ie1", &ie1);
		ImGui::TableNextColumn();
		ImGui::Checkbox("##if1", &if1);
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Timer");
		ImGui::TableNextColumn();
		ImGui::Checkbox("##ie2", &ie2);
		ImGui::TableNextColumn();
		ImGui::Checkbox("##if2", &if2);
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Serial");
		ImGui::TableNextColumn();
		ImGui::Checkbox("##ie3", &ie3);
		ImGui::TableNextColumn();
		ImGui::Checkbox("##if3", &if3);
		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Joypad");
		ImGui::TableNextColumn();
		ImGui::Checkbox("##ie4", &ie4);
		ImGui::TableNextColumn();
		ImGui::Checkbox("##if4", &if4);
		ImGui::EndTable();

		set_flag(&reg_IE, 0, ie0);
		set_flag(&reg_IE, 1, ie1);
		set_flag(&reg_IE, 2, ie2);
		set_flag(&reg_IE, 3, ie3);
		set_flag(&reg_IE, 4, ie4);
		write8_absolute(IE, reg_IE);
		set_flag(&reg_IF, 0, if0);
		set_flag(&reg_IF, 1, if1);
		set_flag(&reg_IF, 2, if2);
		set_flag(&reg_IF, 3, if3);
		set_flag(&reg_IF, 4, if4);
		write8_absolute(IF, reg_IF);
	}
	ImGui::End();
}

void imgui_timers()
{
	if (ImGui::Begin("Timer"))
	{
		ImGui::Text("DIV: %02X", *timers.div);
		ImGui::Text("TAC enable: %d", get_flag(*timers.tac, 2));
		ImGui::Text("TAC clock: %d", *timers.tac & 0b11);
		ImGui::Text("TIMA: %02X", *timers.tima);
		ImGui::Text("TMA: %02X", *timers.tma);
	}
	ImGui::End();
}

void imgui_screen(SDL_Texture* tex_screen, SDL_Texture* tex_screen_next)
{
	if (ImGui::Begin("Screen"))
	{
		SDL_UpdateTexture(tex_screen, NULL, ppu.pixel_buffer_public, WIN_SCREEN_SIZE_X * 4);
		ImGui::Image(tex_screen, ImVec2(WIN_SCREEN_SIZE_X * 2, WIN_SCREEN_SIZE_Y * 2));
	}
	ImGui::End();
	if (ImGui::Begin("Next Frame"))
	{
		SDL_UpdateTexture(tex_screen_next, NULL, ppu.pixel_buffer_private, WIN_SCREEN_SIZE_X * 4);
		ImGui::Image(tex_screen_next, ImVec2(WIN_SCREEN_SIZE_X * 2, WIN_SCREEN_SIZE_Y * 2));
	}
	ImGui::End();
}

void imgui_fullscreen(SDL_Texture* tex_screen)
{
	ImGui::Begin("Full Screen");

	ImVec2 avail = ImGui::GetContentRegionAvail();

	float tex_aspect = (float)WIN_SCREEN_SIZE_X / (float)WIN_SCREEN_SIZE_Y;
	float avail_aspect = avail.x / avail.y;

	ImVec2 size;
	if (avail_aspect > tex_aspect)
	{
		size.y = avail.y;
		size.x = size.y * tex_aspect;
	}
	else
	{
		size.x = avail.x;
		size.y = size.x / tex_aspect;
	}

	ImGui::SetCursorPos(ImVec2(
		(avail.x - size.x) * 0.5f,
		(avail.y - size.y) * 0.5f
	));

	SDL_UpdateTexture(tex_screen, nullptr,
					ppu.pixel_buffer_public,
					WIN_SCREEN_SIZE_X * 4);

	ImGui::Image((ImTextureID)tex_screen, size);

	ImGui::End();
}

void imgui_ppu()
{
	if (ImGui::Begin("PPU"))
	{
		ImGui::Text("Dots: %03d/455", ppu.scanline_cycle);
		ImGui::Text("Line: %03d/153", *ppu.ly);
		ImGui::Text("Scroll: %03d,%03d", *ppu.scx, *ppu.scy);
		ImGui::Text("Window: %03d,%03d", *ppu.wx, *ppu.wy);
		ImGui::Text("DMA: %02X", *ppu.dma);
		// ImGui::Text("Background Palette: %d", *ppu.bgp);
		// ImGui::Text("Obj Palette 1: %d", *ppu.obp0);
		// ImGui::Text("Obj Palette 2: %d", *ppu.obp1);

		ImGui::SeparatorText("LCD Control");
		u8 reg_LCDC = read8_absolute(LCDC);
		bool lcdc0 = get_flag(reg_LCDC, LCDC_0);
		bool lcdc1 = get_flag(reg_LCDC, LCDC_1);
		bool lcdc2 = get_flag(reg_LCDC, LCDC_2);
		bool lcdc3 = get_flag(reg_LCDC, LCDC_3);
		bool lcdc4 = get_flag(reg_LCDC, LCDC_4);
		bool lcdc5 = get_flag(reg_LCDC, LCDC_5);
		bool lcdc6 = get_flag(reg_LCDC, LCDC_6);
		bool lcdc7 = get_flag(reg_LCDC, LCDC_7);

		ImGui::Checkbox("LCD Enable", &lcdc7);
		ImGui::Checkbox("BG & Win Enable", &lcdc0);
		ImGui::Text("Background Tilemap");
		if (ImGui::RadioButton("9800-9BFF##lcdc3", !lcdc3)) { lcdc3 = false; }
		ImGui::SameLine();
		if (ImGui::RadioButton("9C00-9FFF##lcdc3", lcdc3)) { lcdc3 = true; }
		ImGui::Checkbox("Window Enable", &lcdc5);
		ImGui::Text("Window Tilemap");
		if (ImGui::RadioButton("9800-9BFF##lcdc6", !lcdc6)) { lcdc6 = false; }
		ImGui::SameLine();
		if (ImGui::RadioButton("9C00-9FFF##lcdc6", lcdc6)) { lcdc6 = true; }
		ImGui::Text("BG & Win Tile Addressing Mode");
		if (ImGui::RadioButton("8800-97FF##lcdc4", !lcdc4)) { lcdc4 = false; }
		ImGui::SameLine();
		if (ImGui::RadioButton("8000-8FFF##lcdc4", lcdc4)) { lcdc4 = true; }
		ImGui::Checkbox("Object Enable", &lcdc1);
		ImGui::Checkbox("Tall Objects", &lcdc2);

		set_flag(&reg_LCDC, LCDC_0, lcdc0);
		set_flag(&reg_LCDC, LCDC_1, lcdc1);
		set_flag(&reg_LCDC, LCDC_2, lcdc2);
		set_flag(&reg_LCDC, LCDC_3, lcdc3);
		set_flag(&reg_LCDC, LCDC_4, lcdc4);
		set_flag(&reg_LCDC, LCDC_5, lcdc5);
		set_flag(&reg_LCDC, LCDC_6, lcdc6);
		set_flag(&reg_LCDC, LCDC_7, lcdc7);
		write8_absolute(LCDC, reg_LCDC);

		ImGui::SeparatorText("LCD Status");
		u8 reg_STAT = read8_absolute(STAT);
		bool stat2 = get_flag(reg_STAT, STAT_2);
		bool stat3 = get_flag(reg_STAT, STAT_3);
		bool stat4 = get_flag(reg_STAT, STAT_4);
		bool stat5 = get_flag(reg_STAT, STAT_5);
		bool stat6 = get_flag(reg_STAT, STAT_6);
		ImGui::Text("PPU Mode: %d", reg_STAT & 0b11);
		ImGui::Text("LYC: %d", *ppu.lyc);
		ImGui::Checkbox("LYC == LY", &stat2);
		ImGui::Checkbox("Mode 0 int", &stat3);
		ImGui::Checkbox("Mode 1 int", &stat4);
		ImGui::Checkbox("Mode 2 int", &stat5);
		ImGui::Checkbox("LYC int", &stat6);
		ImGui::Checkbox("STAT trigger", (bool*)&ppu.stat_state);
		ImGui::SameLine();
		ImGui::Checkbox("STAT previous", (bool*)&ppu.prev_stat_state);

		set_flag(&reg_STAT, STAT_2, stat2);
		set_flag(&reg_STAT, STAT_3, stat3);
		set_flag(&reg_STAT, STAT_4, stat4);
		set_flag(&reg_STAT, STAT_5, stat5);
		set_flag(&reg_STAT, STAT_6, stat6);
		write8_absolute(STAT, reg_STAT);

	}
	ImGui::End();
}

void imgui_apu()
{
	if (ImGui::Begin("APU"))
	{
		u8 reg_NR52 = read8_absolute(NR52);
		bool nr52_0 = get_flag(reg_NR52, NR52_0);
		bool nr52_1 = get_flag(reg_NR52, NR52_1);
		bool nr52_2 = get_flag(reg_NR52, NR52_2);
		bool nr52_3 = get_flag(reg_NR52, NR52_3);
		bool nr52_7 = get_flag(reg_NR52, NR52_7);

		ImGui::Checkbox("APU Enable", &nr52_7);
		ImGui::Text("APU DIV: %.2X", apu.div_apu);
		ImGui::SeparatorText("Channel 1");
		ImGui::Checkbox("Enable###ch1", &nr52_0);

		// u8 reg_NR11 = read8_absolute(NR11);
		// u8 reg_NR12 = read8_absolute(NR12);
		// u8 reg_NR13 = read8_absolute(NR13);
		// u8 reg_NR14 = read8_absolute(NR14);
		// ImGui::Text("Wave Duty: %d", reg_NR11 >> 6);
		// bool nr14_6 = get_flag(reg_NR14, 6);
		// ImGui::Checkbox("Length enable###ch1length", &nr14_6);
		// set_flag(&reg_NR14, 6, nr14_6);
		// ImGui::Text("Initial length timer: %d", reg_NR11 & 0b00111111);
		// ImGui::SameLine();
		// ImGui::Text("Current: %d", apu.ch1_length_timer);
		// ImGui::Text("Initial volume: %d", reg_NR12 >> 4);
		// ImGui::SameLine();
		// ImGui::Text("Current: %d", apu.ch1_current_volume);
		// const char* ch1EnvDir = (reg_NR12 & 0b0000100) ? "increase" : "decrease";
		// ImGui::Text("Env direction: %s", ch1EnvDir);
		// ImGui::Text("Sweep pace: %d", reg_NR12 & 0b111);
		// ImGui::Text("Period value: %d", ((int)(reg_NR14 & 0b111) << 8) | reg_NR13);
		// ImGui::Checkbox("Trigger###ch1trigger", (bool*)&apu.ch1_request_trigger);
		// write8_absolute(NR14, reg_NR14);


		ImGui::SeparatorText("Channel 2");
		ImGui::Checkbox("Enable###ch2", &nr52_1);

		u8 reg_NR21 = read8_absolute(NR21);
		u8 reg_NR22 = read8_absolute(NR22);
		u8 reg_NR23 = read8_absolute(NR23);
		u8 reg_NR24 = read8_absolute(NR24);
		ImGui::Text("Wave Duty: %d", reg_NR21 >> 6);
		bool nr24_6 = get_flag(reg_NR24, NR24_6);
		ImGui::Checkbox("Length enable###ch2length", &nr24_6);
		set_flag(&reg_NR24, NR24_6, nr24_6);
		ImGui::Text("Initial length timer: %d", reg_NR21 & 0b00111111);
		ImGui::SameLine();
		ImGui::Text("Current: %d", apu.ch2_length_timer);
		ImGui::Text("Initial volume: %d", reg_NR22 >> 4);
		ImGui::SameLine();
		ImGui::Text("Current: %d", apu.ch2_current_volume);
		const char* ch2EnvDir = (reg_NR22 & 0b0000100) ? "increase" : "decrease";
		ImGui::Text("Env direction: %s", ch2EnvDir);
		ImGui::Text("Sweep pace: %d", reg_NR22 & 0b111);
		ImGui::Text("Period value: %d", ((int)(reg_NR24 & 0b111) << 8) | reg_NR23);
		ImGui::Checkbox("Trigger###ch2trigger", (bool*)&apu.ch2_request_trigger);
		write8_absolute(NR24, reg_NR24);

		// bool nr21_0 = get_flag(reg_NR21, NR21_0);
		// bool nr21_1 = get_flag(reg_NR21, NR21_1);
		// bool nr21_2 = get_flag(reg_NR21, NR21_2);
		// bool nr21_3 = get_flag(reg_NR21, NR21_3);
		// bool nr21_4 = get_flag(reg_NR21, NR21_4);
		// bool nr21_5 = get_flag(reg_NR21, NR21_5);
		// bool nr21_6 = get_flag(reg_NR21, NR21_6);
		// bool nr21_7 = get_flag(reg_NR21, NR21_7);

		ImGui::SeparatorText("Channel 3");
		ImGui::Checkbox("Enable###ch3", &nr52_2);
		ImGui::SeparatorText("Channel 4");
		ImGui::Checkbox("Enable###ch4", &nr52_3);

		set_flag(&reg_NR52, NR52_0, nr52_0);
		set_flag(&reg_NR52, NR52_1, nr52_1);
		set_flag(&reg_NR52, NR52_2, nr52_2);
		set_flag(&reg_NR52, NR52_3, nr52_3);
		set_flag(&reg_NR52, NR52_7, nr52_7);
		write8_absolute(NR52, reg_NR52);
	}
	ImGui::End();
}

void imgui_vram(SDL_Texture* tex_vram)
{
	int tex_vram_pixels[WIN_VRAM_SIZE_X * WIN_VRAM_SIZE_Y];

	if (ImGui::Begin("VRAM"))
	{
		size_t tiles_per_row = 16;
		for (size_t y = 0; y < WIN_VRAM_SIZE_Y; y++)
		{
			for (size_t x = 0; x < WIN_VRAM_SIZE_X; x++)
			{
				size_t tile_x = x / 8;
				size_t tile_y = y / 8;
				size_t tile_index = tile_y * tiles_per_row + tile_x;
				pixel_code color_code = get_pixel_code(tiles[tile_index], x % 8, y % 8);
				pixel_color color = get_color(color_code);

				tex_vram_pixels[y * WIN_VRAM_SIZE_X + x] = color;
			}
		}
		SDL_UpdateTexture(tex_vram, NULL, tex_vram_pixels, WIN_VRAM_SIZE_X * 4);
		ImGui::Image(tex_vram, ImVec2(WIN_VRAM_SIZE_X, WIN_VRAM_SIZE_Y));
	}
	ImGui::End();
}

static void draw_line(int pos_x, int pos_y, int dir_x, int dir_y, int length, int* pixels)
{
	for (size_t i = 0; i < length; i++)
	{
		int index = (u8)pos_y * 256 + (u8)pos_x;
		pixels[index] = ~pixels[index];
		pos_x += dir_x;
		pos_y += dir_y;
	}
}
void imgui_maps(SDL_Texture* tex_map9800, SDL_Texture* tex_map9C00)
{
	int tex_map_pixels[WIN_BACKGROUND_SIZE_X * WIN_BACKGROUND_SIZE_Y];
	u8 lcdc = read8_absolute(LCDC);

	if (ImGui::Begin("Map $9800"))
	{
		size_t tiles_per_row = 32;
		for (size_t y = 0; y < WIN_BACKGROUND_SIZE_Y; y++)
		{
			for (size_t x = 0; x < WIN_BACKGROUND_SIZE_X; x++)
			{
				u8 tile_id = memory.video_ram[0x1800 + (y / 8) * tiles_per_row + (x / 8)];
				tile t = tiles[convert_tile_index(tile_id)];
				pixel_code color_code = get_pixel_code(t, x % 8, y % 8);
				pixel_color color = get_color(color_code);

				tex_map_pixels[y * WIN_BACKGROUND_SIZE_X + x] = color;
			}
		}
		if (!get_flag(lcdc, LCDC_3))
		{
			draw_line(read8(SCX), read8(SCY), 1, 0, WIN_SCREEN_SIZE_X, tex_map_pixels);
			draw_line(read8(SCX), read8(SCY), 0, 1, WIN_SCREEN_SIZE_Y, tex_map_pixels);
			draw_line(read8(SCX), read8(SCY) + WIN_SCREEN_SIZE_Y, 1, 0, WIN_SCREEN_SIZE_X, tex_map_pixels);
			draw_line(read8(SCX) + WIN_SCREEN_SIZE_X, read8(SCY), 0, 1, WIN_SCREEN_SIZE_Y, tex_map_pixels);
		}
		SDL_UpdateTexture(tex_map9800, NULL, tex_map_pixels, WIN_BACKGROUND_SIZE_X * 4);
		ImGui::Image(tex_map9800, ImVec2(WIN_BACKGROUND_SIZE_X, WIN_BACKGROUND_SIZE_Y));
	}
	ImGui::End();

	if (ImGui::Begin("Map $9C00"))
	{
		size_t tiles_per_row = 32;
		for (size_t y = 0; y < WIN_BACKGROUND_SIZE_Y; y++)
		{
			for (size_t x = 0; x < WIN_BACKGROUND_SIZE_X; x++)
			{
				u8 tile_id = memory.video_ram[0x1C00 + (y / 8) * tiles_per_row + (x / 8)];
				tile t = tiles[convert_tile_index(tile_id)];
				pixel_code color_code = get_pixel_code(t, x % 8, y % 8);
				pixel_color color = get_color(color_code);

				tex_map_pixels[y * WIN_BACKGROUND_SIZE_X + x] = color;
			}
		}
		if (get_flag(lcdc, LCDC_3))
		{
			draw_line(read8(SCX), read8(SCY), 1, 0, WIN_SCREEN_SIZE_X, tex_map_pixels);
			draw_line(read8(SCX), read8(SCY), 0, 1, WIN_SCREEN_SIZE_Y, tex_map_pixels);
			draw_line(read8(SCX), read8(SCY) + WIN_SCREEN_SIZE_Y, 1, 0, WIN_SCREEN_SIZE_X, tex_map_pixels);
			draw_line(read8(SCX) + WIN_SCREEN_SIZE_X, read8(SCY), 0, 1, WIN_SCREEN_SIZE_Y, tex_map_pixels);
		}
		SDL_UpdateTexture(tex_map9C00, NULL, tex_map_pixels, WIN_BACKGROUND_SIZE_X * 4);
		ImGui::Image(tex_map9C00, ImVec2(WIN_BACKGROUND_SIZE_X, WIN_BACKGROUND_SIZE_Y));
	}
	ImGui::End();
}

void imgui_buttons()
{
	if (ImGui::Begin("Buttons"))
	{
		const char* label = emulator.paused ? "Continue" : " Pause  ";
		if (ImGui::Button(label)) { emulator.paused = !emulator.paused; }
		ImGui::SameLine();
		if (ImGui::Button("> m-cycle")) { emulator.request_cycle = 1; }
		ImGui::SameLine();
		if (ImGui::Button(">> scanline")) { emulator.request_scanline = 1; }
		ImGui::SameLine();
		if (ImGui::Button(">>> frame")) { emulator.request_frame = 1; }
	}
	ImGui::End();
}

int main(int ac, char** av)
{
	if (ac <= 1)
	{
		printf("Usage: ./emu <rom_path>\n");
		return 1;
	}

	if (!load_rom(av[1]))
		return 1;

	// Setup SDL
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD))
	{
		printf("Error: SDL_Init(): %s\n", SDL_GetError());
		return 1;
	}

	// Create window with SDL_Renderer graphics context
	float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
	SDL_WindowFlags window_flags = SDL_WINDOW_RESIZABLE /*| SDL_WINDOW_HIDDEN*/ | SDL_WINDOW_HIGH_PIXEL_DENSITY;
	SDL_Window* window = SDL_CreateWindow("GB", (int)(1280 * main_scale), (int)(720 * main_scale), window_flags);
	if (window == nullptr)
	{
		printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
		return 1;
	}
	SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
	SDL_SetRenderVSync(renderer, 0);
	if (renderer == nullptr)
	{
		SDL_Log("Error: SDL_CreateRenderer(): %s\n", SDL_GetError());
		return 1;
	}
	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	SDL_ShowWindow(window);

	SDL_Texture* tex_screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBX32, SDL_TEXTUREACCESS_STREAMING, WIN_SCREEN_SIZE_X, WIN_SCREEN_SIZE_Y);
	SDL_SetTextureScaleMode(tex_screen, SDL_SCALEMODE_NEAREST);
	SDL_Texture* tex_screen_next = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBX32, SDL_TEXTUREACCESS_STREAMING, WIN_SCREEN_SIZE_X, WIN_SCREEN_SIZE_Y);
	SDL_SetTextureScaleMode(tex_screen_next, SDL_SCALEMODE_NEAREST);
	SDL_Texture* tex_vram = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBX32, SDL_TEXTUREACCESS_STREAMING, WIN_VRAM_SIZE_X, WIN_VRAM_SIZE_Y);
	SDL_Texture* tex_map9800 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBX32, SDL_TEXTUREACCESS_STREAMING, WIN_BACKGROUND_SIZE_X, WIN_BACKGROUND_SIZE_Y);
	SDL_Texture* tex_map9C00 = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBX32, SDL_TEXTUREACCESS_STREAMING, WIN_BACKGROUND_SIZE_X, WIN_BACKGROUND_SIZE_Y);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup scaling
	ImGuiStyle& style = ImGui::GetStyle();
	style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
	style.FontScaleDpi = main_scale;        // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)

	// Setup Platform/Renderer backends
	ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer3_Init(renderer);


	SDL_AudioSpec spec = {
		.format = SDL_AUDIO_S8,
		.channels = 1,
		.freq = SAMPLE_RATE
	};

	SDL_AudioDeviceID dev = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec);
	if (!dev)
	{
		SDL_Log("Failed to open audio device: %s", SDL_GetError());
		return 1;
	}

	SDL_AudioStream *stream = SDL_CreateAudioStream(&spec, &spec);
	if (!stream)
	{
		SDL_Log("Failed to create audio stream: %s", SDL_GetError());
		return 1;
	}

	SDL_BindAudioStream(dev, stream);
	SDL_ResumeAudioDevice(dev);


	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	bool fullscreen = false;
	unsigned long it = 0;
	while (!emulator.quit)
	{
		it++;
		long start_time = get_current_time();

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL3_ProcessEvent(&event);
			if (event.type == SDL_EVENT_QUIT)
				emulator.quit = 1;
			if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window))
				emulator.quit = 1;

			// on key down
			if (event.type == SDL_EVENT_KEY_DOWN)
			{
				SDL_Keycode key = event.key.key;

				if (key == SDLK_UP)
					press_key(KEY_UP);
				if (key == SDLK_DOWN)
					press_key(KEY_DOWN);
				if (key == SDLK_LEFT)
					press_key(KEY_LEFT);
				if (key == SDLK_RIGHT)
					press_key(KEY_RIGHT);
				if (key == SDLK_Z)
					press_key(KEY_A);
				if (key == SDLK_X)
					press_key(KEY_B);
				if (key == SDLK_RETURN)
					press_key(KEY_START);
				if (key == SDLK_BACKSPACE)
					press_key(KEY_SELECT);

				if (key == SDLK_ESCAPE)
					emulator.quit = 1;
				if (key == SDLK_P)
					emulator.paused = !emulator.paused;
				if (key == SDLK_SPACE)
					emulator.fforward = 1;
				if (key == SDLK_R)
					reset();
				if (key == SDLK_B)
					emulator.request_cycle = 1;
				if (key == SDLK_N)
					emulator.request_scanline = 1;
				if (key == SDLK_M)
					emulator.request_frame = 1;
				
			}

			// on key up
			if (event.type == SDL_EVENT_KEY_UP)
			{
				SDL_Keycode key = event.key.key;

				if (key == SDLK_UP)
					release_key(KEY_UP);
				if (key == SDLK_DOWN)
					release_key(KEY_DOWN);
				if (key == SDLK_LEFT)
					release_key(KEY_LEFT);
				if (key == SDLK_RIGHT)
					release_key(KEY_RIGHT);
				if (key == SDLK_Z)
					release_key(KEY_A);
				if (key == SDLK_X)
					release_key(KEY_B);
				if (key == SDLK_RETURN)
					release_key(KEY_START);
				if (key == SDLK_BACKSPACE)
					release_key(KEY_SELECT);

				if (key == SDLK_SPACE)
					emulator.fforward = 0;
				if (key == SDLK_F)
					fullscreen = !fullscreen;
			}

		}

		int cycles_to_run = FRAME_CYCLES;
		if (emulator.paused)
			cycles_to_run = 0;
		if (emulator.request_frame)
		{
			emulator.request_frame = 0;
			cycles_to_run = FRAME_CYCLES;
		}
		if (emulator.request_scanline)
		{
			emulator.request_scanline = 0;
			cycles_to_run = 114;
		}
		if (emulator.request_cycle)
		{
			emulator.request_cycle = 0;
			cycles_to_run = 1;
		}
		// emulator
		run_clock(cycles_to_run);

		if (buffer_size(&apu.rb) > 804 && cycles_to_run == FRAME_CYCLES && !emulator.fforward)
		{
			SDL_PutAudioStreamData(stream, apu.rb.buffer + apu.rb.head_index, buffer_size(&apu.rb));
			buffer_reset(&apu.rb);
		}
		// printf("================\n");

		// Start the Dear ImGui frame
		ImGui_ImplSDLRenderer3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
		ImGui::DockSpaceOverViewport();

		if (!fullscreen)
		{
			imgui_menubar();
			imgui_cpu();
			imgui_timers();
			imgui_screen(tex_screen, tex_screen_next);
			imgui_ppu();
			imgui_apu();
			imgui_vram(tex_vram);
			imgui_maps(tex_map9800, tex_map9C00);
			// imgui_buttons();
		}
		else
		{
			imgui_fullscreen(tex_screen);
		}

		// Rendering
		ImGui::Render();
		SDL_SetRenderScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
		SDL_SetRenderDrawColorFloat(renderer, clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		SDL_RenderClear(renderer);
		ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
		SDL_RenderPresent(renderer);

		if (!emulator.fforward)
		{
			long end_time = get_current_time();
			long sleep_time = start_time + FRAME_TIME - end_time;
			if (sleep_time > 0)
				usleep(sleep_time);
		}
	}
	close_rom();

	// Cleanup
	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyTexture(tex_screen);
	SDL_DestroyTexture(tex_screen_next);
	SDL_DestroyTexture(tex_vram);
	SDL_DestroyTexture(tex_map9800);
	SDL_DestroyTexture(tex_map9C00);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_DestroyAudioStream(stream);
	SDL_CloseAudioDevice(dev);
	SDL_Quit();
}

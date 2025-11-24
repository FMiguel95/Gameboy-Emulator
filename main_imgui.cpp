#include "./imgui/imgui.h"
#include "./imgui/imgui_impl_sdl3.h"
#include "./imgui/imgui_impl_sdlrenderer3.h"
#include <SDL3/SDL.h>
#include <stdio.h>
#include "emulator.h"


int main(int ac, char** av)
{
	if (ac <= 1)
	{
		printf("Usage: ./emu <rom_path>\n");
		return 1;
	}

	if (!read_rom(av[1]))
		return 1;
	if (!init_mbc())
		return 1;
	if (!init_memory())
		return 1;
	emulator.rom_file_name = basename(av[1]);
	if (!load_sram())
		return 1;
	if (!init_tiles())
		return 1;
	if (!init_joypad())
		return 1;
	if (!init_timers())
		return 1;
	if (!init_cpu())
		return 1;
	if (!init_ppu())
		return 1;

	// Setup SDL
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
	{
		printf("Error: SDL_Init(): %s\n", SDL_GetError());
		return 1;
	}

	// Create window with SDL_Renderer graphics context
	float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
	SDL_WindowFlags window_flags = SDL_WINDOW_RESIZABLE /*| SDL_WINDOW_HIDDEN*/ | SDL_WINDOW_HIGH_PIXEL_DENSITY;
	SDL_Window* window = SDL_CreateWindow("Dear ImGui SDL3+SDL_Renderer example", (int)(1280 * main_scale), (int)(720 * main_scale), window_flags);
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

	SDL_Texture* tex_screen = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGBX32,
		SDL_TEXTUREACCESS_STREAMING,
		WIN_SCREEN_SIZE_X,
		WIN_SCREEN_SIZE_Y
	);

	SDL_Texture* tex_vram = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGBX32,
		SDL_TEXTUREACCESS_STREAMING,
		WIN_VRAM_SIZE_X,
		WIN_VRAM_SIZE_Y
	);
	int tex_vram_pixels[WIN_VRAM_SIZE_X * WIN_VRAM_SIZE_Y];

	SDL_Texture* tex_map9800 = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGBX32,
		SDL_TEXTUREACCESS_STREAMING,
		WIN_BACKGROUND_SIZE_X,
		WIN_BACKGROUND_SIZE_Y
	);
	int tex_map9800_pixels[WIN_BACKGROUND_SIZE_X * WIN_BACKGROUND_SIZE_Y];

	SDL_Texture* tex_map9C00 = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGBX32,
		SDL_TEXTUREACCESS_STREAMING,
		WIN_BACKGROUND_SIZE_X,
		WIN_BACKGROUND_SIZE_Y
	);
	int tex_map9C00_pixels[WIN_BACKGROUND_SIZE_X * WIN_BACKGROUND_SIZE_Y];

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



	bool done = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	while (!done)
	{
		long start_time = get_current_time();

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL3_ProcessEvent(&event);
			if (event.type == SDL_EVENT_QUIT)
				done = true;
			if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window))
				done = true;

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

				if (key == SDLK_P)
					emulator.paused = !emulator.paused;
				if (key == SDLK_SPACE)
					emulator.fforward = 1;

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
			}

		}

		// emulator
		if (!emulator.paused)
			process_frame();

		// Start the Dear ImGui frame
		ImGui_ImplSDLRenderer3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
		ImGui::DockSpaceOverViewport();

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Load ROM...")) {}
				if (ImGui::MenuItem("Save State", nullptr, false, false)) {}
				if (ImGui::MenuItem("Load State", nullptr, false, false)) {}
				if (ImGui::MenuItem("Exit")) { done = true; }
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Emulation"))
			{
				if (ImGui::MenuItem("Fast Forward", "Space")) {}
				if (ImGui::MenuItem("Pause", "P")) {}
				if (ImGui::MenuItem("Reset", "R")) {}
				if (ImGui::MenuItem("Stop")) {}
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "Ctrl+X")) {}
				if (ImGui::MenuItem("Copy", "Ctrl+C")) {}
				if (ImGui::MenuItem("Paste", "Ctrl+V")) {}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		if (ImGui::Begin("CPU"))
		{
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
			ImGui::Text("%d", get_flag(reg_IE, VBlank));
			ImGui::TableNextColumn();
			ImGui::Text("%d", get_flag(reg_IF, VBlank));
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("LCD");
			ImGui::TableNextColumn();
			ImGui::Text("%d", get_flag(reg_IE, LCD));
			ImGui::TableNextColumn();
			ImGui::Text("%d", get_flag(reg_IF, LCD));
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Timer");
			ImGui::TableNextColumn();
			ImGui::Text("%d", get_flag(reg_IE, Timer));
			ImGui::TableNextColumn();
			ImGui::Text("%d", get_flag(reg_IF, Timer));
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Serial");
			ImGui::TableNextColumn();
			ImGui::Text("%d", get_flag(reg_IE, Serial));
			ImGui::TableNextColumn();
			ImGui::Text("%d", get_flag(reg_IF, Serial));
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Joypad");
			ImGui::TableNextColumn();
			ImGui::Text("%d", get_flag(reg_IE, Joypad));
			ImGui::TableNextColumn();
			ImGui::Text("%d", get_flag(reg_IF, Joypad));
			ImGui::EndTable();
		}
		ImGui::End();

		if (ImGui::Begin("Timer"))
		{
			ImGui::Text("DIV: %02X", *timers.div);
			ImGui::Text("TAC enable: %d", get_flag(*timers.tac, 2));
			ImGui::Text("TAC clock: %d", *timers.tac & 0b11);
			ImGui::Text("TIMA: %02X", *timers.tima);
			ImGui::Text("TMA: %02X", *timers.tma);
		}
		ImGui::End();

		if (ImGui::Begin("Screen"))
		{
			SDL_UpdateTexture(tex_screen, NULL, ppu.pixel_buffer_public, WIN_SCREEN_SIZE_X * 4);
			ImGui::Image(tex_screen, ImVec2(WIN_SCREEN_SIZE_X * 2, WIN_SCREEN_SIZE_Y * 2));
		}
		ImGui::End();

		if (ImGui::Begin("PPU"))
		{
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
	
						tex_map9800_pixels[y * WIN_BACKGROUND_SIZE_X + x] = color;
					}
				}
				SDL_UpdateTexture(tex_map9800, NULL, tex_map9800_pixels, WIN_BACKGROUND_SIZE_X * 4);
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
	
						tex_map9C00_pixels[y * WIN_BACKGROUND_SIZE_X + x] = color;
					}
				}
				SDL_UpdateTexture(tex_map9C00, NULL, tex_map9C00_pixels, WIN_BACKGROUND_SIZE_X * 4);
				ImGui::Image(tex_map9C00, ImVec2(WIN_BACKGROUND_SIZE_X, WIN_BACKGROUND_SIZE_Y));
			}
			ImGui::End();
		}
		ImGui::End();




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

	// Cleanup
	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyTexture(tex_screen);
	SDL_DestroyTexture(tex_vram);
	SDL_DestroyTexture(tex_map9800);
	SDL_DestroyTexture(tex_map9C00);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

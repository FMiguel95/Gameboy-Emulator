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

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

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
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL3_ProcessEvent(&event);
			if (event.type == SDL_EVENT_QUIT)
				done = true;
			if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window))
				done = true;
		}

		// emulator
		process_frame();
		usleep(16666);

		// Start the Dear ImGui frame
		ImGui_ImplSDLRenderer3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("CPU");
		ImGui::InputInt("A", (int*)cpu.reg8_A);
		ImGui::InputInt("B", (int*)cpu.reg8_B);
		ImGui::InputInt("C", (int*)cpu.reg8_C);
		ImGui::InputInt("D", (int*)cpu.reg8_D);
		ImGui::InputInt("E", (int*)cpu.reg8_E);
		ImGui::InputInt("F", (int*)cpu.reg8_F);
		ImGui::InputInt("H", (int*)cpu.reg8_H);
		ImGui::InputInt("L", (int*)cpu.reg8_L);
		ImGui::InputInt("SP", (int*)&cpu.reg16_SP);
		ImGui::InputInt("PC", (int*)&cpu.reg16_PC);
		ImGui::End();
		

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

		// Rendering
		ImGui::Render();
		SDL_SetRenderScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
		SDL_SetRenderDrawColorFloat(renderer, clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		SDL_RenderClear(renderer);
		ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
		SDL_RenderPresent(renderer);
	}

	// Cleanup
	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

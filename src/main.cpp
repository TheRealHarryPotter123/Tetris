#define IS_USING_IMGUI true			//Allows to turn off all ImGui, since it will not be used in the final version

#include <iostream>
#include <chrono>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if IS_USING_IMGUI
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include <stdio.h>
#endif

#include "Public/Ressource/Grid.h"
#include "Public/Ressource/Tetromino.h"
#include "Public/Ressource/StaticBlock.h"
#include "Public/Ressource/Util.h"

#define IS_TESTING false
#define TYPE_OF_TEST 0

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

constexpr SDL_FColor clear_color = SDL_FColor(1.0,1.0,1.0,1.0);

void DisplayOverlay(float elapsedMillisec, float elapsed_second)
{
#if IS_USING_IMGUI
	static float f = 0.0f;
	static int counter = 0;

	ImGui::Begin("Performance tab");

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", elapsedMillisec, 1.0 / elapsed_second);
	ImGui::End();
#endif
}


int main(int argc, char* argv[])
{
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		return SDL_APP_FAILURE;
	}

	if (!SDL_CreateWindowAndRenderer("Tetris", 1280, 840, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
		std::cerr << "pas capable de générer la fenêtre ou le gpu";
		return SDL_APP_FAILURE;
	}

	//Init grid
	Grid grid{ window, renderer };

#if IS_TESTING
	//Test enabled

	if constexpr (TYPE_OF_TEST == 0)
	{
		//Fill the grid with random blocks
	}
	else
	{
		std::cout << "IS_TESTING is true but no test specified";
	}

#endif // IS_TESTING

#if IS_USING_IMGUI
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
	style.ScaleAllSizes(SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay()));        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
	style.FontScaleDpi = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());        // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true automatically overrides this for every window depending on the current monitor)

	// Setup Platform/Renderer backends
	ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer3_Init(renderer);
#endif

	bool running = true;
	SDL_Event event;

	std::chrono::time_point<std::chrono::high_resolution_clock> pre_time, post_time;
	pre_time = std::chrono::high_resolution_clock::now();

	while (running) {
		// Calcul de performance
		post_time = std::chrono::high_resolution_clock::now();
		auto elapsed_Time = post_time - pre_time;
		auto elapsed_milli = (float)std::chrono::duration<double, std::milli>(elapsed_Time).count();
		auto elapsed_second = (float)std::chrono::duration<double, std::ratio<1, 1>>(elapsed_Time).count();

		std::cout << elapsed_milli << " ms elasped, " << 1.0 / elapsed_second << " fps" << std::endl;

		pre_time = post_time;

		while (SDL_PollEvent(&event)) {
#if IS_USING_IMGUI
			//Send event to ImGui, so it can handle mouse clicks et keyboard input
			ImGui_ImplSDL3_ProcessEvent(&event);
#endif //IS_USING_IMGUI
			if (event.type == SDL_EVENT_QUIT) {
				running = false;
			}
		}


#if IS_USING_IMGUI
		// Start the Dear ImGui frame
		ImGui_ImplSDLRenderer3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
		
		DisplayOverlay(elapsed_milli, elapsed_second);

		//Render
		ImGui::Render();
#endif //IS_USING_IMGUI

		SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
		SDL_RenderClear(renderer);

#if IS_USING_IMGUI
		ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
#endif //IS_USING_IMGUI


		//Draw grid and blocks
		grid.draw(renderer);
		
		SDL_RenderPresent(renderer);
	}
#if IS_USING_IMGUI
	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();
#endif //IS_USING_IMGUI

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}


/* Created by Thomas Lesieur
	Modifiev by
		MS, 11/03/2026: Added Imgui and all setup required to show windows

*/

#include "Public/Ressource/Util.h"

#include <iostream>
#include <chrono>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if IS_USING_IMGUI
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include <stdio.h>
#endif //IS_USING_IMGUI

#include "Public/Ressource/Grid.h"
#include "Public/Ressource/Tetromino.h"
#include "Public/Ressource/StaticBlock.h"
#include "Public/Ressource/DisplayUtil.h"

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

constexpr SDL_FColor clear_color = SDL_FColor(1.0,1.0,1.0,1.0);


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
	SetupImGuiContext(window, renderer);
#endif

	bool running = true;
	SDL_Event event;

	std::chrono::time_point<std::chrono::high_resolution_clock> pre_time, post_time;

	while (running) {
		// Calculate time since last frame (for calculations and performance)
		pre_time = post_time;
		post_time = std::chrono::high_resolution_clock::now();
		auto elapsed_Time = post_time - pre_time;
		auto elapsed_milli = (float)std::chrono::duration<double, std::milli>(elapsed_Time).count();
		auto elapsed_second = (float)std::chrono::duration<double, std::ratio<1, 1>>(elapsed_Time).count();

		std::cout << elapsed_milli << " ms elasped, " << 1.0 / elapsed_second << " fps" << std::endl;


		while (SDL_PollEvent(&event)) {
#if IS_USING_IMGUI
			ImGui_ImplSDL3_ProcessEvent(&event);	//Send event to ImGui, so it can handle mouse clicks et keyboard input
#endif //IS_USING_IMGUI

			if (event.type == SDL_EVENT_QUIT) {
				running = false;
			}
		}
		

		//Clear previous render
		SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
		SDL_RenderClear(renderer);

#if IS_USING_IMGUI
		SetupImGuiFrame();
		DisplayOverlay(elapsed_milli,elapsed_second);

#endif //IS_USING_IMGUI

	/* MAIN LOOP - START */


		//Draw grid and blocks
		grid.draw(renderer);

		
	/* MAIN LOOP - END */

#if IS_USING_IMGUI
		//Render ImGui windows
		ImGui::Render();
		ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
#endif //IS_USING_IMGUI

		SDL_RenderPresent(renderer);
	}

#if IS_USING_IMGUI
	StopImGui();
#endif //IS_USING_IMGUI

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}


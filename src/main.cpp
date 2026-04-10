/* Created by Thomas Lesieur, 03/02/2026: Configuration intiale du projet
*	Modified by
*		Thomas Lesieur : 23/02/2026 : Premier affichage d'un bloc et d'un tetromino
*		Thomas Lesieur : 09/03/2026 : Restructuration de l'affichage en different fichiers
*		Maxime Sevigny : 10/03/2026 : Structuration des fichiers en .h et .cpp
*		Thomas Lesieur : 11/03/2026 : Ajout de la grille et de blocs statiques a l'interieur
*		Maxime Sevigny : 11/03/2026 : Bug fix - merge problem + clean fct main + creer test de remplissage de la grille
*		Maxime Sevigny : 01/04/2026 : Add color per tetromino
*/

#include "Public/Ressource/Util.h"
#include <iostream>
#include <chrono>
#include <fstream>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#if IS_USING_IMGUI
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include <stdio.h>
#endif //IS_USING_IMGUI

#include "Public/Ressource/Grid.h"
#include "Public/Ressource/DisplayUtil.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 840

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;

constexpr SDL_FColor clear_color{ 1.0,1.0,1.0,1.0 };


int main(int argc, char* argv[])
{
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		return SDL_APP_FAILURE;
	}

	if (!SDL_CreateWindowAndRenderer("Tetris", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
		std::cerr << "pas capable de generer la fenetre ou le gpu";
		return SDL_APP_FAILURE;
	}

	SDL_SetRenderLogicalPresentation(renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

	//Creation de la grille à partir de la taille de l'écran
	constexpr float blockSize = WINDOW_HEIGHT / 22;
	constexpr float x = WINDOW_WIDTH / 4;
	constexpr float y = 20.0f;
	
	Grid grid{ x, y, blockSize };

#if IS_TESTING
	//Test enabled

	if constexpr (TYPE_OF_TEST == 0)
	{
		//ajout de bloc statiques pour tester l'affichage des blocs dans la grille
		for (size_t i = 0; i != 20; ++i)
		{
			for (size_t j = 0; j != 10; ++j)
			{
				grid.ActivateBlock(CellCoord(i,j), true);
			}
		}
	}
	else
	{
		std::cout << "IS_TESTING is true but no test specified";
	}

	
#endif // IS_TESTING

#if IS_TRACKING
	std::ofstream frameFile{"../src/log/frames.txt"};
#endif // IS_TRACKING
#if IS_USING_IMGUI
	SetupImGuiContext(window, renderer);
#endif

	bool running = true;
	SDL_Event event;
	
	//gestion de l'écriture du nombre de frames dans un fichier texte
	auto start_time = std::chrono::high_resolution_clock::now();
	unsigned long long frameCount = 0;
	int secondCount = 1;

	//affichage ImGui du nombre de frames par secondes
	std::chrono::time_point<std::chrono::high_resolution_clock> pre_time, post_time;

	while (running) {
		// Calculate time since last frame (for calculations and performance)
		pre_time = post_time;
		post_time = std::chrono::high_resolution_clock::now();
		auto elapsed_Time = post_time - pre_time;
		auto elapsed_milli = (float)std::chrono::duration<double, std::milli>(elapsed_Time).count();
		auto elapsed_second = (float)std::chrono::duration<double, std::ratio<1, 1>>(elapsed_Time).count();


#if IS_TRACKING
		std::cout << elapsed_milli << " ms elasped, " << 1.0 / elapsed_second << " fps" << std::endl;
		++frameCount;
		auto elapsed_time = std::chrono::high_resolution_clock::now() - start_time;
		if (elapsed_time >= std::chrono::seconds(secondCount)) {
			frameFile << secondCount << " seconds elapsed : " << frameCount << "fps" << std::endl;
			++secondCount;
			frameCount = 0;
		}
#endif // IS_TRACKING

		while (SDL_PollEvent(&event)) {
#if IS_USING_IMGUI
			ImGui_ImplSDL3_ProcessEvent(&event);	//Send event to ImGui, so it can handle mouse clicks et keyboard input
#endif //IS_USING_IMGUI
			switch (event.type)
			{
			case SDL_EVENT_QUIT:
				running = false;
				break;
			case SDL_EVENT_WINDOW_RESIZED:
				int width, height;
				SDL_GetWindowSize(window, &width, &height);
				SDL_SetRenderLogicalPresentation(renderer, width, height, SDL_LOGICAL_PRESENTATION_LETTERBOX);
				break;
			case SDL_EVENT_KEY_DOWN:
			case SDL_EVENT_KEY_UP:
				grid.handleInput(event.key);
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

		//update grid
		grid.Update(elapsed_second);

		//Draw grid and blocks
		grid.draw(renderer);

		
	/* MAIN LOOP - END */

#if IS_USING_IMGUI
		grid.DrawDebug();
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


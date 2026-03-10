#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "Util.h"
#include "Tetromino.h"
#include "Grid.h"

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

int main(int argc, char* argv[])
{
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		return SDL_APP_FAILURE;
	}

	if (!SDL_CreateWindowAndRenderer("Tetris", 1280, 840, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
		std::cerr << "pas capable de générer la fenêtre ou le gpu";
		return SDL_APP_FAILURE;
	}

	SDL_SetRenderLogicalPresentation(renderer, 640, 480, SDL_LOGICAL_PRESENTATION_LETTERBOX);

	SDL_FColor color = { 1.0, 0.0, 0.0, 1.0 };

	//Tetromino I = { TetrominoType::I, 200, 200 };
	//Tetromino J = { TetrominoType::J, 350, 200 };
	Grid grid{ window, renderer };
	

	bool running = true;
	SDL_Event event;

	while (running) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				running = false;
			}
		}

		SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
		SDL_RenderClear(renderer);

		//I.draw(renderer);
		//J.draw(renderer);
		grid.draw(renderer);
		/*SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_FRect rect{ 0, 0, 100, 100 };
		SDL_RenderRect(renderer, &rect);*/

		SDL_RenderPresent(renderer);

	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
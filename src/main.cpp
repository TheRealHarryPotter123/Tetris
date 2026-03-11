/* Created by Thomas Lesieur, 03/02/2026: Configuration intiale du projet
*	Modified by
*		Thomas Lesieur : 23/02/2026 : Premier affichage d'un bloc et d'un tetromino
*		Thomas Lesieur : 09/03/2026 : Restructuration de l'affichage en different fichiers
*		Maxime Sevigny : 10/03/2026 : Structuration des fichiers en .h et .cpp
*		Thomas Lesieur : 11/03/2026 : Ajout de la grille et de blocs statiques a l'interieur
*/

#include <iostream>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "Public/Ressource/Grid.h"
#include "Public/Ressource/Tetromino.h"
#include "Public/Ressource/StaticBlock.h"

#include "Public/Ressource/Util.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 840

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

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

	SDL_FColor color = { 1.0, 0.0, 0.0, 1.0 };

	//Tetromino I = { TetrominoType::I, 200, 200 };
	//Tetromino J = { TetrominoType::J, 350, 200 };

	//Creation de la grille à partir de la taille de l'écran
	constexpr float blockSize = WINDOW_HEIGHT / 22;
	constexpr float x = WINDOW_WIDTH / 4;
	constexpr float y = 20.0f;
	Grid grid{ x, y, blockSize };

	//ajout de bloc statiques pour tester l'affichage des blocs dans la grille
	for(size_t i = 0; i != 20; ++i) {
		for (size_t j = 0; j != 10; ++j) {
			StaticBlock block{ grid.getCoord(i, j), blockSize, SDL_FColor{ 0.05f * i, 0.1f * j, 0.5f, 1.0 } };
			grid.addBlock(i, j, block);
		}
	}

	bool running = true;
	SDL_Event event;

	while (running) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				running = false;
			}
			else if (event.type == SDL_EVENT_WINDOW_RESIZED) {
				int width, height;
				SDL_GetWindowSize(window, &width, &height);
				SDL_SetRenderLogicalPresentation(renderer, width, height, SDL_LOGICAL_PRESENTATION_LETTERBOX);
				//TODO : AJOUTER LES AJUSTEMENTS POUR QUE L'ON AFFICHE LES ELEMENTS SELON LA NOUVELLE TAILLE
				//i.e. recharger les objets avec les nouvelles tailles
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
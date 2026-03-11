/* Created by Maxime Sevigny, 10/03/2026 : Structuration des fichiers en .h et .cpp
*	 Modified by
*      Thomas Lesieur, 11/03/2026 : ajout de l'affichage de la grille
*/

#pragma once

#include "Util.h"

class StaticBlock
{
private:
	SDL_FPoint coord;
	SDL_FColor color;
	SDL_Vertex topBorder[4];
	SDL_Vertex LBorder[4];
	SDL_Vertex RBorder[4];
	SDL_Vertex bottomBorder[4];
	SDL_Vertex CenterBlock[4];
	const int indices[6]{ 0, 1, 2, 1, 2, 3 };
public:
	StaticBlock(float, float, float, SDL_FColor);

	StaticBlock(SDL_FPoint, float, SDL_FColor);

	StaticBlock();

	StaticBlock& operator=(const StaticBlock& other);

	void drawBlock(SDL_Renderer* renderer);
};
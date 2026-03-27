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
	EColourPalette color = COUNT;

	SDL_Vertex topBorder[4];
	SDL_Vertex LBorder[4];
	SDL_Vertex RBorder[4];
	SDL_Vertex bottomBorder[4];
	SDL_Vertex CenterBlock[4];
	int indices[6]{ 0, 1, 2, 1, 2, 3 };

	void UpdateColor(EColourPalette newColour);
public:
	static constexpr int NBR_COMPONENT = 5; //the number of visual components that form a static block

	StaticBlock(float, float, float, SDL_FColor);

	StaticBlock(SDL_FPoint, float, SDL_FColor);

	StaticBlock();

	StaticBlock& operator=(const StaticBlock& other);

	void drawBlock(SDL_Renderer* renderer, EColourPalette newColour);
};
/* Created by Thomas Lesieur, 09/03/2026 : Restructuration de l'affichage en different fichiers
*	Modified by
*		Maxime Sevigny, 10/03/2026 : Structuration des fichiers en .h et .cpp
*/

#pragma once

#include "Util.h"

enum TetrominoType
{
	I,
	J,
	L,
	T,
	S,
	Z,
	O
};

class Tetromino {
private:
	
	SDL_FPoint coord;
public:
	Tetromino(TetrominoType type, float x, float y);

	void draw(SDL_Renderer* renderer);
};

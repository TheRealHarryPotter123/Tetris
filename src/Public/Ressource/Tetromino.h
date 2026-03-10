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

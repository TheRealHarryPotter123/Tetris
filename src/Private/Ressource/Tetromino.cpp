/* Created by Maxime Sevigny, 10/03/2026 : Structuration des fichiers en .h et .cpp
*/

#pragma once

#include "../../Public/Ressource/Tetromino.h"
#include <SDL3/SDL_render.h>

Tetromino::Tetromino(TetrominoType type, float x, float y)
		: coord{ x, y }
{
	//SDL_FColor color;
	switch (type) {
		using enum TetrominoType;
		case I:
		{
			//color = { 0.0, 0.7, 0.9, 1.0 };
			/*blocks[0] = new StaticBlock{ coord, color };
			blocks[1] = new StaticBlock{ coord + SDL_FPoint{0, 60}, color };
			blocks[2] = new StaticBlock{ coord + SDL_FPoint{0, 120}, color };
			blocks[3] = new StaticBlock{ coord + SDL_FPoint{0, 180}, color };*/
			break;
		}
		case J:
		{
			//color = { 0.1, 0.1, 0.7, 1.0 };
			/*blocks[0] = new StaticBlock{ coord, color };
			blocks[1] = new StaticBlock{ coord + SDL_FPoint{0, 60}, color };
			blocks[2] = new StaticBlock{ coord + SDL_FPoint{0, 120}, color };
			blocks[3] = new StaticBlock{ coord + SDL_FPoint{-60, 120}, color };*/
		}
	}
}

void Tetromino::draw(SDL_Renderer* renderer) {
	/*blocks[0]->drawBlock(renderer);
	blocks[1]->drawBlock(renderer);
	blocks[2]->drawBlock(renderer);
	blocks[3]->drawBlock(renderer);*/
}

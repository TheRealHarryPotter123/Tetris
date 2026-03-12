/* Created by Thomas Lesieur, 09/03/2026 : Restructuration de l'affichage en different fichiers
*	Modified by
*		Maxime Sevigny, 10/03/2026 : Structuration des fichiers en .h et .cpp
*		Maxime Sevigny, 12/03/2026 : Redefinition of Tetromino to hold the cells that it fills (no drawing logic)
*/

#pragma once

#include "Util.h"

constexpr int NBR_CELLS_PER_TETROMINO = 4;

//All tetromino types that can be used
enum TetrominoType { I, J, L, T, S, Z, O, INVALID_TETROMINO };

enum TypeOfTurn { clockwise, counter_clockwise };

class Tetromino {
private:
	
	//The coords in the grid of each block that composes the tetromino, cells[0] is considered the center
	std::vector<CellCoord> cells = std::vector<CellCoord>(NBR_CELLS_PER_TETROMINO);
	TetrominoType type = INVALID_TETROMINO;

public:
	Tetromino() = default;
	Tetromino(TetrominoType type, CellCoord center);

	TetrominoType GetType() const { return type; };
	const std::vector<CellCoord>& GetCells() const { return cells; };
	
	bool IsValid() const { return type != INVALID_TETROMINO; };
	
	bool Rotate(TypeOfTurn turn);	//Try to rotate the tetromino, if the Rotate fails, return false
	bool Fall(int nbrCells);		//Try to make the tetromino fall by x cells, return false if it fails

	void draw(SDL_Renderer* renderer);
	inline void Reset() { *this = Tetromino();};
};

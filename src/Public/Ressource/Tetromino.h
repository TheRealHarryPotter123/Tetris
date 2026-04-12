/* Created by Thomas Lesieur, 09/03/2026 : Restructuration de l'affichage en different fichiers
*	Modified by
*		Maxime Sevigny, 10/03/2026 : Structuration des fichiers en .h et .cpp
*		Maxime Sevigny, 12/03/2026 : Redefinition of Tetromino to hold the cells that it fills (no drawing logic)
*		Maxime Sevigny, 27/03/2026 : Restructuring tetromino into Struct, so movements can be done into Grid (it will be easier to check the next valid position)
*		Maxime Sevigny, 01/04/2026: Add color per tetromino
*		Maxime Sevigny, 11/04/2026: Add collision handling + basic move and rotation
*/

#pragma once

#include "Util.h"

class Grid;

constexpr int NBR_CELLS_PER_TETROMINO = 4;

enum ETypeOfTurn : std::int8_t { counter_clockwise = -1, clockwise = 1};
enum ETypeOfSidewayMove : std::int8_t { left = -1, right = 1};

class Tetromino {

	//The coords in the grid of each block that composes the tetromino, cells[0] is considered the center
	std::vector<CellCoord> cells = std::vector<CellCoord>(NBR_CELLS_PER_TETROMINO);
	
	TetrominoType type = INVALID_TETROMINO;


public:
	Tetromino() = default;
	Tetromino(TetrominoType type, CellCoord center);

	TetrominoType GetType() const { return type; }

	const std::vector<CellCoord>& GetCells() const { return cells; }
	
	bool IsValid() const { return type != INVALID_TETROMINO; }
	
	//Try to rotate the tetromino, if the Rotate fails, return false
	bool Rotate(ETypeOfTurn turn, const Grid* grid);

	//Try to make the tetromino fall by x cells, return false if it fails
	bool Fall(const Grid* grid);

	//Try to make the tetromino fall by x cells, return false if it fails
	bool MoveSideways(ETypeOfSidewayMove dir, const Grid* grid);

	inline void Reset() { *this = Tetromino();}
};

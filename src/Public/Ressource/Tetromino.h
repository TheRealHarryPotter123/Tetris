/* Created by Thomas Lesieur, 09/03/2026 : Restructuration de l'affichage en different fichiers
*	Modified by
*		Maxime Sevigny, 10/03/2026 : Structuration des fichiers en .h et .cpp
*		Maxime Sevigny, 12/03/2026 : Redefinition of Tetromino to hold the cells that it fills (no drawing logic)
*		Maxime Sevigny, 27/03/2026 : Restructuring tetromino into Struct, so movements can be done into Grid (it will be easier to check the next valid position)
*		Maxime Sevigny, 01/04/2026: Add color per tetromino
*/

#pragma once

#include "Util.h"

class Grid;

constexpr int NBR_CELLS_PER_TETROMINO = 4;

//All tetromino types that can be used
enum TetrominoType : std::uint8_t { I, J, L, T, S, Z, O, INVALID_TETROMINO };

enum ETypeOfTurn : std::uint8_t { clockwise, counter_clockwise };

class Tetromino {

	//The coords in the grid of each block that composes the tetromino, cells[0] is considered the center
	std::vector<CellCoord> cells = std::vector<CellCoord>(NBR_CELLS_PER_TETROMINO);
	
	TetrominoType type = INVALID_TETROMINO;

	EColourPalette color = COUNT;

public:
	Tetromino() = default;
	Tetromino(TetrominoType type, EColourPalette color, CellCoord center);

	TetrominoType GetType() const { return type; }
	EColourPalette GetColor() const { return color; }

	const std::vector<CellCoord>& GetCells() const { return cells; }
	
	bool IsValid() const { return type != INVALID_TETROMINO; }
	
	bool Rotate(ETypeOfTurn turn, const Grid* grid);//Try to rotate the tetromino, if the Rotate fails, return false
	bool Fall(int nbrCells, const Grid* grid);		//Try to make the tetromino fall by x cells, return false if it fails

	inline void Reset() { *this = Tetromino();}
};

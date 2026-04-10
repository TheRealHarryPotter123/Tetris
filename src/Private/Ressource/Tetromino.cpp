/* Created by Maxime Sevigny, 10/03/2026 : Structuration des fichiers en .h et .cpp
*    Modified by
*		Maxime Sevigny, 27/03/2026: Restructuring tetromino into Struct, so movements can be done into Grid (it will be easier to check the next valid position)
*/

#include "../../Public/Ressource/Tetromino.h"

#include "../../Public/Ressource/Grid.h"

class Grid;

Tetromino::Tetromino(TetrominoType type, EColourPalette color, CellCoord center)
	: type { type }, color{ color }
{

	cells[0] = center;

	switch (type) {
		using enum TetrominoType;
		case I:
		{
			cells[1] = CellCoord{ center.x, center.y + 1 };
			cells[2] = CellCoord{ center.x, center.y + 2 };
			cells[3] = CellCoord{ center.x, center.y + 3 };
			break;
		}
		case J:
		{
			cells[1] = CellCoord{ center.x, center.y + 1 };
			cells[2] = CellCoord{ center.x + 1, center.y };
			cells[3] = CellCoord{ center.x + 1, center.y};
			break;
		}
		case O:
		{
			cells[1] = CellCoord{center.x + 1, center.y + 0 };
			cells[2] = CellCoord{center.x, center.y + 1 };
			cells[3] = CellCoord{ center.x + 1, center.y + 1 };
			break;
		}
		case L: 
		{
			cells[1] = CellCoord{ center.x - 1, center.y};
			cells[2] = CellCoord{ center.x + 1, center.y };
			cells[3] = CellCoord{ center.x + 1, center.y + 1 };
			break;
		}
		case S: 
		{
			cells[1] = CellCoord{ center.x - 1, center.y};
			cells[2] = CellCoord{ center.x, center.y + 1 };
			cells[3] = CellCoord{ center.x + 1, center.y + 1 };
			break;
		}
		case T: 
		{
			cells[1] = CellCoord{ center.x + 1, center.y + 0 };
			cells[2] = CellCoord{ center.x, center.y + 1 };
			cells[3] = CellCoord{ center.x - 1, center.y};
			break;
		}
		case Z: 
		{
			cells[1] = CellCoord{ center.x - 1, center.y + +1 };
			cells[2] = CellCoord{ center.x, center.y + 1 };
			cells[3] = CellCoord{ center.x + 1, center.y};
			break;
		}
		
	}
}


bool Tetromino::Rotate(ETypeOfTurn turn, const Grid* grid)
{
	//TODO implement rotating
	return false;
}

bool Tetromino::Fall(int nbrCells, const Grid* grid)
{
	//First check if fall is possible
	std::vector<CellCoord> newCells = GetCells();

	for (size_t i = 0; i < NBR_CELLS_PER_TETROMINO; i++)
	{
		if (!newCells[i].Move(nbrCells, 0))
			return false;
		if (grid->GetCellState(newCells[i]) == ECellState::occupied_static_block)
			return false;
	}

	cells = std::move(newCells);
	return true;
}

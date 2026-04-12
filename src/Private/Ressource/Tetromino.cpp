/* Created by Maxime Sevigny, 10/03/2026 : Structuration des fichiers en .h et .cpp
*    Modified by
*		Maxime Sevigny, 27/03/2026: Restructuring tetromino into Struct, so movements can be done into Grid (it will be easier to check the next valid position)
*		Maxime Sevigny, 11/04/2026: Add collision handling + basic move and rotation
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

/* Since rotations are only by 90deg, it is pretty easy to rotate
 * Using a rotation matrix gives this change:
 *	90deg : (x,y) -> ( y,-x) 
 * -90deg : (x,y) -> (-y, x)
 * 
 * Important to note: 
 *		ETypeOfTurn::counter_clockwise = -1 
 *		ETypeOfTurn::clockwise = 1
 */
bool Tetromino::Rotate(ETypeOfTurn turn, const Grid* grid)
{
	if (type == TetrominoType::O)
		return true;

	//First check if rotation is possible
	std::vector<CellCoord> newCells {GetCells()};

	//Tetromino are created with the middle point in cells[0], so it does not move
	const CellCoord& center = cells[0];
	newCells[0] = cells[0];

	for (size_t i = 1; i < NBR_CELLS_PER_TETROMINO; i++)
	{
		//Set CellCoord so it is local to center of tetromino
		newCells[i] = center - newCells[i];
		std::swap(newCells[i].x, newCells[i].y);
		newCells[i].x = -newCells[i].x;
		newCells[i] *= turn;

		//Set CellCord to be in the Grid's space
		newCells[i] += center;

		if (!Grid::IsCellValid(newCells[i]))
			return false;

		if (grid->GetCellState(newCells[i]) == ECellState::occupied_static_block)
			return false;
	}

	cells = std::move(newCells);
	return true;
}

bool Tetromino::Fall(const Grid* grid)
{
	//First check if fall is possible
	std::vector<CellCoord> newCells = GetCells();

	for (size_t i = 0; i < NBR_CELLS_PER_TETROMINO; i++)
	{
		if (!newCells[i].Move(1, 0))
			return false;
		if (grid->GetCellState(newCells[i]) == ECellState::occupied_static_block)
			return false;
	}

	cells = std::move(newCells);
	return true;
}

bool Tetromino::MoveSideways(ETypeOfSidewayMove dir, const Grid* grid)
{
	//First check if movement is possible
	std::vector<CellCoord> newCells = GetCells();

	for (size_t i = 0; i < NBR_CELLS_PER_TETROMINO; i++)
	{
		if (!newCells[i].Move(0, dir))
			return false;
		if (grid->GetCellState(newCells[i]) == ECellState::occupied_static_block)
			return false;
	}

	cells = std::move(newCells);
	return true;
}

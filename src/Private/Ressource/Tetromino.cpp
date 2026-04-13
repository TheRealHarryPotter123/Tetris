/* Created by Maxime Sevigny, 10/03/2026 : Structuration des fichiers en .h et .cpp
*    Modified by
*		Maxime Sevigny, 27/03/2026: Restructuring tetromino into Struct, so movements can be done into Grid (it will be easier to check the next valid position)
*		Maxime Sevigny, 11/04/2026: Add collision handling + basic move and rotation
*/

#include "../../Public/Ressource/Tetromino.h"

#include "../../Public/Ressource/Grid.h"

class Grid;

Tetromino::Tetromino(TetrominoType type, CellCoord center)
	: type { type }
{

	switch (type) {
		using enum TetrominoType;
		case I:
		{
			cells[0] = CellCoord{ center.x + 1, center.y };
			cells[1] = CellCoord{ center.x + 1, center.y - 1 };
			cells[2] = CellCoord{ center.x + 1, center.y + 1 };
			cells[3] = CellCoord{ center.x + 1, center.y + 2 };
			break;
		}
		case J:
		{
			cells[0] = CellCoord{ center.x + 1, center.y };
			cells[1] = CellCoord{ center.x, center.y + 1 };
			cells[2] = CellCoord{ center.x + 1, center.y + 1 };
			cells[3] = CellCoord{ center.x + 1, center.y - 1};
			break;
		}
		case L: 
		{
			cells[0] = CellCoord{ center.x + 1, center.y };
			cells[1] = CellCoord{ center.x, center.y - 1 };
			cells[2] = CellCoord{ center.x + 1, center.y - 1 };
			cells[3] = CellCoord{ center.x + 1, center.y + 1 };
			break;
		}
		case T: 
		{
			cells[0] = center;
			cells[1] = CellCoord{ center.x + 1, center.y - 1 };
			cells[2] = CellCoord{ center.x + 1, center.y };
			cells[3] = CellCoord{ center.x + 1, center.y + 1};
			break;
		}
		case S: 
		{
			cells[0] = CellCoord{center.x + 1, center.y};
			cells[1] = center;
			cells[2] = CellCoord{ center.x, center.y + 1};
			cells[3] = CellCoord{ center.x + 1, center.y - 1 };
			break;
		}
		case Z: 
		{
			cells[0] = center;
			cells[1] = CellCoord{ center.x, center.y - 1 };
			cells[2] = CellCoord{ center.x + 1, center.y };
			cells[3] = CellCoord{ center.x + 1, center.y + 1};
			break;
		}
		case O:
		{
			cells[0] = CellCoord{center.x, center.y + 1 };
			cells[1] = center;
			cells[2] = CellCoord{center.x + 1, center.y };
			cells[3] = CellCoord{ center.x + 1, center.y + 1 };
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
	
	//we change the coordinates for each rotation test
	std::vector<CellCoord> testTable;
	if (type == TetrominoType::I)
		testTable = std::vector<CellCoord>(std::begin(kickBackLogicTableLTetromino[4 * static_cast<int>(turn == counter_clockwise) + rotationState]),
										   std::end(kickBackLogicTableLTetromino[4 * static_cast<int>(turn == counter_clockwise) + rotationState]));
	else
		testTable = std::vector<CellCoord>(std::begin(kickBackLogicTable[4 * static_cast<int>(turn == counter_clockwise) + rotationState]),
                                           std::end(kickBackLogicTable[4 * static_cast<int>(turn == counter_clockwise) + rotationState]));

	bool rotationWorked = false;
	//First check if rotation is possible
	std::vector<CellCoord> rotatedCells{ GetCells() };

	//Tetromino are created with the middle point in cells[0], so it does not move
	const CellCoord& center = cells[0];
	rotatedCells[0] = cells[0];
	for (size_t i = 0; i != NBR_CELLS_PER_TETROMINO; ++i)
	{
		//Set CellCoord so it is local to center of tetromino
		rotatedCells[i] = center - rotatedCells[i];
		std::swap(rotatedCells[i].x, rotatedCells[i].y);
		rotatedCells[i].x = -rotatedCells[i].x;
		rotatedCells[i] *= turn;

		//Set CellCord to be in the Grid's space
		rotatedCells[i] += center;
	}
	
	std::vector<CellCoord> newCells; 
	newCells.resize(4);
	for (size_t k = 0; k != NBR_TEST_ROTATION; ++k) {
		rotationWorked = true;
		for (size_t i = 0; i != NBR_CELLS_PER_TETROMINO; ++i)
		{
			newCells[i] = rotatedCells[i] + testTable[k];

			if (!Grid::IsCellValid(newCells[i]))
				rotationWorked = false;

			if (grid->GetCellState(newCells[i]) == ECellState::occupied_static_block)
				rotationWorked = false;
		}
		if (rotationWorked == true)
			break;
	}

	if (rotationWorked) {
		cells = std::move(newCells);
		rotationState = rotationState + static_cast<int>(turn);
		if (rotationState == -1)
			rotationState = 3;
		rotationState = rotationState % 4;
		return true;
	}
	else
		return false;
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

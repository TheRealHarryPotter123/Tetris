/* Created by Maxime Sevigny, 10/03/2026 : Structuration des fichiers en .h et .cpp
*    Modified by
*       Thomas Lesieur, 11/03/2026 : ajout de l'affichage de la grille
*		Maxime Sevigny, 11/03/2026: Added DrawDebug()
*		Maxime Sevigny, 12/03/2026: Added Update() to handle the changes to the grid each frame + some support function to read the grid
*		Maxime Sevigny, 27/03/2026: Added pre-defined colours and Split display and logic, so it is easier to move blocks without messing with the display
*/
#pragma once

#include "Util.h"
#include "StaticBlock.h"
#include "Tetromino.h"

enum ECellState : std::uint8_t {empty, occupied_static_block, occupied_tetromino};

struct Cell
{
	ECellState state = empty;
	EColourPalette color = purple;

	bool IsEmpty() const { return state == empty; };
};

class Grid {

/*---------- BEGIN : Display -------*/
private:
	class Rectangle {
	private:
		SDL_Vertex points[4];
		const int indices[6]{ 0, 1, 2, 2, 3, 0 };
	public:
		Rectangle(float = 0, float = 0, float = 0, float = 0);
		Rectangle& operator=(const Rectangle&);
		void draw(SDL_Renderer*);
	};
	struct requestHandler {
		bool moveLeftRequested;
		bool moveRightRequested;
		bool rotateRightRequested;
		bool rotateLeftRequested;
		bool accelerateRequested;
		bool instadropRequested;
	};

	float x, y;
	float blockSize;
	Rectangle rectsHorizontaux[NBR_CELL_HORIZONTAL + 1];
	Rectangle rectsVerticaux[NBR_CELL_VERTICAL + 1];
	StaticBlock blocks[NBR_CELL_HORIZONTAL][NBR_CELL_VERTICAL];
	
public:
	void draw(SDL_Renderer* renderer);
	SDL_FPoint getCoord(size_t, size_t) const;
	SDL_FPoint getCoord(CellCoord) const;

	/*---------- END : Display -------*/

	/*---------- BEGIN : Logic-------*/
private:
	Cell cells[NBR_CELL_HORIZONTAL][NBR_CELL_VERTICAL];

	Tetromino tetromino;
	requestHandler handler;
	
	float timeBetweenFalls = 0.75;
	float timeToNextFall = timeBetweenFalls;

	Cell& GetCell(CellCoord coord) { return cells[coord.x][coord.y]; }
	const Cell& GetCell(CellCoord coord) const { return cells[coord.x][coord.y]; }

	static bool IsCellValid(CellCoord coord) 
	{ 
		return 0 <= coord.x && coord.x < NBR_CELL_HORIZONTAL 
			&& 0 <= coord.y && coord.y < NBR_CELL_VERTICAL;
	};

	bool IsCellOccupied(CellCoord coord) const;
	ECellState GetCellState(CellCoord coord) const;

#if IS_USING_IMGUI
	bool ShouldTetrominoFall = true;
#endif

public:
	Grid(float = 0, float = 0, float = 0);
	
	void handleInput(SDL_KeyboardEvent);
	void Update(float deltaTime);
	
	void AddTetromino();

	friend bool Tetromino::Fall(int, const Grid*);
	friend bool Tetromino::Rotate(ETypeOfTurn, const Grid*);

#if IS_USING_IMGUI
	void DrawDebug();
#endif

};
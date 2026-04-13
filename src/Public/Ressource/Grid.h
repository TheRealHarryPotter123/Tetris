/* Created by Maxime Sevigny, 10/03/2026 : Structuration des fichiers en .h et .cpp
*    Modified by
*       Thomas Lesieur, 11/03/2026 : ajout de l'affichage de la grille
*		Maxime Sevigny, 11/03/2026: Added DrawDebug()
*		Maxime Sevigny, 12/03/2026: Added Update() to handle the changes to the grid each frame + some support function to read the grid
*		Maxime Sevigny, 27/03/2026: Added pre-defined colors and Split display and logic, so it is easier to move blocks without messing with the display
*		Maxime Sevigny, 11/04/2026: Add collision handling + basic move and rotation
*		Maxime Sevigny, 11/04/2026: Small tweaks to enhance input (add delay when holding left-right)
*/
#pragma once

#include <variant>
#include <random>
#include <algorithm>

#include "Util.h"
#include "StaticBlock.h"
#include "Tetromino.h"

enum ECellState : std::uint8_t {empty, occupied_static_block, occupied_tetromino};

struct Rotation_CW {};
struct Rotation_CounterCW {};
struct Fall {};
struct Right {};
struct Left {};

using movementType = std::variant<Rotation_CW, Rotation_CounterCW, Fall, Right, Left>;

struct Cell
{
	ECellState state = empty;
	TetrominoType lastTetrominoType = INVALID_TETROMINO;

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
	struct RequestHandler {
		bool holdingLeft = false;
		bool holdingRight = false;
		bool rotateRightRequested = false;
		bool rotateLeftRequested = false;
		bool accelerateRequested = false;
		bool instadropRequested = false;

		//Reset all request even if they were not realised
		void ResetInstantRequest()
		{
			rotateRightRequested = false;
			rotateLeftRequested = false;
			instadropRequested = false;
		}
	};

	

	float x, y;
	float blockSize;
	Rectangle rectsHorizontaux[NBR_CELL_HORIZONTAL + 1];
	Rectangle rectsVerticaux[NBR_CELL_VERTICAL + 1];
	StaticBlock blocks[NBR_CELL_HORIZONTAL][NBR_CELL_VERTICAL];
	std::mt19937 prng{ std::random_device{}() };
	TetrominoType randomTetrominoBag[TetrominoType::INVALID_TETROMINO] = {
		TetrominoType::I,
		TetrominoType::J,
		TetrominoType::L,
		TetrominoType::T,
		TetrominoType::S,
		TetrominoType::Z,
		TetrominoType::O,
	};
	std::size_t currentBagIndex;
public:
	void draw(SDL_Renderer* renderer);
	SDL_FPoint getCoord(size_t, size_t) const;
	SDL_FPoint getCoord(CellCoord) const;

	/*---------- END : Display -------*/

	/*---------- BEGIN : Logic-------*/
private:
	Cell cells[NBR_CELL_HORIZONTAL][NBR_CELL_VERTICAL];

	Tetromino tetromino;
	RequestHandler handler;
	
	float timeBetweenFalls = 0.5f;
	float minTimeBetweenMove = 0.16f;	//Small buffer between registering input
	float timeToNextFall = timeBetweenFalls;
	float timeToNextMove = 0;
	bool isAccelerated = false;

	Cell& GetCell(CellCoord coord) { return cells[coord.x][coord.y]; }
	const Cell& GetCell(CellCoord coord) const { return cells[coord.x][coord.y]; }

	static bool IsCellValid(CellCoord coord) 
	{ 
		return 0 <= coord.x && coord.x < NBR_CELL_HORIZONTAL 
			&& 0 <= coord.y && coord.y < NBR_CELL_VERTICAL;
	};

	bool IsCellOccupied(CellCoord coord) const;
	bool IsLineFull(int line) const;
	ECellState GetCellState(CellCoord coord) const;
	
	bool MoveTetromino(movementType move);	//Moves the tetromino according the given type of movement and returns if the tetromino was stopped
	void UpdateMove(float deltaTime);
	void UpdateFall(float deltaTime);
	void ClearLine(int line);

	bool ShouldTetrominoFall = true;

public:
	Grid(float = 0, float = 0, float = 0);
	
	void handleInput(SDL_KeyboardEvent);
	void Update(float deltaTime);
	bool AddTetromino();


	friend bool Tetromino::Fall(const Grid*);
	friend bool Tetromino::Rotate(ETypeOfTurn, const Grid*);
	friend bool Tetromino::MoveSideways(ETypeOfSidewayMove, const Grid*);

#if IS_USING_IMGUI
	void DrawDebug();
#endif

};
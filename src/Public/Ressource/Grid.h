/* Created by Maxime Sevigny, 10/03/2026 : Structuration des fichiers en .h et .cpp
*    Modified by
*       Thomas Lesieur, 11/03/2026 : ajout de l'affichage de la grille
*		Maxime Sevigny, 11/03/2026: Added DrawDebug()
*		Maxime Sevigny, 12/03/2026: Added Update() to handle the changes to the grid each frame + some support function to read the grid
*/
#pragma once

#include "Util.h"
#include "StaticBlock.h"
#include "Tetromino.h"

class Grid {
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

private:
	float x, y;
	float blockSize;
	Rectangle rectsHorizontaux[NBR_CELL_HORIZONTAL + 1];
	Rectangle rectsVerticaux[NBR_CELL_VERTICAL + 1];
	StaticBlock blocks[NBR_CELL_HORIZONTAL][NBR_CELL_VERTICAL];
	bool activeBlocks[NBR_CELL_HORIZONTAL][NBR_CELL_VERTICAL];

	Tetromino tetromino{};
	
	float timeBetweenFalls = 0.75;
	float timeToNextFall = timeBetweenFalls;

	inline bool IsCellValid(CellCoord coord) 
	{ 
		return 0 <= coord.x && coord.x < NBR_CELL_HORIZONTAL 
			&& 0 <= coord.y && coord.y < NBR_CELL_VERTICAL;
	};
	inline bool IsCellOccupied(CellCoord coord) { return activeBlocks[coord.x][coord.y]; };


#if IS_USING_IMGUI
	bool ShouldTetrominoFall = false;
#endif

public:
	Grid(float = 0, float = 0, float = 0);
	
	void Update(float deltaTime);
	
	void AddTetromino();
	inline void ActivateBlock(CellCoord coord, bool shouldActivate) { activeBlocks[coord.x][coord.y] = shouldActivate; };

	void draw(SDL_Renderer* renderer);
	SDL_FPoint getCoord(size_t, size_t) const;
	SDL_FPoint getCoord(CellCoord) const;

#if IS_USING_IMGUI
	void DrawDebug();
#endif

};
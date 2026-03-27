/* Created by Maxime Sevigny, 10/03/2026 : Structuration des fichiers en .h et .cpp
*    Modified by
*    	Thomas Lesieur : 11/03/2026 : ajout de l'affichage de la grille
*		Maxime Sevigny, 11/03/2026: Added DrawDebug()
*		Maxime Sevigny, 12/03/2026: First messy pass for base tetromino physic - tetromino are generated, fall and land (only on bottom of grid)
*		Maxime Sevigny, 27/03/2026: Split display and logic, so it is easier to move blocks without messing with the display
*/
#pragma once

#include "../../Public/Ressource/Grid.h"
#include <iostream>


bool Grid::IsCellOccupied(CellCoord coord) const
{
	return cells[coord.x][coord.y].state != empty;
}

Grid::Grid(float x, float y, float blockSize) 
	: x{x}
	, y{y}
	, blockSize{ blockSize }
	, blocks{}
	, cells {}
{
	for (size_t i = 0; i != NBR_CELL_HORIZONTAL + 1; ++i)
		rectsHorizontaux[i] = Rectangle{ x, y + i * blockSize - 1, 10 * blockSize, 2 };
	for (size_t i = 0; i != NBR_CELL_VERTICAL + 1; ++i)
		rectsVerticaux[i] = Rectangle{ x + blockSize * i - 1, y, 2, 20 * blockSize };

	for (size_t i = 0; i != NBR_CELL_HORIZONTAL; ++i)
		for (size_t j = 0; j != NBR_CELL_VERTICAL; ++j)
			blocks[i][j] = StaticBlock(getCoord(i, j), blockSize, SDL_FColor(1.0,1.0,1.0,1.0));
}

void Grid::Update(float deltaTime)
{
#if IS_USING_IMGUI
	if (!ShouldTetrominoFall)
		return; //do nothing
#endif

	if (!tetromino.IsValid())
	{
		AddTetromino();
		timeToNextFall = timeBetweenFalls;
		return;
	}

	timeToNextFall -= deltaTime;

	if (timeToNextFall <= 0)
	{
		std::vector<CellCoord> oldTetrominoCells = tetromino.GetCells();
		
		if (!tetromino.Fall(1))
		{//If fall fails, the tetromino stops falling and becomes a normal StaticBlock
			tetromino.Reset();
			return;
		}

		std::vector<CellCoord> newTetrominoCells = tetromino.GetCells();
		

		for (int i = 0; i != NBR_CELLS_PER_TETROMINO; ++i)
		{
			GetCell(oldTetrominoCells[i]).state = empty;
		}

		//We need to use 2 loops, otherwise we could deactivate a block right after we already activated it
		// TODO: we will need to implement a way to know if the block is the current tetromino or if it is a static block to avoid 
		for (int i = 0; i != NBR_CELLS_PER_TETROMINO; ++i)
		{
			GetCell(oldTetrominoCells[i]).state = occupied_tetromino;
		}
		
		timeToNextFall = timeBetweenFalls;
	}
}

void Grid::AddTetromino()
{
	TetrominoType randTetrominoType = (TetrominoType)(rand() % INVALID_TETROMINO);

	tetromino = Tetromino{ randTetrominoType, CellCoord{0,NBR_CELL_VERTICAL/2 - 1}};

	for (CellCoord cell : tetromino.GetCells())
	{
		GetCell(cell).state = occupied_tetromino;
	}
}
	
void Grid::draw(SDL_Renderer* renderer) {
	for (size_t i = 0; i != NBR_CELL_HORIZONTAL + 1; ++i)
		rectsHorizontaux[i].draw(renderer);
	
	for (size_t i = 0; i != NBR_CELL_VERTICAL + 1; ++i)
		rectsVerticaux[i].draw(renderer);

	for (size_t i = 0; i != NBR_CELL_HORIZONTAL; ++i) {
		for (size_t j = 0; j != NBR_CELL_VERTICAL; ++j) {
			if (!cells[i][j].IsEmpty())
				blocks[i][j].drawBlock(renderer, cells[i][j].color);
		}
	}

}

SDL_FPoint Grid::getCoord(size_t i, size_t j) const
{
	return SDL_FPoint{ x + blockSize * j, y + blockSize * i };
}

SDL_FPoint Grid::getCoord(CellCoord coord) const
{
	return getCoord(coord.x, coord.y);
}

Grid::Rectangle::Rectangle(float x, float y, float w,  float h)
{
	points[0] = transform(SDL_FPoint{ x, y }, SDL_FColor{ 0.0 });
	points[1] = transform(SDL_FPoint{ x + w, y }, SDL_FColor{ 0.0 });
	points[2] = transform(SDL_FPoint{ x + w, y + h }, SDL_FColor{ 0.0 });
	points[3] = transform(SDL_FPoint{ x, y + h }, SDL_FColor{ 0.0 });
}

Grid::Rectangle& Grid::Rectangle::operator=(const Rectangle& other)
{
	std::copy(std::begin(other.points), std::end(other.points), std::begin(points));
	return *this;
}

void Grid::Rectangle::draw(SDL_Renderer* renderer) {
	SDL_RenderGeometry(renderer, NULL, points, 4, indices, 6);
}

#if IS_USING_IMGUI
void Grid::DrawDebug()
{
	ImGui::Begin("Grid");
	ImGui::Text("This is the debug window for the play grid");

	ImGui::Text("Time until next fall: %f / %f", timeToNextFall, timeBetweenFalls);

	ImGui::Checkbox("Should tetromino fall?", &ShouldTetrominoFall);

	ImGui::SliderFloat("Time between block update", &timeBetweenFalls, 0.01f, 5.0f);

	if (ImGui::BeginTable("BlocksTable", NBR_CELL_HORIZONTAL))
	{
		for (int i = 0; i != NBR_CELL_HORIZONTAL; ++i)
		{
			for (int j = 0; j != NBR_CELL_VERTICAL; ++j)
			{
				//ImGui::TextColored(ImVec4(1.0, !activeBlocks[i][j], !activeBlocks[i][j],1.0), "%d", activeBlocks[i][j]);

				ImGui::TableNextColumn();
			}

			ImGui::TableNextRow();
		}

		ImGui::EndTable();
	}

	ImGui::End();
}
#endif // IS_USING_IMGUI

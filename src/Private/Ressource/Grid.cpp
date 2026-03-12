/* Created by Maxime Sevigny, 10/03/2026 : Structuration des fichiers en .h et .cpp
*    Modified by
*    	Thomas Lesieur : 11/03/2026 : ajout de l'affichage de la grille
*		Maxime Sevigny, 11/03/2026: Added DrawDebug()
*/
#pragma once

#include "../../Public/Ressource/Grid.h"
#include <iostream>


Grid::Grid(float x, float y, float blockSize) :
	blockSize{ blockSize }, x{x}, y{y},
	blocks{}, activeBlocks { false }
{
	for (size_t i = 0; i != sizeof rectsHorizontaux / sizeof rectsHorizontaux[0]; ++i)
		rectsHorizontaux[i] = Rectangle{ x, y + i * blockSize - 1, 10 * blockSize, 2};
	for (size_t i = 0; i != sizeof rectsVerticaux / sizeof rectsVerticaux[0]; ++i)
		rectsVerticaux[i] = Rectangle{ x + blockSize * i - 1, y, 2, 20 * blockSize};
}

void Grid::addBlock(size_t x, size_t y, StaticBlock block)
{
	blocks[x][y] = block;
	activeBlocks[x][y] = true;
}
	
void Grid::draw(SDL_Renderer* renderer) {
	for (size_t i = 0; i != sizeof rectsHorizontaux / sizeof rectsHorizontaux[0]; ++i)
		rectsHorizontaux[i].draw(renderer);
	for (size_t i = 0; i != sizeof rectsVerticaux / sizeof rectsVerticaux[0]; ++i)
		rectsVerticaux[i].draw(renderer);
	for (size_t i = 0; i != sizeof blocks / sizeof blocks[0]; ++i) {
		for (size_t j = 0; j != sizeof blocks[0] / sizeof blocks[0][0]; ++j) {
			if (activeBlocks[i][j])
				blocks[i][j].drawBlock(renderer);
		}
	}
}

SDL_FPoint Grid::getCoord(size_t i, size_t j) const
{
	return SDL_FPoint{ x + blockSize * j, y + blockSize * i };
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

	ImGui::End();
}
#endif // IS_USING_IMGUI

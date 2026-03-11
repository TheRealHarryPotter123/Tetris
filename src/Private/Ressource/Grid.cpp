#pragma once

#include <iostream>
#include "../../Public/Ressource/Grid.h"


Grid::Grid(SDL_Renderer* renderer) {
	int width, height;
	SDL_GetRenderOutputSize(renderer, &width, &height);
	float widthf = static_cast<float>(width);
	float heightf = static_cast<float>(height);

	std::cout << widthf << " " << heightf << std::endl;
	for (size_t i = 0; i != sizeof rectsHorizontaux / sizeof rectsHorizontaux[0]; ++i)
		rectsHorizontaux[i] = Rectangle{ widthf / 4, heightf / 22 * i + 20.0f, 10 * heightf / 22, 2};
	for (size_t i = 0; i != sizeof rectsVerticaux / sizeof rectsVerticaux[0]; ++i)
		rectsVerticaux[i] = Rectangle{ width / 4 + heightf / 22 * i, 20.0f, 2, 20 * heightf / 22};
	
}
	
void Grid::draw(SDL_Renderer* renderer) {
	for (size_t i = 0; i != sizeof rectsHorizontaux / sizeof rectsHorizontaux[0]; ++i)
		rectsHorizontaux[i].draw(renderer);
	for (size_t i = 0; i != sizeof rectsVerticaux / sizeof rectsVerticaux[0]; ++i)
		rectsVerticaux[i].draw(renderer);
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
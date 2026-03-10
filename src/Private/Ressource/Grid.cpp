#pragma once

#include <iostream>
#include "../../Public/Ressource/Grid.h"


Grid::Grid(SDL_Window* window, SDL_Renderer* renderer) {
	int width, height;
	//SDL_GetWindowSize(window, &width, &height);
	SDL_GetRenderOutputSize(renderer, &width, &height);
	std::cout << width << ", " << height << std::endl;
	x = width / 3;
	y = 0;
	w = width / 3;
	h = height;
	rect.x = 0, rect.y = 0, rect.w = width, rect.h = height;
	for (size_t i = 0; i != NB_CASES_HORIZONTALES; ++i) {
		for (size_t j = 0; j != NB_CASES_VERTICALES; j++) {
			rects[i][j].w = w / NB_CASES_HORIZONTALES;
			rects[i][j].h = w / NB_CASES_HORIZONTALES;
			rects[i][j].x = x + (w / NB_CASES_HORIZONTALES) * i;
			rects[i][j].y = y + (h / NB_CASES_VERTICALES) * j;
			std::cout << "x : " << rects[i][j].x << ", y : " << rects[i][j].y
				<< ", w : " << rects[i][j].w << ", h : " << rects[i][j].h << std::endl;
		}
	}
}
	
void Grid::draw(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0.0, 0.0, 0.0, 0.0);
	for (size_t i = 0; i != NB_CASES_HORIZONTALES; ++i) {
		for (size_t j = 0; j != NB_CASES_VERTICALES; j++) {
			SDL_RenderRect(renderer, &rects[i][j]);
		}
	}
	SDL_RenderRect(renderer, &rects[0][0]);
	SDL_RenderRect(renderer, &rect);
}
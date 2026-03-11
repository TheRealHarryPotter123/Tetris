#pragma once

#include "Util.h"
#include <iostream>


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
	float x, y, w, h;
	Rectangle rectsHorizontaux[21];
	Rectangle rectsVerticaux[11];
public:
	Grid(SDL_Renderer* renderer);
	
	void draw(SDL_Renderer* renderer);

};
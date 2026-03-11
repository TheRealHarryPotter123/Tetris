#pragma once
#pragma once

#include "Util.h"
#include "StaticBlock.h"


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
	float x, y;
	float blockSize;
	Rectangle rectsHorizontaux[21];
	Rectangle rectsVerticaux[11];
	StaticBlock blocks[20][10];
	bool activeBlocks[20][10];
public:
	Grid(float = 0, float = 0, float = 0);
	void addBlock(size_t, size_t, StaticBlock);
	void draw(SDL_Renderer* renderer);
	SDL_FPoint getCoord(size_t, size_t) const;

};
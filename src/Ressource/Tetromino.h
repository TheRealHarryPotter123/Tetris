#pragma once

class StaticBlock {
private:
	SDL_FPoint coord;
	SDL_FColor color;
	SDL_Vertex topBorder[4];
	SDL_Vertex LBorder[4];
	SDL_Vertex RBorder[4];
	SDL_Vertex bottomBorder[4];
	SDL_Vertex CenterBlock[4];
	const int indices[6]{ 0, 1, 2, 1, 2, 3 };
public:
	StaticBlock(float x, float y, SDL_FColor color) :
		coord{ x, y }, color(color)
	{
		const SDL_FPoint points[]{ {-30, -30}, {-30, 30},
								   {-20, -20}, {-20, 20},
								   {20, -20},  {20, 20},
								   {30, -30},  {30, 30} };

		CenterBlock[0].position = coord + points[2];
		CenterBlock[1].position = coord + points[3];
		CenterBlock[2].position = coord + points[4];
		CenterBlock[3].position = coord + points[5];
		for (size_t i = 0; i != sizeof CenterBlock / sizeof CenterBlock[0]; ++i)
			CenterBlock[i].color = color;

		topBorder[0].position = coord + points[2];
		topBorder[1].position = coord + points[0];
		topBorder[2].position = coord + points[4];
		topBorder[3].position = coord + points[6];
		for (size_t i = 0; i != sizeof topBorder / sizeof topBorder[0]; ++i)
			topBorder[i].color = color + SDL_FColor{ 0.5f, 0.5f, 0.5f, 0.0f };

		bottomBorder[0].position = coord + points[3];
		bottomBorder[1].position = coord + points[1];
		bottomBorder[2].position = coord + points[5];
		bottomBorder[3].position = coord + points[7];
		for (size_t i = 0; i != sizeof bottomBorder / sizeof bottomBorder[0]; ++i)
			bottomBorder[i].color = color * 0.5;

		LBorder[0].position = coord + points[0];
		LBorder[1].position = coord + points[1];
		LBorder[2].position = coord + points[2];
		LBorder[3].position = coord + points[3];
		for (size_t i = 0; i != sizeof LBorder / sizeof LBorder[0]; ++i)
			LBorder[i].color = color * 0.75;

		RBorder[0].position = coord + points[4];
		RBorder[1].position = coord + points[5];
		RBorder[2].position = coord + points[6];
		RBorder[3].position = coord + points[7];
		for (size_t i = 0; i != sizeof RBorder / sizeof RBorder[0]; ++i)
			RBorder[i].color = color * 0.75;
	}
	StaticBlock(SDL_FPoint coord, SDL_FColor color) : StaticBlock(coord.x, coord.y, color)
	{
	}

	StaticBlock() :
		coord{ 0, 0 }, color{ 0.0f,0.0f,0.0f,0.0f },
		topBorder{ 0 }, LBorder{ 0 }, RBorder{ 0 },
		bottomBorder{ 0 }, CenterBlock{ 0 }
	{
	}

	StaticBlock operator=(const StaticBlock& other) const {
		StaticBlock block;
		block.coord = other.coord;
		block.color = other.color;
		for (size_t i = 0; i < 4; i++) {
			block.CenterBlock[i] = other.CenterBlock[i];
			block.topBorder[i] = other.topBorder[i];
			block.bottomBorder[i] = other.bottomBorder[i];
			block.LBorder[i] = other.LBorder[i];
			block.RBorder[i] = other.RBorder[i];
		}
		return block;
	}

	void drawBlock(SDL_Renderer* renderer) {
		SDL_RenderGeometry(renderer, NULL, CenterBlock, 4, indices, 6);
		SDL_RenderGeometry(renderer, NULL, topBorder, 4, indices, 6);
		SDL_RenderGeometry(renderer, NULL, bottomBorder, 4, indices, 6);
		SDL_RenderGeometry(renderer, NULL, LBorder, 4, indices, 6);
		SDL_RenderGeometry(renderer, NULL, RBorder, 4, indices, 6);
	}
};

enum TetrominoType {
	I = 0,
	J = 1,
	L = 2,
	T = 3,
	S = 4,
	Z = 5,
	O = 6
};

class Tetromino {
private:
	StaticBlock* blocks[4];
	SDL_FPoint coord;
public:
	Tetromino(TetrominoType type, float x, float y) :
		blocks{ 0 }, coord{ x, y }
	{
		SDL_FColor color;
		using enum TetrominoType;
		switch (type) {
		case I:
			color = { 0.0, 0.7, 0.9, 1.0 };
			blocks[0] = new StaticBlock{ coord, color };
			blocks[1] = new StaticBlock{ coord + SDL_FPoint{0, 60}, color };
			blocks[2] = new StaticBlock{ coord + SDL_FPoint{0, 120}, color };
			blocks[3] = new StaticBlock{ coord + SDL_FPoint{0, 180}, color };
			break;
		case J:
			color = { 0.1, 0.1, 0.7, 1.0 };
			blocks[0] = new StaticBlock{ coord, color };
			blocks[1] = new StaticBlock{ coord + SDL_FPoint{0, 60}, color };
			blocks[2] = new StaticBlock{ coord + SDL_FPoint{0, 120}, color };
			blocks[3] = new StaticBlock{ coord + SDL_FPoint{-60, 120}, color };
		}
	}

	void draw(SDL_Renderer* renderer) {
		blocks[0]->drawBlock(renderer);
		blocks[1]->drawBlock(renderer);
		blocks[2]->drawBlock(renderer);
		blocks[3]->drawBlock(renderer);
	}
};

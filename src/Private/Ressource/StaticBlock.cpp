
#include "../../Public/Ressource/StaticBlock.h"

StaticBlock::StaticBlock(float x, float y, SDL_FColor color)
	: coord{ x, y }, color(color)
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

StaticBlock::StaticBlock(SDL_FPoint coord, SDL_FColor color) 
	: StaticBlock(coord.x, coord.y, color)
{}

StaticBlock::StaticBlock() 
	: coord{ 0, 0 }, color{ 0.0f,0.0f,0.0f,0.0f }
	, topBorder{ 0 }, LBorder{ 0 }, RBorder{ 0 }
	, bottomBorder{ 0 }, CenterBlock{ 0 }
{}

StaticBlock StaticBlock::operator=(const StaticBlock& other) const
{
	StaticBlock block;
	block.coord = other.coord;
	block.color = other.color;
	for (size_t i = 0; i < 4; i++)
	{
		block.CenterBlock[i] = other.CenterBlock[i];
		block.topBorder[i] = other.topBorder[i];
		block.bottomBorder[i] = other.bottomBorder[i];
		block.LBorder[i] = other.LBorder[i];
		block.RBorder[i] = other.RBorder[i];
	}
	return block;
}

void StaticBlock::drawBlock(SDL_Renderer* renderer)
{
	SDL_RenderGeometry(renderer, NULL, CenterBlock, 4, indices, 6);
	SDL_RenderGeometry(renderer, NULL, topBorder, 4, indices, 6);
	SDL_RenderGeometry(renderer, NULL, bottomBorder, 4, indices, 6);
	SDL_RenderGeometry(renderer, NULL, LBorder, 4, indices, 6);
	SDL_RenderGeometry(renderer, NULL, RBorder, 4, indices, 6);
}

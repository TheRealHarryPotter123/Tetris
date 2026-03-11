/* Created by Maxime Sevigny, 10/03/2026 : Structuration des fichiers en .h et .cpp
*	 Modified by
*      Thomas Lesieur, 11/03/2026 : ajout de l'affichage de la grille
*/

#include "../../Public/Ressource/StaticBlock.h"

StaticBlock::StaticBlock(float x, float y, float blockSize, SDL_FColor color)
	: coord{ x, y }, color(color)
{
	const float factor = 1.0f / 6.0f;
	const SDL_FPoint points[]{
		{ 0, 0 },
		{ 0, blockSize },
		{factor * blockSize, factor * blockSize},
		{factor * blockSize, 5 * factor * blockSize},
		{5 * factor * blockSize, factor * blockSize},
		{5 * factor * blockSize, 5 * factor * blockSize},
		{blockSize, 0},
		{blockSize, blockSize}
	};

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

StaticBlock::StaticBlock(SDL_FPoint coord, float blockSize, SDL_FColor color) 
	: StaticBlock(coord.x, coord.y, blockSize, color)
{}

StaticBlock::StaticBlock() 
	: coord{ 0, 0 }, color{ 0.0f,0.0f,0.0f,0.0f }
	, topBorder{ 0 }, LBorder{ 0 }, RBorder{ 0 }
	, bottomBorder{ 0 }, CenterBlock{ 0 }
{}

StaticBlock& StaticBlock::operator=(const StaticBlock& other)
{
	coord = other.coord;
	color = other.color;
	for (size_t i = 0; i < 4; i++)
	{
		CenterBlock[i] = other.CenterBlock[i];
		topBorder[i] = other.topBorder[i];
		bottomBorder[i] = other.bottomBorder[i];
		LBorder[i] = other.LBorder[i];
		RBorder[i] = other.RBorder[i];
	}
	return *this;
}

void StaticBlock::drawBlock(SDL_Renderer* renderer)
{
	SDL_RenderGeometry(renderer, NULL, CenterBlock, 4, indices, 6);
	SDL_RenderGeometry(renderer, NULL, topBorder, 4, indices, 6);
	SDL_RenderGeometry(renderer, NULL, bottomBorder, 4, indices, 6);
	SDL_RenderGeometry(renderer, NULL, LBorder, 4, indices, 6);
	SDL_RenderGeometry(renderer, NULL, RBorder, 4, indices, 6);
}

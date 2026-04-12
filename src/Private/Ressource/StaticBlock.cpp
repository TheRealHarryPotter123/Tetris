/* Created by Maxime Sevigny, 10/03/2026 : Structuration des fichiers en .h et .cpp
*	 Modified by
*      Thomas Lesieur, 11/03/2026 : ajout de l'affichage de la grille
*      Maxime Sevigny, 27/03/2026 : Added pre-defined coloured
*/

#include "../../Public/Ressource/StaticBlock.h"

StaticBlock::StaticBlock(float x, float y, float blockSize, SDL_FColor color)
	: coord{ x, y }
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
	: coord{ 0, 0 }
	, topBorder{ 0 }, LBorder{ 0 }, RBorder{ 0 }
	, bottomBorder{ 0 }, CenterBlock{ 0 }
{}

StaticBlock& StaticBlock::operator=(const StaticBlock& other)
{
	coord = other.coord;
	type = other.type;

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

void StaticBlock::UpdateType(TetrominoType newType)
{
	if (type == newType)
		return; //no need to change colors

	type = newType;
	SDL_FColor newPalette = ColorPalettes[type];

	for (size_t i = 0; i != 4; ++i)
	{
		CenterBlock[i].color = newPalette;
		topBorder[i].color = newPalette + SDL_FColor{ 0.5f, 0.5f, 0.5f, 0.0f };
		bottomBorder[i].color = newPalette * 0.5;
		LBorder[i].color = newPalette * 0.75;
		RBorder[i].color = newPalette * 0.75;
	}
}

void StaticBlock::drawBlock(SDL_Renderer* renderer, TetrominoType newType)
{
	UpdateType(newType);

	SDL_RenderGeometry(renderer, nullptr, CenterBlock, 4, indices, 6);
	SDL_RenderGeometry(renderer, nullptr, topBorder, 4, indices, 6);
	SDL_RenderGeometry(renderer, nullptr, bottomBorder, 4, indices, 6);
	SDL_RenderGeometry(renderer, nullptr, LBorder, 4, indices, 6);
	SDL_RenderGeometry(renderer, nullptr, RBorder, 4, indices, 6);
}
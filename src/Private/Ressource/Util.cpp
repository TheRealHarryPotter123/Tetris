/* Created by Maxime Sevigny, 10/03/2026 : Structuration des fichiers en .h et .cpp
*    Modified by
*        Thomas Lesieur : 10/03/2026 : Ajout de la grille
*/
#include "../../Public/Ressource/Util.h"

SDL_FPoint operator+(SDL_FPoint p1, SDL_FPoint p2)
{
	return { p1.x + p2.x, p1.y + p2.y };
}

SDL_FColor operator*(SDL_FColor color, float x)
{
	return { x * color.r, x * color.g, x * color.b, color.a };
}

SDL_FColor operator+(SDL_FColor c1, SDL_FColor c2)
{
	return { c1.r + c2.r, c1.g + c2.g, c1.b + c2.b, c1.a };
}

SDL_Vertex transform(SDL_FPoint point, SDL_FColor color)
{
	SDL_Vertex ret;
	ret.position = point;
	ret.color = color;
	return ret;
}


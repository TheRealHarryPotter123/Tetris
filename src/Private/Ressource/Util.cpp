/* Created by Maxime Sevigny, 10/03/2026 : Structuration des fichiers en .h et .cpp
*    Modified by
*        Thomas Lesieur : 10/03/2026 : Ajout de la grille
*/
#include "../../Public/Ressource/Util.h"

#include <chrono>

SDL_Vertex transform(SDL_FPoint point, SDL_FColor color)
{
	SDL_Vertex ret;
	ret.position = point;
	ret.color = color;
	return ret;
}


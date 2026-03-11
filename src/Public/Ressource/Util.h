/* Created by Thomas Lesieur, 09/03/2026 : Restructuration de l'affichage en different fichiers
*    Modified by
*        Maxime Sevigny, 10/03/2026 : Structuration des fichiers en .h et .cpp
*        Thomas Lesieur, 10/03/2026 : Ajout de la grille
*/

#pragma once

#include <SDL3/SDL.h>

SDL_FPoint operator+(SDL_FPoint p1, SDL_FPoint p2);

SDL_FColor operator*(SDL_FColor color, float x);

SDL_FColor operator+(SDL_FColor c1, SDL_FColor c2);

SDL_Vertex transform(SDL_FPoint, SDL_FColor);

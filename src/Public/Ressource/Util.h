#pragma once

#include <SDL3/SDL.h>

SDL_FPoint operator+(SDL_FPoint p1, SDL_FPoint p2);

SDL_FColor operator*(SDL_FColor color, float x);

SDL_FColor operator+(SDL_FColor c1, SDL_FColor c2);

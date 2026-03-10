#pragma once

#include <SDL3/SDL.h>

SDL_FPoint operator+(SDL_FPoint p1, SDL_FPoint p2) {
	return { p1.x + p2.x, p1.y + p2.y };
}

SDL_FColor operator*(SDL_FColor color, float x) {
	return { x * color.r, x * color.g, x * color.b, color.a };
}

SDL_FColor operator+(SDL_FColor c1, SDL_FColor c2) {
	return { c1.r + c2.r, c1.g + c2.g, c1.b + c2.b, c1.a };
}
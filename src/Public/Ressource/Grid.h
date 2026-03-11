/* Created by Thomas Lesieur
	Modified by
		MS, 11/03/2026: Added DrawDebug()

*/

#pragma once

#include "Util.h"

#define NB_CASES_HORIZONTALES 10
#define NB_CASES_VERTICALES 20

class Grid {
private:
	float x, y, w, h;
	SDL_FRect rects[NB_CASES_HORIZONTALES][NB_CASES_VERTICALES];
	SDL_FRect rect;
public:
	Grid(SDL_Window* window, SDL_Renderer* renderer);
	
	void draw(SDL_Renderer* renderer);

#if IS_USING_IMGUI
	void DrawDebug();
#endif

};
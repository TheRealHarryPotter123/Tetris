/* Created by Thomas Lesieur, 09/03/2026 : Added operators for points and color
*    Modified by
*       Maxime Sevigny, 10/03/2026 : Structuration des fichiers en .h et .cpp
*       Thomas Lesieur, 10/03/2026 : Ajout de la grille
*       Maxime Sevigny, 11/03/2026: moved #define for ImGui and testing to be in this file, Added function test()
*       Maxime Sevigny, 12/03/2026: added CellCoord
*/

#pragma once

#define IS_USING_IMGUI true 		//Allows to turn off all ImGui, since it will not be used in the final version
#define IS_TESTING false            //Allows to turn on testing
#define IS_TRACKING true            //Allows to turn on tracking of performance data in text files
#define TYPE_OF_TEST 0              //Specify which test is used, 0 = full grid, 1 = etc...

#include <SDL3/SDL.h>
#include <chrono>

#if IS_USING_IMGUI
#include "imgui.h"
#endif // IS_USING_IMGUI

//The dimensions of the grid
constexpr size_t NBR_CELL_HORIZONTAL = 20;
constexpr size_t NBR_CELL_VERTICAL = 10;

//This struct represent a coord in the grid
struct CellCoord 
{
    int x = 0, y = 0;

    CellCoord() = default;
    CellCoord(int x, int y)
        : x{ x }, y{ y } {};

    bool Move(int deltaX, int deltaY) 
    {
        int newX = x + deltaX;
        int newY = y + deltaY;

        if (newX < 0 || newY < 0 || newX >= NBR_CELL_HORIZONTAL || newY >= NBR_CELL_VERTICAL)
            return false;

        x = newX;
        y = newY;
        return true;
    }
};



//operators for SDL classes
inline SDL_FPoint operator+(SDL_FPoint p1, SDL_FPoint p2)
{
	return { p1.x + p2.x, p1.y + p2.y };
}

SDL_FColor operator+(SDL_FColor c1, SDL_FColor c2);

SDL_Vertex transform(SDL_FPoint, SDL_FColor);


inline SDL_FColor operator*(SDL_FColor color, float x)
{
	return { x * color.r, x * color.g, x * color.b, color.a };
}

inline SDL_FColor operator+(SDL_FColor c1, SDL_FColor c2)
{
	return { c1.r + c2.r, c1.g + c2.g, c1.b + c2.b, c1.a };
}

template<class F, class ... Args>
inline auto test(F f, Args&&... args)
{
    using namespace std;
    using namespace std::chrono;
    auto pre = high_resolution_clock::now();
    auto res = f(std::forward<Args>(args)...);
    auto post = high_resolution_clock::now();
    return pair{ res, post - pre };
}

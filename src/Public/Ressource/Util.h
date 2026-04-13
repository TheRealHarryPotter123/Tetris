/* Created by Thomas Lesieur, 09/03/2026 : Added operators for points and color
*    Modified by
*       Maxime Sevigny, 10/03/2026 : Structuration des fichiers en .h et .cpp
*       Thomas Lesieur, 10/03/2026 : Ajout de la grille
*       Maxime Sevigny, 11/03/2026: moved #define for ImGui and testing to be in this file, Added function test()
*       Maxime Sevigny, 12/03/2026: added CellCoord
*       Maxime Sevigny, 27/03/2026 : Added pre-defined colored in ColourPalettes
*		Maxime Sevigny, 01/04/2026: Add color per tetromino
*		Maxime Sevigny, 11/04/2026: Add collision handling + basic move and rotation
*/

#pragma once

#define IS_USING_IMGUI false 		//Allows to turn off all ImGui, since it will not be used in the final version
#define IS_TESTING false            //Allows to turn on testing
#define IS_TRACKING false            //Allows to turn on tracking of performance data in text files
#define TYPE_OF_TEST 0              //Specify which test is used, 0 = full grid, 1 = etc...

#include <SDL3/SDL.h>
#include <chrono>

#if IS_USING_IMGUI
#include "imgui.h"
#endif // IS_USING_IMGUI

//The dimensions of the grid
constexpr size_t NBR_CELL_HORIZONTAL = 20;
constexpr size_t NBR_CELL_VERTICAL = 10;
constexpr size_t NBR_TEST_ROTATION = 5;
constexpr size_t DIFFERENT_ROTATION_STATES = 8;

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
    
	CellCoord& operator*=(int scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }
	
	CellCoord& operator+=(const CellCoord& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }
	CellCoord& operator-=(const CellCoord& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    CellCoord operator+(const CellCoord& other) const {
        return CellCoord{ this->x + other.x, this->y + other.y };
    }
    CellCoord operator-(const CellCoord& other) const
    {
        return CellCoord{this->x - other.x, this->y - other.y };
    }

};

//All tetromino types that can be used
enum TetrominoType : std::uint8_t { I, J, L, T, S, Z, O, INVALID_TETROMINO };

static constexpr SDL_FColor ColorPalettes[TetrominoType::INVALID_TETROMINO]
{
	SDL_FColor{0.0f, 0.9f, 0.9f, 1.0f}, //cyan
    SDL_FColor{0.1f, 0.1f, 0.9f, 1.0f}, //blue
    SDL_FColor{0.9f, 0.5f, 0.1f, 1.0f}, //orange
    SDL_FColor{0.4f, 0.1f, 0.6f, 1.0f}, //purple
    SDL_FColor{0.0f, 0.9f, 0.0f, 1.0f}, //green
    SDL_FColor{0.9f, 0.0f, 0.0f, 1.0f}, //red
    SDL_FColor{0.9f, 0.9f, 0.0f, 1.0f}  //yellow
};

static CellCoord kickBackLogicTable[DIFFERENT_ROTATION_STATES][NBR_TEST_ROTATION] = {
    { CellCoord{0, 0}, CellCoord{-1, 0}, CellCoord{-1, -1}, CellCoord{0,  2}, CellCoord{-1,  2},}, //0->R
    { CellCoord{0, 0}, CellCoord{ 1, 0}, CellCoord{ 1,  1}, CellCoord{0, -2}, CellCoord{ 1, -2},}, //R->0
    { CellCoord{0, 0}, CellCoord{ 1, 0}, CellCoord{ 1,  1}, CellCoord{0, -2}, CellCoord{ 1, -2},}, //R->2
    { CellCoord{0, 0}, CellCoord{-1, 0}, CellCoord{-1, -1}, CellCoord{0,  2}, CellCoord{-1,  2},}, //2->R
    { CellCoord{0, 0}, CellCoord{ 1, 0}, CellCoord{ 1, -1}, CellCoord{0,  2}, CellCoord{ 1,  2},}, //2->L
    { CellCoord{0, 0}, CellCoord{-1, 0}, CellCoord{-1,  1}, CellCoord{0, -2}, CellCoord{-1, -2},}, //L->2
    { CellCoord{0, 0}, CellCoord{-1, 0}, CellCoord{-1,  1}, CellCoord{0, -2}, CellCoord{-1, -2},}, //L->0
    { CellCoord{0, 0}, CellCoord{ 1, 0}, CellCoord{ 1, -1}, CellCoord{0,  2}, CellCoord{ 1,  2},}  //0->L
};

static CellCoord kickBackLogicTableLTetromino[DIFFERENT_ROTATION_STATES][NBR_TEST_ROTATION] = {
    { CellCoord{0, 0}, CellCoord{-2, 0}, CellCoord{ 1,  0}, CellCoord{-2,  1}, CellCoord{ 1, -2},}, //0->R
    { CellCoord{0, 0}, CellCoord{-1, 0}, CellCoord{ 2,  0}, CellCoord{-1, -2}, CellCoord{ 2,  1},}, //R->2
    { CellCoord{0, 0}, CellCoord{ 2, 0}, CellCoord{-1,  0}, CellCoord{ 2, -1}, CellCoord{-1,  2},}, //2->L
    { CellCoord{0, 0}, CellCoord{ 1, 0}, CellCoord{-2,  0}, CellCoord{ 1,  2}, CellCoord{-2, -1},}, //L->0
    { CellCoord{0, 0}, CellCoord{ 2, 0}, CellCoord{-1,  0}, CellCoord{ 2, -1}, CellCoord{-1,  2},}, //R->0
    { CellCoord{0, 0}, CellCoord{ 1, 0}, CellCoord{-2,  0}, CellCoord{ 1,  2}, CellCoord{-2, -1},}, //2->R
    { CellCoord{0, 0}, CellCoord{-2, 0}, CellCoord{ 1,  0}, CellCoord{-2,  1}, CellCoord{ 1, -2},}, //L->2
    { CellCoord{0, 0}, CellCoord{-1, 0}, CellCoord{ 2,  0}, CellCoord{-1, -2}, CellCoord{ 2,  1},}  //0->L
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

//Used to handle different movement type
template <class ... P> struct Combine : P... 
{
    Combine(P... ps) : P{ ps }... {}
    using P::operator()...;
};
template <class ... F>
Combine<F...> combine(F... fs)
{
    return { fs ... };
}
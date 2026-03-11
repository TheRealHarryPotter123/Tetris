
#include "../../Public/Ressource/Util.h"

#include <chrono>

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

template<class F, class ...Args>
inline auto test(F f, Args && ...args)
{
    using namespace std;
    using namespace std::chrono;
    auto pre = high_resolution_clock::now();
    auto res = f(std::forward<Args>(args)...);
    auto post = high_resolution_clock::now();
    return pair{ res, post - pre };
}
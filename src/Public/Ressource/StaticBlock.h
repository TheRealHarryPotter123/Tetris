
#include "Util.h"

class StaticBlock
{
private:
	SDL_FPoint coord;
	SDL_FColor color;
	SDL_Vertex topBorder[4];
	SDL_Vertex LBorder[4];
	SDL_Vertex RBorder[4];
	SDL_Vertex bottomBorder[4];
	SDL_Vertex CenterBlock[4];
	const int indices[6]{ 0, 1, 2, 1, 2, 3 };
public:
	StaticBlock(float x, float y, SDL_FColor color);

	StaticBlock(SDL_FPoint coord, SDL_FColor color);

	StaticBlock();

	StaticBlock operator=(const StaticBlock& other) const;

	void drawBlock(SDL_Renderer* renderer);
};
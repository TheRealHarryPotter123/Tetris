/* Created by Maxime Sevigny, 10/03/2026 : Structuration des fichiers en .h et .cpp
*    Modified by
*    	Thomas Lesieur : 11/03/2026 : ajout de l'affichage de la grille
*		Maxime Sevigny, 11/03/2026: Added DrawDebug()
*		Maxime Sevigny, 12/03/2026: First messy pass for base tetromino physic - tetromino are generated, fall and land (only on bottom of grid)
*		Maxime Sevigny, 27/03/2026: Split display and logic, so it is easier to move blocks without messing with the display
*		Maxime Sevigny, 01/04/2026: Add color per tetromino
*		Maxime Sevigny, 11/04/2026: Add collision handling + basic move and rotation
*		Maxime Sevigny, 11/04/2026: Small tweaks to enhance input (add delay when holding left-right)
*/
#pragma once

#include "../../Public/Ressource/Grid.h"


bool Grid::IsCellOccupied(CellCoord coord) const
{
	return cells[coord.x][coord.y].state != empty;
}

bool Grid::IsLineFull(int line) const
{
	for (int i = 0; i != NBR_CELL_VERTICAL; ++i) {
		if (!IsCellOccupied(CellCoord{ line, i }))
			return false;
	}
	return true;
}

ECellState Grid::GetCellState(CellCoord coord) const
{
	return cells[coord.x][coord.y].state;
}

Grid::Grid(float x, float y, float blockSize) 
	: x{x}
	, y{y}
	, blockSize{ blockSize }
	, blocks{}
	, cells {}
	, currentBagIndex { 0 }
{
	for (size_t i = 0; i != NBR_CELL_HORIZONTAL + 1; ++i)
		rectsHorizontaux[i] = Rectangle{ x, y + i * blockSize - 1, 10 * blockSize, 2 };
	for (size_t i = 0; i != NBR_CELL_VERTICAL + 1; ++i)
		rectsVerticaux[i] = Rectangle{ x + blockSize * i - 1, y, 2, 20 * blockSize };

	for (size_t i = 0; i != NBR_CELL_HORIZONTAL; ++i)
		for (size_t j = 0; j != NBR_CELL_VERTICAL; ++j)
			blocks[i][j] = StaticBlock(getCoord(i, j), blockSize, SDL_FColor(1.0,1.0,1.0,1.0));
}

void Grid::handleInput(SDL_KeyboardEvent event)
{
	if (event.type == SDL_EVENT_KEY_DOWN)
	{
		switch (event.scancode)
		{
		case SDL_SCANCODE_LEFT:
			// move left
			handler.holdingLeft = true;
			break;
		case SDL_SCANCODE_RIGHT:
			// move right
			handler.holdingRight = true;
			break;
		case SDL_SCANCODE_LCTRL:
		case SDL_SCANCODE_Z:
			// rotate counter-clockwise (left)
			if (!event.repeat)
				handler.rotateLeftRequested = true;
			break;
		case SDL_SCANCODE_UP:
		case SDL_SCANCODE_X:
			// rotate clockwise (right)
			if (!event.repeat)
				handler.rotateRightRequested = true;
			break;
		case SDL_SCANCODE_DOWN:
			// accelerate
			handler.accelerateRequested = true;
			break;
		case SDL_SCANCODE_SPACE:
			// hard drop
			if (!event.repeat)
				handler.instadropRequested = true;
			break;
		case SDL_SCANCODE_R:
			if (!ShouldTetrominoFall)
				ClearGrid();
		}
	}
	else
	{
		switch (event.scancode)
		{
		case SDL_SCANCODE_LEFT:
			// move left
			handler.holdingLeft = false;
			break;
		case SDL_SCANCODE_RIGHT:
			// move right
			handler.holdingRight = false;
			break;
		case SDL_SCANCODE_DOWN:
			// accelerate
			handler.accelerateRequested = false;
			break;
		}
	}
}

void Grid::Update(float deltaTime)
{
	if (!ShouldTetrominoFall)
		return; //do nothing

	if (!tetromino.IsValid())
	{
		if (!AddTetromino())
			ShouldTetrominoFall = false;
		timeToNextFall = timeBetweenFalls;
		return;
	}

	UpdateMove(deltaTime);
	UpdateFall(deltaTime);

#if IS_USING_IMGUI
	DrawDebug();
#endif

	handler.ResetInstantRequest();
}

bool Grid::MoveTetromino(movementType move)
{
	std::vector<CellCoord> oldTetrominoCells = tetromino.GetCells();

	bool isTetrominoStuck = std::visit(combine(
		[this](Fall)				{ return !tetromino.Fall(this); },
		[this](Rotation_CW)		{ tetromino.Rotate(ETypeOfTurn::clockwise, this); return false; },
		[this](Rotation_CounterCW)	{ tetromino.Rotate(ETypeOfTurn::counter_clockwise, this); return false; },
		[this](Right)				{ tetromino.MoveSideways(ETypeOfSidewayMove::right, this); return false; },
		[this](Left)				{ tetromino.MoveSideways(ETypeOfSidewayMove::left, this); return false; }
	), move);

	if (isTetrominoStuck)
	{//If the tetromino is stuck, it can no longer be moved, the tetromino stops moving and becomes a normal StaticBlock
		return true;
	}

	//if tetromino is not stuck, the grid needs to be updated
	for (int i = 0; i != NBR_CELLS_PER_TETROMINO; ++i)
	{
		GetCell(oldTetrominoCells[i]).state = empty;
	}

	std::vector<CellCoord> newTetrominoCells = tetromino.GetCells();

	//We need to use 2 loops, otherwise we could deactivate a block right after we already activated it
	for (int i = 0; i != NBR_CELLS_PER_TETROMINO; ++i)
	{
		GetCell(newTetrominoCells[i]).state = occupied_tetromino;
		GetCell(newTetrominoCells[i]).lastTetrominoType = tetromino.GetType();
	}

	//the tetromino is not stuck
	return false;
}

void Grid::UpdateMove(float deltaTime)
{
	if(handler.instadropRequested) {
		while (!MoveTetromino(movementType(Fall{})));
		UpdateFall(timeToNextFall);
		handler.ResetInstantRequest();
		return;
	}

	if (handler.accelerateRequested && !isAccelerated) {
		isAccelerated = true;
		timeToNextFall /= 2;
		timeBetweenFalls /= 2;
	}
	else if (!handler.accelerateRequested && isAccelerated) {
		isAccelerated = false;
		timeBetweenFalls *= 2;
	}

	//Add delay between sideway moves when holding the buttons
	if(handler.holdingLeft || handler.holdingRight)
	{
		if (timeToNextMove > 0)
		{
			timeToNextMove -= deltaTime;
		}
		else
		{
			//If both move left and right are requested at the same time, do nothing
			if (handler.holdingLeft != handler.holdingRight)
			{
				MoveTetromino(handler.holdingLeft ? movementType(Left{}) : movementType(Right{}));
			}
			
			timeToNextMove = minTimeBetweenMove;
		}
	}
	else
	{
		timeToNextMove = 0;
	}

	//If both rotation are requested at the same time, do nothing
	if (handler.rotateLeftRequested != handler.rotateRightRequested)
	{
		MoveTetromino(handler.rotateLeftRequested ? movementType(Rotation_CounterCW{}) : movementType(Rotation_CW{}));
	}
}

void Grid::UpdateFall(float deltaTime)
{
	//Check if it is time to fall
	if (timeToNextFall > 0)
	{
		timeToNextFall -= deltaTime;
		return;
	}

	//If fall failed, check if lines were filled
	if (MoveTetromino(Fall{}))
	{
		std::vector<int> linesInvolved;
		for (auto cell : tetromino.GetCells())
		{
			GetCell(cell).state = occupied_static_block;
			if (std::find(begin(linesInvolved), end(linesInvolved), cell.x) == end(linesInvolved))
				linesInvolved.push_back(cell.x);
		}

		std::vector<int> linesToClear;
		for (auto line : linesInvolved) {
			if (IsLineFull(line))
				linesToClear.push_back(line);
		}
		
		std::sort(std::begin(linesToClear), std::end(linesToClear));
  		for (auto line : linesToClear)
			ClearLine(line);

		tetromino.Reset();
	}

	timeToNextFall = timeBetweenFalls;
}

void Grid::ClearLine(int line)
{
	// on efface les blocs de la ligne
	for (size_t i = 0; i != NBR_CELL_VERTICAL; ++i) {
		cells[line][i].state = empty;
		cells[line][i].lastTetrominoType = INVALID_TETROMINO;
	}

	for (size_t i = line; i != 1; --i) {
		std::swap(cells[i], cells[i - 1]);
	}
}

void Grid::ClearGrid()
{
	*this = Grid{ x, y, blockSize };
}

bool Grid::AddTetromino()
{
	if (currentBagIndex == 0) {
		std::shuffle(std::begin(randomTetrominoBag), std::end(randomTetrominoBag), prng);
	}
	TetrominoType randTetrominoType = randomTetrominoBag[currentBagIndex];
	currentBagIndex = (currentBagIndex + 1) % TetrominoType::INVALID_TETROMINO;

	tetromino = Tetromino(randTetrominoType, CellCoord{0,NBR_CELL_VERTICAL/2 - 1});

	for (CellCoord cell : tetromino.GetCells())
	{
		if (!IsCellValid(cell))
			return false;
		if (GetCellState(cell) == ECellState::occupied_static_block)
			return false;

		GetCell(cell).state = occupied_tetromino;
		GetCell(cell).lastTetrominoType = tetromino.GetType();

	}

	return true;
}
	
void Grid::draw(SDL_Renderer* renderer) {
	for (size_t i = 0; i != NBR_CELL_HORIZONTAL + 1; ++i)
		rectsHorizontaux[i].draw(renderer);
	
	for (size_t i = 0; i != NBR_CELL_VERTICAL + 1; ++i)
		rectsVerticaux[i].draw(renderer);

	for (size_t i = 0; i != NBR_CELL_HORIZONTAL; ++i) {
		for (size_t j = 0; j != NBR_CELL_VERTICAL; ++j) {
			if (!cells[i][j].IsEmpty())
				blocks[i][j].drawBlock(renderer, cells[i][j].lastTetrominoType);
		}
	}

}

SDL_FPoint Grid::getCoord(size_t i, size_t j) const
{
	return SDL_FPoint{ x + blockSize * j, y + blockSize * i };
}

SDL_FPoint Grid::getCoord(CellCoord coord) const
{
	return getCoord(coord.x, coord.y);
}

Grid::Rectangle::Rectangle(float x, float y, float w,  float h)
{
	points[0] = transform(SDL_FPoint{ x, y }, SDL_FColor{ 0.0 });
	points[1] = transform(SDL_FPoint{ x + w, y }, SDL_FColor{ 0.0 });
	points[2] = transform(SDL_FPoint{ x + w, y + h }, SDL_FColor{ 0.0 });
	points[3] = transform(SDL_FPoint{ x, y + h }, SDL_FColor{ 0.0 });
}

Grid::Rectangle& Grid::Rectangle::operator=(const Rectangle& other)
{
	std::copy(std::begin(other.points), std::end(other.points), std::begin(points));
	return *this;
}

void Grid::Rectangle::draw(SDL_Renderer* renderer) {
	SDL_RenderGeometry(renderer, NULL, points, 4, indices, 6);
}

#if IS_USING_IMGUI
void Grid::DrawDebug()
{
	ImGui::Begin("Grid");
	ImGui::Text("This is the debug window for the play grid");
	
	if (ImGui::TreeNodeEx("Tetromino state", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (tetromino.IsValid())
		{
			std::string typeStr;
			switch (tetromino.GetType())
			{
			case I: typeStr = "I";
				break;
			case J: typeStr = "J";
				break;
			case L: typeStr = "L";
				break;
			case T: typeStr = "T";
				break;
			case S: typeStr = "S";
				break;
			case Z: typeStr = "Z";
				break;
			case O: typeStr = "O";
				break;

			default: typeStr = "NOT VALID COLOR DEFINED";
			}
			ImGui::Text("Tetromino color: %s", typeStr.c_str());
		}
		else
		{
			ImGui::Text("Tetromino is invalid");
		}

		ImGui::TreePop();
	}



	ImGui::Text("Time until next fall: %f / %f", timeToNextFall, timeBetweenFalls);
	ImGui::Text("Time until next move: %f / %f", timeToNextMove, minTimeBetweenMove);

	ImGui::Checkbox("Should tetromino fall?", &ShouldTetrominoFall);

	ImGui::SliderFloat("Time between block update", &timeBetweenFalls, 0.01f, 3.0f);
	ImGui::SliderFloat("Time between block move", &minTimeBetweenMove, 0.01f, 1.0f);

	if (ImGui::TreeNodeEx("Grid state", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::BeginTable("BlocksTable", NBR_CELL_VERTICAL))
		{
			for (int i = 0; i != NBR_CELL_HORIZONTAL; ++i)
			{
				for (int j = 0; j != NBR_CELL_VERTICAL; ++j)
				{
					const Cell& currentCell = cells[i][j];
					ImVec4 currentColor;

					if (!currentCell.IsEmpty()) {
						SDL_FColor color = ColorPalettes[currentCell.lastTetrominoType];
						currentColor = { color.r, color.g, color.b, color.a };
					}
					else
						currentColor = { 1,1,1,1 };

					ImGui::TableNextColumn();
					ImGui::TextColored(currentColor, "%d", currentCell.state);
				}

				ImGui::TableNextRow();
			}

			ImGui::EndTable();
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNodeEx("Input", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("Holding Left\t: %d", handler.holdingLeft);
		ImGui::Text("Holding Right\t: %d", handler.holdingRight);
		ImGui::Text("Rotate Right\t: %d", handler.rotateRightRequested);
		ImGui::Text("Rotate Left\t: %d", handler.rotateLeftRequested);
		ImGui::Text("Accelerate\t: %d", handler.accelerateRequested);
		ImGui::Text("Instant Drop\t: %d", handler.instadropRequested);

		ImGui::TreePop();
	}

	ImGui::End();
}
#endif // IS_USING_IMGUI

#pragma once
#include <vector>
#include "BoardSlot.h"
class Board
{
public:
	std::vector<BoardSlot*> board;
	Board();
	~Board();
};


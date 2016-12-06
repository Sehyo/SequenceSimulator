#pragma once
#include "Board.h"
class Player
{
	Board* board;
public:
	bool activate();
	int performTurn();
	Player(Board* board);
	~Player();
};


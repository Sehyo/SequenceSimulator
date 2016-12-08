#pragma once
#include "Board.h"
#include "Card.h"

class Player
{
	Board* board;
public:
	std::vector<Card*> cards;
	bool activate();
	int performTurn();
	Player(Board* board);
	~Player();
};
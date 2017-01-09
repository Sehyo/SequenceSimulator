#pragma once
#include "Board.h"
#include "Card.h"

class Player
{
	Board* board;
public:
	std::vector<Card*> cards;
	std::vector<Card*> discardedCards;
	int team;
	bool activate();
	int performTurn();
	Player(Board* board, int team);
	~Player();
};
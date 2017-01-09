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
	bool isPlayableCard(int index);
	int performTurn();
	bool isRandomPlayer;
	bool isLearner;
	Player(Board* board, int team);
	
	~Player();
};
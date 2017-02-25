#pragma once
#include "Board.h"
#include "Card.h"
#include <random>

class Player
{
	Board* board;
	
public:
	std::vector<Card*> cards;
	std::vector<Card*> discardedCards;
	int team;
	int sequences;
	bool activate();
	bool isPlayableCard(int index);
	int performTurn();

	void useCard(int desiredCardIndex);
	void writeHTMLFile();
	void takeCard();

	bool isRandomPlayer;
	bool isLearner;
	std::random_device rd;
	std::mt19937 gen;
	std::uniform_int_distribution<unsigned long long> dis;
	Player(Board* board, int team);
	Player::Player(Board* board, int team, bool isRealPlayer); // false means learner ㅇㅅㅇ
	
	~Player();
};
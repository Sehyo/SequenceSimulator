#include "stdafx.h"
#include "Player.h"
#include "Board.h"


Player::Player(Board* board)
{
	this->board = board;
}


Player::~Player()
{
	for(int i = 0; i < this->cards.size(); i++) delete cards[i];
}

bool Player::activate() // Return true if it's our turn.
{
	return false; // We should have some kind of detection in case the game ended...
}

int Player::performTurn()
{
	return 0;
}
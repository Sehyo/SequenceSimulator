#include "stdafx.h"
#include "BoardSlot.h"

BoardSlot::BoardSlot(int suit, int number, int x, int y)
{
	this->suit = suit;
	this->number = number;
	this->x = x;
	this->y = y;
	switch(suit) // 0 == Spades, 1 == Clubs, 2 == Diamonds, 3 == Hearts
	{
		case 0:
			this->card = "S";
			break;
		case 1:
			this->card = "C";
			break;
		case 2:
			this->card = "D";
			break;
		case 3:
			this->card = "H";
			break;
	}
	this->card += std::to_string(number);
	teamChip = -1;
}

BoardSlot::BoardSlot(int x, int y) // Only corner pieces uses this constructor
{
	this->card = "DS"; // Dead Space
	this->suit = -1;
	this->number = -1;
	teamChip = -1;
	this->x = x;
	this->y = y;
}


BoardSlot::~BoardSlot()
{}
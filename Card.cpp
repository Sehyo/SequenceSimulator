#include "stdafx.h"
#include "Card.h"
#include <iostream>


Card::Card(int suit, int number)
{
	this->suit = suit;
	this->number = number;
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
}

Card::~Card()
{
	//std::cout << "Card Destroyed" << std::endl;
}
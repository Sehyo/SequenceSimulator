#include "stdafx.h"
#include "Board.h"

// So we have 2 decks, or each card is pictured twice on the board.
// I'm not sure if the official board games has a certain order of the cards
// but I'll just go with a made-up order..

Board::Board()
{
	// Initialize board
	int counter = 0; // How many cards added so far
	for(int i = 0; i < 10; i++)
	{
		for(int y = 0; y < 10; y++)
		{

			++counter;
		}
	}
}


Board::~Board()
{}

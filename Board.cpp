#include "stdafx.h"
#include "Board.h"

// So we have 2 decks, or each card is pictured twice on the board.
// I'm not sure if the official board games has a certain order of the cards
// but I'll just go with a made-up order..

Board::Board()
{
	// Initialize board

	int counter = 12; // How many cards remaining till suit change
	int currentSuit = 0; // 0 == Spades, 1 == Clubs, 2 == Diamonds, 3 == Hearts
	for(int i = 0; i < 10; i++)
	{
		for(int y = 0; y < 10; y++)
		{
			// Corner slot?
			if((i == 0 && y == 0) || (i == 0 && y == 9) || (i == 9 && y == 0) || (i == 9 && y == 9))
				board.push_back(new BoardSlot()); // card = "DS" by default
			else
			{
				// Need genius way to determine what suit we are on..
				// We dont account for jacks in the numbers
				// So card number 11 represents a queen, 12 a king and 1 is an ace.
				// Other numbers are whatever they correspond.
				board.push_back(new BoardSlot(currentSuit, counter));
				--counter;
				if(counter == 0)
				{
					++currentSuit;
					counter = 12;
					if(currentSuit == 4) currentSuit = 0;
				}
			}
		}
	}
}


Board::~Board()
{
	for(int i = 0; i < board.size(); i++) delete board[i];
}
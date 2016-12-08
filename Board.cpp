#include "stdafx.h"
#include "Board.h"

// So we have 2 decks, or each card is pictured twice on the board.
// I'm not sure if the official board games has a certain order of the cards
// but I'll just go with a made-up order..

Board::Board()
{
	// Initialize board

	int counter = 13; // How many cards remaining till suit change
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
				// 11 jack, 12 queen, 13 king. But the board doesn't have jacks, so we skip an extra time if it's 11.
				// Other numbers are whatever they correspond.
				board.push_back(new BoardSlot(currentSuit, counter));
				--counter;
				if(counter == 11) --counter;
				if(counter == 0)
				{
					++currentSuit;
					counter = 13;
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
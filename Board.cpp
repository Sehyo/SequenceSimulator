#include "stdafx.h"
#include "Board.h"
#include <fstream>
#include <iostream>

// So we have 2 decks, or each card is pictured twice on the board.
// I'm not sure if the official board games has a certain order of the cards
// but I'll just go with a made-up order..

Board::Board(std::vector<Card*>* cardStack)
{
	// Initialize board
	writeHTMLFile();
	this->cardStack = cardStack;
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
				board.push_back(new BoardSlot(currentSuit, counter, i, y));
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

void Board::writeHTMLFile()
{
	BoardSlot current;
	std::ofstream output;
	output.open("board.html"); // Truncate to 0~
	output.close();
	output.open("board.html", std::ios::out | std::ios::app);
	output << "<html><head><link rel='stylesheet' type='text/css' href='css/style.css'></head><body>";
	output << "<table style='width: 100%'>";
	for(int x = 0; x < 10; x++)
	{
		output << "<tr>";
		for(int y = 0; y < 10; y++)
		{
			current = getBoardSlot(x, y);
			output << "<td><div class='img-container'>";
			switch(current.teamChip)
			{
				case 0: //  blue chip
					output << "<img class='chip' src='images/team" << current.teamChip << ".png' alt=''>";
					break;
				case 1: // red chip
					output << "<img class='chip' src='images/team" << current.teamChip << ".png' alt=''>";
					break;
				case 2: // green chip
					output << "<img class='chip' src='images/team" << current.teamChip << ".png' alt=''>";
					break;
				default: // Nothing
					break;
			}
			output << "<img class='suit' src='images/suit" << current.suit << ".png' alt=''>";
			output << "<img class='number' src='images/" << current.number << ".png' alt=''>";
			output << "<img class='cardBackground' src='images/cardbg.png' alt=''>";
			output << "</div></td>";
		}
		output << "</tr>";
	}
	output << "</table>";
	output << "</body></html>";
	output.close();
	std::cout << "Finished writing html file" << std::endl;
}

// Terribly inefficient method, since we have to loop through the entire thing (or almost) to find the index.
// Was too lazy to make a more clever way for now, but this will only be used to get the positions for writeHTMLFile(), which shouldnt be used when learning later on anyway.
BoardSlot Board::getBoardSlot(int x, int y)
{
	BoardSlot placeHolder;
	for(int i = 0; i < board.size(); i++)
		if(board[i]->x == x && board[i]->y == y)
		{
			placeHolder.card = board[i]->card;
			placeHolder.number = board[i]->number;
			placeHolder.suit = board[i]->suit;
			placeHolder.teamChip = board[i]->teamChip;
		}
	return placeHolder;
}

Board::~Board()
{
	for(int i = 0; i < board.size(); i++) delete board[i];
}
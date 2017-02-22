#include "stdafx.h"
#include "Board.h"
#include <fstream>
#include <iostream>
#include <algorithm>

// So we have 2 decks, or each card is pictured twice on the board.
// I'm not sure if the official board games has a certain order of the cards
// but I'll just go with a made-up order..

Board::Board(std::vector<Card*>* cardStack)
{
	// Initialize board
	this->cardStack = cardStack;
	int counter = 13; // How many cards remaining till suit change
	int currentSuit = 0; // 0 == Spades, 1 == Clubs, 2 == Diamonds, 3 == Hearts
	for(int i = 0; i < 10; i++)
	{
		for(int y = 0; y < 10; y++)
		{
			// Corner slot?
			if((i == 0 && y == 0) || (i == 0 && y == 9) || (i == 9 && y == 0) || (i == 9 && y == 9))
				board.push_back(new BoardSlot(i, y)); // card = "DS" by default (Dead Space), but still need to give index
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
	int l;
	std::cout << "BOARD HAS BEEN MADE THIS SIZE IS: " << board.size() << std::endl;
	std::cin >> l;
	std::cout << "Calling writeHTMLFile()" << std::endl;
	writeHTMLFile();
}

void Board::writeHTMLFile(bool bare)
{
	std::cout << "Writing to file about to occur!" << std::endl;
	int blabla;
	//std::cin >> blabla;
	BoardSlot current(-1, -1);
	std::ofstream output;
	if(!bare)
	{
		output.open("board.html"); // Truncate to 0~
		output.close();
	}
	output.open("board.html", std::ios::out | std::ios::app);
	if(!bare)	output << "<html><head><link rel='stylesheet' type='text/css' href='css/style.css'></head><body>";
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
	if(!bare)	output << "</body></html>";
	output.close();
	std::cout << "Finished writing html file" << std::endl;
}

// int slot is the newest card to have been put on the board.
// We need to check if it completes a new sequence
// and do stuff accordingly
void Board::checkSequence(int slot)
{
	int y = (slot / 10);
	int x = slot - y * 10;
	int team = board[slot]->teamChip;
	int chipsLeft = 0, chipsRight = 0, chipsUp = 0, chipsDown = 0;
	std::vector<int> sequenceIDsSharing; // Sequences that we would be sharing a slot with. Can be max 1 per sequence.
	for(int x2 = x; x2 >= 0; x2--) // How far left can this sequence travel?
	{
		BoardSlot temp = *board[TwoDtoOneD(x2, y)];
		if(temp.teamChip)
		{


			++chipsLeft;
			sequenceIDsSharing.push_back(0);
		}
		else break;
	}
}

std::vector<int> Board::sequenceIDs(int slot)
{
	std::vector<int> ids;
	for(int i = 0; i < sequences.size(); i++)
		// Check if the current sequence has the slot inside it
		if(std::find(sequences[i].indices.begin(), sequences[i].indices.end(), slot) != sequences[i].indices.end())
			ids.push_back(sequences[i].sequenceID);
	return ids; // Return ID of all sequences that contains this slot.
}

int Board::TwoDtoOneD(int x, int y)
{
	return (x * 10 + y);
	//return (y * 10 + x);
}

BoardSlot Board::getBoardSlot(int x, int y)
{
	BoardSlot placeHolder(x, y);
	placeHolder.teamChip = 232;
	int d = TwoDtoOneD(x, y);
	placeHolder.card = board[d]->card;
	placeHolder.number = board[d]->number;
	placeHolder.suit = board[d]->suit;
	placeHolder.teamChip = board[d]->teamChip;
	 // Terrible way
	std::cout << "SIZE OF BOARD: " << board.size();
	/*for(int i = 0; i < board.size(); i++)
		if(board[i]->x == x && board[i]->y == y)
		{
			std::cout << "FUNCTION GAVE: " << d << std::endl;
			std::cout << "FOR LOOP GAVE: " << i << std::endl;
			std::cout << "X: " << x << " Y: " << y << std::endl;
			std::cin >> d;
			placeHolder.card = board[i]->card;
			placeHolder.number = board[i]->number;
			placeHolder.suit = board[i]->suit;
			placeHolder.teamChip = board[i]->teamChip;
			std::cout << "HALLELUJAH" << std::endl;
			std::cout << board[i]->teamChip << " is the chip" << std::endl;
		}
		*/
	std::cout << "RETURNING THE PLACEHOLDER TEAMCHIP IS: " << placeHolder.teamChip << std::endl;
	return placeHolder;
}

Board::~Board()
{
	for(int i = 0; i < board.size(); i++) delete board[i];
}
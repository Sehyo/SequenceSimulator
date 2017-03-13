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
	this->amountOfTeams = 0; // Main function will change this.
	this->team0Score = 0;
	this->team1Score = 0;
	this->team2Score = 0;
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
	for(int x = 0; x < 10; x++) // x is y..
	{
		output << "<tr>";
		for(int y = 0; y < 10; y++) // and y is x..
		{
			current = getBoardSlot(x, y);
			output << "<td><div class='img-container'>";
			switch(current.teamChip)
			{
				case 0: //  blue chip
					output << "<img class='chip' src='images/team" << current.teamChip << ".png' alt='" << TwoDtoOneD(x,y) << "'>";
					break;
				case 1: // red chip
					output << "<img class='chip' src='images/team" << current.teamChip << ".png' alt='" << TwoDtoOneD(x, y) << "'>";
					break;
				case 2: // green chip
					output << "<img class='chip' src='images/team" << current.teamChip << ".png' alt='" << TwoDtoOneD(x, y) << "'>";
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
// Will refactor this massive function at the end of the project..
void Board::checkSequence(int slot)
{
	std::cout << "Scary Function entered" << std::endl;
	int x = (slot / 10);
	int y = slot - x * 10;
	int team = board[slot]->teamChip;
	int scoreDelta = 0;
	int xBackTraversalChips = 0, xForwardTraversalChips = 0, yBackTraversalChips = 0, yForwardTraversalChips = 0;
	std::vector<int> sequenceIDsSharing; // Sequences that we would be sharing a slot with. Can be max 1 per sequence.
	int sizeBeforeXAxisTraversal = sequences.size();
	// Check if we can make a sequence by only traversing through X axis.
	for(int xIterator = x - 1; xIterator >= 0; xIterator--)
	{
		BoardSlot temp = *board[TwoDtoOneD(xIterator, y)];
		// Check if we can use this to help with the sequence.
		if(!(temp.teamChip == team || temp.suit == -1)) break; // This slot is not helpful, break out.
		// Potentially helpful, For each sequence, we can max share one slot with each. Check this won't create complications.
		std::vector<int> sequenceParticipations = sequenceIDs(TwoDtoOneD(xIterator, y)); // Fetch all IDs of sequences that have this slot in them.
		for(int i = 0; i < sequenceParticipations.size(); i++)
			if(std::find(sequenceIDsSharing.begin(), sequenceIDsSharing.end(), sequenceParticipations.at(i)) != sequenceIDsSharing.end()) // Check that, if we add this slot, if we would then share 2 slots with this particular sequence.
			{
				xIterator = -1; // We can't use this. Let's prepare for exit of this for loop.
				break;
			}
			else
				sequenceIDsSharing.push_back(sequenceParticipations.at(i)); // We can use this chip, let's save which sequence it is shared with.
		if(xIterator != -1) ++xBackTraversalChips; // Current chip slot must be valid to add to the sequence.
		if(xBackTraversalChips > 3) xIterator = -1; // We have enough chips for a sequence. Prepare exit.
	}
	// xBackTraversalChips now contains how many steps we can go in one direction to create a sequence.
	// Check the other direction for xTraversal
	for(int xIterator = x + 1; xIterator <= 9; xIterator++)
	{
		BoardSlot temp = *board[TwoDtoOneD(xIterator, y)];
		if(!(temp.teamChip == team || temp.suit == -1)) break;
		std::vector<int> sequenceParticipations = sequenceIDs(TwoDtoOneD(xIterator, y));
		for(int i = 0; i < sequenceParticipations.size(); i++)
			if(std::find(sequenceIDsSharing.begin(), sequenceIDsSharing.end(), sequenceParticipations.at(i)) != sequenceIDsSharing.end())
			{
				xIterator = 10;
				break;
			}
			else
				sequenceIDsSharing.push_back(sequenceParticipations.at(i));
		if(xIterator != -1) ++xForwardTraversalChips;
		if(xForwardTraversalChips > 3) xIterator = 10;
	}
	sequenceIDsSharing.clear(); // Clear some crap.
	// Do the same but for Y Axis traversal.
	for(int yIterator = y - 1; yIterator >= 0; yIterator--)
	{
		BoardSlot temp = *board[TwoDtoOneD(x, yIterator)];
		if(!(temp.teamChip == team || temp.suit == -1)) break;
		std::vector<int> sequenceParticipations = sequenceIDs(TwoDtoOneD(x, yIterator));
		for(int i = 0; i < sequenceParticipations.size(); i++)
			if(std::find(sequenceIDsSharing.begin(), sequenceIDsSharing.end(), sequenceParticipations.at(i)) != sequenceIDsSharing.end())
			{
				yIterator = -1;
				break;
			}
			else
				sequenceIDsSharing.push_back(sequenceParticipations.at(i));
		if(yIterator != -1) ++yBackTraversalChips;
		if(yBackTraversalChips > 3) yIterator = -1;
	}
	for(int yIterator = y + 1; yIterator <= 9; yIterator++)
	{
		BoardSlot temp = *board[TwoDtoOneD(x, yIterator)];
		if(!(temp.teamChip == team || temp.suit == -1)) break;
		std::vector<int> sequenceParticipations = sequenceIDs(TwoDtoOneD(x, yIterator));
		for(int i = 0; i < sequenceParticipations.size(); i++)
			if(std::find(sequenceIDsSharing.begin(), sequenceIDsSharing.end(), sequenceParticipations.at(i)) != sequenceIDsSharing.end())
			{
				yIterator = 10;
				break;
			}
			else
				sequenceIDsSharing.push_back(sequenceParticipations.at(i));
		if(yIterator != -1) ++yForwardTraversalChips;
		if(yForwardTraversalChips > 3) yIterator = 10;
	}
	// Okay, now we have how far we can go in each direction.
	// Let's create all sequences that we can make from here.
	std::vector<int> indices;
	if((xBackTraversalChips < 4 && xForwardTraversalChips < 4) && (xBackTraversalChips + xForwardTraversalChips >= 4)) // Can only make X sequence by going back and forth.
	{
		indices.push_back(TwoDtoOneD(x, y)); // Add the origin slot to the sequence.
		for(int i = 1; i <= xBackTraversalChips; i++) indices.push_back(TwoDtoOneD(x - i, y));
		for(int i = 1; indices.size() < 5; i++) indices.push_back(TwoDtoOneD(x + i, y));
		std::cout << "Creating < -- X -- > Sequence" << std::endl;
		Sequence newSequence(team, indices);
		sequences.push_back(newSequence);
		++scoreDelta;
		indices.clear();
	}
	if(xBackTraversalChips >= 4) // We can make a sequence by just going in this direction, let's make this one aswell :) (Note, if above if was true, this one can't be).
	{
		indices.push_back(TwoDtoOneD(x, y));
		for(int i = 1; indices.size() < 5; i++) indices.push_back(TwoDtoOneD(x - i, y));
		std::cout << "Creating < -- X Sequence" << std::endl;
		Sequence newSequence(team, indices);
		sequences.push_back(newSequence);
		++scoreDelta;
		indices.clear();
	}
	if(xForwardTraversalChips >= 4)
	{
		indices.push_back(TwoDtoOneD(x, y));
		for(int i = 1; indices.size() < 5; i++) indices.push_back(TwoDtoOneD(x + i, y));
		std::cout << "Creating X -- > Sequence" << std::endl;
		Sequence newSequence(team, indices);
		sequences.push_back(newSequence);
		++scoreDelta;
		indices.clear();
	}
	// Now do the same stuff but for the Y axis.
	if((yBackTraversalChips < 4 && yForwardTraversalChips < 4) && (yBackTraversalChips + yForwardTraversalChips >= 4)) // Can only make Y sequence by going back and forth.
	{
		indices.push_back(TwoDtoOneD(x, y)); // Add the origin slot to the sequence.
		for(int i = 1; i <= yBackTraversalChips; i++) indices.push_back(TwoDtoOneD(x, y - i));
		for(int i = 1; indices.size() < 5; i++) indices.push_back(TwoDtoOneD(x, y + i));
		std::cout << "Creating < -- Y -- > Sequence" << std::endl;
		Sequence newSequence(team, indices);
		sequences.push_back(newSequence);
		++scoreDelta;
		indices.clear();
	}
	if(yBackTraversalChips >= 4) // We can make a sequence by just going in this direction, let's make this one aswell :) (Note, if above if was true, this one can't be).
	{
		indices.push_back(TwoDtoOneD(x, y));
		for(int i = 1; indices.size() < 5; i++) indices.push_back(TwoDtoOneD(x, y - i));
		std::cout << "Creating < -- Y Sequence" << std::endl;
		Sequence newSequence(team, indices);
		sequences.push_back(newSequence);
		++scoreDelta;
		indices.clear();
	}
	if(yForwardTraversalChips >= 4)
	{
		indices.push_back(TwoDtoOneD(x, y));
		for(int i = 1; indices.size() < 5; i++) indices.push_back(TwoDtoOneD(x, y + i));
		std::cout << "Creating Y -- > Sequence" << std::endl;
		Sequence newSequence(team, indices);
		sequences.push_back(newSequence);
		++scoreDelta;
		indices.clear();
	}
	if(team == 0) team0Score += scoreDelta;
	else if(team == 1) team1Score += scoreDelta;
	else team2Score += scoreDelta;
	std::cout << "Scary Function exited" << std::endl;
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
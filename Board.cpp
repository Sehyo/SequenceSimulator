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
// Will refactor this massive function at the end of the project..
void Board::checkSequence(int slot)
{
	int y = (slot / 10);
	int x = slot - y * 10;
	int team = board[slot]->teamChip;
	int chipsLeft = 0, chipsRight = 0, chipsUp = 0, chipsDown = 0;
	std::vector<int> sequenceIDsSharing; // Sequences that we would be sharing a slot with. Can be max 1 per sequence.
	int sizeBeforeHorizontal = sequences.size();
	for(int x2 = x-1; x2 >= 0; x2--) // How far left can this sequence travel?
	{
		BoardSlot temp = *board[TwoDtoOneD(x2, y)];
		if(temp.teamChip == team || temp.teamChip == -1) // The chip is on the right team or a DS slot.
		{
			std::vector<int> sequenceParticipations = sequenceIDs(TwoDtoOneD(x2, y));
			for(int i = 0; i < sequenceParticipations.size(); i++) // if sequenceIDsSharing contains one of these values already, we can't build a sequence with this, since we would share more than 1 thing.
			{
				if(std::find(sequenceIDsSharing.begin(), sequenceIDsSharing.end(), sequenceParticipations.at(i)) != sequenceIDsSharing.end())
				{
					// We already share a chip with this sequence!
					// Therefore we can't build a sequence to the left.
					x2 = -1; // To exit the loop
					break;
				}
				else
				{
					// Remember that we now share a chip with this sequence.
					sequenceIDsSharing.push_back(sequenceIDsSharing.at(i));
				}
			}
			if(x2 != -1) ++chipsLeft;
			if(chipsLeft >= 4) x2 = -1; // Exit the outher loop since we have enough chips for a sequence.
		}
		else break;
	}
	sequenceIDsSharing.clear(); // Clear sequenceIDsSharing
	if(chipsLeft >= 4) // We can construct a sequence to the left.
	{
		std::vector<int> indices;
		indices.push_back(TwoDtoOneD(x, y));
		indices.push_back(TwoDtoOneD(x-1, y));
		indices.push_back(TwoDtoOneD(x-2, y));
		indices.push_back(TwoDtoOneD(x-3, y));
		indices.push_back(TwoDtoOneD(x-4, y));
		Sequence newSequence(team, indices);
		sequences.push_back(newSequence);
		switch(team)
		{
			case 0:
				++team0Score;
				if(team0Score >= winCondition)
				{
					// Team 0 has won!
					// No need to continue execution of this function.
					return;
				}
				break;
			case 1:
				++team1Score;
				if(team1Score >= winCondition)
				{
					// Team 1 has won!
					// No need to continue execution of this function.
					return;
				}
				break;
			case 2:
				++team2Score;
				if(team2Score >= winCondition)
				{
					// Team 2 has won!
					// No need to continue execution of this function.
					return;
				}
				break;
			default:
				std::cout << "Something is wrong?" << std::endl;
				int blabla;
				std::cin >> blabla;
				break;
		}
	}

	/*
	* I couldn't think of a simple fast way to refactor the above stuff to work for all 4 directions in an elegant way
	* So hugeeeeeeeeeee copy paste about to occur
	* <------------------------------------------------------------------------------------------------------------------>
	*/
	for(int x2 = x + 1; x2 <= 9; x2++) // How far right can this sequence travel?
	{
		BoardSlot temp = *board[TwoDtoOneD(x2, y)];
		if(temp.teamChip == team || temp.teamChip == -1) // The chip is on the right team or a DS slot.
		{
			std::vector<int> sequenceParticipations = sequenceIDs(TwoDtoOneD(x2, y));
			for(int i = 0; i < sequenceParticipations.size(); i++) // if sequenceIDsSharing contains one of these values already, we can't build a sequence with this, since we would share more than 1 thing.
			{
				if(std::find(sequenceIDsSharing.begin(), sequenceIDsSharing.end(), sequenceParticipations.at(i)) != sequenceIDsSharing.end())
				{
					// We already share a chip with this sequence!
					// Therefore we can't build a sequence to the left.
					x2 = -1; // To exit the loop
					break;
				}
				else
				{
					// Remember that we now share a chip with this sequence.
					sequenceIDsSharing.push_back(sequenceIDsSharing.at(i));
				}
			}
			if(x2 != -1) ++chipsRight;
			if(chipsRight >= 4) x2 = -1; // Exit the outher loop since we have enough chips for a sequence.
		}
		else break;
	}
	sequenceIDsSharing.clear(); // Clear sequenceIDsSharing
	if(chipsRight >= 4) // We can construct a sequence to the left.
	{
		std::vector<int> indices;
		indices.push_back(TwoDtoOneD(x, y));
		indices.push_back(TwoDtoOneD(x + 1, y));
		indices.push_back(TwoDtoOneD(x + 2, y));
		indices.push_back(TwoDtoOneD(x + 3, y));
		indices.push_back(TwoDtoOneD(x + 4, y));
		Sequence newSequence(team, indices);
		sequences.push_back(newSequence);
		switch(team)
		{
			case 0:
				++team0Score;
				if(team0Score >= winCondition)
				{
					// Team 0 has won!
					// No need to continue execution of this function.
					return;
				}
				break;
			case 1:
				++team1Score;
				if(team1Score >= winCondition)
				{
					// Team 1 has won!
					// No need to continue execution of this function.
					return;
				}
				break;
			case 2:
				++team2Score;
				if(team2Score >= winCondition)
				{
					// Team 2 has won!
					// No need to continue execution of this function.
					return;
				}
				break;
			default:
				std::cout << "Something is wrong?" << std::endl;
				int blabla;
				std::cin >> blabla;
				break;
		}
	}


	if(sequences.size() == sizeBeforeHorizontal)
	{
		// We did not manage to build a sequence by going only left or only right.
		// Maybe we can make one by going left and right?
		if(chipsLeft + chipsRight >= 4)
		{
			// We have enough chips to make a sequence!
			std::vector<int> indices;
			for(int i = 1; i <= chipsLeft; i++) // Add all the left chips
				indices.push_back(TwoDtoOneD(x - i, y));
			indices.push_back(TwoDtoOneD(x, y)); // Add the origin chip
			for(int i = 1; i <= chipsRight && indices.size() < 5; i++) // Add as many right chips as we need
				indices.push_back(TwoDtoOneD(x + i, y));
			Sequence newSequence(team, indices);
			sequences.push_back(newSequence);
			switch(team)
			{
				case 0:
					++team0Score;
					if(team0Score >= winCondition)
					{
						// Team 0 has won!
						// No need to continue execution of this function.
						return;
					}
					break;
				case 1:
					++team1Score;
					if(team1Score >= winCondition)
					{
						// Team 1 has won!
						// No need to continue execution of this function.
						return;
					}
					break;
				case 2:
					++team2Score;
					if(team2Score >= winCondition)
					{
						// Team 2 has won!
						// No need to continue execution of this function.
						return;
					}
					break;
				default:
					std::cout << "Something is wrong?" << std::endl;
					int blabla;
					std::cin >> blabla;
					break;
			}
		}
	}
	sequenceIDsSharing.clear();


	/*
	*
	* Now time to check vertical sequences.
	*
	*/

	int sizeBeforeVertical = sequences.size();
	for(int y2 = y - 1; y2 >= 0; y2--) // How far left can this sequence travel?
	{
		BoardSlot temp = *board[TwoDtoOneD(x, y2)];
		if(temp.teamChip == team || temp.teamChip == -1) // The chip above team or a DS slot.
		{
			std::vector<int> sequenceParticipations = sequenceIDs(TwoDtoOneD(x, y2));
			for(int i = 0; i < sequenceParticipations.size(); i++) // if sequenceIDsSharing contains one of these values already, we can't build a sequence with this, since we would share more than 1 thing.
			{
				if(std::find(sequenceIDsSharing.begin(), sequenceIDsSharing.end(), sequenceParticipations.at(i)) != sequenceIDsSharing.end())
				{
					// We already share a chip with this sequence!
					// Therefore we can't build a sequence to the left.
					y2 = -1; // To exit the loop
					break;
				}
				else
				{
					// Remember that we now share a chip with this sequence.
					sequenceIDsSharing.push_back(sequenceIDsSharing.at(i));
				}
			}
			if(y2 != -1) ++chipsUp;
			if(chipsUp >= 4) y2 = -1; // Exit the outher loop since we have enough chips for a sequence.
		}
		else break;
	}
	sequenceIDsSharing.clear(); // Clear sequenceIDsSharing
	if(chipsUp >= 4) // We can construct a sequence to the left.
	{
		std::vector<int> indices;
		indices.push_back(TwoDtoOneD(x, y));
		indices.push_back(TwoDtoOneD(x, y - 1));
		indices.push_back(TwoDtoOneD(x, y - 2));
		indices.push_back(TwoDtoOneD(x, y - 3));
		indices.push_back(TwoDtoOneD(x, y - 4));
		Sequence newSequence(team, indices);
		sequences.push_back(newSequence);
		switch(team)
		{
			case 0:
				++team0Score;
				if(team0Score >= winCondition)
				{
					// Team 0 has won!
					// No need to continue execution of this function.
					return;
				}
				break;
			case 1:
				++team1Score;
				if(team1Score >= winCondition)
				{
					// Team 1 has won!
					// No need to continue execution of this function.
					return;
				}
				break;
			case 2:
				++team2Score;
				if(team2Score >= winCondition)
				{
					// Team 2 has won!
					// No need to continue execution of this function.
					return;
				}
				break;
			default:
				std::cout << "Something is wrong?" << std::endl;
				int blabla;
				std::cin >> blabla;
				break;
		}
	}

	/*
	* I couldn't think of a simple fast way to refactor the above stuff to work for all 4 directions in an elegant way
	* So hugeeeeeeeeeee copy paste about to occur
	* <------------------------------------------------------------------------------------------------------------------>
	*/
	for(int y2 = y + 1; y2 <= 9; y2++) // How far right can this sequence travel?
	{
		BoardSlot temp = *board[TwoDtoOneD(x, y2)];
		if(temp.teamChip == team || temp.teamChip == -1) // The chip is on the right team or a DS slot.
		{
			std::vector<int> sequenceParticipations = sequenceIDs(TwoDtoOneD(x, y2));
			for(int i = 0; i < sequenceParticipations.size(); i++) // if sequenceIDsSharing contains one of these values already, we can't build a sequence with this, since we would share more than 1 thing.
			{
				if(std::find(sequenceIDsSharing.begin(), sequenceIDsSharing.end(), sequenceParticipations.at(i)) != sequenceIDsSharing.end())
				{
					// We already share a chip with this sequence!
					// Therefore we can't build a sequence to the left.
					y2 = -1; // To exit the loop
					break;
				}
				else
				{
					// Remember that we now share a chip with this sequence.
					sequenceIDsSharing.push_back(sequenceIDsSharing.at(i));
				}
			}
			if(y2 != -1) ++chipsDown;
			if(chipsDown >= 4) y2 = -1; // Exit the outher loop since we have enough chips for a sequence.
		}
		else break;
	}
	sequenceIDsSharing.clear(); // Clear sequenceIDsSharing
	if(chipsDown >= 4) // We can construct a sequence to the left.
	{
		std::vector<int> indices;
		indices.push_back(TwoDtoOneD(x, y));
		indices.push_back(TwoDtoOneD(x, y + 1));
		indices.push_back(TwoDtoOneD(x, y + 2));
		indices.push_back(TwoDtoOneD(x, y + 3));
		indices.push_back(TwoDtoOneD(x, y + 4));
		Sequence newSequence(team, indices);
		sequences.push_back(newSequence);
		switch(team)
		{
			case 0:
				++team0Score;
				if(team0Score >= winCondition)
				{
					// Team 0 has won!
					// No need to continue execution of this function.
					return;
				}
				break;
			case 1:
				++team1Score;
				if(team1Score >= winCondition)
				{
					// Team 1 has won!
					// No need to continue execution of this function.
					return;
				}
				break;
			case 2:
				++team2Score;
				if(team2Score >= winCondition)
				{
					// Team 2 has won!
					// No need to continue execution of this function.
					return;
				}
				break;
			default:
				std::cout << "Something is wrong?" << std::endl;
				int blabla;
				std::cin >> blabla;
				break;
		}
	}


	if(sequences.size() == sizeBeforeVertical)
	{
		// We did not manage to build a sequence by going only left or only right.
		// Maybe we can make one by going left and right?
		if(chipsDown + chipsRight >= 4)
		{
			// We have enough chips to make a sequence!
			std::vector<int> indices;
			for(int i = 1; i <= chipsDown; i++) // Add all the left chips
				indices.push_back(TwoDtoOneD(x, y - i));
			indices.push_back(TwoDtoOneD(x, y)); // Add the origin chip
			for(int i = 1; i <= chipsUp && indices.size() < 5; i++) // Add as many right chips as we need
				indices.push_back(TwoDtoOneD(x, y + i));
			Sequence newSequence(team, indices);
			sequences.push_back(newSequence);
			switch(team)
			{
				case 0:
					++team0Score;
					if(team0Score >= winCondition)
					{
						// Team 0 has won!
						// No need to continue execution of this function.
						return;
					}
					break;
				case 1:
					++team1Score;
					if(team1Score >= winCondition)
					{
						// Team 1 has won!
						// No need to continue execution of this function.
						return;
					}
					break;
				case 2:
					++team2Score;
					if(team2Score >= winCondition)
					{
						// Team 2 has won!
						// No need to continue execution of this function.
						return;
					}
					break;
				default:
					std::cout << "Something is wrong?" << std::endl;
					int blabla;
					std::cin >> blabla;
					break;
			}
		}
	}
	sequenceIDsSharing.clear();
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
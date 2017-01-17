#include "stdafx.h"
#include "Player.h"
#include "Board.h"
#include <random>
#include <iostream>


Player::Player(Board* board, int team)
{
	this->board = board;
	this->team = team;
	this->isRandomPlayer = true;
	this->isLearner = false;
	gen = std::mt19937(rd());
}


Player::~Player()
{
	for(int i = 0; i < this->cards.size(); i++) delete cards[i];
}

bool Player::activate() // Return true if it's our turn.
{
	return true; // We should have some kind of detection in case the game ended...
}

int Player::performTurn()
{
	// Get which card we want to play
	int desiredCardIndex;
	int stopVar = -1;
	if(this->isRandomPlayer)
	{
		desiredCardIndex = dis(gen) % cards.size();
		while(!isPlayableCard(desiredCardIndex)) desiredCardIndex = dis(gen) % cards.size(); // repeat until we have a playable card.
		std::cout << "Playable card found." << std::endl;
		std::cout << "desired index: " << desiredCardIndex;
		std::cout << "Card is: " << cards[desiredCardIndex]->card << std::endl;
		std::cout << "Card Number: " << cards[desiredCardIndex]->number << " Card suit: " << cards[desiredCardIndex]->suit << "\n";
		std::cin >> stopVar;
		// Accommodate for jack special cases.
		if(cards[desiredCardIndex]->number == 11 && (cards[desiredCardIndex]->suit == 0 || cards[desiredCardIndex]->suit == 3)) // Two eyed jack
		{
			// Make move
			std::cout << "Playing 2 eyed jack" << std::endl;
			int possibleMoves = 0;
			int desiredMove;
			for(int i = 0; i < board->board.size(); i++)
				if(board->board[i]->teamChip == -1 && board->board[i]->suit != -1) ++possibleMoves;
			desiredMove = dis(gen) % possibleMoves;
			int boardIndex = 0;
			for(boardIndex = 0; boardIndex < board->board.size(); boardIndex++)
			{
				if(board->board[boardIndex]->teamChip == -1 && board->board[boardIndex]->suit != -1) --desiredMove;
				if(desiredMove == 0) break;
			}
			std::cout << "Placing team marker." << std::endl;
			int x;
			std::cin >> x;
			board->board[boardIndex]->teamChip = team; // Place team marker.
											  // Place card in discard pile.
			discardedCards.push_back(cards.at(desiredCardIndex));
			cards.erase(cards.begin() + desiredCardIndex);
			// Take new card
			if(board->cardStack->size() > 0)
			{
				cards.push_back(board->cardStack->back());
				board->cardStack->pop_back();
			}
			board->writeHTMLFile();
			return 0;
		}
		else if(cards[desiredCardIndex]->number == 11) // One eyed jack
		{
			// Make move
			std::cout << "Playing 1 eyed jack" << std::endl;
			int possibleMoves = 0;
			int desiredMove;
			for(int i = 0; i < board->board.size(); i++) // Can probably move this above the if statement, have 2 counters, and then remove the for loop in the above one.. Can do that later..
				if(board->board[i]->teamChip != -1 && board->board[i]->teamChip != team) ++possibleMoves;
			desiredMove = dis(gen) % possibleMoves;
			for(int i = 0; i < board->board.size(); i++)
			{
				if(board->board[i]->teamChip != -1 && board->board[i]->teamChip != team) --desiredMove;
				if(desiredMove == 0)
				{
					board->board[i]->teamChip = -1;
					std::cout << "Removing team marker." << std::endl;
					int x;
					std::cin >> x;
					discardedCards.push_back(cards.at(desiredCardIndex));
					cards.erase(cards.begin() + desiredCardIndex);
					if(board->cardStack->size() > 0)
					{
						cards.push_back(board->cardStack->back());
						board->cardStack->pop_back();
					}
					board->writeHTMLFile();
					return 0;
				}
			}
		}
		else // Normal card // Jacks can cause bugs since they can make both spots taken
		{
			std::cout << "Playing normal card" << std::endl;
			std::cout << "Cards size: " << cards.size() << std::endl;
			bool twoPossibleMoves = true;
			int desiredMove = 0; // A normal card can have one or two places to be chosen to be put in.
			for(int i = 0; (i < board->board.size()) && twoPossibleMoves; i++)
				if(board->board[i]->number == cards[desiredCardIndex]->number && board->board[i]->suit == cards[desiredCardIndex]->suit && board->board[i]->teamChip != -1)
					twoPossibleMoves = false;
			if(twoPossibleMoves) desiredMove = dis(gen) % 2;
			for(int i = 0; i < board->board.size(); i++)
			{
				if(board->board[i]->number == cards[desiredCardIndex]->number && board->board[i]->suit == cards[desiredCardIndex]->suit && board->board[i]->teamChip == -1)
					if(desiredMove == 0)
					{
						// Make Move
						board->board[i]->teamChip = team;
						std::cout << "Placing team marker." << std::endl;
						int x;
						std::cin >> x;
						discardedCards.push_back(cards.at(desiredCardIndex));
						std::cout << "press" << std::endl;
						std::cin >> x;
						std::cout << "Cards size: " << cards.size() << std::endl;
						cards.erase(cards.begin() + desiredCardIndex);
						std::cout << "1 card erased, size: " << cards.size() << std::endl;
						std::cin >> x;
						std::cout << "Press" << std::endl;
						if(board->cardStack->size() > 0)
						{
							cards.push_back(board->cardStack->back());
							board->cardStack->pop_back();
						}
					}
					else
					{
						std::cout << "reducing desired move" << std::endl;
						--desiredMove;
						std::cout << "Its now: " << desiredMove << std::endl;
					}
				else std::cout << "big fat false" << std::endl;
			}
		}
	}
	else if(this->isLearner)
	{
		// To do..
	}
	else // real player
	{
		std::cout << "Team " << team << " 's turn!\n Your Cards are:" << std::endl;
		for(int i = 0; i < cards.size(); i++)
		{
			std::cout << i << " : " << cards[i]->card;
			if(!isPlayableCard(i)) std::cout << " (dead card).";
			std::cout << std::endl;
		}
		std::cout << "Enter index for card you wish to play: " << std::endl;
		std::cin >> desiredCardIndex;
		while(!isPlayableCard(desiredCardIndex))
		{
			std::cout << "You can't play that card! Choose a playable one!\n Enter index: " << std::endl;
			std::cin >> desiredCardIndex;
		}
	}
	// Place marker / Remove marker

	board->writeHTMLFile();
	return 0;
}

bool Player::isPlayableCard(int index) // Make sure it's not a dead card
{
	int desiredSuit = this->cards[index]->suit;
	int desiredNumber = this->cards[index]->number;
	if(desiredNumber == 11) // The card being evaluated is a jack. We have some special properties we need to accommodate.
	{
		// Return true if it's a two eyed jack, (Spade and hearts are one eyed).
		// If it is a one eyed jack we must check that there is an enemy marker placed on the board.
		if(desiredSuit == 0 || desiredSuit == 3) return true;
		else
		{
			for(int i = 0; i < board->board.size(); i++)
				if(board->board[i]->teamChip != -1 && board->board[i]->teamChip != team) return true;
			return false; // No marker that can be removed if we play this card
		}
	}
	bool breakOnNext = false;
	for(int i = 0; i < board->board.size(); i++)
		if(board->board[i]->number == desiredNumber && board->board[i]->suit == desiredSuit)
			if(board->board[i]->teamChip == -1) return true; // There is somewhere to place this
			else if(breakOnNext && board->board[i]->teamChip == -1) return true;
			else if(breakOnNext) return false; // A jack must have been used to place a card on one of the spots.
			else breakOnNext = true;
	return false;
}
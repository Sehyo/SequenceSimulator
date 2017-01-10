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
	if(this->isRandomPlayer)
	{
		desiredCardIndex = dis(gen) % cards.size();
		while(!isPlayableCard(desiredCardIndex)) desiredCardIndex = dis(gen) % cards.size(); // repeat until we have a playable card.
		// Accommodate for jack special cases.
		if(cards[desiredCardIndex]->number == 11 && (cards[desiredCardIndex]->suit == 0 || cards[desiredCardIndex]->suit == 3)) // Two eyed jack
		{
			// Make move
			int possibleMoves = 0;
			int desiredMove;
			for(int i = 0; i < board->board.size(); i++)
				if(board->board[i]->teamChip == -1 && board->board[i]->suit != -1) ++possibleMoves;
			desiredMove = dis(gen) % possibleMoves;
			for(int i = 0; i < board->board.size(); i++)
			{
				if(board->board[i]->teamChip == -1 && board->board[i]->suit != -1) --desiredMove;
				if(desiredMove == 0)
				{
					board->board[i]->teamChip = team; // Place team marker.
					// Place card in discard pile.
					discardedCards.push_back(cards.at(desiredCardIndex));
					cards.erase(cards.begin() + desiredCardIndex);
					// Take new card
				}
			}
		}
		else if(cards[desiredCardIndex]->number == 11) // One eyed jack
		{
			// Make move
		}
		else // Normal card
		{
			bool twoPossibleMoves = true;
			for(int i = 0; (i < board->board.size()) && twoPossibleMoves; i++)
			{
				if(board->board[i]->number == cards[desiredCardIndex]->number && board->board[i]->suit == cards[desiredCardIndex]->suit && board->board[i]->teamChip != -1)
					twoPossibleMoves = false;
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
			if(board->board[i]->teamChip == -1) return true;
			else if(breakOnNext) break;
			else breakOnNext = true;
	return true;
}
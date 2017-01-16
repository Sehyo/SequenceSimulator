// SequenceSimulator.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "Player.h"
#include "Card.h"
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <random>
#include <iterator>
// Ah how to model this game...
// I'm following rules found here: http://jaxgames.com/seq2.htm
// 100 cards on the board. No jacks, so 96 cards and 4 "dead" spaces.
// cardStack has 104 cards in it (all cards).
// Some things note worthing:
// Cards are drawn automatically for players, the "Loss of Card" rule from the link above is irrelevant.
// "Dead cards" are also replaced automatically at the player's turn. No special play through needed for that.
// The 2 decisions above were made since it feels weird to have those rules in a digitalized version and it shouldn't impact learning regardless.

// TO DO:
// Change Board to have 2D array of BoardSlot instead of a std::vector?
//
//
int main()
{
	std::vector<Player*> players;
	std::vector<Card*> cardStack;
	Board board(&cardStack);
	auto engine = std::default_random_engine(std::random_device{}());
	int amountOfPlayers;
	int amountOfTeams;
	int cardsEach;
	std::cout << "How many players?" << std::endl;
	std::cin >> amountOfPlayers;
	if(amountOfPlayers % 2 == 0 && amountOfPlayers % 3 == 0) // We have an amount of players that make either 2 or 3 teams possible.
	{
		std::cout << "Do you wish to be split up into 2 or 3 teams? (reply: 2/3)." << std::endl;
		std::cin >> amountOfTeams;
	}
	else if(amountOfPlayers % 2 == 0) amountOfTeams = 2;
	else amountOfTeams = 3;
	for(int i = 0; i < amountOfPlayers; i++) // Add players to the list 
		players.push_back(new Player(&board, i % amountOfTeams));
	if(amountOfPlayers == 2) cardsEach = 7;
	else if(amountOfPlayers == 3 || amountOfPlayers == 4) cardsEach = 6;
	else if(amountOfPlayers == 6) cardsEach = 5;
	else if(amountOfPlayers == 8 || amountOfPlayers == 9) cardsEach = 4;
	else if(amountOfPlayers == 10 || amountOfPlayers == 12) cardsEach = 3;
	else return -1; // Invalid # of players.
	for(int z = 0; z < 2; z++) // Add all cards to cardStack.
		for(int i = 0; i < 4; i++)
			for(int y = 1; y < 14; y++) // Start with ace
				cardStack.push_back(new Card(i, y));
	std::shuffle(std::begin(cardStack), std::end(cardStack), engine); // Shuffle
	for(int i = 0; i < cardsEach; i++)
		for(int y = 0; y < amountOfPlayers; y++)
		{
			players[y]->cards.push_back(cardStack.back());
			cardStack.pop_back();
		}
	//std::cout << "Player 0 card amount: " << players[0]->cards.size() << std::endl << "player 1 card amount: " << players[1]->cards.size() << std::endl << "card stack amount: " << cardStack.size();
	while(true) // Change to detect end condition later
		for(int i = 0; i < players.size(); i++)
			if(players[i]->activate())
				players[i]->performTurn();
	// Clean up
	for(int i = 0; i < players.size(); i++)	delete players[i];
	for(int i = 0; i < cardStack.size(); i++) delete cardStack[i];
    return 0;
}
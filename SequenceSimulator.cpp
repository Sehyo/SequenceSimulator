// SequenceSimulator.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "Player.h"
#include "Card.h"
#include <vector>
#include <iostream>
#include <string>
// Ah how to model this game...
// I'm following rules found here: http://jaxgames.com/seq2.htm
// 100 cards on the board. No jacks, so 96 cards and 4 "dead" spaces.
// cardStack has 104 cards in it (all cards).

int main()
{
	Board board;
	std::vector<Player*> players;
	std::vector<Card*> cardStack;
	int amountOfPlayers;
	int cardsEach;
	std::cout << "How many players?" << std::endl;
	std::cin >> amountOfPlayers;
	for(int i = 0; i < amountOfPlayers; i++) // Add players to the list
		players.push_back(new Player(&board));
	if(amountOfPlayers == 2) cardsEach = 7;
	else if(amountOfPlayers == 3 || amountOfPlayers == 4) cardsEach = 6;
	else if(amountOfPlayers == 6) cardsEach = 5;
	else if(amountOfPlayers == 8 || amountOfPlayers == 9) cardsEach = 4;
	else if(amountOfPlayers == 10 || amountOfPlayers == 12) cardsEach = 3;
	else return -1; // Invalid # of players.
	for(int z = 0; z < 2; z++) // Add all cards to cardStack.
		for(int i = 0; i < 4; i++)
			for(int y = 0; y < 13; y++)
			{

			}
	while(true) // Change to detect end condition later
		for(int i = 0; i < players.size(); i++)
			if(players.at(i)->activate())
				players.at(i)->performTurn();
	// Clean up
	for(int i = 0; i < players.size(); i++)	delete players[i];
	for(int i = 0; i < cardStack.size(); i++)	delete cardStack[i];
    return 0;
}
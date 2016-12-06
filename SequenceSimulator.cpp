// SequenceSimulator.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "Player.h"
#include <vector>
#include <iostream>
// Ah how to model this game...
// I'm following rules found here: http://jaxgames.com/seq2.htm

int main()
{
	Board board;
	std::vector<Player*> players;
	int amountOfPlayers;
	std::cout << "How many players?" << std::endl;
	std::cin >> amountOfPlayers;
	for(int i = 0; i < amountOfPlayers; i++) // Add players to the list
		players.push_back(new Player(&board));
	while(true) // Change to detect end condition later
		for(int i = 0; i < players.size(); i++)
			if(players.at(i)->activate())
				players.at(i)->performTurn();
	// Clean up
	for(int i = 0; i < players.size(); i++)
		delete players[i];
    return 0;
}
// SequenceSimulator.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "fann.h"
#include "Player.h"
#include "Card.h"
#include "Record.h"
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <random>
#include <iterator>

typedef unsigned int uint;

// Ah how to model this game...
// I'm following rules found here: http://jaxgames.com/seq2.htm
// (They removed the link appearently, archive: https://web.archive.org/web/20160623192227/http://www.jaxgames.com/seq2.htm)
// 100 cards on the board. No jacks, so 96 cards and 4 "dead" spaces.
// cardStack has 104 cards in it (all cards).
// Some things note worthing:
// Cards are drawn automatically for players, the "Loss of Card" rule from the link above is irrelevant.
// "Dead cards" are also replaced automatically at the player's turn. No special play through needed for that.
// The 2 decisions above were made since it feels weird to have those rules in a digitalized version and it shouldn't impact learning regardless.

int atleastOnePlayerHasCards(std::vector<Player*>* players);
bool noWin(Board& board);

// TO DO:
// Implement function to check for sequences (end conditions). - Done? Potentially buggy? Not sure.
int main()
{
	/* FANN Stuffs */
	// Network design.. 96 inputs for each slot (We don't need the corner slots).
	// 1 Input (0,1,2) for which team we want the output for.
	// Output == Value of configuration for team in input.
	const uint num_input = 96 + 1;
	const uint num_output = 1; // Value of state
	const uint num_layers = 3;
	const uint num_neurons_hidden = 40;
	struct fann *ann = fann_create_standard(num_layers, num_input, num_neurons_hidden, num_output);
	// Activiation Functions //struct fann *ann = fann_create_from_file("sequence_game_double.net");
	fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
	fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);
	// Execution Vars
	fann_type* calc_out;
	fann_type input[num_input];
	uint executions = 10000;
	/* --- Ends --- */
	std::vector<Player*> players;
	std::vector<Card*> cardStack;
	Board board(&cardStack);
	auto engine = std::default_random_engine(std::random_device{}());
	int amountOfPlayers;
	int amountOfRealPlayers = 99;
	int amountOfTeams;
	int cardsEach;
	std::cout << "How many players? (Total, computer and real combined)" << std::endl;
	std::cin >> amountOfPlayers;
	if(amountOfPlayers % 2 == 0 && amountOfPlayers % 3 == 0) // We have an amount of players that make either 2 or 3 teams possible.
	{
		std::cout << "Do you wish to be split up into 2 or 3 teams? (reply: 2/3)." << std::endl;
		std::cin >> amountOfTeams;
	}
	else if(amountOfPlayers % 2 == 0) amountOfTeams = 2;
	else amountOfTeams = 3;
	board.amountOfTeams = amountOfTeams;
	if(amountOfTeams == 3) board.winCondition = 1;
	else board.winCondition = 2;
	while(amountOfRealPlayers > amountOfPlayers)
	{
		std::cout << "How many real players?" << std::endl;
		std::cin >> amountOfRealPlayers;
	}
	for(int i = 0; i < amountOfRealPlayers; i++) players.push_back(new Player(&board, i % amountOfTeams, true));
	for(int i = players.size(); i < amountOfPlayers; i++) // Add players to the list
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
	std::vector<Record> trainingData;
	for(;executions > 0; executions--)
	{
		std::vector<Record> gamePlaythrough; // Record one game in here.
		while(atleastOnePlayerHasCards(&players) == 0 && noWin(board))
		{
			for(int i = 0; i < players.size(); i++)
				if(players[i]->activate())
				{
					// We only need to record the states into record where it's the learners turn.
					if(players[i]->isLearner)
					{
						Record currentConfig;
						for(int boardIter = 0; boardIter < board.board.size(); boardIter++)
						{
							if(board.board[boardIter]->suit == -1) continue; // A "DS" slot. No need to add to inputs.
							currentConfig.inputs.push_back(board.board[boardIter]->teamChip); // Push back what team "owns" this slot.
						}
						if(currentConfig.inputs.size() != 96)
						{
							std::cout << "Something went wrong?" << std::endl;
							int asd;
							std::cin >> asd;
						}
						// If this input model isn't sufficient enough we could also add like.. What cards we currently have here

						// All board slot states have been added, let's add what team we are on.
						currentConfig.inputs.push_back(players[i]->team);
						gamePlaythrough.push_back(currentConfig); // Save the config.
					}
					players[i]->performTurn();
				}
			// We should successfully have recorded a game playthrough.
			// Let's learn on it.
		}
	}
	std::cout << "Game has been completed" << std::endl;
	std::cout << "Score of Team 0: " << board.team0Score << std::endl;
	std::cout << "Score of Team 1: " << board.team1Score << std::endl;
	std::cout << "Score of Team 2: " << board.team2Score << std::endl;
	while(true)
	{

	}
	// Clean up
	for(int i = 0; i < players.size(); i++)	delete players[i];
	for(int i = 0; i < cardStack.size(); i++) delete cardStack[i];
    return 0;
}

bool noWin(Board& board)
{
	if(board.team0Score >= board.winCondition) return false;
	else if(board.team1Score >= board.winCondition) return false;
	else if(board.team2Score >= board.winCondition) return false;
	return true;
}

int atleastOnePlayerHasCards(std::vector<Player*>* players)
{
	for(int i = 0; i < players->size(); i++)
		if(players->at(i)->cards.size() > 0) return 0;
	return -1;
}
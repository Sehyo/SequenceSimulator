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
//#include "time.h"
#include <time.h>

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
	fann_type *target = new fann_type();
	fann_type *currentV = new fann_type(0);
	float alpha = 0.1;
	/* --- Ends --- */
	std::vector<Player*> players;
	std::vector<Card*> cardStack;
	int fann_wins = 0, random_wins = 0;
	Board board(&cardStack);
	auto engine = std::default_random_engine(std::random_device{}());
	int amountOfPlayers;
	int amountOfRealPlayers = 99;
	int amountOfTeams;
	int cardsEach;
	bool learnerEnabled = true;
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

	if(learnerEnabled)
	{
		// Make one random player the learner. (Assuming 0 real players)
		int learnerIndex = engine() % 2;
		players[learnerIndex]->isLearner = true;
		players[learnerIndex]->isRandomPlayer = false;

	}


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

	time_t timer;
	double initialTime = time(&timer);
	double totalTime = 0;
	double averageTime = 0;
	double lastTime = initialTime;
	double now = 0;
	int games = -1;
	for(;executions > 0; executions--)
	{
		++games;
		if(games > 0)
		{
			now = time(&timer);
			totalTime += time(&timer) - lastTime;
			lastTime = now;
			averageTime = totalTime / games;
			std::cout << "AVERAGE TIME OVER " << games << " IS: " << averageTime << std::endl;
			int adasd;
			std::cin >> adasd; 
		}
 		//std::cout << "GAME NUMBER: " << 10000 - executions << std::endl;
		//std::cout << "ENTERING EXECUTION!" << std::endl;
		int bla;
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
						currentConfig.sequenceAmountAtInput = players[i]->sequences;
						gamePlaythrough.push_back(currentConfig); // Save the config.

						// Now let's perform the turn.
						// Get all the possible actions.
						// Perform the one that gives worst value for the opponent.
						std::vector<Record> possibleDerivations;
						//std::cout << "OUR CARD SIZE BEFORE DISASTER: " << players[i]->cards.size() << std::endl;
						//std::cout << "GAME NUMBER: " << 10000 - executions << std::endl;
						for(int cCardIndex = 0; cCardIndex < players[i]->cards.size(); cCardIndex++)
						{
							std::vector<Record> derivations = players[i]->derivationsFromCard(cCardIndex);
							for(int b = 0; b < derivations.size(); b++) possibleDerivations.push_back(derivations[b]);
						}
						int infLoop = 0;
						//std::cout << "INF LOOP? " << infLoop++ << std::endl;
						//std::cout << "POSSIBLE DERIVATION COUNT B4 DISASTER: " << possibleDerivations.size() << std::endl;
						if(possibleDerivations.size() == 0 || board.team0Score != 0 || board.team1Score != 0)
						{
							/*std::cout << "REMAINING CARD INFO--------" << std::endl; // Debugging code
							std::cout << "Player 0 cards: " << std::endl;
							for(int card = 0; card < players[0]->cards.size(); card++)
							{
								std::cout << players[0]->cards[card]->card << std::endl;
							}
							std::cout << "Player 1 cards: " << std::endl;
							for(int card = 0; card < players[1]->cards.size(); card++)
							{
								std::cout << players[1]->cards[card]->card << std::endl;
							}
							std::cout << "Card Stack Cards: " << std::endl;
							for(int card = 0; card < board.cardStack->size(); card++)
							{
								std::cout << board.cardStack->at(card)->card << std::endl;
							}
							if(players[0]->isLearner) std::cout << "Learner is player 0." << std::endl;
							else std::cout << "Learner is player 1." << std::endl;
							std::cout << "Player 0 sequence count: " << board.team0Score << std::endl;
							std::cout << "Player 1 sequence count: " << board.team1Score << std::endl;
							std::cout << "Fann wins: " << fann_wins << std::endl;
							std::cout << "Random wins or draws: " << random_wins << std::endl;
							std::cout << "---------------------------" << std::endl; */
						}
						int enemyTeam = 1; // I will only do training for 2 team games.
						if(players[i]->team == 1) enemyTeam = 0;
						for(int cDerivation = 0; cDerivation < possibleDerivations.size(); cDerivation++)
						{
							// Prepare input.
							for(int cInput = 0; cInput < num_input - 1; cInput++)
								input[cInput] = possibleDerivations[cDerivation].inputs[cInput];
							input[num_input - 1] = enemyTeam;
							*currentV = fann_run(ann, input)[0]; // *currentV is now the value of this state for the learning player's team.
							possibleDerivations[cDerivation].value = *currentV; // Save the value.
						}

						if(possibleDerivations.size() == 0)
						{
							// We can't make a move :(
							players[i]->takeCard(); // Just take a card.
						}
						else
						{
							std::cout << "INF LOOP? " << infLoop++ << std::endl;
							// Find index of derivation with lowest value.
							int lowestValuedIndex = 0;
							for(int cDerivation = 1; cDerivation < possibleDerivations.size(); cDerivation++)
								if(possibleDerivations[cDerivation].value < possibleDerivations[lowestValuedIndex].value) lowestValuedIndex = cDerivation;
							// Decision of action is final!!!
							std::cout << "INF LOOP? " << infLoop++ << std::endl;
							if(possibleDerivations[lowestValuedIndex].cardRemoved)
							{
								players[i]->board->board[possibleDerivations[lowestValuedIndex].boardIndexUsed]->teamChip = -1;
							}
							else
							{
								players[i]->board->board[possibleDerivations[lowestValuedIndex].boardIndexUsed]->teamChip = players[i]->team;
								players[i]->board->checkSequence(i);
							}
							players[i]->useCard(possibleDerivations[lowestValuedIndex].cardIndexUsed); // Remove card from hand.
						}
					}
					else players[i]->performTurn();
				}
		}
		// We should successfully have recorded a game playthrough.
		// Let's learn on it.
		//std::cout << "INF LOOP? at learning " << std::endl;
		for(int currentRecord = 0; currentRecord < gamePlaythrough.size(); currentRecord++)
		{
			if(num_input != gamePlaythrough[currentRecord].inputs.size())
			{
				std::cout << "SOMETHING IS WRONG WITH INPUT SIZE!" << std::endl;
				int asdas;
				std::cin >> asdas;
			}
			// Prepare input.
			for(int cInput = 0; cInput < num_input; cInput++)
				input[cInput] = gamePlaythrough[currentRecord].inputs[cInput];
			*currentV = fann_run(ann, input)[0]; // *currentV is now the value of this state for the learning player's team.
			int reward;
			fann_type sDerived;
			if(currentRecord == gamePlaythrough.size() - 1) // Last config recorded.
			{
				sDerived = 0;
				// If we won, we should have the amount of needed sequences. Otherwise we lost.
				(gamePlaythrough[currentRecord].sequenceAmountAtInput >= board.winCondition) ? reward = 1 : reward = 0;
				reward == 1 ? ++fann_wins : ++random_wins;
			}
			else
			{
				// Fetch derived state, prepare input.
				for(int cInput = 0; cInput < num_input; cInput++)
					input[cInput] = gamePlaythrough[currentRecord + 1].inputs[cInput];
				sDerived = fann_run(ann, input)[0];
				// Put the input back
				for(int cInput = 0; cInput < num_input; cInput++)
					input[cInput] = gamePlaythrough[currentRecord].inputs[cInput];
				reward = 0;
			}
			*target = ((1 - alpha) * *currentV) + ((reward + sDerived) * alpha);
			fann_train(ann, input, target);
			//std::cout << "TRAINING PERFORMED" << std::endl;
			//std::cin >> bla;
		}
		// Reset stuff!!
		board.sequences.clear();
		board.team0Score = 0;
		board.team1Score = 0;
		board.team2Score = 0;
		for(int i = 0; i < board.board.size(); i++)	board.board[i]->teamChip = -1;
		for(int i = 0; i < players.size(); i++)	delete players[i];
		players.clear();
		for(int i = 0; i < cardStack.size(); i++) delete cardStack[i];
		cardStack.clear();
		// Below stuff is just a copy paste of the stuff at the top.
		for(int i = players.size(); i < 2; i++) // Add players to the list
			players.push_back(new Player(&board, i % 2));
		int learnerIndex = engine() % 2;
		if(learnerEnabled)
		{
			players[learnerIndex]->isLearner = true;
			players[learnerIndex]->isRandomPlayer = false;
		}
		cardsEach = 7;
		
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
	}
	//std::cout << "Learning Iterations have been executed." << std::endl;
	//std::cout << "FANN Won: " << fann_wins << " times." << std::endl;
	//std::cout << "Random Won: " << random_wins << " times." << std::endl;
	//std::cout << "Note: Wins were recorded throughout gaining learning data. Not tested afterwards" << std::endl;
	//std::cout << "Information therefore only valuable if network has been trained before." << std::endl;
	fann_save(ann, "nim_game_double.net");
	fann_destroy(ann);
	while(true)
	{
		std::cout << "Finished" << std::endl;
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
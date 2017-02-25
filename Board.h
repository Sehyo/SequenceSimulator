#pragma once
#include <vector>
#include "BoardSlot.h"
#include "Card.h"
#include "Sequence.h"

class Board
{
public:
	std::vector<BoardSlot*> board;
	std::vector<Card*>* cardStack;
	std::vector<Sequence> sequences;
	std::vector<int> sequenceIDs(int slot);
	int TwoDtoOneD(int x, int y);
	int winCondition = 0;
	int amountOfTeams;
	int team0Score, team1Score, team2Score;
	void checkSequence(int slot);
	void writeHTMLFile(bool bare = false);
	BoardSlot Board::getBoardSlot(int x, int y);
	Board(std::vector<Card*>* cardStack);
	~Board();
};
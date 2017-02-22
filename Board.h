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
	void checkSequence(int slot);
	void writeHTMLFile(bool bare = false);
	BoardSlot Board::getBoardSlot(int x, int y);
	Board(std::vector<Card*>* cardStack);
	~Board();
};
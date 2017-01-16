#pragma once
#include <vector>
#include "BoardSlot.h"
#include "Card.h"

class Board
{
public:
	std::vector<BoardSlot*> board;
	std::vector<Card*>* cardStack;
	void writeHTMLFile();
	BoardSlot Board::getBoardSlot(int x, int y);
	Board(std::vector<Card*>* cardStack);
	~Board();
};
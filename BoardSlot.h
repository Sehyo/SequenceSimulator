#pragma once
#include <string>
class BoardSlot
{
public:
	std::string card; // Example: "D4" (Diamond 4)
	int suit, number; // Not sure if I want to go with string, or ints, or both.
	int teamChip; // Which team has a chip here. -1 for no chip.
	BoardSlot();
	BoardSlot(int suit, int number);
	~BoardSlot();
};


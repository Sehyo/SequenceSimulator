#pragma once
#include <string>
class BoardSlot
{
public:
	std::string card; // Example: "D4" (Diamond 4)
	int teamChip; // Which team has a chip here. -1 for no chip.
	BoardSlot(std::string card);
	~BoardSlot();
};


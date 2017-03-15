#pragma once
#include <string>
#include <vector>
class Record
{
public:
	std::vector<int> inputs;
	int sequenceAmountAtInput;
	int cardIndexUsed;  // Not applicable for recording, but for action decision, I use this to remember.
	int boardIndexUsed;
	bool cardRemoved;
	float value; // Not applicable for recording, but for action decision, I use this to remember.
	Record();
	~Record();
};
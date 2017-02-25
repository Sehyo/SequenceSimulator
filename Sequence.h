#pragma once
#include <vector>
class Sequence
{
public:
	int team; // What team the sequence represents.
	int sequenceID;
	std::vector<int> indices; // what indices in the board vector this sequence is made of.
	Sequence(int team, std::vector<int> indices);
	~Sequence();
};
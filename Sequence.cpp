#include "stdafx.h"
#include "Sequence.h"


Sequence::Sequence(int team, std::vector<int> indices)
{
	this->indices = indices;
	this->team = team;
	this->sequenceID = indices.at(0) + indices.at(1) + indices.at(2) + indices.at(3) + indices.at(4); // Some checksum.. Haven't checked if it is actually unique 100 % of the time
}

Sequence::~Sequence()
{}
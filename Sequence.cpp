#include "stdafx.h"
#include "Sequence.h"


Sequence::Sequence(int team, std::vector<int> indices, int id)
{
	this->indices = indices;
	this->team = team;
	this->sequenceID = id;
}

Sequence::~Sequence()
{}
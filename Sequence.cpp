#include "stdafx.h"
#include "Sequence.h"
#include <iostream>


Sequence::Sequence(int team, std::vector<int> indices)
{
	this->indices = indices;
	this->team = team;
	this->sequenceID = indices.at(0) + indices.at(1) + indices.at(2) + indices.at(3) + indices.at(4); // Some checksum.. Haven't checked if it is actually unique 100 % of the time
	std::cout << "WE MADE A SEQUENCE!?" << std::endl;
	std::cout << "Indices: " << std::endl << indices.at(0) << std::endl << indices.at(1) << std::endl << indices.at(2) << std::endl << indices.at(3) << std::endl << indices.at(4) << std::endl;
	int b;
	//std::cin >> b;
}

Sequence::~Sequence()
{}
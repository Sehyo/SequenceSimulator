#pragma once
#include <string>
class Card
{
public:
	int number, suit;
	std::string card; // Example: "D4" (Diamond 4)
	Card(int suit, int number);
	~Card();
};
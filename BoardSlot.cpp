#include "stdafx.h"
#include "BoardSlot.h"


BoardSlot::BoardSlot(std::string card)
{
	this->card = card;
	teamChip = -1;
}


BoardSlot::~BoardSlot()
{}

#include "stdafx.h"
#include "Player.h"


Player::Player()
{}


Player::~Player()
{}

bool Player::activate() // Return true if it's our turn.
{
	return true; // We should have some kind of detection in case the game ended...
}
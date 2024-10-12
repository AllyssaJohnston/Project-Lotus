#include "movementStanding.h"

#include <iostream>

StandingState::StandingState(PositionData& pos, MovementData& data) : mPositionData(pos), mMovementData(data)
{
}

void StandingState::printState()
{
	std::cout << "StandingState" << "\n";
}

void StandingState::tickUpdate(bool moveHorizontal)
{
	MovementState::tickUpdate(moveHorizontal);
	//Attempt to fall
	mPositionData.mHitbox.updateTopLeftY(mMovementData.mCurMovementVect2.getY()); 
}
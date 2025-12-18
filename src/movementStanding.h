#pragma once
#include <iostream>
#include "movementHelperClass.h"

class StandingState : public MovementState
{
	MovementData& mMovementData;
public:
	StandingState(PositionData&, MovementData&, AttemptMove&);
	virtual void printState();
	virtual void calcMove(bool moveHorizontal);
};
#pragma once

#include "movementHelperClass.h"

class StandingState : public MovementState
{
	PositionData& mPositionData;
	MovementData& mMovementData;
public:
	StandingState(PositionData&, MovementData&);
	virtual void printState();
	virtual void tickUpdate(bool moveHorizontal);
};
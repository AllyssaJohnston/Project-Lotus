#pragma once
#include <iostream>
#include "movementHelperClass.h"

class StationaryState : public MovementState
{
	MovementData& mMovementData;
	JumpingData& mJumpData;
public:
	StationaryState(PositionData&, MovementData&, JumpingData&, AttemptMove&);
	virtual void printState();
	virtual void calcMove(bool moveHorizontal);
};
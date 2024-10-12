#pragma once

#include "movementHelperClass.h"

class WalkingState : public MovementState
{
	PositionData& mPositionData;
	MovementData& mMovementData;
public:
	WalkingState(PositionData&, MovementData&);
	virtual void printState();
	virtual void tickUpdate(bool moveHorizontal);
	void autoMove();
};
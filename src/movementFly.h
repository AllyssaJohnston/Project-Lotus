#pragma once

#include "movementHelperClass.h"

class FlyingState : public MovementState
{
	PositionData& mPositionData;
	MovementData& mMovementData;
public:
	FlyingState(PositionData&, MovementData&);
	virtual void printState();
	virtual void tickUpdate(bool moveHorizontal);
};
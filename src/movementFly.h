#pragma once

#include "movementHelperClass.h"

class FlyingState : public MovementState
{	
	MovementData& mMovementData;
public:
	FlyingState(PositionData&, MovementData&, AttemptMove&);
	virtual void printState();
	virtual void calcMove(bool moveHorizontal);
};
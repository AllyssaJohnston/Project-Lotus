#pragma once
#include <iostream>
#include "movementHelperClass.h"

class WalkingState : public MovementState
{
	MovementData& mMovementData;
public:
	WalkingState(PositionData&, MovementData&, AttemptMove&);
	virtual void printState();
	virtual void calcMove(bool moveHorizontal);
	void autoMove();
};
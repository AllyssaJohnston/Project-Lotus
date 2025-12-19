#pragma once
#include <iostream>
#include "movementHelperClass.h"

class FlyingState : public MovementState
{	
	MovementData& mMovementData;
public:
	FlyingState(PositionData&, MovementData&, AttemptMove&);
	virtual void calcMove(bool moveHorizontal);
};
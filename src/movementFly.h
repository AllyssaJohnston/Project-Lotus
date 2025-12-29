#pragma once
#include "movementHelpers.h"

class FlyingState : public MovementState
{	
	MovementData& mMovementData;
public:
	FlyingState(PositionData&, MovementData&, AttemptMove&);
	virtual void calcMove(bool moveHorizontal);
};
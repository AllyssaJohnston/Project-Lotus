#pragma once
#include <iostream>
#include "movementHelperClass.h"

class StandingState : public MovementState
{
	MovementData& mMovementData;
public:
	StandingState(PositionData&, MovementData&, AttemptMove&);
	void calcMove(bool moveHorizontal);
};
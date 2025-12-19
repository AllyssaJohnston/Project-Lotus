#pragma once
#include <iostream>
#include "movementHelperClass.h"

class StationaryState : public MovementState
{
	MovementData& mMovementData;
public:
	StationaryState(PositionData&, MovementData&, AttemptMove&);
	void calcMove(bool moveHorizontal);
};
#pragma once
#include "movementHelpers.h"

class StationaryState : public MovementState
{
	MovementData& mMovementData;
public:
	StationaryState(PositionData&, MovementData&, AttemptMove&);
	void calcMove(bool moveHorizontal);
};
#pragma once
#include "movementHelpers.h"

class StandingState : public MovementState
{
	MovementData& mMovementData;
public:
	StandingState(PositionData&, MovementData&, AttemptMove&);
	void calcMove(bool moveHorizontal);
};
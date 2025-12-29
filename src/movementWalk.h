#pragma once
#include "movementHelpers.h"

class WalkingState : public MovementState
{
	MovementData& mMovementData;
public:
	WalkingState(PositionData&, MovementData&, AttemptMove&);
	void calcMove(bool moveHorizontal);
	void autoMove();
};
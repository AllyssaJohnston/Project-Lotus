#pragma once
#include <iostream>
#include "movementHelperClass.h"

class FallingState : public MovementState
{
	MovementData& mMovementData;
	JumpingData& mJumpData;
public:
	FallingState(PositionData&, MovementData&, JumpingData&, AttemptMove&);
	virtual void printState();
	virtual void calcMove(bool moveHorizontal);
	bool canJump();
};
#pragma once

#include "movementHelperClass.h"

class FallingState : public MovementState
{
	PositionData& mPositionData;
	MovementData& mMovementData;
	JumpingData& mJumpData;
public:
	FallingState(PositionData&, MovementData&, JumpingData&);
	virtual void printState();
	virtual void tickUpdate(bool moveHorizontal);
	bool canJump();
};
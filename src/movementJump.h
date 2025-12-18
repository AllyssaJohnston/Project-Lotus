#pragma once
#include <iostream>
#include "movementHelperClass.h"

class JumpingState : public MovementState
{
	MovementData& mMovementData;
	JumpingData& mJumpData;
public:
	JumpingState(PositionData&, MovementData&, JumpingData&, AttemptMove&);
	virtual void printState();
	virtual void calcMove(bool moveHorizontal);
	bool isOver();

	void updateJumpDistanceLeft();
	void startJump(float jumpMultiplier);
	void endJump();
	void landed();
	void startWallJump();
};

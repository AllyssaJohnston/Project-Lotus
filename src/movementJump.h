#pragma once

#include "movementHelperClass.h"

class JumpingState : public MovementState
{
	PositionData& mPositionData;
	MovementData& mMovementData;
	JumpingData& mJumpData;
public:
	JumpingState(PositionData&, MovementData&, JumpingData&);
	virtual void printState();
	virtual void tickUpdate(bool moveHorizontal);
	bool isOver();

	void updateJumpDistanceLeft();
	void startJump(float jumpMultiplier);
	void endJump();
	void landed();
	void startWallJump();
	void continueWallJump();
	void updateWallJumpDistanceXLeft();
	void updateWallJumpDistanceYLeft();
	bool isMostlyDoneWithWallJump();
};

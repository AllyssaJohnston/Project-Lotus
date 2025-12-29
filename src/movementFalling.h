#pragma once
#include "movementHelpers.h"

class FallingState : public MovementState
{
	MovementData& mMovementData;
	JumpingData& mJumpData;

public:
	FallingState(PositionData&, MovementData&, JumpingData&, AttemptMove&);
	void calcMove(bool moveHorizontal);
	bool canJump();
	void startedState() override;
	void tickUpdate(bool moveHorizontal) override;

protected:
	void updateAccelerationY() override;
};
#include "movementFalling.h"

#include <iostream>

FallingState::FallingState(PositionData& pos, MovementData& mov, JumpingData& jump) : mPositionData(pos), mMovementData(mov), mJumpData(jump)
{
}

void FallingState::printState()
{
	std::cout << "FallingState" << "\n";
}

void FallingState::tickUpdate(bool moveHorizontal)
{
	MovementState::tickUpdate(moveHorizontal);
	//Fall
	mPositionData.mHitbox.updateTopLeftY(int(mMovementData.mCurMovementVect2.getY() * mMovementData.mAccelerationY));
	mMovementData.mCurDirectionY = EDirection_DOWN;

	if (moveHorizontal)
	{
		if (mMovementData.mCurDirection == EDirection_LEFT)
		{
			mPositionData.mHitbox.updateTopLeftX(mMovementData.mCurMovementVect2.getX() * -1);
		}
		if (mMovementData.mCurDirection == EDirection_RIGHT)
		{
			mPositionData.mHitbox.updateTopLeftX(mMovementData.mCurMovementVect2.getX());
		}
	}
}


bool FallingState::canJump()
{
	if (mJumpData.mNumCurJumps < mJumpData.mNumMaxJumps)
	{
		return true;
	}
	return false;
}

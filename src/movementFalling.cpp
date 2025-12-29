#include "movementFalling.h"

FallingState::FallingState(PositionData& pos, MovementData& mov, JumpingData& jump, AttemptMove& move) : mMovementData(mov), mJumpData(jump), MovementState(pos, move) {  mStateName = "FallingState"; }

void FallingState::tickUpdate(bool moveHorizontal)
{
	updateAccelerationY();
	MovementState::tickUpdate(moveHorizontal);
}

void FallingState::calcMove(bool moveHorizontal)
{
	Vect2 topLeft = mPositionData.mHitbox.getTopLeft();
	mAttemptMove.mWantToMoveTo = topLeft;
	mAttemptMove.mWantToMoveTo.changeY(int(mMovementData.mCurMovementVect2.getY() * mMovementData.mAccelerationY));
	mMovementData.mCurDirectionY = EDirection_DOWN;

	if (moveHorizontal)
	{
		if (mMovementData.mCurDirection == EDirection_LEFT)
		{
			mAttemptMove.mWantToMoveTo.changeX(-mMovementData.mCurMovementVect2.getX());
		}
		else if (mMovementData.mCurDirection == EDirection_RIGHT)
		{
			mAttemptMove.mWantToMoveTo.changeX(mMovementData.mCurMovementVect2.getX());
		}
	}
}

bool FallingState::canJump() { return mJumpData.mNumCurJumps < mJumpData.mNumMaxJumps; }

void FallingState::startedState()
{
	MovementState::startedState();
	mMovementData.mAccelerationY = 1.0f;
}

void FallingState::updateAccelerationY()
{
	if (mFramesInState >= mMovementData.mMinFramesToAccelerate && !mPositionData.mOnGround)
	{
		int framesSinceAccelerating = mFramesInState - mMovementData.mMinFramesToAccelerate;
		mMovementData.mAccelerationY = 1.0f + (float(framesSinceAccelerating) / 60.0f);
		if (ceil(mMovementData.mCurMovementVect2.getY() * mMovementData.mAccelerationY) > 12.0f)
		{
			mMovementData.mAccelerationY = 12.0f / (float)mMovementData.mCurMovementVect2.getY();
		}
	}
	else
	{
		mMovementData.mAccelerationY = 1.0f;
	}
}
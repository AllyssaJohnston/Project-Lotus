#include "movementFalling.h"

#include <iostream>

FallingState::FallingState(PositionData& pos, MovementData& mov, JumpingData& jump, AttemptMove& move) : mMovementData(mov), mJumpData(jump), MovementState(pos, move) {;}

void FallingState::printState() { std::cout << "FallingState" << "\n"; }

void FallingState::calcMove(bool moveHorizontal)
{
	//Fall
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

#include "movementWalk.h"

#include <iostream>

WalkingState::WalkingState(PositionData& pos, MovementData& movData, AttemptMove& move) : mMovementData(movData), MovementState(pos, move){;}

void WalkingState::printState() { std::cout << "WalkingState" << "\n"; }

void WalkingState::calcMove(bool moveHorizontal) {
	//Attempt to fall
	Vect2 topLeft = mPositionData.mHitbox.getTopLeft();
	mAttemptMove.mWantToMoveTo = topLeft;
	mAttemptMove.mWantToMoveTo.changeY(int(mMovementData.mCurMovementVect2.getY() * mMovementData.mAccelerationY));

	if (moveHorizontal)
	{
		int movementEffect = mMovementData.mUseMovementEffect ? mPositionData.mCurGroundMovementEffect : 0;
		int moveX = mMovementData.mCurMovementVect2.getX() + movementEffect;
		int multiplier = mMovementData.mCurDirection == EDirection_LEFT ? -1 : 1;
		mAttemptMove.mWantToMoveTo.changeX(moveX * multiplier);
	}
}

void WalkingState::autoMove()
{
	int movementEffect = mMovementData.mUseMovementEffect ? mPositionData.mCurGroundMovementEffect : 0;

	/*if (mMovementData.mMovementAutoMoveRule == EMovementAutoMoveRule_USE_FACING_DIRECTION)
	{
		if (mPositionData.mFacing == EDirection_LEFT)
		{
			mPositionData.mHitbox.updateTopLeftX(-1 * (mMovementData.mCurMovementVect2.getX() + movementEffect));
			mMovementData.mCurDirection = EDirection_LEFT;
		}
		if (mPositionData.mFacing == EDirection_RIGHT)
		{
			mPositionData.mHitbox.updateTopLeftX(mMovementData.mCurMovementVect2.getX() + movementEffect);
			mMovementData.mCurDirection = EDirection_RIGHT;
		}
	}
	else if (mMovementData.mMovementAutoMoveRule == EMovementAutoMoveRule_USE_CUR_DIRECTION)
	{
		if (mMovementData.mLastFrameDirection == EDirection_LEFT or mMovementData.mCurDirection == EDirection_LEFT)
		{
			mPositionData.mHitbox.updateTopLeftX(-1 * (mMovementData.mCurMovementVect2.getX() + movementEffect));
			mMovementData.mCurDirection = EDirection_LEFT;
		}
		if (mMovementData.mLastFrameDirection == EDirection_RIGHT or mMovementData.mCurDirection == EDirection_RIGHT)
		{
			mPositionData.mHitbox.updateTopLeftX(mMovementData.mCurMovementVect2.getX() + movementEffect);
			mMovementData.mCurDirection = EDirection_RIGHT;
		}
	}*/

	//TODO fix box on ice
	
	if (mPositionData.mFacing == EDirection_LEFT)
	{
		mPositionData.mHitbox.updateTopLeftX(-1 * (mMovementData.mCurMovementVect2.getX() + movementEffect));
		mMovementData.mCurDirection = EDirection_LEFT;
	}
	else if (mPositionData.mFacing == EDirection_RIGHT)
	{
		mPositionData.mHitbox.updateTopLeftX(mMovementData.mCurMovementVect2.getX() + movementEffect);
		mMovementData.mCurDirection = EDirection_RIGHT;
	}

}

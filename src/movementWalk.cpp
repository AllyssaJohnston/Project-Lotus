#include "movementWalk.h"

#include <iostream>

WalkingState::WalkingState(PositionData& pos, MovementData& mov) : mPositionData(pos), mMovementData(mov)
{
}

void WalkingState::printState()
{
	std::cout << "WalkingState" << "\n";
}

void WalkingState::tickUpdate(bool moveHorizontal)
{
	MovementState::tickUpdate(moveHorizontal);
	//Attempt to fall
	mPositionData.mHitbox.updateTopLeftY(int(mMovementData.mCurMovementVect2.getY() * mMovementData.mAccelerationY));

	if (moveHorizontal)
	{
		int movementEffect =  mPositionData.mCurGroundMovementEffect;
		if (mMovementData.mUseMovementEffect == false)
		{
			movementEffect = 0;
		}

		if (mMovementData.mCurDirection == EDirection_LEFT)
		{
			mPositionData.mHitbox.updateTopLeftX(-1 * (mMovementData.mCurMovementVect2.getX() + movementEffect));
		}
		if (mMovementData.mCurDirection == EDirection_RIGHT)
		{
			mPositionData.mHitbox.updateTopLeftX(mMovementData.mCurMovementVect2.getX() + movementEffect);
		}
	}
}

void WalkingState::autoMove()
{
	int movementEffect = mPositionData.mCurGroundMovementEffect;
	if (mMovementData.mUseMovementEffect == false)
	{
		movementEffect = 0;
	}

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
	if (mPositionData.mFacing == EDirection_RIGHT)
	{
		mPositionData.mHitbox.updateTopLeftX(mMovementData.mCurMovementVect2.getX() + movementEffect);
		mMovementData.mCurDirection = EDirection_RIGHT;
	}

}

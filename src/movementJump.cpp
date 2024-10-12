#include "movementJump.h"

#include <iostream>

void JumpingData:: stopJump()
{
	mAmJump					= false;
	mAmWallJump				= false;
	mWallJumpDirection		= EDirection_NONE;

	mJumpDistanceLeft		= 0;
	mWallJumpDistanceXLeft	= 0;
	mWallJumpDistanceYLeft	= 0;
}

JumpingState::JumpingState(PositionData& pos, MovementData& mov, JumpingData& jump) : mPositionData(pos), mMovementData(mov), mJumpData(jump)
{
}

void JumpingState::printState()
{
	std::cout << "JumpingState" << "\n";
}

void JumpingState::tickUpdate(bool moveHorizontal)
{
	MovementState::tickUpdate(moveHorizontal);
	//Update Y
	mPositionData.mHitbox.updateTopLeftY(-mMovementData.mCurMovementVect2.getY() * 2);
	updateJumpDistanceLeft();

	//Update X
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

void JumpingState::updateJumpDistanceLeft()
{
	mJumpData.mJumpDistanceLeft -= mMovementData.mCurMovementVect2.getY();

	if (mJumpData.mJumpDistanceLeft <= 0)
	{
		endJump();
	}
}

void JumpingState::startJump(float jumpMultiplier)
{
	if (mJumpData.mNumCurJumps < mJumpData.mNumMaxJumps)
	{
		mJumpData.mAmJump			= true;
		mJumpData.mJumpDistanceLeft = int(mJumpData.mJumpDistance * jumpMultiplier);
		mJumpData.mNumCurJumps		+= 1;
	}
}

void JumpingState::endJump()
{
	//		mJumpingData.mBreakNextChange = true;
	//std::cout << "Jump Over" << "\n";
	mJumpData.stopJump();
}

void JumpingState::landed()
{
	mJumpData.mNumCurJumps = 0;
}

bool JumpingState::isOver()
{
	return mJumpData.mAmJump == false;
}

void JumpingState::startWallJump()
{
	endJump();
	landed();
	mJumpData.mAmJump					= true;
	mJumpData.mAmWallJump				= true;
	mJumpData.mJumpDistanceLeft			= mJumpData.mJumpDistance;
	mJumpData.mWallJumpDistanceXLeft	= mJumpData.mWallJumpDistanceX;
	mJumpData.mWallJumpDistanceYLeft	= mJumpData.mWallJumpDistanceY;

	if (mPositionData.mFacing == EDirection_LEFT)
	{
		mMovementData.mCurDirection		= EDirection_RIGHT;
		mJumpData.mWallJumpDirection	= EDirection_RIGHT;
		mPositionData.mFacing           = EDirection_RIGHT;
		
	}
	else
	{
		mMovementData.mCurDirection		= EDirection_LEFT;
		mJumpData.mWallJumpDirection	= EDirection_LEFT;
		mPositionData.mFacing           = EDirection_LEFT;
	}
}

void JumpingState::continueWallJump()
{
	if (mJumpData.mAmWallJump)
	{
		mPositionData.mHitbox.updateTopLeftY(-mMovementData.mCurMovementVect2.getY() * 2);
		updateWallJumpDistanceYLeft();
		
		if (mJumpData.mWallJumpDistanceYLeft <= 0)
		{
			endJump();
		}

		int xMovement = mMovementData.mCurMovementVect2.getX();
		if (mJumpData.mWallJumpDirection == EDirection_LEFT)
		{
			xMovement *= -1;
		}
		else if (mJumpData.mWallJumpDirection == EDirection_RIGHT)
		{

		}
		else
		{
			//SDL_assert(false);
			endJump();
			return;
		}
		mPositionData.mHitbox.updateTopLeftX(xMovement);
	}
}

void JumpingState::updateWallJumpDistanceXLeft()
{
	mJumpData.mWallJumpDistanceXLeft -= mMovementData.mCurMovementVect2.getX();

	if (mJumpData.mWallJumpDistanceXLeft <= 0)
	{
		endJump();
	}
}

void JumpingState::updateWallJumpDistanceYLeft()
{
	mJumpData.mWallJumpDistanceYLeft -= mMovementData.mCurMovementVect2.getY();

	if (mJumpData.mWallJumpDistanceYLeft <= 0)
	{
		endJump();
	}
}

bool JumpingState::isMostlyDoneWithWallJump()
{
	if (mJumpData.mWallJumpDistanceXLeft < mMovementData.mCurMovementVect2.getX() or mJumpData.mWallJumpDistanceYLeft < mMovementData.mCurMovementVect2.getY())
	{
		return true;
	}
	return false;
}

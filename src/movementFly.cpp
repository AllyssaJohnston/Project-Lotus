#include "movementFly.h"

#include <iostream>

FlyingState::FlyingState(PositionData& pos, MovementData& mov) : mPositionData(pos), mMovementData(mov)
{
}

void FlyingState::printState()
{
	std::cout << "FlyingState" << "\n";
}

void FlyingState::tickUpdate(bool moveHorizontal)
{
	MovementState::tickUpdate(moveHorizontal);
	//TODO
	int curMovementX = 0;
	int curMovementY = 0;
	if (mMovementData.mPath == EEntityMovementPath_HORIZONTAL or mMovementData.mPath == EEntityMovementPath_DIAGONAL)
	{
		curMovementX = mMovementData.mCurMovementVect2.getX();
		if (mMovementData.mCurDirection == EDirection_LEFT)
		{
			curMovementX *= -1;
		}
	}
	if (mMovementData.mPath == EEntityMovementPath_VERTICAL or mMovementData.mPath == EEntityMovementPath_DIAGONAL)
	{
		curMovementY = mMovementData.mCurMovementVect2.getY();
		if (mMovementData.mCurDirectionY == EDirection_UP)
		{
			curMovementY *= -1;
		}
	}

	mPositionData.mHitbox.updateTopLeft(Vect2(curMovementX, curMovementY));

	return;
}

#include "movementFly.h"

FlyingState::FlyingState(PositionData& pos, MovementData& movData, AttemptMove& move) : mMovementData(movData) , MovementState(pos, move) { mStateName = "FlyingState"; }

void FlyingState::calcMove(bool moveHorizontal) {
	
	int curMovementX = 0;
	int curMovementY = 0;
	if (mMovementData.mPath == EEntityMovementPath_HORIZONTAL or mMovementData.mPath == EEntityMovementPath_DIAGONAL)
	{
		int multiplier = mMovementData.mCurDirection == EDirection_LEFT ? -1 : 1;
		curMovementX = mMovementData.mCurMovementVect2.getX() * multiplier;
	}
	if (mMovementData.mPath == EEntityMovementPath_VERTICAL or mMovementData.mPath == EEntityMovementPath_DIAGONAL)
	{
		int multiplier = mMovementData.mCurDirectionY == EDirection_UP ? -1 : 1;
		curMovementY = mMovementData.mCurMovementVect2.getY() * multiplier;
	}

	mAttemptMove.mWantToMoveTo = mPositionData.mHitbox.getTopLeft() + Vect2(curMovementX, curMovementY);
}

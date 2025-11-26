#include "movementStationary.h"

#include <iostream>

StationaryState::StationaryState(PositionData& pos, MovementData& mov, JumpingData& jump, AttemptMove& move) : mMovementData(mov), mJumpData(jump), MovementState(pos, move) { ; }

void StationaryState::printState() { std::cout << "StationaryState" << "\n"; }

void StationaryState::calcMove(bool moveHorizontal)
{
	//Fall
	Vect2 topLeft = mPositionData.mHitbox.getTopLeft();
	mAttemptMove.mWantToMoveTo = topLeft;
	mAttemptMove.mWantToMoveTo.changeY(mMovementData.mCurMovementVect2.getY());
	mMovementData.mCurDirectionY = EDirection_DOWN;
}

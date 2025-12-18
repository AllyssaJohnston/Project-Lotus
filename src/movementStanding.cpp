#include "movementStanding.h"

StandingState::StandingState(PositionData& pos, MovementData& data, AttemptMove& move) : mMovementData(data), MovementState(pos, move){;}

void StandingState::printState() { std::cout << "StandingState" << "\n"; }

void StandingState::calcMove(bool moveHorizontal) 
{
	mAttemptMove.mWantToMoveTo = mPositionData.mHitbox.getTopLeft();
	mAttemptMove.mWantToMoveTo.changeY(mMovementData.mCurMovementVect2.getY());
}
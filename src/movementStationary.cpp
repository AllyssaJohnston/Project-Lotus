#include "movementStationary.h"
#include "movementHelpers.h"

StationaryState::StationaryState(PositionData& pos, MovementData& mov, AttemptMove& move) : mMovementData(mov), MovementState(pos, move) { mStateName = "StationaryState"; }

void StationaryState::calcMove(bool moveHorizontal)
{
	Vect2 topLeft = mPositionData.mHitbox.getTopLeft();
	mAttemptMove.mWantToMoveTo = topLeft;
}


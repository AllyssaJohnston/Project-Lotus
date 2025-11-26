#include "movementHelperClass.h"

KeyData::KeyData(int key, int repeat) : mKey(key), mRepeat(repeat) { ; }

MovementState::MovementState(PositionData& pos, AttemptMove& move) : mPositionData(pos), mAttemptMove(move) { ; }

void MovementState::tickUpdate(bool moveHorizontal) {
	mFramesInState++;
	calcMove(moveHorizontal);
	mAttemptMove.mInterrupted = false;
	//move(); //TODO call after collision resolution
}



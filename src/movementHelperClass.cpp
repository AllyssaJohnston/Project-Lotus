#include "movementHelperClass.h"

MovementState::MovementState(PositionData& pos, AttemptMove& move) : mPositionData(pos), mAttemptMove(move) { ; }

void MovementState::tickUpdate(bool moveHorizontal) 
{
	mFramesInState++;
	calcMove(moveHorizontal);
	mAttemptMove.mInterrupted = false;
}

void MovementState::startedState()  {  mFramesInState = 0;  }

void MovementState::printState() { std::cout << mStateName << "\n"; }


bool PositionData::inGroundCharacteristics(EEntityCharacteristicsTypes type) const { return (std::find(mCurGroundCharacteristics.begin(), mCurGroundCharacteristics.end(), type) != mCurGroundCharacteristics.end()); }



void MovementData::setUp(const std::vector<EEntityMovements>& movementCodes,  const EEntityMovements& curMovementCode, const int& movementCodeInterval, 
						 const ECharacterModes& curCharacterMode)
{
	mMovementCodes = movementCodes;
	mCurMovementCode = curMovementCode;
	mMovementCodeCountDown = movementCodeInterval;
	mMovementCodeInterval = movementCodeInterval;

	mCharacterMode = curCharacterMode;
}

void MovementData::setCurCharacterMode(ECharacterModes newMode) { mCharacterMode = newMode; }

ECharacterModes MovementData::getCharacterMode() const { return mCharacterMode;  }

void MovementData::updateMovementCodeCountDown(bool onGround)
{
	if (mMovementCodes.size() > 1)
	{
		mMovementCodeCountDown -= 1;
		if (mMovementCodeCountDown < 0 && onGround)
		{
			updateCurMovementCode();
		}
	}
}

void MovementData::resetMovementCodeCountDown() { mMovementCodeCountDown = mMovementCodeInterval; }

void MovementData::updateCurMovementCode()
{
	resetMovementCodeCountDown();
	mCurMovementCodeIndex += 1;
	if (mCurMovementCodeIndex >= mMovementCodes.size())
	{
		mCurMovementCodeIndex = 0;
	}
	mCurMovementCode = mMovementCodes[mCurMovementCodeIndex];
}

void MovementData::setCurMovementCode(EEntityMovements newCode)
{
	for (int count = 0; count < mMovementCodes.size(); count++)
	{
		if (mMovementCodes[count] == newCode)
		{
			mCurMovementCode = newCode;
			mCurMovementCodeIndex = count;
			return;
		}
	}
}

EEntityMovements MovementData::getCurMovementCode() const { return mCurMovementCode; }

void MovementData::reset()
{
	resetMovementCodeCountDown();
	setCurMovementCode(mMovementCodes[0]);
}


void JumpingData::stopJump()
{
	mAmJump = false;
	mAmWallJump = false;
	mWallJumpDirection = EDirection_NONE;

	mJumpDistanceLeft = 0;
}
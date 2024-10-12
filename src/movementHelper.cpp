#include "movementHelper.h"
#include <iostream>
#include <list>
#include <SDL3/SDL.h>
#include <cmath>
#include <vector>

#include "movementStanding.h"
#include "movementWalk.h"
#include "movementJump.h"
#include "movementFalling.h"
#include "movementFly.h"

extern const int numMovementStates;

MovementManager::MovementManager()
{
	mMovementStates[EMovementStateIndex_STANDING]	=	new StandingState(mPositionData, mMovementData);
	mMovementStates[EMovementStateIndex_WALKING]	=	new WalkingState( mPositionData, mMovementData);
	mMovementStates[EMovementStateIndex_JUMPING]	=	new JumpingState( mPositionData, mMovementData, mJumpingData);
	mMovementStates[EMovementStateIndex_FALLING]	=	new FallingState( mPositionData, mMovementData, mJumpingData);
	mMovementStates[EMovementStateIndex_FLYING]		=	new FlyingState(  mPositionData, mMovementData);
}

void MovementManager::setupMovementManager(Vect2 startPosition, CCharacterPreset* preset)
{
	setUpMovementManagerInternal(startPosition, preset);
	mPositionData.mHitbox = Hitbox(mStartPosition, preset->mWidth, preset->mHeight);
}

void MovementManager::setupMovementManager(Vect2 startPosition, CCharacterPreset* preset, EDirection curDirection)
{
	setUpMovementManagerInternal(startPosition, preset);
	mPositionData.mHitbox = Hitbox(mStartPosition, preset->mWidth, preset->mHeight);
	mMovementData.mCurDirection = curDirection;
}

void MovementManager::setupMovementManager(Vect2 startPosition, CCharacterPreset* preset, int widthInput, int heightInput)
{
	setUpMovementManagerInternal(startPosition, preset);
	mPositionData.mHitbox = Hitbox(mStartPosition, widthInput, heightInput);
}

void MovementManager::setupMovementManager(Vect2 startPosition, CCharacterPreset* preset, EEntityMovementPath path, EDirection curDirection, int widthInput, int heightInput)
{
	setUpMovementManagerInternal(startPosition, preset);
	mPositionData.mHitbox = Hitbox(mStartPosition, widthInput, heightInput);
	mMovementData.mPath   = path;
	mMovementData.mCurDirection = curDirection;
}

void MovementManager::setupMovementManager(Vect2 startPosition, Vect2 movementVect, CCharacterPreset* preset, EDirection curDirectionX, EDirection curDirectionY)
{
	setUpMovementManagerInternal(startPosition, preset);
	mPositionData.mHitbox = Hitbox(mStartPosition, preset->mWidth, preset->mHeight);
	mMovementData.mCurDirection = curDirectionX;
	mMovementData.mCurDirectionY = curDirectionY;
	mMovementData.mBaseMovementVect2 = movementVect;
	mMovementData.mCurMovementVect2 = movementVect;
}

void MovementManager::setUpMovementManagerInternal(Vect2 startPosition, CCharacterPreset* preset)
{
	mStartPosition							=	startPosition;
	mCheckpointPosition						=	startPosition;
	mPositionData.mLastFramePosition		=	startPosition;

	mMovementData.mCharacterModes			=	preset->mCharacterModes;
	mMovementData.mCurCharacterMode			=	preset->mCharacterModes[0];
	mMovementData.mCurCharacterModeIndex	=	0;
	mMovementData.mCharacterModeInterval	=	preset->mCharacterModeInterval;
	mMovementData.mMovementAutoMoveRule     =   preset->mAutoMoveRule;

	mMovementData.mBaseMovementVect2		=	preset->mMovementVect2;
	mMovementData.mCurMovementVect2			=	preset->mMovementVect2;
	mJumpingData.mJumpDistance				=	preset->mJumpDistance;
	mJumpingData.mWallJumpDistanceX			=	preset->mWallJumpDistanceX;
	mJumpingData.mWallJumpDistanceY			=	preset->mWallJumpDistanceY;
	mJumpingData.mNumCurJumps				=	0;
	mJumpingData.mNumMaxJumps				=	1;
	mMovementData.mUseMovementEffect		=   preset->mUseMovementEffect;
	mMovementData.mMovementCodes            =   preset->mMovementCodes;
	mMovementData.mCurMovementCode			=   preset->mMovementCodes[0];
	mMovementData.mCurMovementCodeIndex     =   0;
	mMovementData.mMovementCodeInterval     =	preset->mMovementCodeInterval;

	mMovementData.mPath						=	preset->mMovementPath;
	mPositionData.mOriginalHitboxEdges		=	preset->mHitboxEdges;
	mPositionData.mCurHitboxEdges			=	preset->mHitboxEdges;

}

MovementManager::~MovementManager()
{
	
}

void MovementManager::setInputDriven(bool input)
{
	mInputDriven = input;
}



void MovementManager::setMovementState(EMovementStateIndex newState)
{
	if (mCurMovementState != newState)
	{
		mCurMovementState = newState;

		if (mDebugPrint)
		{
			mMovementStates[mCurMovementState]->printState();
		}
		mMovementStates[mCurMovementState]->startedState();
	}
}

std::array<MovementState*, numMovementStates> MovementManager::getMovementStates()
{
	return mMovementStates;
}

EMovementStateIndex MovementManager:: getCurMovementState()
{
	return mCurMovementState;
}

void MovementManager::setMovementStateToCharacterMode()
{
	mMoveHorizontal = true;
	switch (getCode())
	{
	case EEntityMovements_FLY:
	{
		setMovementState(EMovementStateIndex_FLYING);
		mMoveHorizontal = false;
		break;
	}
	case EEntityMovements_JUMP:
	{
		if (mPositionData.mOnGround)
		{
			jump(1.0f);
		}
		else if (mJumpingData.mAmJump == false)
		{
			setMovementState(EMovementStateIndex_FALLING);
		}
		break;
	}
	case EEntityMovements_WALK:
		setMovementState(EMovementStateIndex_WALKING);
		break;
	case EEntityMovements_FALL:
	{
		setMovementState(EMovementStateIndex_STANDING);
		mMoveHorizontal = false;
		break;
	}
	default:
		break;
	}

}



void MovementManager::resetStats()
{
	mPositionData.mHitbox.setTopLeft(mStartPosition);
	mJumpingData.stopJump();

	mPositionData.mOnGround					=	false;
	mPositionData.mCurGroundMovementEffect	=   0;
	mPositionData.mCurGroundCharacteristics.clear();

	mMovementData.mCurDirection				=	EDirection_LEFT;
	mMovementData.mLastFrameDirection       =	EDirection_LEFT;
	mMovementData.mCurDirectionY			=	EDirection_DOWN;
	mMovementData.mLastFrameDirectionY      =	EDirection_DOWN;

	mCollidedThisFrame						=	false;
	mReceivedInputThisFrame					=	false;
	resetCharacterModeCountDown();
	setCurCharacterMode(mMovementData.mCharacterModes[0]);
	resetMovementCodeCountDown();
	setCurMovementCode(mMovementData.mMovementCodes[0]);
	//resetMovementVect2();

}

void MovementManager::resetToCheckpoint()
{
	mPositionData.mHitbox.setTopLeft(mCheckpointPosition);
	mJumpingData.stopJump();

	mPositionData.mOnGround						=	false;
	mPositionData.mCurGroundMovementEffect		=	0;
	mPositionData.mCurGroundCharacteristics.clear();

	mMovementData.mCurDirection					=	EDirection_LEFT;
	mMovementData.mLastFrameDirection		    =	EDirection_LEFT;
	mMovementData.mCurDirectionY				=	EDirection_DOWN;
	mMovementData.mLastFrameDirectionY			=	EDirection_DOWN;

	mCollidedThisFrame							=	false;
	mReceivedInputThisFrame						=	false;
	resetCharacterModeCountDown();
	setCurCharacterMode(mMovementData.mCharacterModes[0]);
	resetMovementCodeCountDown();
	setCurMovementCode(mMovementData.mMovementCodes[0]);
	//resetMovementVect2();
}



void MovementManager::preTick()
{
	mCollidedThisFrame					= false;
	mSwappedDirThisFrame				= false;
	mPositionData.mLastFramePosition	= mPositionData.mHitbox.getTopLeft();

	if (mPositionData.mOnGround == true and mJumpingData.mAmJump == false)
	{
		mJumpingData.mNumCurJumps = 0;
	}

	mCurHitboxEdgesIntervalLeft -= 1;
	if (mCurHitboxEdgesIntervalLeft <= 0)
	{
		resetCurHitboxEdges();
	}
	//updateCurMovementVect2CountDown();
}

void MovementManager::postTick()
{
	switch (mMovementData.mCurMovementCode)
	{
	case EEntityMovements_NONE:
	case EEntityMovements_FLY:
		break;

	case EEntityMovements_INPUT:
	default:
		calculateYDirection();
		break;
	}

	if (mPositionData.mOnGround == false)
	{
		mPositionData.mCurGroundMovementEffect = 0;
		mPositionData.mCurGroundCharacteristics.clear();
	}

	mMovementData.mLastFrameDirection     = mMovementData.mCurDirection;
	mMovementData.mLastFrameDirectionY    = mMovementData.mCurDirectionY;
	if (mLastMovementState == mCurMovementState)
	{
		mFramesInState += 1;
	}
	else
	{
		mFramesInState = 0;
	}
	mLastMovementState      = mCurMovementState;
}



void MovementManager::updateMovement()
{

	updateCharacterModeCountDown();
	updateMovementCodeCountDown();

	//There is nothing to update, exit early.  Could make a state that just returned
	if (mMovementData.mCurCharacterMode == ECharacterModes_STATIC)
	{
		return;
	}

	//This can only be pushed around?, exit early.  Could make a state but not worth it right now.
	if (mMovementData.mCurCharacterMode == ECharacterModes_STATIONARY)
	{
		mPositionData.mHitbox.updateTopLeftY(mMovementData.mCurMovementVect2.getY());
		return;
	}

	// This type of entity is Dyanamic and we should let it move. 
	updateAccelerationY();
	mMovementStates[mCurMovementState]->tickUpdate(mMoveHorizontal);


	//////////////////////////////////////////////////////////////////
	// Transition states if needed.

	// Is jump over and we are now falling?
	if (mCurMovementState == EMovementStateIndex_JUMPING)
	{
		if (((JumpingState*)mMovementStates[mCurMovementState])->isOver())
		{
			setMovementState(EMovementStateIndex_FALLING);
		}
	}
	else if (mPositionData.mOnGround == false)
	{
		setMovementState(EMovementStateIndex_FALLING);
	}

}

void MovementManager::updateAccelerationY()
{
	int mMinFramesToAccelerate = 120;
	if (mFramesInState >= mMinFramesToAccelerate) 
	{
		if (mCurMovementState == EMovementStateIndex_FALLING)
		{
			int timeSinceAccelerating = mFramesInState - mMinFramesToAccelerate;
			mMovementData.mAccelerationY = 1.0f + (float(timeSinceAccelerating) / 60);
			std::cout << mMovementData.mAccelerationY;
			if (ceil(mMovementData.mCurMovementVect2.getY() * mMovementData.mAccelerationY) > 12)
			{
				mMovementData.mAccelerationY = 12.0f / (float)mMovementData.mCurMovementVect2.getY();
			}
		}

	}
	else
	{
		mMovementData.mAccelerationY = 1.0f;
	}
}

void MovementManager::push(int changeMovement, EDirection newDirection)
{
	int movementX = 0;
	int movementY = 0;
	switch (newDirection)
	{
	case EDirection_UP:
		movementY = -changeMovement - mMovementData.mBaseMovementVect2.getY();
		mMovementData.mCurDirectionY = newDirection;
		break;
	case EDirection_DOWN:
		movementY = changeMovement;
		mMovementData.mCurDirectionY = newDirection;
		break;
	case EDirection_LEFT:
		movementX = -changeMovement;
		mMovementData.mCurDirection = newDirection;
		break;
	default:
		movementX = changeMovement;
		mMovementData.mCurDirection = newDirection;
		break;
	}

	mPositionData.mHitbox.updateTopLeft(Vect2(movementX, movementY));
}


void MovementManager::updateCharacterModeCountDown()
{
	if (mMovementData.mCharacterModes.size() > 1)
	{
		mMovementData.mCharacterModeCountDown -= 1;
		if (mMovementData.mCharacterModeCountDown < 0)
		{
			updateCurCharacterMode();
		}
	}
}

void MovementManager::resetCharacterModeCountDown()
{
	mMovementData.mCharacterModeCountDown = mMovementData.mCharacterModeInterval;
}

void MovementManager::updateCurCharacterMode()
{
	if (mPositionData.mOnGround == true)
	{
		resetCharacterModeCountDown();
		mMovementData.mCurCharacterModeIndex += 1;
		if (mMovementData.mCurCharacterModeIndex >= mMovementData.mCharacterModes.size())
		{
			mMovementData.mCurCharacterModeIndex = 0;
		}
		mMovementData.mCurCharacterMode = mMovementData.mCharacterModes[mMovementData.mCurCharacterModeIndex];
	}
	else
	{
		mMovementData.mCharacterModeCountDown -= 1;
	}
}

void MovementManager::changeCharacterModes(std::vector<ECharacterModes> givenCharacterModes)
{
	mMovementData.mCharacterModes.clear();
	resetCharacterModeCountDown();

	for (ECharacterModes mode : givenCharacterModes)
	{
		mMovementData.mCharacterModes.push_back(mode);
	}
}

void MovementManager::setCurCharacterMode(ECharacterModes newMode)
{
	for (int count = 0; count < mMovementData.mCharacterModes.size(); count++)
	{
		if (mMovementData.mCharacterModes[count] == newMode)
		{
			mMovementData.mCurCharacterMode = newMode;
			mMovementData.mCurCharacterModeIndex = count;
			return;
		}
	}
}




void MovementManager::updateMovementCodeCountDown()
{
	if (mMovementData.mMovementCodes.size() > 1)
	{
		mMovementData.mMovementCodeCountDown -= 1;
		if (mMovementData.mMovementCodeCountDown < 0)
		{
			updateCurMovementCode();
		}
	}
}

void MovementManager::resetMovementCodeCountDown()
{
	mMovementData.mMovementCodeCountDown = mMovementData.mMovementCodeInterval;
}

void MovementManager::updateCurMovementCode()
{
	if (mPositionData.mOnGround == true)
	{
		resetMovementCodeCountDown();
		mMovementData.mCurMovementCodeIndex += 1;
		if (mMovementData.mCurMovementCodeIndex >= mMovementData.mMovementCodes.size())
		{
			mMovementData.mCurMovementCodeIndex = 0;
		}
		mMovementData.mCurMovementCode = mMovementData.mMovementCodes[mMovementData.mCurMovementCodeIndex];
	}
	else
	{
		mMovementData.mCharacterModeCountDown -= 1;
	}
}

void MovementManager::setCurMovementCode(EEntityMovements newCode)
{
	for (int count = 0; count < mMovementData.mMovementCodes.size(); count++)
	{
		if (mMovementData.mMovementCodes[count] == newCode)
		{
			mMovementData.mCurMovementCode = newCode;
			mMovementData.mCurMovementCodeIndex = count;
			return;
		}
	}
}


void MovementManager::useInput(std::vector <KeyData> & eventVect, bool useHorizontalInput)
{
	//Ignore input while in the walljump
	if (mCurMovementState == EMovementStateIndex_JUMPING and mJumpingData.mAmWallJump)
		return;

	mReceivedInputThisFrame = false;
	mMovementData.mCurDirection = EDirection_NONE;

	for(int count = 0; count < eventVect.size(); count++)
	{
		int curEventEnum = eventVect[count].mKey;
		switch (curEventEnum)
		{
			case EKeyboardInput_LEFT:
				if (useHorizontalInput)
				{
					left();
					mReceivedInputThisFrame = true;
				}
			break;

			case EKeyboardInput_RIGHT:
				if (useHorizontalInput)
				{
					right();
					mReceivedInputThisFrame = true;
				}
			break;

			case EKeyboardInput_UP:
				if (eventVect[count].mRepeat == 2)  //Only jump when key pressed down for at least 2 frames.
				{
					jump(1.0f);
					mReceivedInputThisFrame = true;
				}
			break;

			default:
			break;
		}
	}

	if ( (mInputDriven == true) and (mReceivedInputThisFrame == false) )
	{	// Transition to standing
		if(mCurMovementState == EMovementStateIndex_WALKING) 
		{
			setMovementState(EMovementStateIndex_STANDING);
		}
	}
}

void MovementManager::collided(EDirection directionOfCollision)
{
	if (mMovementData.mCurMovementCode == EEntityMovements_FALL)
	{

	}
	else
	{
		if (mCollidedThisFrame == false)
		{
			mCollidedThisFrame = true;
			if ( (mMovementData.mPath == EEntityMovementPath_HORIZONTAL or mMovementData.mPath == EEntityMovementPath_HORIZONTAL_CAN_FALL) 
					and (directionOfCollision == EDirection_LEFT or directionOfCollision == EDirection_RIGHT ) )
			{
				mSwappedDirThisFrame = true;

				if (mMovementData.mCurDirection == EDirection_LEFT)
				{
					mMovementData.mCurDirection = EDirection_RIGHT;
				}
				else
				{
					mMovementData.mCurDirection = EDirection_LEFT;
				}
			}
			if (mMovementData.mPath == EEntityMovementPath_VERTICAL)
			{
				mSwappedDirThisFrame = true;
				if (mMovementData.mCurDirectionY == EDirection_UP)
				{
					mMovementData.mCurDirectionY = EDirection_DOWN;
				}
				else
				{
					mMovementData.mCurDirectionY = EDirection_UP;
				}
			}
			if (mMovementData.mPath == EEntityMovementPath_DIAGONAL)
			{
				if (directionOfCollision == EDirection_LEFT)
				{
					//mSwappedDirThisFrame = true;
					mMovementData.mCurDirection = EDirection_RIGHT;
				}
				else if (directionOfCollision == EDirection_RIGHT)
				{
					//mSwappedDirThisFrame = true;
					mMovementData.mCurDirection = EDirection_LEFT;
				}
				else if (directionOfCollision == EDirection_UP)
				{
					//mSwappedDirThisFrame = true;
					mMovementData.mCurDirectionY = EDirection_DOWN;
				}
				else if (directionOfCollision == EDirection_DOWN)
				{
					//mSwappedDirThisFrame = true;
					mMovementData.mCurDirectionY = EDirection_UP;
				}
				else
				{
					SDL_assert(false);
				}
			}
		}
	}
}


void MovementManager::setOnGroundFalse()
{
	mPositionData.mOnGround = false;
	mPositionData.mCurGroundMovementEffect  = 0;
	mPositionData.mCurGroundCharacteristics.clear();
}

void MovementManager::setOnGroundTrue(int curGroundMovementEffect, std::vector <EEntityCharacteristicsTypes> curGroundCharacteristics, EEntityEdgeType curGroundTop)
{
	mPositionData.mOnGround = true;
	mPositionData.mCurGroundMovementEffect  = curGroundMovementEffect;
	mPositionData.mCurGroundCharacteristics = curGroundCharacteristics;
	mPositionData.mCurGroundTop             = curGroundTop;
	
	mJumpingData.stopJump();


	if (mInputDriven)
	{
		if (mReceivedInputThisFrame)
		{
			setMovementState(EMovementStateIndex_WALKING);
		}
		else
		{
			setMovementState(EMovementStateIndex_STANDING);
		}
	}
}

bool MovementManager::isOnGround()
{
	return mPositionData.mOnGround;
}

int MovementManager::getMovementEffect()
{
	return mPositionData.mCurGroundMovementEffect;
}



bool MovementManager::isAmJump()
{
	return mJumpingData.mAmJump;
}

bool MovementManager::isAmWallJump()
{
	return mJumpingData.mAmWallJump;
}

void MovementManager::jump(float jumpMultiplier)
{
	for (int count = 0; count < mPositionData.mCurGroundCharacteristics.size(); count++)
	{
		if (mPositionData.mCurGroundCharacteristics[count] == EEntityCharacteristicsTypes_MAGNETIC)
		{
			return;
		}
	}

	bool jump = false;

	if		(mCurMovementState == EMovementStateIndex_STANDING or mCurMovementState == EMovementStateIndex_WALKING or mCurMovementState ==EMovementStateIndex_JUMPING)
	{
		jump = true;
		setMovementState(EMovementStateIndex_JUMPING);
	}
	else if (mCurMovementState == EMovementStateIndex_FALLING)
	{
		if (((FallingState*)mMovementStates[mCurMovementState])->canJump())
		{
			jump = true;
			setMovementState(EMovementStateIndex_JUMPING);
		}
	}
	/*else if (mCurMovementState == EMovementStateIndex_AUTO_MOVE)
	{
		if (((AutoMoveState*)mMovementStates[mCurMovementState])->canJump())
		{
			jump = true;
			setMovementState(EMovementStateIndex_JUMPING);
		}
	}*/

	if (jump)
	{
		((JumpingState*)mMovementStates[mCurMovementState])->startJump(jumpMultiplier);
	}
}

bool MovementManager::collideWithBouncy()
{
	setMovementState(EMovementStateIndex_JUMPING);

	if (getCode() == EEntityMovements_JUMP or getCode() == EEntityMovements_WALK_AND_JUMP)
	{
		((JumpingState*)mMovementStates[mCurMovementState])->startJump(3.0f);

	}
	else
	{
		((JumpingState*)mMovementStates[mCurMovementState])->startJump(1.5f);
	}
	return true;
}

void MovementManager::startWallJump()
{
	setMovementState(EMovementStateIndex_JUMPING);
	((JumpingState*)mMovementStates[mCurMovementState])->startWallJump();
}

void MovementManager::setMaxJumps(int maxJumps)
{
	mJumpingData.mNumMaxJumps = maxJumps;
}



void MovementManager::left()
{
	// First frame, just switch direction
	mMovementData.mCurDirection = EDirection_NONE;
	if (mPositionData.mFacing != EDirection_LEFT)
	{
		mPositionData.mFacing = EDirection_LEFT;
		return;
	}
	
	//Second frame, start moving.
	if (mCurMovementState == EMovementStateIndex_STANDING)
	{
		setMovementState(EMovementStateIndex_WALKING);
	}
	
	mMovementData.mCurDirection = EDirection_LEFT;
	
}

void MovementManager::right()
{
	// First frame, just switch direction
	mMovementData.mCurDirection = EDirection_NONE;
	if (mPositionData.mFacing != EDirection_RIGHT)
	{
		mPositionData.mFacing = EDirection_RIGHT;
		return;
	}

	//Second frame, start moving.
	if (mCurMovementState == EMovementStateIndex_STANDING)
	{
		setMovementState(EMovementStateIndex_WALKING);
	}
	
	mMovementData.mCurDirection = EDirection_RIGHT;
}



Vect2 MovementManager::getPosition() const
{
	return mPositionData.mHitbox.getTopLeft();
}

int MovementManager::getX() const
{
	return mPositionData.mHitbox.getTopLeft().getX();
}

int MovementManager::getY() const
{
	return mPositionData.mHitbox.getTopLeft().getY();
}

int MovementManager::getXChange() const
{
	return mPositionData.mHitbox.getTopLeft().getX() - mPositionData.mLastFramePosition.getX();
}

int MovementManager::getYChange() const
{
	return mPositionData.mHitbox.getTopLeft().getY() - mPositionData.mLastFramePosition.getY();
}

Vect2 MovementManager::getMovementVect2() const
{
	return mMovementData.mCurMovementVect2;
}

int MovementManager::getWidth()  const
{
	return mPositionData.mHitbox.getWidth();
}

int MovementManager::getHeight()  const
{
	return mPositionData.mHitbox.getHeight();
}

Hitbox & MovementManager::getHitbox() 
{
	return mPositionData.mHitbox;
}


EDirection MovementManager::getCurFacingDirection() const
{
	return mPositionData.mFacing;
}

void MovementManager::setCurFacingDirection(EDirection direction)
{
	mPositionData.mFacing = direction;
}

EDirection MovementManager::getCurDirection()  const
{
	return mMovementData.mCurDirection;
}

EDirection MovementManager::getCurDirectionY()  const
{
	return mMovementData.mCurDirectionY;
}

EDirection MovementManager::getLastFrameDirection() const
{
	return mMovementData.mLastFrameDirection;
}

bool MovementManager::getSwitchedDir()
{
	return mSwappedDirThisFrame;
}


EEntityMovements MovementManager::getCode() const
{
	return mMovementData.mCurMovementCode;
}

EEntityMovementPath MovementManager::getPath() const
{
	return mMovementData.mPath;
}

ECharacterModes MovementManager::getCurMode() const
{
	return mMovementData.mCurCharacterMode;
}


HitboxEdges MovementManager::getHitboxEdges() const
{
	return mPositionData.mCurHitboxEdges;
}

void MovementManager::setCurHitboxEdges(const HitboxEdges& edges, int interval)
{
	mPositionData.mCurHitboxEdges = edges;
	mCurHitboxEdgesInterval		  = interval;
	mCurHitboxEdgesIntervalLeft   = interval;
	mHaveStatusEffect             = true;
}

void MovementManager::permSetHitboxEdges(const HitboxEdges& edges)
{
	mPositionData.mOriginalHitboxEdges = edges;
	mPositionData.mCurHitboxEdges      = edges;
}

void MovementManager::resetCurHitboxEdges()
{
	mPositionData.mCurHitboxEdges = mPositionData.mOriginalHitboxEdges;
	mHaveStatusEffect			  = false;
}

EEntityEdgeType MovementManager::getEdgeType(EBoxSide boxSide) const
{
	if      (boxSide == EBoxSide_TOP)
	{
		return mPositionData.mCurHitboxEdges.mTop;
	}
	else if (boxSide == EBoxSide_BOTTOM)
	{
		return mPositionData.mCurHitboxEdges.mBottom;
	}
	else if (boxSide == EBoxSide_LEFT)
	{
		return mPositionData.mCurHitboxEdges.mLeft;
	}
	else if (boxSide == EBoxSide_RIGHT)
	{
		return mPositionData.mCurHitboxEdges.mRight;
	}
	else
	{
		SDL_assert(false);
	}
	return EEntityEdgeType_INVALID;
}


void MovementManager::calculateXDirection()
{
	if (mPositionData.mHitbox.getTopLeft().getX() > mPositionData.mLastFramePosition.getX())
	{
		mMovementData.mCurDirection = EDirection_RIGHT;

	}
	else if (mPositionData.mHitbox.getTopLeft().getX() == mPositionData.mLastFramePosition.getX())
	{
		mMovementData.mCurDirection = EDirection_NONE;
	}
	else
	{
		mMovementData.mCurDirection = EDirection_LEFT;
	}
}

void MovementManager::calculateYDirection()
{
	if (!mInputDriven)
	{
		if (mPositionData.mHitbox.getTopLeft().getY() > mPositionData.mLastFramePosition.getY())
		{
			mMovementData.mCurDirectionY = EDirection_DOWN;
		} 
		else if (mPositionData.mHitbox.getTopLeft().getY() == mPositionData.mLastFramePosition.getY())
		{
			if (mMovementData.mPath == EEntityMovementPath_VERTICAL)
			{
			}
			else
			{
				mMovementData.mCurDirectionY = EDirection_NONE;
			}
		}
		else
		{
			mMovementData.mCurDirectionY = EDirection_UP;
		}
	}
}


void MovementManager::setStartPosition(Vect2* newStartingPosition)
{
	mStartPosition = *newStartingPosition;
}

void MovementManager::setCheckpointPosition()
{
	mCheckpointPosition = mPositionData.mHitbox.getTopLeft();
}


JumpingData MovementManager::getJumpingData()
{
	return mJumpingData;
}

MovementData MovementManager::getMovementData()
{
	return mMovementData;
}

PositionData MovementManager::getPositionData()
{
	return mPositionData;
}

bool MovementManager::receivedInputThisFrame()
{
	return mReceivedInputThisFrame;
}


void MovementManager::setDebugPrint(bool onOff)
{
	mDebugPrint = onOff;
}

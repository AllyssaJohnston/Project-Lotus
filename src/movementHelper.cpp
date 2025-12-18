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
	mMovementStates[EMovementStateIndex_STANDING]	=	new StandingState(mPositionData, mMovementData,					mAttemptMove);
	mMovementStates[EMovementStateIndex_WALKING]	=	new WalkingState( mPositionData, mMovementData,					mAttemptMove);
	mMovementStates[EMovementStateIndex_JUMPING]	=	new JumpingState( mPositionData, mMovementData, mJumpingData,	mAttemptMove);
	mMovementStates[EMovementStateIndex_FLYING]		=	new FlyingState(  mPositionData, mMovementData,					mAttemptMove);
	mMovementStates[EMovementStateIndex_FALLING]	=	new FallingState( mPositionData, mMovementData, mJumpingData,	mAttemptMove);
	mMovementStates[EMovementStateIndex_STATIONARY] =	new FlyingState(  mPositionData, mMovementData,					mAttemptMove);
}

void MovementManager::setupMovementManager(const Vect2 startPosition, const EntityPreset& preset)
{
	setUpMovementManagerInternal(startPosition, preset);
	mPositionData.mHitbox = Hitbox(mStartPosition, preset.mWidth, preset.mHeight);
	mAttemptMove.mWantToMoveTo = mPositionData.mHitbox.getTopLeft();
}

void MovementManager::setupMovementManager(const Vect2 startPosition, const EntityPreset& preset, const EDirection curDirection)
{
	setUpMovementManagerInternal(startPosition, preset);
	mPositionData.mHitbox = Hitbox(mStartPosition, preset.mWidth, preset.mHeight);
	mMovementData.mCurDirection = curDirection;
	mAttemptMove.mWantToMoveTo = mPositionData.mHitbox.getTopLeft();
}

void MovementManager::setupMovementManager(const Vect2 startPosition, const EntityPreset& preset, const int widthInput, const int heightInput)
{
	setUpMovementManagerInternal(startPosition, preset);
	mPositionData.mHitbox = Hitbox(mStartPosition, widthInput, heightInput);
	mAttemptMove.mWantToMoveTo = mPositionData.mHitbox.getTopLeft();
}

void MovementManager::setupMovementManager(const Vect2 startPosition, const EntityPreset& preset, const EEntityMovementPath path, const EDirection curDirection, const int widthInput, const int heightInput)
{
	setUpMovementManagerInternal(startPosition, preset);
	mPositionData.mHitbox = Hitbox(mStartPosition, widthInput, heightInput);
	mMovementData.mPath   = path;
	mMovementData.mCurDirection = curDirection;
	mAttemptMove.mWantToMoveTo = mPositionData.mHitbox.getTopLeft();
}

void MovementManager::setupMovementManager(const Vect2 startPosition, const Vect2 movementVect, const EntityPreset& preset, const EDirection curDirectionX, const EDirection curDirectionY)
{
	setUpMovementManagerInternal(startPosition, preset);
	mPositionData.mHitbox = Hitbox(mStartPosition, preset.mWidth, preset.mHeight);
	mMovementData.mCurDirection = curDirectionX;
	mMovementData.mCurDirectionY = curDirectionY;
	mMovementData.mBaseMovementVect2 = movementVect;
	mMovementData.mCurMovementVect2 = movementVect;
	mAttemptMove.mWantToMoveTo = mPositionData.mHitbox.getTopLeft();
}

void MovementManager::setUpMovementManagerInternal(const Vect2 startPosition, const EntityPreset& preset)
{
	mStartPosition							=	startPosition;
	mCheckpointPosition						=	startPosition;
	mPositionData.mLastFramePosition		=	startPosition;

	mMovementData.setUp(preset.mMovementCodes, preset.mMovementCodes[0], preset.mMovementCodeInterval, preset.mCharacterMode);
	mMovementData.mMovementAutoMoveRule     =	preset.mAutoMoveRule;
	mMovementData.mBaseMovementVect2		=	preset.mMovementVect2;
	mMovementData.mCurMovementVect2			=	preset.mMovementVect2;
	mJumpingData.mJumpDistance				=	preset.mJumpDistance;
	mJumpingData.mNumCurJumps				=	0;
	mJumpingData.mNumMaxJumps				=	1;
	mMovementData.mUseMovementEffect		=	preset.mUseMovementEffect;

	mMovementData.mPath						=	preset.mMovementPath;
	mPositionData.mOriginalHitboxEdges		=	preset.mHitboxEdges;
	mPositionData.mCurHitboxEdges			=	preset.mHitboxEdges;
}


void MovementManager::setInputDriven(bool input) { mInputDriven = input; }

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

std::array<MovementState*, numMovementStates> MovementManager::getMovementStates() const { return mMovementStates; }

EMovementStateIndex MovementManager::getCurMovementState() const { return mCurMovementState; }

void MovementManager::setMovementStateToCharacterMode()
{
	mMoveHorizontal = true;
	switch (getMovementCode())
	{
	case EEntityMovements_FLY:
		setMovementState(EMovementStateIndex_FLYING);
		mMoveHorizontal = false;
		break;
	case EEntityMovements_JUMP:
		if (mPositionData.mOnGround)
		{
			jump(1.0f);
		}
		else if (!mJumpingData.mAmJump)
		{
			setMovementState(EMovementStateIndex_FALLING);
		}
		break;
	case EEntityMovements_WALK:
		setMovementState(EMovementStateIndex_WALKING);
		break;
	case EEntityMovements_FALL:
		setMovementState(EMovementStateIndex_STANDING);
		mMoveHorizontal = false;
		break;
	case EEntityMovements_NONE:
		setMovementState(EMovementStateIndex_STATIONARY);
		mMoveHorizontal = false;
		break;
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
	
	mMovementData.reset();
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
	mMovementData.reset();
}



void MovementManager::preTick()
{
	mCollidedThisFrame					= false;
	mSwappedDirThisFrame				= false;
	mPositionData.mLastFramePosition	= mPositionData.mHitbox.getTopLeft();

	if (mPositionData.mOnGround and !mJumpingData.mAmJump)
	{
		mJumpingData.mNumCurJumps = 0;
	}

	mCurHitboxEdgesIntervalLeft -= 1;
	if (mCurHitboxEdgesIntervalLeft <= 0)
	{
		resetCurHitboxEdges();
	}
}

void MovementManager::postTick()
{
	switch (mMovementData.getCurMovementCode())
	{
	case EEntityMovements_NONE:
	case EEntityMovements_FLY:
		break;

	case EEntityMovements_INPUT:
	default:
		calculateYDirection();
		break;
	}

	if (!mPositionData.mOnGround)
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
	mLastMovementState = mCurMovementState;
}



void MovementManager::calcMovement()
{
	mMovementData.updateMovementCodeCountDown(mPositionData.mOnGround);

	// There is nothing to update, exit early.  Could make a state that just returned
	if (mMovementData.getCurCharacterMode() == ECharacterModes_STATIC)
	{
		mAttemptMove.mWantToMoveTo = mPositionData.mHitbox.getTopLeft();
		mAttemptMove.mMoveTo = mPositionData.mHitbox.getTopLeft();
		return;
	}

	// Dynamic Entity
	updateAccelerationY();
	mMovementStates[mCurMovementState]->tickUpdate(mMoveHorizontal);


	// Transition states if needed.
	if (mCurMovementState == EMovementStateIndex_JUMPING)
	{
		// Is jump over, start falling
		if (((JumpingState*)mMovementStates[mCurMovementState])->isOver())
		{
			setMovementState(EMovementStateIndex_FALLING);
		}
	}
	else if (!mPositionData.mOnGround) // falling
	{
		setMovementState(EMovementStateIndex_FALLING);
	}
}

void MovementManager::move() { mPositionData.mHitbox.setTopLeft(mAttemptMove.mMoveTo); }

#if COLLISION_SYSTEM == 0
void MovementManager::moveToWantToMoveTo()  { mPositionData.mHitbox.setTopLeft(mAttemptMove.mWantToMoveTo); }
#endif

void MovementManager::updateAccelerationY()
{
	if (mFramesInState >= mMinFramesToAccelerate) 
	{
		if (mCurMovementState == EMovementStateIndex_FALLING)
		{
			int timeSinceAccelerating = mFramesInState - mMinFramesToAccelerate;
			mMovementData.mAccelerationY = 1.0f + (float(timeSinceAccelerating) / 60);
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


void MovementManager::useInput(std::vector <KeyData> & eventVect, bool useHorizontalInput, bool canWallJump)
{
	// Ignore input while in the walljump
	if (mCurMovementState == EMovementStateIndex_JUMPING and mJumpingData.mAmWallJump) 
	{
		return;
	}
		

	mReceivedInputThisFrame = false;
	mMovementData.mCurDirection = EDirection_NONE;

	for (int count = 0; count < eventVect.size(); count++)
	{
		int curEventEnum = eventVect[count].mKey;
		switch (curEventEnum)
		{
			case EKeyboardInput_LEFT:
			case EKeyboardInput_A:
				if (useHorizontalInput)
				{
					left();
					mReceivedInputThisFrame = true;
				}
				break;

			case EKeyboardInput_RIGHT:
			case EKeyboardInput_D:
				if (useHorizontalInput)
				{
					right();
					mReceivedInputThisFrame = true;
				}
				break;

			case EKeyboardInput_UP:
			case EKeyboardInput_W:
			case EKeyboardInput_SPACE_BAR:
				if (eventVect[count].mRepeat == 2)  // Only jump when key pressed down for at least 2 frames.
				{
					if (canWallJump) 
					{
						startWallJump();
					}
					else 
					{
						jump(1.0f);
					}
				
					mReceivedInputThisFrame = true;
				}
				break;

			default:
				break;
		}
	}

	if ( mInputDriven && !mReceivedInputThisFrame && mCurMovementState == EMovementStateIndex_WALKING)
	{	
		// Transition to standing
		setMovementState(EMovementStateIndex_STANDING);
	}
}

void MovementManager::collided(EDirection directionOfCollision)
{
	if (mMovementData.getCurMovementCode() != EEntityMovements_FALL && !mCollidedThisFrame)
	{
		mCollidedThisFrame = true;
		switch (mMovementData.mPath) 
		{
			case EEntityMovementPath_HORIZONTAL:
			case EEntityMovementPath_HORIZONTAL_CAN_FALL:
				if (directionOfCollision == EDirection_LEFT or directionOfCollision == EDirection_RIGHT) {
					mSwappedDirThisFrame = true;
					mMovementData.mCurDirection = returnOppositeDirection(mMovementData.mCurDirection);
				}
				break;

			case EEntityMovementPath_VERTICAL:
				mSwappedDirThisFrame = true;
				mMovementData.mCurDirectionY = returnOppositeDirection(mMovementData.mCurDirectionY);
				break;

			case EEntityMovementPath_DIAGONAL:
				switch (directionOfCollision) {
				case EDirection_LEFT:
				case EDirection_RIGHT:
					mMovementData.mCurDirection = returnOppositeDirection(directionOfCollision);
					break;
				case EDirection_UP:
				case EDirection_DOWN:
					mMovementData.mCurDirectionY = returnOppositeDirection(directionOfCollision);
					break;
				default:
					SDL_assert(false);
					break;
				}

				break;

			default:
				break;
		}
	}
}


void MovementManager::setCurCharacterMode(ECharacterModes givenCharacterMode) { mMovementData.setCurCharacterMode(givenCharacterMode); }

void MovementManager::setCurMovementCode(EEntityMovements givenCharacterCode) { mMovementData.setCurMovementCode(givenCharacterCode); }


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

bool MovementManager::isOnGround() const { return mPositionData.mOnGround; }

int MovementManager::getMovementEffect() const { return mPositionData.mCurGroundMovementEffect; }



bool MovementManager::inJump() const { return mJumpingData.mAmJump; }

bool MovementManager::inWallJump() const { return mJumpingData.mAmWallJump; }

void MovementManager::jump(float jumpMultiplier)
{
	if (std::find(mPositionData.mCurGroundCharacteristics.begin(), mPositionData.mCurGroundCharacteristics.end(), EEntityCharacteristicsTypes_MAGNETIC) != mPositionData.mCurGroundCharacteristics.end())
	{
		return; //on magnetic
	}

	bool jump = false;

	switch (mCurMovementState) 
	{
		case EMovementStateIndex_STANDING:
		case EMovementStateIndex_WALKING:
		case EMovementStateIndex_JUMPING:
			jump = true;
			setMovementState(EMovementStateIndex_JUMPING);
			break;
		case EMovementStateIndex_FALLING:
			if (((FallingState*)mMovementStates[mCurMovementState])->canJump())
			{
				jump = true;
				setMovementState(EMovementStateIndex_JUMPING);
			}
			break;
		default:
			break;
	}

	if (jump)
	{
		((JumpingState*)mMovementStates[mCurMovementState])->startJump(jumpMultiplier);
	}
}

bool MovementManager::collideWithBouncy()
{
	setMovementState(EMovementStateIndex_JUMPING);

	if (getMovementCode() == EEntityMovements_JUMP or getMovementCode() == EEntityMovements_WALK_AND_JUMP)
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

void MovementManager::setMaxJumps(int maxJumps) { mJumpingData.mNumMaxJumps = maxJumps; }



void MovementManager::left()
{
	// First frame, just switch direction
	mMovementData.mCurDirection = EDirection_NONE;
	if (mPositionData.mFacing != EDirection_LEFT)
	{
		mPositionData.mFacing = EDirection_LEFT;
		return;
	}
	
	// Second frame on, start moving.
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

	// Second frame on, start moving.
	if (mCurMovementState == EMovementStateIndex_STANDING)
	{
		setMovementState(EMovementStateIndex_WALKING);
	}
	
	mMovementData.mCurDirection = EDirection_RIGHT;
}

int MovementManager::getXChange() const { return mPositionData.mHitbox.getTopLeft().getX() - mPositionData.mLastFramePosition.getX(); }

int MovementManager::getYChange() const { return mPositionData.mHitbox.getTopLeft().getY() - mPositionData.mLastFramePosition.getY(); }


Vect2 MovementManager::getMovementVect2() const { return mMovementData.mCurMovementVect2; }

Hitbox& MovementManager::getHitbox() { return mPositionData.mHitbox; }


EDirection MovementManager::getCurFacingDirection() const { return mPositionData.mFacing; }

void MovementManager::setCurFacingDirection(EDirection direction) { mPositionData.mFacing = direction; }

EDirection MovementManager::getCurDirection()  const { return mMovementData.mCurDirection; }

EDirection MovementManager::getCurDirectionY()  const { return mMovementData.mCurDirectionY; }

void MovementManager::setCurDirection(EDirection dir) { mMovementData.mCurDirection = dir; mPositionData.mFacing = dir; }

EDirection MovementManager::getLastFrameDirection() const { return mMovementData.mLastFrameDirection; }

EDirection MovementManager::getLastFrameDirectionY() const { return mMovementData.mLastFrameDirectionY; }

bool MovementManager::getDidSwitchedDir() const { return mSwappedDirThisFrame; }


EEntityMovements MovementManager::getMovementCode() const { return mMovementData.getCurMovementCode(); }

EEntityMovementPath MovementManager::getPath() const { return mMovementData.mPath; }

ECharacterModes MovementManager::getCurMode() const { return mMovementData.getCurCharacterMode(); }


HitboxEdges MovementManager::getHitboxEdges() const { return mPositionData.mCurHitboxEdges; }

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
	switch (boxSide) 
	{
	case EBoxSide_TOP:		return mPositionData.mCurHitboxEdges.mTop;
	case EBoxSide_BOTTOM:	return mPositionData.mCurHitboxEdges.mBottom;
	case EBoxSide_LEFT:		return mPositionData.mCurHitboxEdges.mLeft;
	case EBoxSide_RIGHT:	return mPositionData.mCurHitboxEdges.mRight;
	default:
		SDL_assert(false);
		return EEntityEdgeType_INVALID;
	}
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
	if (mInputDriven)
	{
		return;
	}


	if (mPositionData.mHitbox.getTopLeft().getY() > mPositionData.mLastFramePosition.getY())
	{
		mMovementData.mCurDirectionY = EDirection_DOWN;
	} 
	else if (mPositionData.mHitbox.getTopLeft().getY() == mPositionData.mLastFramePosition.getY())
	{
		if (mMovementData.mPath == EEntityMovementPath_VERTICAL)
		{
			// pass
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


void MovementManager::setStartPosition(Vect2 newStartingPosition) { mStartPosition = newStartingPosition; }

void MovementManager::setCheckpointPosition() { mCheckpointPosition = mPositionData.mHitbox.getTopLeft(); }


JumpingData& MovementManager::getJumpingData() { return mJumpingData; }

MovementData& MovementManager::getMovementData() { return mMovementData; }

PositionData& MovementManager::getPositionData() { return mPositionData; }

AttemptMove& MovementManager::getAttemptMove() { return mAttemptMove; }


bool MovementManager::getReceivedInputThisFrame() const { return mReceivedInputThisFrame; }

void MovementManager::setDebugPrint(bool onOff) { mDebugPrint = onOff; }

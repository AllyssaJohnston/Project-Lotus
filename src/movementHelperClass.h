#pragma once
#include "globals.h"
#include "helperClass.h"
#include "hitboxHelper.h"
#include <iostream>
#include <vector>

class Entity;
struct AttemptMove;
struct PositionData;

enum EMovementAutoMoveRule
{
	EMovementAutoMoveRule_INVALID = -1,
	EMovementAutoMoveRule_USE_FACING_DIRECTION,
	EMovementAutoMoveRule_USE_CUR_DIRECTION,
	EMovementAutoMoveRule_MAX
};

enum EMovementStateIndex 
{ 
	EMovementStateIndex_INVALID = -1,
	EMovementStateIndex_STANDING, 
	EMovementStateIndex_WALKING, 
	EMovementStateIndex_JUMPING, 
	EMovementStateIndex_FLYING, 
	EMovementStateIndex_FALLING,
	EMovementStateIndex_STATIONARY,
	EMovementStateIndex_MAX
};

enum EEntityType
{
	EEntityType_INVALID = -1,
	EEntityType_STATIC,
	EEntityType_NON_STATIC,
	EEntityType_MAX
};

enum ECharacterModes
{
	ECharacterModes_INVALID = -1,
	ECharacterModes_STATIC,
	ECharacterModes_MOVING,
	ECharacterModes_MAX
};

enum EEntityMovements
{
	EEntityMovements_INVALID = -1,
	EEntityMovements_NONE,
	EEntityMovements_INPUT,
	EEntityMovements_FALL,
	EEntityMovements_WALK,
	EEntityMovements_WALK_AND_JUMP,
	EEntityMovements_WALK_AND_HIDE,
	EEntityMovements_WALK_AND_SHOOT,
	EEntityMovements_WALK_AND_LEAVE_TRAIL,
	EEntityMovements_FLY,
	EEntityMovements_JUMP,
	EEntityMovements_WALL_CLIMB,
	EEntityMovements_MAX
};

enum EEntityMovementPath
{
	EEntityMovementPath_INVALID = -1,
	EEntityMovementPath_NONE,
	EEntityMovementPath_HORIZONTAL,
	EEntityMovementPath_HORIZONTAL_CAN_FALL,
	EEntityMovementPath_VERTICAL,
	EEntityMovementPath_DIAGONAL,
	EEntityMovementPath_MAX
};


class MovementState
{
protected:
	std::string mStateName;
	AttemptMove& mAttemptMove;
	PositionData& mPositionData;

public:
	int mFramesInState = 0;
	MovementState(PositionData& pos, AttemptMove& move);
	void printState();
	virtual void calcMove(bool moveHorizontal) = 0;
	virtual void startedState();
	virtual void tickUpdate(bool moveHorizontal);
	virtual void left() { ; }
	virtual void right() { ; }
	virtual void landed() { ; }
	virtual void autoMove() { ; }

protected:
	virtual void updateAccelerationY() { ; }
};

struct PositionData
{
	Hitbox		mHitbox;
	HitboxEdges mOriginalHitboxEdges;
	HitboxEdges	mCurHitboxEdges;
	
	bool		mOnGround = false;
	std::vector <EEntityCharacteristicsTypes>	mCurGroundCharacteristics;
	int											mCurGroundMovementEffect	= 0;  // Addition to X velocity
	EEntityEdgeType								mCurGroundTop				= EEntityEdgeType_INVALID;
	
	EDirection  mFacing = EDirection_RIGHT;

	Vect2 mCurPosition;
	Vect2 mLastFramePosition;

	bool inGroundCharacteristics(EEntityCharacteristicsTypes type) const;
};

class MovementData
{
public:
	EDirection				mCurDirection			= EDirection_NONE;
	EDirection				mCurDirectionY			= EDirection_NONE;

	EDirection				mLastFrameDirection		= EDirection_NONE;
	EDirection				mLastFrameDirectionY	= EDirection_NONE;
	Vect2					mBaseMovementVect2;  // Base velocity of entity
	Vect2					mCurMovementVect2;   // Current base velocity of entity

	const int				mMinFramesToAccelerate	= 120;
	float					mAccelerationY			= 1.0f;

	bool					mUseMovementEffect      = true; // is entity immune to ground movement effects
	EMovementAutoMoveRule	mMovementAutoMoveRule	= EMovementAutoMoveRule_INVALID;
	
	EEntityMovementPath mPath = EEntityMovementPath_INVALID;

	void setUp(	const std::vector<EEntityMovements>& movementCodes,  const EEntityMovements& curMovementCode, const int& movementCodeInterval, 
				const ECharacterModes& characterMode);

	void setCurCharacterMode(ECharacterModes newMode);



	ECharacterModes getCharacterMode() const;

	void updateMovementCodeCountDown(bool onGround);

	void resetMovementCodeCountDown();

	void updateCurMovementCode();

	void setCurMovementCode(EEntityMovements newCode);

	EEntityMovements getCurMovementCode() const;



	void reset();


private:
	std::vector<EEntityMovements> mMovementCodes;
	EEntityMovements			  mCurMovementCode	= EEntityMovements_INVALID;
	int							  mCurMovementCodeIndex		= 0;
	int							  mMovementCodeCountDown	= -1; //how long before switching movement codes
	int							  mMovementCodeInterval		= -1;

	ECharacterModes				  mCharacterMode			= ECharacterModes_INVALID;
};

struct JumpingData
{
	int			mNumCurJumps			= 0;
	int			mNumMaxJumps			= 1;
	int			mJumpDistance			= 0;
	int			mJumpDistanceLeft		= 0;
	bool		mAmJump					= false;
	bool		mAmWallJump				= false;
	EDirection	mWallJumpDirection		= EDirection_NONE;
	void stopJump();
};

struct AttemptMove
{
	Vect2 mWantToMoveTo;
	Vect2 mMoveTo;
	bool mInterrupted = false;

	AttemptMove() { ; }
	AttemptMove(Vect2 wantToMoveTo) : mWantToMoveTo(wantToMoveTo) { ; }
};


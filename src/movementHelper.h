#pragma once
#include <SDL3/SDL.h>
#include "helperClass.h"
#include "hitboxHelper.h"
#include "entityPresets.h"
#include "movementHelperClass.h"
#include "globals.h"
#include <list>
#include <array>
#include <vector>

const int numMovementStates = 6;

class MovementManager 
{
private:

	Vect2 mStartPosition;
	Vect2 mCheckpointPosition;

	std::array<MovementState*, numMovementStates> mMovementStates;
	EMovementStateIndex mCurMovementState			= EMovementStateIndex_STANDING;
	EMovementStateIndex mLastMovementState          = EMovementStateIndex_STANDING;

	PositionData		mPositionData;
	MovementData		mMovementData;
	JumpingData			mJumpingData;
	AttemptMove         mAttemptMove;

	int                 mCurHitboxEdgesInterval     = 0;
	int                 mCurHitboxEdgesIntervalLeft = 0;

	int	const			mCurVect2Interval			= 5;
	int					mCurVect2IntervalLeft		= 0;

	bool				mCollidedThisFrame			= false;
	bool			    mSwappedDirThisFrame		= false;

	bool				mReceivedInputThisFrame		= false;
	bool				mInputDriven				= false;

	bool				mMoveHorizontal				= true;
	bool				mAutoMove					= false;
	Vect2				mAutoMoveVect2				= Vect2(0, 0);

	bool				mHaveStatusEffect			= false;

	void setUpMovementManagerInternal(const Vect2 startPosition, const EntityPreset& preset);

public:
	

	MovementManager();
	void setupMovementManager(const Vect2 startPosition, const EntityPreset& preset);
	void setupMovementManager(const Vect2 startPosition, const EntityPreset& preset, const EDirection curDirection);
	void setupMovementManager(const Vect2 startPosition, const EntityPreset& preset, const int widthInput, const int heightInput);
	void setupMovementManager(const Vect2 startPosition, const EntityPreset& preset, const EEntityMovementPath path, const EDirection curDirection, const int widthInput, const int heightInput);
	void setupMovementManager(const Vect2 startPosition, const Vect2 movementVect, const EntityPreset& preset, const EDirection curDirectionX, const EDirection curDirectionY);


	void setMovementState(EMovementStateIndex index);

	std::array<MovementState*, numMovementStates> getMovementStates() const;

	EMovementStateIndex getCurMovementState() const;

	void setMovementStateToCharacterMode();


	void resetStats();

	void resetToCheckpoint();


	void preTick();

	void postTick();


	void calcMovement();

	void move();

#if COLLISION_SYSTEM == 0
	void moveToWantToMoveTo(); // for old collision system
#endif


	void push(int changeMovement, EDirection givenDirection);
		
		

	void useInput(std::vector<KeyData>& eventVect, bool useHorizontalInput, bool canWallJump);

	void collided(EDirection direction);


	void setOnGroundFalse();

	void setOnGroundTrue(int curGroundMovementEffect, std::vector<EEntityCharacteristicsTypes> curGroundCharacteristics, EEntityEdgeType curGroundTop);

	bool isOnGround() const;

	int getMovementEffect() const;

	bool inGroundCharacteristics(EEntityCharacteristicsTypes type) const;
		

	bool inJump() const;

	void jump(float jumpMultipler);

	bool collideWithBouncy();

	void startWallJump();

	int getCurJumps();

	void setMaxJumps(int maxJumps);


	void left();

	void right();

	int getXChange() const;

	int getYChange() const;

	Vect2 getMovementVect2() const;

	int getWidth() const;

	int getHeight() const;

	Hitbox& getHitbox();
		

	EDirection getCurFacingDirection() const;

	void setCurFacingDirection(EDirection direction);

	EDirection getCurDirection() const;

	EDirection getCurDirectionY() const;

	void setCurDirection(EDirection dir);

	EDirection getLastFrameDirection() const;
		
	EDirection getLastFrameDirectionY() const;

	bool getDidSwitchedDir() const;

	void calculateXDirection();

	void calculateYDirection();



	void setCurCharacterMode(ECharacterModes givenCharacterMode);

	void setCurMovementCode(EEntityMovements givenCharacterCode);

	EEntityMovements getMovementCode() const;

	EEntityMovementPath getPath() const;

	ECharacterModes getCurMode() const;


		
	HitboxEdges getHitboxEdges() const;

	void setCurHitboxEdges(const HitboxEdges&, int interval);

	void permSetHitboxEdges(const HitboxEdges&);

	void resetCurHitboxEdges();

	EEntityEdgeType getEdgeType(EBoxSide boxSide) const;


	bool haveStatusEffect();


	void setStartPosition(Vect2 newStartingPosition);

	void setCheckpointPosition();


	
	AttemptMove& getAttemptMove();


	bool getReceivedInputThisFrame() const;

	void printState() const;


};
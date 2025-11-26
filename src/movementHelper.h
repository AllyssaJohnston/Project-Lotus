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

const int numMovementStates = 5;

class MovementManager 
{
private:

		Vect2 mStartPosition;
		Vect2 mCheckpointPosition;

		std::array<MovementState*, numMovementStates> mMovementStates;

		EMovementStateIndex mCurMovementState			= EMovementStateIndex_STANDING;
		EMovementStateIndex mLastMovementState          = EMovementStateIndex_STANDING;

		bool				mDebugPrint					= false;

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

		int					mFramesInState              = 0;

		bool				mMoveHorizontal				= true;
		bool				mAutoMove					= false;
		Vect2				mAutoMoveVect2				= Vect2(0, 0);

		//helper methods
		void setUpMovementManagerInternal(Vect2 startPosition, CCharacterPreset* preset);

		void updateCharacterModeCountDown();

		void resetCharacterModeCountDown();

		void updateCurCharacterMode();


		void updateMovementCodeCountDown();

		void resetMovementCodeCountDown();

		void updateCurMovementCode();

	public:
		bool mHaveStatusEffect = false;

		MovementManager();

		void setupMovementManager(Vect2 startPosition, CCharacterPreset* preset);

		void setupMovementManager(Vect2 startPosition, CCharacterPreset* preset, EDirection curDirection);

		void setupMovementManager(Vect2 startPosition, CCharacterPreset* preset, int widthInput, int heightInput);

		void setupMovementManager(Vect2 startPosition, CCharacterPreset* preset, EEntityMovementPath path, EDirection curDirection, int widthInput, int heightInput);

		void setupMovementManager(Vect2 startPosition, Vect2 movementVect, CCharacterPreset* preset, EDirection curDirectionX, EDirection curDirectionY);

		~MovementManager();


		void setInputDriven(bool input);


		void setMovementState(EMovementStateIndex);

		std::array<MovementState*, numMovementStates>& getMovementStates();

		EMovementStateIndex getCurMovementState();

		void setMovementStateToCharacterMode();


		void resetStats();

		void resetToCheckpoint();


		void preTick();

		void postTick();


		void calcMovement();

		void move();

#ifdef COLLISION_SYSTEM == 0
		void moveToWantToMoveTo(); // for old collision system
#endif

		void updateAccelerationY();


		//void useGroundEffect();

		void push(int changeMovement, EDirection givenDirection);
		

		void setCurCharacterMode(ECharacterModes givenCharacterMode);

		void changeCharacterModes(std::vector<ECharacterModes> givenCharacterModes);

		void setCurMovementCode(EEntityMovements givenCharacterCode);


		void useInput(std::vector<KeyData>& eventVect, bool useHorizontalInput, bool canWallJump = false);

		void collided(EDirection direction);


		void setOnGroundFalse();

		void setOnGroundTrue(int curGroundMovementEffect, std::vector<EEntityCharacteristicsTypes> curGroundCharacteristics, EEntityEdgeType curGroundTop);

		bool isOnGround() const;

		int getMovementEffect() const;
		

		bool inJump() const;

		bool inWallJump() const;

		void jump(float jumpMultipler);

		bool collideWithBouncy();

		void startWallJump();

		void setMaxJumps(int maxJumps);


		void left();

		void right();

		int getXChange() const;

		int getYChange() const;

		Vect2 getMovementVect2() const;

		int getWidth() const;

		int getHeight() const;

		Hitbox & getHitbox();
		

		EDirection getCurFacingDirection() const;

		void setCurFacingDirection(EDirection direction);

		EDirection getCurDirection() const;

		EDirection getCurDirectionY() const;

		void setCurDirection(EDirection dir);


		EDirection getLastFrameDirection() const;
		
		EDirection getLastFrameDirectionY() const;

		bool getDidSwitchedDir() const;



		EEntityMovements getMovementCode() const;

		EEntityMovementPath getPath() const;

		ECharacterModes getCurMode() const;


		
		HitboxEdges getHitboxEdges() const;

		void setCurHitboxEdges(const HitboxEdges&, int interval);

		void permSetHitboxEdges(const HitboxEdges&);

		void resetCurHitboxEdges();

		EEntityEdgeType getEdgeType(EBoxSide boxSide) const;

		

		void calculateXDirection();

		void calculateYDirection();


		void setStartPosition(Vect2 newStartingPosition);

		void setCheckpointPosition();



		JumpingData& getJumpingData();

		MovementData& getMovementData();

		PositionData& getPositionData();

		AttemptMove& getAttemptMove();

		bool receivedInputThisFrame() const;


		void setDebugPrint(bool);
};
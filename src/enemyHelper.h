#pragma once
#include <SDL3/SDL.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <list>
#include <chrono>

#include "imageHelper.h"
#include "helperClass.h"
#include "movementHelper.h"
#include "entityHelper.h"
#include "entityPresets.h"
#include "enemyPresets.h"

class Brain
{
private:
	std::chrono::time_point<std::chrono::steady_clock> mTimeOfLastDirSwitch;
	bool mTrapped			= false;
	bool mCheckpointTrapped = false;

public:

	Brain();

	void isPathBlocked();

	bool isMovementPaused();


	void tick();


	void resetStats();

	void resetToCheckpoint();

	void setCheckpointStats();
};

class Enemy : public Entity
{
private:
	EAnimationType mNextAnimationToPlay = EAnimationType_INVALID;
protected:
	EEnemyPreset*  mpPreset   = nullptr;
	EEnemyType     mEnemyType = EEnemyType_STANDARD;
	Brain		   mBrain;

	int				mProjectileCountDown					= 0;
	int				mProjectileInterval						= 0;
	bool			mShouldShootProjectile					= false;
	int				mShotProjectileInvincibilityCountDown	= 0;
	const int		mShotProjectileInvincibilityInterval	= 5;
	bool			mInvincibleToOwnProjectile				= false;
	
public:
	
	Enemy(const Vect2 & positionInput, EEnemyPreset* preset);

	Enemy();

	~Enemy();

	void setUpEnemyBaseStats(const Vect2& positionInput, EEnemyPreset* preset);

	void preTick();

	void tick();

	void postTick();


	void updateAnimationManager();

	void setNextAnimationToPlay(EAnimationType nextAnimation);


	void isPathBlocked();


	CCharacterPreset* getPreset();


	void updateProjectileCountDown();

	void resetProjectileCountDown();

	void shootProjectile();

	bool shouldShootProjectile();

	void updateInvincibilityCountDown();

	bool takeDamageFromProjectile(std::string projectileHostName);

	void resetStats();

	void resetToCheckpoint();

	void setCheckpointStats();
};

class MultiStagedEnemy : public Enemy
{
protected:
	std::vector <ECharacterModes> mStages;
	int mCurStage           = 0;
	int mCheckpointCurStage = 0;

public:

	MultiStagedEnemy(const Vect2 & positionInput, EEnemyPreset* preset);

	MultiStagedEnemy();

	~MultiStagedEnemy();


	void nextState() ;


	void updateDamage();

	void died();
	

	void setCheckpointStats();

	void resetStats();

	void resetToCheckpoint();
};
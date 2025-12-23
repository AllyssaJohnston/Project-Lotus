#pragma once
#include <SDL3/SDL.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <chrono>

#include "entity.h"
#include "enemyPresets.h"

class Brain
{
private:
	std::chrono::time_point<std::chrono::steady_clock> mTimeOfLastDirSwitch;
	bool mTrapped			= false;
	bool mCheckpointTrapped = false;

public:

	Brain();

	void setTrapped();

	bool isMovementPaused() const;


	void tick();


	void resetStats();

	void resetToCheckpoint();

	void setCheckpointStats();
};

class Enemy : public Entity
{
protected:
	EAnimationType mNextAnimationToPlay = EAnimationType_INVALID;
	EEnemyType     mEnemyType = EEnemyType_STANDARD;
	Brain		   mBrain;

	int  mProjectileCountDown					= 0;
	int  mProjectileInterval					= 0;
	bool mShouldShootProjectile					= false;
	int  mShotProjectileInvincibilityCountDown	= 0;
	int  mShotProjectileInvincibilityInterval	= 5;
	bool mInvincibleToOwnProjectile				= false;
	
public:
	
	Enemy(const Vect2& positionInput, const EEnemyPreset& preset);

	~Enemy();

	void setUpEnemyBaseStats(const Vect2& positionInput, const EEnemyPreset& preset);

	void preTick() override;

	void tick() override;

	void postTick() override;


	void updateAnimationManager() override;

	void setNextAnimationToPlay(EAnimationType nextAnimation);


	void setTrapped() override;


	void updateProjectileCountDown();

	void resetProjectileCountDown();

	void shootProjectile();

	bool shouldShootProjectile() const;


	void updateInvincibilityCountDown();

	bool shouldTakeDamageFromProjectile(std::string& projectileHostName);


	void resetStats() override;

	void resetToCheckpoint() override;

	void setCheckpointStats() override;
};

class MultiStagedEnemy : public Enemy
{
protected:
	std::vector <ECharacterModes> mStages;
	int mCurStage           = 0;
	int mCheckpointCurStage = 0;

public:

	MultiStagedEnemy(const Vect2& positionInput, const EEnemyPreset& pPreset);

	~MultiStagedEnemy();


	void nextState();


	void updateDamage() override;

	void died() override;
	

	void setCheckpointStats() override;

	void resetStats() override;

	void resetToCheckpoint() override;
};
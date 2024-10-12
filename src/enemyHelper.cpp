#include "enemyHelper.h"

Brain::Brain()
{
	mTimeOfLastDirSwitch = std::chrono::high_resolution_clock::now();
}

void Brain::isPathBlocked()
{
	const auto deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - mTimeOfLastDirSwitch);
	mTimeOfLastDirSwitch = std::chrono::high_resolution_clock::now();

	if (deltaTime.count() / (1000 * 1000) < 250)
	{
		mTrapped = true;
	}
}

bool Brain::isMovementPaused()
{
	return mTrapped;
}


void Brain::tick()
{
	const auto deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - mTimeOfLastDirSwitch);
	if (deltaTime.count() / (1000 * 1000) > 500) // More than 1/2 second has passed, reset timer.
	{
		mTrapped = false;
	}
}


void Brain::resetStats()
{
	mTrapped = false;
}

void Brain::resetToCheckpoint()
{
	mTrapped = mCheckpointTrapped;
}

void Brain::setCheckpointStats()
{
	mCheckpointTrapped = mTrapped;
}





Enemy::Enemy(const Vect2 & positionInput, EEnemyPreset* preset)
{
	setUpEnemyBaseStats(positionInput, preset);
}

Enemy::Enemy()
{
	mName = "Invalid";
}

Enemy::~Enemy()
{
	Entity::~Entity();
	mpPreset = nullptr;
}

void Enemy::setUpEnemyBaseStats(const Vect2& positionInput, EEnemyPreset* preset)
{
	setUpBaseStats(preset);
	mAnimationManager.setupAnimationManager(preset->mAnimationPresets, EHowToDetermineWidthHeight_GET_BEST_IMAGE_RATIO);
	mMovementManager.setupMovementManager(positionInput, preset);
	mMovementManager.setInputDriven(false);

	mImageObjectHitbox.setupImageObject("blue.bmp", preset->mWidth, preset->mHeight, EHowToDetermineWidthHeight_USE_WIDTH_AND_HEIGHT_INPUT);
	mpPreset = preset;

	mProjectileInterval  = preset->mProjectileInterval;
	mProjectileCountDown = mProjectileInterval;
}

void Enemy::preTick()
{
	Entity::preTick();
	mMovementManager.preTick();
	updateProjectileCountDown();
}

void Enemy::tick()
{
	Entity::tick();
	mBrain.tick();
	if (!mBrain.isMovementPaused() )
	{
		mMovementManager.updateMovement();
	}
	else
	{
		//TODO set global debug variable
		std::cout << mName<<"Trapped\n";
	}
}

void Enemy::postTick()
{
	Entity::postTick();
	updateAnimationManager();
	mMovementManager.postTick();
	mMovementManager.setMovementStateToCharacterMode();
	mMovementManager.setCurFacingDirection(mMovementManager.getMovementData().mCurDirection);
}


void Enemy::updateAnimationManager()
{
	if (mNextAnimationToPlay != EAnimationType_INVALID)
	{
		mAnimationManager.updateAnimation(mNextAnimationToPlay);
		mNextAnimationToPlay = EAnimationType_INVALID;
	}
	else if (mMovementManager.getCurMode() == ECharacterModes_STATIC)
	{
		mAnimationManager.updateAnimation(EAnimationType_STATIONARY);
	}
	else if (mMovementManager.getCode() == EEntityMovements_FLY)
	{
		mAnimationManager.updateAnimation(EAnimationType_FLY);
	}
	else if (mMovementManager.isAmJump())
	{
		mAnimationManager.updateAnimation(EAnimationType_JUMP);
	}
	else if (mMovementManager.isOnGround() == false)
	{
		mAnimationManager.updateAnimation(EAnimationType_FALL);
	}
	else if (mAnimationManager.getCurAnimation()->mMustFinish and mAnimationManager.isCurAnimationFinished() == false)
	{
		mAnimationManager.updateAnimation(mAnimationManager.getCurAnimation()->mAnimationType);
	}
	else if (mMovementManager.getCurMode() == ECharacterModes_STATIONARY or mMovementManager.getCurMode() == ECharacterModes_STATIC)
	{
		mAnimationManager.updateAnimation(EAnimationType_STATIONARY);
	}
	else
	{
		mAnimationManager.updateAnimation(EAnimationType_RUN);
	}
}

void Enemy::setNextAnimationToPlay(EAnimationType nextAnimation)
{
	mNextAnimationToPlay = nextAnimation;
}


void Enemy::isPathBlocked()
{
	mBrain.isPathBlocked();
}


CCharacterPreset* Enemy::getPreset()
{
	return mpPreset;
}


void Enemy::updateProjectileCountDown()
{
	mProjectileCountDown -= 1;
	if (mProjectileCountDown <= 0 and mProjectileInterval > 0)
	{
		mShouldShootProjectile = true;
	}
}

void Enemy::resetProjectileCountDown()
{
	mProjectileCountDown   = mProjectileInterval;
	mShouldShootProjectile = false;
}

void Enemy::shootProjectile()
{
	resetProjectileCountDown();
	setNextAnimationToPlay(EAnimationType_SHOOTING_PROJECTILE);
	mShotProjectileInvincibilityCountDown = mShotProjectileInvincibilityInterval;
	mInvincibleToOwnProjectile = true;
}

bool Enemy::shouldShootProjectile()
{
	return mShouldShootProjectile;
}

void Enemy::updateInvincibilityCountDown()
{
	mShotProjectileInvincibilityCountDown -= 1;
	if (mShotProjectileInvincibilityCountDown <= 0)
	{
		mInvincibleToOwnProjectile = false;
	}
	else
	{
		mInvincibleToOwnProjectile = true;
	}
}

bool Enemy::takeDamageFromProjectile(std::string projectileHostName)
{
	if (projectileHostName == mName and mInvincibleToOwnProjectile)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void Enemy::resetStats()
{
	Entity::resetStats();
	mBrain.resetStats();
	mAnimationManager.reset();
	resetProjectileCountDown();
}

void Enemy::resetToCheckpoint()
{
	Entity::resetToCheckpoint();
	mBrain.resetToCheckpoint();
	mAnimationManager.reset();
	resetProjectileCountDown();
}

void Enemy::setCheckpointStats()
{
	Entity::setCheckpointStats();
	mBrain.setCheckpointStats();
}




MultiStagedEnemy::MultiStagedEnemy(const Vect2 & positionInput, EEnemyPreset* preset)
{
	setUpEnemyBaseStats(positionInput, preset);
	mStages    = preset->mStages;
	mEnemyType = EEnemyType_MULTISTAGED;

}

MultiStagedEnemy::MultiStagedEnemy()
{
	mName = "Invalid";
	mEnemyType = EEnemyType_MULTISTAGED;
}

MultiStagedEnemy::~MultiStagedEnemy()
{
	Enemy::~Enemy();
}


void MultiStagedEnemy::nextState() 
{
	mCurStage += 1;
	if (mCurStage >= (int)mStages.size())
	{
		mAmAlive = false;
		mCurStage = (int)mStages.size() - 1;
	}
	else
	{
		mMovementManager.changeCharacterModes(std::vector<ECharacterModes>{mStages[mCurStage]});
		mMovementManager.setCurCharacterMode(mStages[mCurStage]);
	}
}


void MultiStagedEnemy::updateDamage()
{
	if (mTookDamageThisFrame)
	{
		mFramesTakingDamage += 1;
	}
	else
	{
		mFramesTakingDamage = 0;
	}
	if (mFramesTakingDamage >= mFramesTakingDamageBeforeDeath)
	{
		nextState();
	}
}

void MultiStagedEnemy::died()
{
	nextState();
}


void MultiStagedEnemy::setCheckpointStats() 
{
	Entity::setCheckpointStats();
	mCheckpointCurStage = mCurStage;
	mCheckpointAmAlive = mAmAlive;
	mMovementManager.setCheckpointPosition();
}

void MultiStagedEnemy::resetStats()
{
	Entity::resetStats();
	mCurStage = 0;
	mMovementManager.changeCharacterModes(std::vector<ECharacterModes>{mStages[mCurStage]});
	mMovementManager.setCurCharacterMode(mStages[mCurStage]);
}

void MultiStagedEnemy::resetToCheckpoint()
{
	Entity::resetToCheckpoint();
	mCurStage = mCheckpointCurStage;
	mMovementManager.changeCharacterModes(std::vector<ECharacterModes>{mStages[mCurStage]});
	mMovementManager.setCurCharacterMode(mStages[mCurStage]);
}
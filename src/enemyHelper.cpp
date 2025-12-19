#include "enemyHelper.h"

Brain::Brain()
{
	mTimeOfLastDirSwitch = std::chrono::high_resolution_clock::now();
}

void Brain::setTrapped()
{
	const auto deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - mTimeOfLastDirSwitch);
	mTimeOfLastDirSwitch = std::chrono::high_resolution_clock::now();

	if (deltaTime.count() / (1000 * 1000) < 250)
	{
		mTrapped = true;
	}
}

bool Brain::isMovementPaused() const { return mTrapped; }


void Brain::tick()
{
	const auto deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - mTimeOfLastDirSwitch);
	if (deltaTime.count() / (1000 * 1000) > 500) // More than 1/2 second has passed, reset timer.
	{
		mTrapped = false;
	}
}


void Brain::resetStats() { mTrapped = false; }

void Brain::resetToCheckpoint() { mTrapped = mCheckpointTrapped; }

void Brain::setCheckpointStats() { mCheckpointTrapped = mTrapped; }





Enemy::Enemy(const Vect2& positionInput, const EEnemyPreset& preset) { setUpEnemyBaseStats(positionInput, preset); }

Enemy::~Enemy() { Entity::~Entity(); }

void Enemy::setUpEnemyBaseStats(const Vect2& positionInput, const EEnemyPreset& preset)
{
	setUpBaseStats(preset);
	mAnimationManager.setupAnimationManager(preset.mAnimationPresets, EHowToDetermineWidthHeight_GET_BEST_IMAGE_RATIO);
	mMovementManager.setupMovementManager(positionInput, preset);

	if (DEMO == 0)
	{
		mImageObjectHitbox.setupImageObject("blue.bmp", preset.mWidth, preset.mHeight, EHowToDetermineWidthHeight_USE_WIDTH_AND_HEIGHT_INPUT);
	}

	mProjectileInterval  = preset.mProjectileInterval;
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
	if (!mBrain.isMovementPaused())
	{
		mMovementManager.calcMovement();
	}
}

void Enemy::postTick()
{
	Entity::postTick();
	updateAnimationManager();
	mMovementManager.postTick();
	mMovementManager.setMovementStateToCharacterMode();
	mMovementManager.setCurFacingDirection(mMovementManager.getCurDirection());
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
	else if (mMovementManager.getMovementCode() == EEntityMovements_FLY)
	{
		mAnimationManager.updateAnimation(EAnimationType_FLY);
	}
	else if (mMovementManager.inJump())
	{
		mAnimationManager.updateAnimation(EAnimationType_JUMP);
	}
	else if (!mMovementManager.isOnGround())
	{
		mAnimationManager.updateAnimation(EAnimationType_FALL);
	}
	else if (mAnimationManager.getCurAnimation()->mMustFinish and mAnimationManager.isCurAnimationFinished() == false)
	{
		mAnimationManager.updateAnimation(mAnimationManager.getCurAnimation()->mAnimationType);
	}
	else if (mMovementManager.getMovementCode() == EEntityMovements_NONE or mMovementManager.getCurMode() == ECharacterModes_STATIC)
	{
		mAnimationManager.updateAnimation(EAnimationType_STATIONARY);
	}
	else
	{
		mAnimationManager.updateAnimation(EAnimationType_RUN);
	}
}

void Enemy::setNextAnimationToPlay(EAnimationType nextAnimation) { mNextAnimationToPlay = nextAnimation; }


void Enemy::setTrapped() { mBrain.setTrapped(); }


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

bool Enemy::shouldShootProjectile() const { return mShouldShootProjectile; }

void Enemy::updateInvincibilityCountDown()
{
	mShotProjectileInvincibilityCountDown -= 1;
	mInvincibleToOwnProjectile = mShotProjectileInvincibilityCountDown > 0;
}

bool Enemy::shouldTakeDamageFromProjectile(std::string& projectileHostName) { return !(projectileHostName == mName and mInvincibleToOwnProjectile); }

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




MultiStagedEnemy::MultiStagedEnemy(const Vect2& positionInput, const EEnemyPreset& preset) : Enemy(positionInput, preset), mStages(preset.mStages)
{
	setUpEnemyBaseStats(positionInput, preset);
	mEnemyType = EEnemyType_MULTISTAGED;
}

MultiStagedEnemy::~MultiStagedEnemy() { Enemy::~Enemy(); }


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

void MultiStagedEnemy::died() { nextState(); }


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
	mMovementManager.setCurCharacterMode(mStages[mCurStage]);
}

void MultiStagedEnemy::resetToCheckpoint()
{
	Entity::resetToCheckpoint();
	mCurStage = mCheckpointCurStage;
	mMovementManager.setCurCharacterMode(mStages[mCurStage]);
}
#include "player.h"

Player::Player() : Entity()
{
	CPlayerPreset gStaticPlayerPreset = CPlayerPreset();
	mName				      = "lotus";
	mEntityType				  = EEntityType_NON_STATIC;
	mEntityClassType          = EEntityClassTypes_PLAYER;
	mEntityCharacterType      = EEntityCharacterTypes_C_PLAYER;
	mOriginalEntityCharacteristicTypes = { EEntityCharacteristicsTypes_NONE };

	mFramesTakingDamageBeforeDeath = 7;

	mMovementManager.setupMovementManager(Vect2(0, 0), gStaticPlayerPreset);
	mAnimationManager.setupAnimationManager(gStaticPlayerPreset.mAnimationPresets, EHowToDetermineWidthHeight_GET_BEST_IMAGE_RATIO);
	if (DEMO == 0)
	{
		mImageObjectHitbox.setupImageObject("blue.bmp", gStaticPlayerPreset.mWidth, gStaticPlayerPreset.mHeight, EHowToDetermineWidthHeight_USE_WIDTH_AND_HEIGHT_INPUT);
	}

	mVulnerableToProjectiles = true;
	mSwordSlashWidth		 = gStaticPlayerPreset.mSwordSlashWidth;
	mSwordSlashHeight		 = gStaticPlayerPreset.mSwordSlashHeight;
}

Player::~Player()
{
	for (Collectible* pCollectible : mpCurHeldCollectibles)
	{
		pCollectible = nullptr; // let level delete the collectibles
	}
	mpCurHeldCollectibles.clear();
	Entity::~Entity();
}

void Player::preTick()
{
	Entity::preTick();
	if (mMovementManager.isOnGround() and mMovementManager.inGroundCharacteristics(EEntityCharacteristicsTypes_SLIPPERY))
	{
		mUseHorizontalInputThisFrame = false;
	}
	mMovementManager.preTick();
}

void Player::tick()
{
	Entity::tick();
	if (!mUseHorizontalInputThisFrame)
	{
		mMovementManager.getMovementStates()[EMovementStateIndex_WALKING]->autoMove();
	}
	mMovementManager.calcMovement();
}

void Player::postTick()
{
	Entity::postTick();
	updateAnimationManager();
	mMovementManager.postTick();
	mUseHorizontalInputThisFrame = true;
	if (mMovementManager.isOnGround())
	{
		for (Collectible* curHeldCollectible : mpCurHeldCollectibles)
		{
			curHeldCollectible->setPermanentlyPickedUp(true);
		}
	}
	if (DEMO == 0)
	{
		mMovementManager.printState();
	}
}


void Player::useInput(std::vector <KeyData>& eventVect) { mMovementManager.useInput(eventVect, mUseHorizontalInputThisFrame, mCanWallJump); }


void Player::updateAnimationManager()
{
	if (mMovementManager.inJump())
	{
		mAnimationManager.updateAnimation(EAnimationType_JUMP);
	}
	else if (mCanWallJump) 
	{
		mAnimationManager.updateAnimation(EAnimationType_WALL_GRIP);
	}
	else if (!mMovementManager.isOnGround())
	{
		mAnimationManager.updateAnimation(EAnimationType_FALL);
	}
	else if (mMovementManager.getReceivedInputThisFrame())
	{
		mAnimationManager.updateAnimation(EAnimationType_RUN);
	}
	else if (mAnimationManager.getCurAnimation()->mMustFinish and !mAnimationManager.isCurAnimationFinished())
	{
		mAnimationManager.updateAnimation(mAnimationManager.getCurAnimation()->mAnimationType);
	}
	else 
	{
		mAnimationManager.updateAnimation(EAnimationType_STATIONARY);
	}

	if (mMovementManager.getCurJumps() == 2)
	{
		mAnimationManager.changeOutfit(mDoubleJumpOutfit); 
	}
	else
	{
		mAnimationManager.changeOutfit(mStandardOutfit);
	}
}


void Player::resetStats()
{
	Entity::resetStats();
	mKeys							= mStartingKeys;
	mTargets						= mStartingTargets;
	mUseHorizontalInputThisFrame	= true;
	mCanWallJump					= false;
}

void Player::resetToCheckpoint()
{
	Entity::resetToCheckpoint();
	mKeys							= mCheckpointKeys;
	mTargets						= mCheckpointTargets;
	mAmAlive						= true;
	mUseHorizontalInputThisFrame	= true;
	mCanWallJump					= mCheckpointCanWallJump;
}

void Player::setCheckpointStats()
{
	Entity::setCheckpointStats();
	mCheckpointKeys				= mKeys;
	mCheckpointTargets			= mTargets;
	mCheckpointCanWallJump		= mCanWallJump;
}


void Player::updateKeys(int keys) { mKeys += keys; }

void Player::updateTargets(int targets) { mTargets += targets; }

void Player::addHeldCollectible(Collectible* pCurCollectible) { mpCurHeldCollectibles.push_back(pCurCollectible); }


void Player::setUpAllTextures(SDL_Renderer* pRenderer) { mAnimationManager.setUpAllTextures(pRenderer); }

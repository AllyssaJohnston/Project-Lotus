#include "playerHelper.h"


Player::Player() : Entity()
{
	CPlayerPreset* preset = new CPlayerPreset();

	mName				      = "lotus";
	mEntityClassType          = EEntityClassTypes_PLAYER;
	mEntityCharacterType      = EEntityCharacterTypes_C_PLAYER;
	mOriginalEntityCharacteristicTypes.push_back(EEntityCharacteristicsTypes_NONE);

	mFramesTakingDamageBeforeDeath = 7;

	mMovementManager.setupMovementManager(Vect2(0, 0), preset);
	mMovementManager.setInputDriven(true);

	mAnimationManager.setupAnimationManager(preset->mAnimationPresets, EHowToDetermineWidthHeight_GET_BEST_IMAGE_RATIO);
	mImageObjectHitbox.setupImageObject("blue.bmp", preset->mWidth, preset->mHeight, EHowToDetermineWidthHeight_USE_WIDTH_AND_HEIGHT_INPUT);

	mVulnerableToProjectiles = true;
	mSwordSlashWidth		 = preset->mSwordSlashWidth;
	mSwordSlashHeight		 = preset->mSwordSlashHeight;

	mMovementManager.setDebugPrint(true);
}

Player::~Player()
{
	for (Collectible* collectible : mpCurHeldCollectibles)
	{
		delete collectible;
	}
	Entity::~Entity();
}

void Player::preTick()
{
	Entity::preTick();
	if (mMovementManager.getPositionData().mOnGround)
	{
		for (EEntityCharacteristicsTypes type : mMovementManager.getPositionData().mCurGroundCharacteristics)
		{
			if (type == EEntityCharacteristicsTypes_SLIPPERY)
			{
				mUseHorizontalInputThisFrame = false;
			}
		}
	}
	mMovementManager.preTick();
}

void Player::tick()
{
	Entity::tick();
	if (mUseHorizontalInputThisFrame == false)
	{
		mMovementManager.getMovementStates()[EMovementStateIndex_WALKING]->autoMove();
	}
	mMovementManager.updateMovement();

}

void Player::postTick()
{
	Entity::postTick();
	updateAnimationManager();
	mMovementManager.postTick();
	mUseHorizontalInputThisFrame = true;
	if (mMovementManager.isOnGround() == true)
	{
		for (Collectible* curHeldCollectible : mpCurHeldCollectibles)
		{
			curHeldCollectible->setPermanentlyPickedUp(true);
		}
	}
}


void Player::useInput(std::vector <KeyData>& eventVect)
{
	// Right now this just passes it on, but someday you will be state based.
	mMovementManager.useInput(eventVect, mUseHorizontalInputThisFrame);
}


void Player::updateAnimationManager()
{
	if (mMovementManager.isAmJump())
	{
		mAnimationManager.updateAnimation(EAnimationType_JUMP);
	}
	else if (mMovementManager.isOnGround() == false)
	{
		mAnimationManager.updateAnimation(EAnimationType_FALL);
	}
	else if (mMovementManager.receivedInputThisFrame())
	{
		mAnimationManager.updateAnimation(EAnimationType_RUN);
	}
	else if (mAnimationManager.getCurAnimation()->mMustFinish and mAnimationManager.isCurAnimationFinished() == false)
	{
		mAnimationManager.updateAnimation(mAnimationManager.getCurAnimation()->mAnimationType);
	}

	if (mMovementManager.getJumpingData().mNumCurJumps == 2)
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
	mKeys	 = mStartingKeys;
	mTargets = mStartingTargets;
	mUseHorizontalInputThisFrame = true;
}

void Player::resetToCheckpoint()
{
	Entity::resetToCheckpoint();
	mKeys	 = mCheckpointKeys;
	mTargets = mCheckpointTargets;
	mAmAlive = true;
	mUseHorizontalInputThisFrame = true;
}

void Player::setCheckpointStats()
{
	Entity::setCheckpointStats();
	mCheckpointKeys		= mKeys;
	mCheckpointTargets  = mTargets;
}


void Player::updateKeys(int keys)
{
	mKeys += keys;
}

void Player::updateTargets(int targets)
{
	mTargets += targets;
}

void Player::addHeldCollectible(Collectible* curCollectible)
{
	mpCurHeldCollectibles.push_back(curCollectible);
}


void Player::setUpAllTextures(SDL_Renderer* pRenderer)
{
	mAnimationManager.setUpAllTextures(pRenderer);
}
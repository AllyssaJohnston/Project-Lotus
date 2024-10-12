#include "collectibleHelper.h"

Collectible::Collectible() 
{ 
	mVulnerableToProjectiles = false;
}

Collectible::Collectible(const Vect2 position, CCollectiblePreset* preset)
{
	mEntityClassType	 = preset->mEntityClassType;
	mEntityCharacterType = preset->mEntityCharacterType;

	mMovementManager.setupMovementManager(position, preset, preset->mWidth, preset->mHeight);
	mAnimationManager.setupAnimationManager(preset->mAnimationPresets, mMovementManager.getHitbox().getWidth(), mMovementManager.getHitbox().getHeight(), EHowToDetermineWidthHeight_USE_WIDTH_AND_HEIGHT_INPUT);
	mpsgPreset				 = preset;
	mVulnerableToProjectiles = false;
}

Collectible::~Collectible()
{
	mpsgPreset = nullptr;
	Entity::~Entity();
}

bool Collectible::isAmPickedUp() const
{
	return mAmPickedUp;
}

void Collectible::setAmPickedUp(bool amPickedUp)
{
	mAmPickedUp = amPickedUp;
}


void Collectible::setCheckpointStats()
{
	mMovementManager.setCheckpointPosition();
	mCheckpointAmPickedUp = mAmPickedUp;
}

void Collectible::resetStats()
{
	mMovementManager.resetStats();
	mAmPickedUp = mStartingAmPickedUp;
}

void Collectible::resetToCheckpoint()
{
	mMovementManager.resetToCheckpoint();
	mAmPickedUp = mCheckpointAmPickedUp;
}


bool Collectible::isPermanentlyPickedUp() const
{
	return mPermanentlyAmPickedUp;
}

void Collectible::setPermanentlyPickedUp(bool amPickedUp)
{
	mPermanentlyAmPickedUp = amPickedUp;
}



SavePoint::SavePoint() : Collectible(){}

SavePoint::SavePoint(const Vect2 position) : Collectible()
{
	mpsgPreset			 = &gStaticSavePointPreset;
	mEntityClassType	 = mpsgPreset->mEntityClassType;
	mEntityCharacterType = mpsgPreset->mEntityCharacterType;

	mMovementManager.setupMovementManager(position, mpsgPreset, mpsgPreset->mWidth, mpsgPreset->mHeight);
	mAnimationManager.setupAnimationManager(mpsgPreset->mAnimationPresets, mpsgPreset->mAnimationPresets[0].mMaxImageWidth, mpsgPreset->mAnimationPresets[0].mMaxImageHeight, EHowToDetermineWidthHeight_GET_BEST_IMAGE_RATIO);
}

SavePoint::~SavePoint()
{
	Collectible::~Collectible();
}


EndOfLevelCollectible::EndOfLevelCollectible() : Collectible(){}

EndOfLevelCollectible::EndOfLevelCollectible(Vect2 position, CEndOfLevelPreset* preset) : Collectible()
{
	mpsgPreset			 = preset;
	mEntityClassType	 = mpsgPreset->mEntityClassType;
	mEntityCharacterType = mpsgPreset->mEntityCharacterType;

	mMovementManager.setupMovementManager(position, mpsgPreset, mpsgPreset->mWidth, mpsgPreset->mHeight);
	mAnimationManager.setupAnimationManager(mpsgPreset->mAnimationPresets, mMovementManager.getHitbox().getWidth(), mMovementManager.getHitbox().getHeight(), EHowToDetermineWidthHeight_USE_WIDTH_AND_HEIGHT_INPUT);
}

EndOfLevelCollectible::~EndOfLevelCollectible()
{
	Collectible::~Collectible();
}



PermanentCollectible::PermanentCollectible() : Collectible(){}

PermanentCollectible::PermanentCollectible(const Vect2 position) : Collectible(position, &gStaticLotusCollectiblePreset){}

PermanentCollectible::~PermanentCollectible()
{
	Collectible::~Collectible();
}

void PermanentCollectible::resetStats()
{
	mMovementManager.resetStats();
	mAmPickedUp = mPermanentlyAmPickedUp;
}

void PermanentCollectible::resetToCheckpoint()
{
	mMovementManager.resetToCheckpoint();
	mAmPickedUp = mPermanentlyAmPickedUp;
}



LotusCollectible::LotusCollectible() : PermanentCollectible(){}

LotusCollectible::LotusCollectible(Vect2 position) : PermanentCollectible(position)
{
	mpsgPreset			 = &gStaticLotusCollectiblePreset;
	mEntityClassType	 = mpsgPreset->mEntityClassType;
	mEntityCharacterType = mpsgPreset->mEntityCharacterType;

	mMovementManager.setupMovementManager(position, mpsgPreset, mpsgPreset->mWidth, mpsgPreset->mHeight);
	mAnimationManager.setupAnimationManager(mpsgPreset->mAnimationPresets, mpsgPreset->mAnimationPresets[0].mMaxImageWidth, mpsgPreset->mAnimationPresets[0].mMaxImageHeight, EHowToDetermineWidthHeight_GET_BEST_IMAGE_RATIO);
}

LotusCollectible::~LotusCollectible()
{
	PermanentCollectible::~PermanentCollectible();
}
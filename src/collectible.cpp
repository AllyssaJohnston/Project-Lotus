#include "collectible.h"

Collectible::Collectible(const Vect2 position, const CCollectiblePreset& preset)
{
	mEntityType			 = preset.mEntityType;
	mEntityClassType	 = preset.mEntityClassType;
	mEntityCharacterType = preset.mEntityCharacterType;

	mMovementManager.setupMovementManager(position, preset, preset.mWidth, preset.mHeight);
	mAnimationManager.setupAnimationManager(preset.mAnimationPresets, preset.mAnimationPresets[0].mMaxImageWidth, preset.mAnimationPresets[0].mMaxImageHeight, EHowToDetermineWidthHeight_GET_BEST_IMAGE_RATIO);
	mVulnerableToProjectiles = false;
}

Collectible::~Collectible() { Entity::~Entity(); }

bool Collectible::isAmPickedUp() const { return mAmPickedUp; }

void Collectible::setAmPickedUp(bool amPickedUp) { mAmPickedUp = amPickedUp; }

void Collectible::tick() 
{
	mMovementManager.calcMovement();
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

bool Collectible::isPermanentlyPickedUp() const { return mPermanentlyAmPickedUp; }

void Collectible::setPermanentlyPickedUp(bool amPickedUp) { mPermanentlyAmPickedUp = amPickedUp; }



SavePoint::SavePoint(const Vect2 position) : Collectible(position, gStaticSavePointPreset) {}

SavePoint::~SavePoint() { Collectible::~Collectible(); }


EndOfLevelCollectible::EndOfLevelCollectible(const Vect2 position, const CEndOfLevelPreset& preset) : Collectible(position, preset) {}

EndOfLevelCollectible::~EndOfLevelCollectible() { Collectible::~Collectible(); }



MiniGameLevelCollectible::MiniGameLevelCollectible(const Vect2 position, const CMiniGameLevelPreset preset) : Collectible(position, preset), mNextLevelData(preset.nextLevelData) {}

MiniGameLevelCollectible::~MiniGameLevelCollectible() { Collectible::~Collectible(); }

void MiniGameLevelCollectible::tick() 
{
	mAnimationManager.updateAnimation(EAnimationType_RUN);
	mMovementManager.setCurDirection(mImageDirection);
}



PermanentCollectible::PermanentCollectible(const Vect2 position) : Collectible(position, gStaticLotusCollectiblePreset){}

PermanentCollectible::~PermanentCollectible() { Collectible::~Collectible(); }

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



LotusCollectible::LotusCollectible(const Vect2 position) : PermanentCollectible(position) {}

LotusCollectible::~LotusCollectible() { PermanentCollectible::~PermanentCollectible(); }
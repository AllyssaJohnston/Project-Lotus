#include "entityHelper.h"

Entity::~Entity()
{
	mpTexture=nullptr;
	mpHitboxTexture = nullptr;
}

void Entity::setUpBaseStats(CCharacterPreset* preset)
{
	mEntityClassType				   = preset->mEntityClassType;
	mEntitySubClassType 			   = preset->mEntitySubClassType;
	mEntityCharacterType			   = preset->mEntityCharacterType;
	mOriginalEntityCharacteristicTypes = preset->mEntityCharacteristicTypes;
	mCurrentEntityCharacteristicTypes  = preset->mEntityCharacteristicTypes;
	mEntityType						   = preset->mEntityType;

	mVulnerableToProjectiles		   = preset->mVulnerableToProjectiles;
	mRideable						   = preset->mRideable;
	mHasAttachmentPoint			       = preset->mHasAttachmentPoint;
	mAttachmentPoint				   = preset->mAttachmentPoint;

	mSpreadEdges					   = preset->mSpreadEdges;
	mSpreadEdgesInterval			   = preset->mSpreadEdgesInterval;
	mImmuneToStatusEffects             = preset->mImmuneToStatusEffects;
}

void Entity::resetStats()
{
	mAmAlive = true;
	mTookDamageThisFrame = false;
	mFramesTakingDamage  = 0;
	mMovementManager.resetStats();
	mCurrentEntityCharacteristicTypes = mOriginalEntityCharacteristicTypes;
}

void Entity::resetToCheckpoint()
{
	mAmAlive = mCheckpointAmAlive;
	mTookDamageThisFrame = mCheckpointTookDamageThisFrame;
	mFramesTakingDamage  = mCheckpointFramesTakingDamage;
	mMovementManager.resetToCheckpoint();
	mCurrentEntityCharacteristicTypes = mOriginalEntityCharacteristicTypes;

}

void Entity::setCheckpointStats() 
{
	mCheckpointAmAlive = mAmAlive;
	mCheckpointTookDamageThisFrame = mTookDamageThisFrame;
	mCheckpointFramesTakingDamage  = mFramesTakingDamage;
	mMovementManager.setCheckpointPosition();
}


void Entity::takeDamage() 
{
	mTookDamageThisFrame = true;
	//mAmAlive = false;
}

void Entity::updateDamage()
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
		mAmAlive = false;
	}
}

bool Entity::takingDamage()
{
	return mTookDamageThisFrame;
}

void Entity::died()
{
	mAmAlive = false;
}

bool Entity::getAmAlive() const
{
	return mAmAlive;
}


void Entity::preTick()
{
	mTookDamageThisFrame = false;
}

void Entity::tick()
{
	return;
}

void Entity::postTick()
{
	mCurrentEntityCharacteristicTypes = mOriginalEntityCharacteristicTypes;
	updateDamage();
}


ImageObject& Entity::getImageObjectHitbox() 
{
	return mImageObjectHitbox;
}


SDL_Texture* Entity::getTexture() const
{
	return mpTexture;
}

void Entity::setTexture(SDL_Texture* texture)
{
	mpTexture = texture;
}

SDL_Texture* Entity::getHitboxTexture() const
{
	return mpHitboxTexture;
}

void Entity::setHitboxTexture(SDL_Texture* texture)
{
	mpHitboxTexture = texture;
}


MovementManager& Entity::getMovementManager()
{
	return mMovementManager;
}

int Entity::getMovementEffect() const
{
	return mMovementEffect;
}


EEntityType Entity::getType() const
{
	return mEntityType;
}

int Entity::getClassType() const
{
	return mEntityClassType;
}

int Entity::getSubClassType() const
{
	return mEntitySubClassType;
}

EEntityCharacterTypes Entity::getCharacterType()  const
{
	return mEntityCharacterType;
}

std::vector <EEntityCharacteristicsTypes> Entity::getCurCharacteristics() const
{
	return mCurrentEntityCharacteristicTypes;
}

bool Entity::hasCharacteristic(EEntityCharacteristicsTypes characteristic) const
{
	for (EEntityCharacteristicsTypes characterIter : mCurrentEntityCharacteristicTypes)
	{
		if (characterIter == characteristic)
		{
			return true;
		}
	}
	return false;
}

//TODO rename to setTrapped
void Entity::isPathBlocked()
{
	return;
}

std::string Entity::getHostName() const
{
	return "";
}
#pragma once

#include  <memory>
#include "imageHelper.h"
#include "movementHelper.h"


class Entity
{
protected:

	EEntityClassTypes						  mEntityClassType					= EEntityClassTypes_ENTITY;
	EEntitySubClassTypes                      mEntitySubClassType               = EEntitySubClassTypes_NA;
	EEntityCharacterTypes					  mEntityCharacterType				= EEntityCharacterTypes_C_ENTITY;
	EEntityType								  mEntityType						= EEntityType_INVALID;

	std::vector <EEntityCharacteristicsTypes> mOriginalEntityCharacteristicTypes;
	std::vector <EEntityCharacteristicsTypes> mCurrentEntityCharacteristicTypes;

	ImageObject								  mImageObjectHitbox;
	SDL_Texture*							  mpTexture							= nullptr;
	SDL_Texture*							  mpHitboxTexture					= nullptr;

	MovementManager							  mMovementManager; 
	int										  mMovementEffect					= 0;

	bool									  mAmAlive							= true;
	bool									  mCheckpointAmAlive				= true;

	bool									  mTookDamageThisFrame              = false;
	int										  mFramesTakingDamage				= 0;
	bool                                      mCheckpointTookDamageThisFrame    = false;
	int										  mCheckpointFramesTakingDamage     = 0;
	int								          mFramesTakingDamageBeforeDeath    = 1;

public:
	std::string								  mName								= "Invalid";
	AnimationManager						  mAnimationManager;

	bool								      mIsVisible						= true;
	bool									  mStartingIsVisible				= true;
	bool									  mCheckpointIsVisible				= true;

	bool									  mIsActivated						= false;
	bool									  mStartingIsActivated				= false;
	bool								      mCheckpointIsActivated			= false;

	bool									  mVulnerableToProjectiles			= false;
	bool									  mRideable							= false;
	bool									  mHasAttachmentPoint				= false;
	Vect2									  mAttachmentPoint					= Vect2(0,0);
	bool									  mSpreadEdges						= false; //to other entities
	int										  mSpreadEdgesInterval				= false; //for other entities
	bool									  mImmuneToStatusEffects			= false;

	Entity() {}

	virtual ~Entity();

	void setUpBaseStats(CCharacterPreset* preset);

	virtual void resetStats();

	virtual void resetToCheckpoint();

	virtual void setCheckpointStats();


	virtual void updateAnimationManager() {}

	virtual void takeDamage();

	virtual void updateDamage();

	bool takingDamage();

	virtual void died();

	bool getAmAlive() const;


	virtual void preTick();

	virtual void tick();

	virtual void postTick();


	ImageObject& getImageObjectHitbox();


	SDL_Texture* getTexture() const;

	void setTexture(SDL_Texture* texture);

	SDL_Texture* getHitboxTexture() const;

	void setHitboxTexture(SDL_Texture* texture);



	MovementManager& getMovementManager();
	
	int getMovementEffect() const;


	EEntityType getType() const;

	int getClassType() const;

	int getSubClassType() const;

	EEntityCharacterTypes getCharacterType()  const;

	std::vector <EEntityCharacteristicsTypes> getCurCharacteristics() const;

	bool hasCharacteristic(EEntityCharacteristicsTypes characteristic) const;
	

	virtual void isPathBlocked();

	virtual std::string getHostName() const;
};


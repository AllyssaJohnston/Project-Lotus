#pragma once
#include "vector"
#include "helpers.h"
#include "hitbox.h"
#include "movementHelpers.h"
#include "image.h"
#include "animationManager.h"


struct EntityPreset
{
	EEntityClassTypes							mEntityClassType		    = EEntityClassTypes_ENTITY;
	EEntitySubClassTypes						mEntitySubClassType			= EEntitySubClassTypes_NA;
	EEntityCharacterTypes						mEntityCharacterType		= EEntityCharacterTypes_C_CHARACTER;
	std::vector <EEntityCharacteristicsTypes>	mEntityCharacteristicTypes;
	EEntityType									mEntityType					= EEntityType_INVALID;
	std::string									mTypeName					= "INVALID CHARACTER";

	ECharacterModes								mCharacterMode				= ECharacterModes_INVALID;
	std::vector <ECharacterModes>				mStages;
	std::vector<EEntityMovements>				mMovementCodes				= { EEntityMovements_NONE };
	EEntityMovementPath							mMovementPath				= EEntityMovementPath_NONE;
	int											mMovementCodeInterval		= 0;
	
	

	EMovementAutoMoveRule	mAutoMoveRule				= EMovementAutoMoveRule_USE_FACING_DIRECTION;		
	bool					mInputDriven				= false;


	EDirection				mCurDirection				= EDirection_NONE;
	Vect2					mMovementVect2				= Vect2(0, 0);
	int						mJumpDistance				= 0;

	bool					mVulnerableToProjectiles	= true;
	bool					mRideable					= false;
	bool					mHasAttachmentPoint			= false;
	Vect2					mAttachmentPoint			= Vect2(0, 0);

	HitboxEdges				mHitboxEdges;
	bool					mSpreadEdges				= false;
	int						mSpreadEdgesInterval		= 0;
	bool					mImmuneToStatusEffects		= false;

	int						mMovementEffect				= 0;
	bool					mUseMovementEffect			= true;
	

	int mWidth = -1;		// for the hitbox
	int mHeight = -1;		// for the hitbox
	int mExtraWidth = 0;	// for the image
	int mExtraHeight = 0;	// for the image
	int mImageOffsetX = 0;
	int mImageOffsetY = 0;
	std::vector <AnimationPreset> mAnimationPresets;

	EntityPreset(){}

	~EntityPreset(){}
};

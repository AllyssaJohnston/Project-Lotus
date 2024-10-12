#pragma once
#include "helperClass.h"
#include "hitboxHelper.h"
#include "movementHelperClass.h"
#include "imageHelper.h"
#include <SDL3/SDL.h>

//TODO reanme to EEntityPreset
struct CCharacterPreset
{
	EEntityClassTypes						  mEntityClassType		    = EEntityClassTypes_ENTITY;
	EEntitySubClassTypes					  mEntitySubClassType		= EEntitySubClassTypes_NA;
	EEntityCharacterTypes				      mEntityCharacterType      = EEntityCharacterTypes_C_CHARACTER;
	std::vector <EEntityCharacteristicsTypes> mEntityCharacteristicTypes;
	EEntityType								  mEntityType				= EEntityType_INVALID;
	std::vector <ECharacterModes>			  mCharacterModes;
	std::vector <ECharacterModes>			  mStages;

	HitboxEdges mHitboxEdges;

	std::vector<EEntityMovements> mMovementCodes	= { EEntityMovements_NONE };
	EEntityMovementPath mMovementPath				= EEntityMovementPath_NONE;
	int					mMovementCodeInterval		= 0;
	int				    mCharacterModeInterval		= 0;
	
	int					mMovementEffect				= 0;
	bool                mUseMovementEffect			= true;
	EMovementAutoMoveRule mAutoMoveRule				= EMovementAutoMoveRule_USE_FACING_DIRECTION;		


	EDirection			mCurDirection				= EDirection_NONE;
	Vect2				mMovementVect2				= Vect2(0, 0);
	int				    mJumpDistance				= 0;
	int					mWallJumpDistanceX			= 0;
	int					mWallJumpDistanceY			= 0;

	int					mWidth						= -1;
	int					mHeight						= -1;
	int					mExtraWidth = 0;
	int					mExtraHeight = 0;
	int					mImageOffsetX = 0;
	int					mImageOffsetY = 0;

	std::vector <AnimationPreset> mAnimationPresets;

	std::string mTypeName = "INVALID CHARACTER";

	bool  mVulnerableToProjectiles = true;
	bool  mRideable                = false;
	bool  mHasAttachmentPoint      = false;
	Vect2 mAttachmentPoint         = Vect2(0, 0);

	bool mSpreadEdges           = false;
	int	 mSpreadEdgesInterval   = 0;
	bool mImmuneToStatusEffects = false;

	CCharacterPreset(){}

	~CCharacterPreset(){}
};

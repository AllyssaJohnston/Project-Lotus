#include "projectilePresets.h"

ProjectilePreset::ProjectilePreset() : CCharacterPreset()
{
	mEntityClassType          = EEntityClassTypes_PROJECTILE;

	mHitboxEdges			  = HitboxEdges(EEntityEdgeType_HAZARDOUS, EEntityEdgeType_HAZARDOUS, EEntityEdgeType_HAZARDOUS, EEntityEdgeType_HAZARDOUS);
	mEntityType               = EEntityType_NON_STATIC;

	mCharacterModes.push_back(ECharacterModes_MOVING);
	mMovementCodes			  = {EEntityMovements_FLY};
	mMovementPath             = EEntityMovementPath_DIAGONAL;

	mImageOffsetX = 0;
	mImageOffsetY = 0;

	mVulnerableToProjectiles = true;

	mSpreadEdges			 = false;
	mSpreadEdgesInterval     = 0;
	mImmuneToStatusEffects   = false;
}

PlayerProjectilePreset::PlayerProjectilePreset(EEntityMovementPath movementPath) : ProjectilePreset()
{
	mEntityCharacterType      = EEntityCharacterTypes_PJ_PLAYER_PROJECTILE;

	mMovementVect2            = Vect2(6, 6);
	mMovementPath             = movementPath;

	std::vector <std::string> animationStationaryFileNames;
	if (mMovementPath == EEntityMovementPath_HORIZONTAL)
	{
		mWidth                    = 30;
		mHeight                   = 13;
		animationStationaryFileNames.push_back("Projectiles/sword.bmp");
	}
	else
	{
		mWidth                    = 13;
		mHeight                   = 30;
		animationStationaryFileNames.push_back("Projectiles/SwordV.bmp");
	}

	int maxImageWidth            = 80;
	int maxImageHeight           = 80;

	mTypeName = "PLAYER PROJECTILE";


	int  stationaryFrameRate = 0;
	bool stationaryMustFinish = false;
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY, animationStationaryFileNames, maxImageWidth, maxImageHeight, mImageOffsetX, mImageOffsetY, stationaryFrameRate, stationaryMustFinish));
}

EnemyProjectilePreset::EnemyProjectilePreset() : ProjectilePreset()
{
	mEntityCharacterType      = EEntityCharacterTypes_PJ_ENEMY_PROJECTILE;

	mMovementVect2            = Vect2(5, 5);

	mWidth                    = 25;
	mHeight                   = 25;

	int maxImageWidth         = 35;
	int maxImageHeight        = 35;

	mTypeName = "ENEMY PROJECTILE";


	int  stationaryFrameRate = 0;
	bool stationaryMustFinish = false;
	std::vector <std::string> animationStationaryFileNames;
	animationStationaryFileNames.push_back("Projectiles/Fireball.bmp");
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY, animationStationaryFileNames, maxImageWidth, maxImageHeight, mImageOffsetX, mImageOffsetY, stationaryFrameRate, stationaryMustFinish));
}
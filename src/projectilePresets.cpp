#include "projectilePresets.h"

ProjectilePreset::ProjectilePreset() : EntityPreset()
{
	mEntityClassType			= EEntityClassTypes_PROJECTILE;
	mEntityType					= EEntityType_NON_STATIC;

	mCharacterMode = ECharacterModes_MOVING;
	mMovementCodes = { EEntityMovements_FLY };
	mMovementPath = EEntityMovementPath_DIAGONAL;

	mHitboxEdges				= HitboxEdges(EEntityEdgeType_HAZARDOUS, EEntityEdgeType_HAZARDOUS, EEntityEdgeType_HAZARDOUS, EEntityEdgeType_HAZARDOUS);
	mSpreadEdges = false;
	mSpreadEdgesInterval = 0;
	mImmuneToStatusEffects = false;

	mVulnerableToProjectiles = true;
	
	mImageOffsetX = 0;
	mImageOffsetY = 0;
}

PlayerProjectilePreset::PlayerProjectilePreset(EEntityMovementPath movementPath) : ProjectilePreset()
{
	mEntityCharacterType      = EEntityCharacterTypes_PJ_PLAYER_PROJECTILE;
	mTypeName = "PLAYER PROJECTILE";

	mMovementVect2            = Vect2(6, 6);
	mMovementPath             = movementPath;

	std::vector <std::string> animationStationaryFileNames;
	if (mMovementPath == EEntityMovementPath_HORIZONTAL)
	{
		mWidth  = 30;
		mHeight = 13;
		animationStationaryFileNames = { "Projectiles/sword.bmp" };
	}
	else
	{
		mWidth  = 13;
		mHeight = 30;
		animationStationaryFileNames = { "Projectiles/SwordV.bmp" };
	}

	int maxImageWidth  = 80;
	int maxImageHeight = 80;
	int  stationaryFrameRate = 0;
	bool stationaryMustFinish = false;
	mAnimationPresets = { AnimationPreset(EAnimationType_STATIONARY, animationStationaryFileNames, maxImageWidth, maxImageHeight, mImageOffsetX, mImageOffsetY, stationaryFrameRate, stationaryMustFinish) };
}

EnemyProjectilePreset::EnemyProjectilePreset() : ProjectilePreset()
{
	mEntityCharacterType      = EEntityCharacterTypes_PJ_ENEMY_PROJECTILE;
	mTypeName = "ENEMY PROJECTILE";

	mMovementVect2            = Vect2(5, 5);

	mWidth                    = 25;
	mHeight                   = 25;

	int maxImageWidth         = 35;
	int maxImageHeight        = 35;

	int  stationaryFrameRate = 0;
	bool stationaryMustFinish = false;
	std::vector <std::string> animationStationaryFileNames = { "Projectiles/Fireball.bmp" };
	mAnimationPresets = { AnimationPreset(EAnimationType_STATIONARY, animationStationaryFileNames, maxImageWidth, maxImageHeight, mImageOffsetX, mImageOffsetY, stationaryFrameRate, stationaryMustFinish) };
}
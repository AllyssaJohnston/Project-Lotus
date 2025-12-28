#include "platformPresets.h"

PPlatformPreset::PPlatformPreset() : EntityPreset()
{
	mEntityClassType			= EEntityClassTypes_PLATFORM;
	mEntityCharacteristicTypes	= { EEntityCharacteristicsTypes_INVINCIBLE };
	mEntityType					= EEntityType_STATIC;

	mCharacterMode  = ECharacterModes_STATIC;
	mMovementCodes  = { EEntityMovements_NONE };
	mMovementPath	= EEntityMovementPath_NONE;
	
	mCurDirection = EDirection_NONE;

	mMovementVect2 = Vect2(0, 0);

	mVulnerableToProjectiles	= false;
	mRideable					= true;

	mSpreadEdges			= false;
	mSpreadEdgesInterval	= 0;
	mImmuneToStatusEffects	= false;

	mMovementEffect = 0;

	mWidth  = -1;
	mHeight = -1;

	mImageOffsetX = 0;
	mImageOffsetY = 0;
	mExtraWidth = 0;
	mExtraHeight = 0;
}



PStandardPreset::PStandardPreset(int worldNumber) : PPlatformPreset()
{
	mEntityCharacterType      = EEntityCharacterTypes_P_STANDARD;
	mTypeName = "STANDARD";

	mHitboxEdges = HitboxEdges(EEntityEdgeType_NEUTRAL, EEntityEdgeType_NON_EXISTENT, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);

	setUpAnimationPresets(EWorldType(worldNumber));
}

void PStandardPreset::setUpAnimationPresets(EWorldType worldType)
{
	int  frameRate = 0;
	bool mustFinish = false;

	std::vector <std::string> horizontalFileNames;
	std::vector <std::string> verticalFileNames;
	switch (worldType)
	{
	case EWorldType_EARTH:
		horizontalFileNames = { "Platforms/platformStandardEarth.bmp" };
		verticalFileNames	= { "Platforms/platformStandardEarthV.bmp" };
		break;
	case EWorldType_FIRE:
		horizontalFileNames = { "Platforms/platformStandardFire.bmp" };
		verticalFileNames	= { "Platforms/platformStandardFireV.bmp" };
		break;
	case EWorldType_WATER:
		horizontalFileNames = { "Platforms/platformStandardWater.bmp" };
		verticalFileNames	= { "Platforms/platformStandardWaterV.bmp"};
		break;
	case EWorldType_AIR:
	default:
		horizontalFileNames = { "Platforms/platformStandardAir.bmp" };
		verticalFileNames	= { "Platforms/platformStandardAirV.bmp"};
		break;
	}
	
	mHorizontalAnimationPresets = { AnimationPreset(EAnimationType_STATIONARY,		horizontalFileNames,	mImageOffsetX, mImageOffsetY, frameRate, mustFinish) };
	mVerticalAnimationPresets	= { AnimationPreset(EAnimationType_STATIONARY,		verticalFileNames,		mImageOffsetX, mImageOffsetY, frameRate, mustFinish) };
}



PSolidPreset::PSolidPreset(int worldNumber) : PPlatformPreset()
{
	mEntityCharacterType = EEntityCharacterTypes_P_SOLID;
	mTypeName = "SOLID";

	mHitboxEdges = HitboxEdges(EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);

	setUpAnimationPresets(EWorldType(worldNumber));
}

void PSolidPreset::setUpAnimationPresets(EWorldType worldType)
{
	int  frameRate = 0;
	bool mustFinish = false;

	std::vector <std::string> horizontalFileNames;
	std::vector <std::string> verticalFileNames;
	switch (worldType)
	{
	case EWorldType_EARTH:
		horizontalFileNames = { "Platforms/platformSolidEarth.bmp" };
		verticalFileNames	= { "Platforms/platformSolidEarthV.bmp"};
		break;
	case EWorldType_FIRE:
		horizontalFileNames = { "Platforms/platformSolidFire.bmp" };
		verticalFileNames	= { "Platforms/platformSolidFireV.bmp"};
		break;
	case EWorldType_WATER:
	default:
		horizontalFileNames = { "Platforms/platformSolidWater.bmp" };
		verticalFileNames	= { "Platforms/platformSolidWaterV.bmp" };
		break;
	}

	mHorizontalAnimationPresets = { AnimationPreset(EAnimationType_STATIONARY,		horizontalFileNames,	mImageOffsetX, mImageOffsetY, frameRate, mustFinish) };
	mVerticalAnimationPresets	= { AnimationPreset(EAnimationType_STATIONARY,		verticalFileNames,		mImageOffsetX, mImageOffsetY, frameRate, mustFinish) };
}



PStickyPreset::PStickyPreset() : PPlatformPreset()
{
	mEntityCharacterType = EEntityCharacterTypes_P_STICKY;
	mTypeName = "STICKY";
	
	mHitboxEdges = HitboxEdges(EEntityEdgeType_STICKY, EEntityEdgeType_NON_EXISTENT, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);

	mMovementEffect = -4;

	int  frameRate = 0;
	bool mustFinish = false;

	std::vector <std::string> horizontalFileNames = { "Platforms/platformSticky.bmp" };
	mHorizontalAnimationPresets = { AnimationPreset(EAnimationType_STATIONARY, horizontalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish) };
}



PIcyPreset::PIcyPreset() : PPlatformPreset()
{
	mEntityCharacterType		= EEntityCharacterTypes_P_ICY;
	mEntityCharacteristicTypes = { EEntityCharacteristicsTypes_SLIPPERY };
	mTypeName = "ICY";
	
	mHitboxEdges				= HitboxEdges(EEntityEdgeType_ICY, EEntityEdgeType_NON_EXISTENT, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);

	mMovementEffect = 2;

	int  frameRate = 0;
	bool mustFinish = false;

	std::vector <std::string> horizontalFileNames = { "Platforms/platformIcy.bmp" };
	mHorizontalAnimationPresets = { AnimationPreset(EAnimationType_STATIONARY, horizontalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish) };
}



PMagneticPreset::PMagneticPreset() : PPlatformPreset()
{
	mEntityCharacterType		= EEntityCharacterTypes_P_MAGNETIC;
	mEntityCharacteristicTypes = { EEntityCharacteristicsTypes_MAGNETIC };
	mTypeName = "METALLIC";
	
	mHitboxEdges				= HitboxEdges(EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);

	int  frameRate = 0;
	bool mustFinish = false;
	std::vector <std::string> horizontalFileNames = { "Platforms/platformMetallic.bmp" };
	mHorizontalAnimationPresets = { AnimationPreset(EAnimationType_STATIONARY, horizontalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish) };	
}



PCrumblingPreset::PCrumblingPreset() : PPlatformPreset()
{
	mEntityCharacterType = EEntityCharacterTypes_P_CRUMBLING;
	mTypeName = "CRUMBLING";
	
	mHitboxEdges =		 HitboxEdges(EEntityEdgeType_CRUMBLING,    EEntityEdgeType_NON_EXISTENT, EEntityEdgeType_NEUTRAL,	   EEntityEdgeType_NEUTRAL);
	mHiddenHitboxEdges = HitboxEdges(EEntityEdgeType_NON_EXISTENT, EEntityEdgeType_NON_EXISTENT, EEntityEdgeType_NON_EXISTENT, EEntityEdgeType_NON_EXISTENT);

	int  frameRate  = 4;
	bool mustFinish = true;

	std::vector <std::string> horizontalFileNames = {"Platforms/platformCrumbling01.bmp", "Platforms/platformCrumbling02.bmp",
		"Platforms/platformCrumbling03.bmp", "Platforms/platformCrumbling04.bmp",
		"Platforms/platformCrumbling05.bmp", "Platforms/platformCrumbling06.bmp",
		"Platforms/platformCrumbling07.bmp", "Platforms/platformCrumbling08.bmp",
		"Platforms/platformCrumbling09.bmp", "Platforms/platformCrumbling10.bmp"};
	mHorizontalAnimationPresets = { AnimationPreset(EAnimationType_PLAY, horizontalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish) };
}



PSpikePreset::PSpikePreset() : PPlatformPreset()
{
	mEntityCharacterType      = EEntityCharacterTypes_P_SPIKE;
	mTypeName = "SPIKE";
	
	mHitboxEdges = HitboxEdges(EEntityEdgeType_SPIKEY, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);

	int frameRate = 0;
	bool mustFinish = false;
	mPrintViaChunk = false;
	mHowToDetermineWidthHeight = EHowToDetermineWidthHeight_SPLICE;

	mSplice = true;
	std::vector <std::string> horizontalFileNames = { "Platforms/platformSpike1.bmp", "Platforms/platformSpike2.bmp", "Platforms/platformSpike3.bmp" };
	mHorizontalAnimationPresets = { AnimationPreset(EAnimationType_STATIONARY, horizontalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish) };	
}



PLavaPreset::PLavaPreset() : PPlatformPreset()
{
	mEntityCharacterType      = EEntityCharacterTypes_P_LAVA;
	mTypeName = "LAVA";

	mHitboxEdges = HitboxEdges(EEntityEdgeType_BURNING, EEntityEdgeType_BURNING, EEntityEdgeType_BURNING, EEntityEdgeType_BURNING);

	int  frameRate = 0;
	bool mustFinish = false;

	std::vector <std::string> horizontalFileNames = { "Platforms/platformLava.bmp" };
	mHorizontalAnimationPresets = { AnimationPreset(EAnimationType_STATIONARY, horizontalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish) };

	std::vector <std::string> verticalFileNames = { "Platforms/platformLavaV.bmp" };
	mVerticalAnimationPresets = { AnimationPreset(EAnimationType_STATIONARY, verticalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish) };
}



PElectricPreset::PElectricPreset() : PPlatformPreset()
{
	mEntityCharacterType      = EEntityCharacterTypes_P_ELECTRIC;
	mTypeName = "ELECTRIC";
	
	mHitboxEdges = HitboxEdges(EEntityEdgeType_ELECTRIC, EEntityEdgeType_ELECTRIC, EEntityEdgeType_ELECTRIC, EEntityEdgeType_ELECTRIC);
	mSpreadEdges = true;
	mSpreadEdgesInterval = 15;

	int  frameRate = 0;
	bool mustFinish = false;

	std::vector <std::string> horizontalFileNames = { "Platforms/platformElectric.bmp" };
	mHorizontalAnimationPresets = { AnimationPreset(EAnimationType_STATIONARY, horizontalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish) };

	std::vector <std::string> verticalFileNames = { "Platforms/platformElectricV.bmp" };
	mVerticalAnimationPresets = { AnimationPreset(EAnimationType_STATIONARY, verticalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish) };
}



PBouncyPreset::PBouncyPreset() : PPlatformPreset()
{
	mEntityCharacterType      = EEntityCharacterTypes_P_BOUNCY;
	mTypeName = "BOUNCY";
	
	mHitboxEdges = HitboxEdges(EEntityEdgeType_BOUNCY, EEntityEdgeType_NON_EXISTENT, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);

	int  frameRate = 0;
	bool mustFinish = false;

	std::vector <std::string> horizontalFileNames = { "Platforms/platformBouncy.bmp" };
	mHorizontalAnimationPresets = { AnimationPreset(EAnimationType_STATIONARY, horizontalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish) };

	std::vector <std::string> verticalFileNames = { "Platforms/platformBouncyV.bmp" };
	mVerticalAnimationPresets = { AnimationPreset(EAnimationType_STATIONARY, verticalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish) };
}



PWallJumpablePreset::PWallJumpablePreset() : PPlatformPreset()
{
	mEntityCharacterType      = EEntityCharacterTypes_P_WALL_JUMPABLE;
	mTypeName = "WALL JUMPABLE";
	
	mHitboxEdges = HitboxEdges(EEntityEdgeType_NEUTRAL, EEntityEdgeType_NON_EXISTENT, EEntityEdgeType_WALL_JUMPABLE, EEntityEdgeType_WALL_JUMPABLE);

	int  frameRate = 0;
	bool mustFinish = false;

	std::vector <std::string> horizontalFileNames = { "Platforms/platformBouncy.bmp" };
	mHorizontalAnimationPresets = { AnimationPreset(EAnimationType_STATIONARY, horizontalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish) };

	std::vector <std::string> verticalFileNames = { "Platforms/platformBouncyV.bmp" };
	mVerticalAnimationPresets = { AnimationPreset(EAnimationType_STATIONARY, verticalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish) };
}



PGatePreset::PGatePreset(int worldNumber) : PPlatformPreset()
{
	mEntityCharacterType	= EEntityCharacterTypes_P_GATE;
	mTypeName = "GATE";
	
	mHitboxEdges			= HitboxEdges(EEntityEdgeType_NEUTRAL,	   EEntityEdgeType_NEUTRAL,		 EEntityEdgeType_NEUTRAL,	   EEntityEdgeType_NEUTRAL);
	mHiddenHitboxEdges		= HitboxEdges(EEntityEdgeType_NON_EXISTENT, EEntityEdgeType_NON_EXISTENT, EEntityEdgeType_NON_EXISTENT, EEntityEdgeType_NON_EXISTENT);
	
	mPrintViaChunk			= false;
	mHowToDetermineWidthHeight = EHowToDetermineWidthHeight_USE_WIDTH_AND_HEIGHT_INPUT;

	setUpAnimationPresets(EWorldType(worldNumber));
}

void PGatePreset::setUpAnimationPresets(EWorldType worldType)
{
	int  frameRate = 0;
	bool mustFinish = false;

	mExtraWidth = 25;

	std::vector <std::string> verticalFileNames;
	switch (worldType)
	{
	case EWorldType_EARTH:
		verticalFileNames = { "Platforms/platformGateEarth.bmp" };
		mExtraWidth = 35;
		break;
	case EWorldType_FIRE:
		verticalFileNames = { "Platforms/platformGateFire.bmp" };
		break;
	case EWorldType_WATER:
	default:
		verticalFileNames = { "Platforms/platformGateWater.bmp" };
		break;
	}

	mVerticalAnimationPresets = {AnimationPreset(EAnimationType_STATIONARY,   verticalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish)};
}


PTargetGatePreset::PTargetGatePreset(int worldNumber) : PGatePreset(worldNumber)
{
	mEntityCharacterType = EEntityCharacterTypes_P_TARGET_GATE;
	mTypeName = "TARGET GATE";

	setUpAnimationPresets(EWorldType(worldNumber));
}

void PTargetGatePreset::setUpAnimationPresets(EWorldType worldType)
{
	int  frameRate = 0;
	bool mustFinish = false;

	mExtraWidth = 25;

	std::vector <std::string> verticalFileNames;
	switch (worldType)
	{
	case EWorldType_EARTH:
		verticalFileNames = { "Platforms/platformTargetGateEarth.bmp" };
		mExtraWidth = 35;
		break;
	case EWorldType_FIRE:
		verticalFileNames = { "Platforms/platformTargetGateFire.bmp" };
		break;
	case EWorldType_WATER:
		verticalFileNames = { "Platforms/platformTargetGateWater.bmp" };
		break;
	}
	
	mVerticalAnimationPresets = { AnimationPreset(EAnimationType_STATIONARY,   verticalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish) };
}


PPressureOperatedGatePreset::PPressureOperatedGatePreset() : PGatePreset(0)
{
	mEntityCharacterType      = EEntityCharacterTypes_P_PRESSURE_OPERATED_GATE;
	mTypeName = "PRESSURE OPERATED GATE";

	mExtraWidth = 25;
	int frameRate = 0;
	bool mustFinish = false;
	mHowToDetermineWidthHeight = EHowToDetermineWidthHeight_USE_WIDTH_AND_HEIGHT_INPUT;

	std::vector <std::string> verticalFileNames = { "Platforms/platformPressureOperatedGate.bmp" };
	mVerticalAnimationPresets = { AnimationPreset(EAnimationType_STATIONARY, verticalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish) };
}



PTargetPreset::PTargetPreset(int worldNumber) : PPlatformPreset()
{
	mEntityCharacterType = EEntityCharacterTypes_P_TARGET;
	mTypeName = "TARGET";

	mVulnerableToProjectiles = true;

	mHitboxEdges =		 HitboxEdges(EEntityEdgeType_NEUTRAL,	   EEntityEdgeType_NEUTRAL,      EEntityEdgeType_NEUTRAL,      EEntityEdgeType_NEUTRAL);
	mHiddenHitboxEdges = HitboxEdges(EEntityEdgeType_NON_EXISTENT, EEntityEdgeType_NON_EXISTENT, EEntityEdgeType_NON_EXISTENT, EEntityEdgeType_NON_EXISTENT);
	
	
	mPrintViaChunk = false;
	mHowToDetermineWidthHeight = EHowToDetermineWidthHeight_GET_BEST_IMAGE_RATIO;
	mExtraWidth  = 10;
	mExtraHeight = 10;

	setUpAnimationPresets(EWorldType(worldNumber));
}

void PTargetPreset::setUpAnimationPresets(EWorldType worldType)
{
	int  frameRate = 0;
	bool mustFinish = false;

	std::vector <std::string> horizontalFileNames;
	switch (worldType)
	{
	case EWorldType_EARTH:
		horizontalFileNames = { "Platforms/platformTargetEarth.bmp" };
		break;
	case EWorldType_FIRE:
		horizontalFileNames = { "Platforms/platformTargetFire.bmp" };
		break;
	case EWorldType_WATER:
	default:
		horizontalFileNames = { "Platforms/platformTargetWater.bmp" };
		break;
	}
	
	mHorizontalAnimationPresets = { AnimationPreset(EAnimationType_STATIONARY,   horizontalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish) };
}



PCratePreset::PCratePreset() : PPlatformPreset()
{
	mEntityCharacterType		= EEntityCharacterTypes_P_CRATE;
	mEntitySubClassType			= EEntitySubClassTypes_CRATE;
	mEntityType					= EEntityType_NON_STATIC;
	mEntityCharacteristicTypes = { EEntityCharacteristicsTypes_CRUSH_RESISTENT, EEntityCharacteristicsTypes_SPIKE_RESISTENT, EEntityCharacteristicsTypes_ELECTRICITY_RESISTENT };
	mTypeName = "CRATE";

	mCharacterMode  = ECharacterModes_MOVING;
	mMovementCodes	= { EEntityMovements_FALL };
	mMovementPath	= EEntityMovementPath_HORIZONTAL_CAN_FALL;
	
	mAutoMoveRule   = EMovementAutoMoveRule_USE_CUR_DIRECTION;

	mCurDirection = EDirection_LEFT;
	mMovementVect2 = Vect2(7, 5);
	mJumpDistance	= 75;

	mVulnerableToProjectiles = true;

	mHitboxEdges = HitboxEdges(EEntityEdgeType_NEUTRAL, EEntityEdgeType_NON_EXISTENT, EEntityEdgeType_MOVEABLE, EEntityEdgeType_MOVEABLE);
	mSpreadEdges = false;
	mSpreadEdgesInterval = 0;
	mImmuneToStatusEffects = false;

	mPrintViaChunk = false;
	mImageOffsetX = 0;
	mImageOffsetY = 0;
	int frameRate	= 0;
	bool mustFinish = false;
	mHowToDetermineWidthHeight = EHowToDetermineWidthHeight_USE_WIDTH_AND_HEIGHT_INPUT;

	std::vector <std::string> horizontalFileNames = { "Platforms/platformCrate.bmp" };
	mHorizontalAnimationPresets = { AnimationPreset(EAnimationType_STATIONARY, horizontalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish) };
}

PMagneticCratePreset::PMagneticCratePreset() : PCratePreset()
{
	mEntityCharacterType		=	EEntityCharacterTypes_P_MAGNETIC_CRATE;
	mEntityCharacteristicTypes	= { EEntityCharacteristicsTypes_CRUSH_RESISTENT, EEntityCharacteristicsTypes_SPIKE_RESISTENT, EEntityCharacteristicsTypes_FIRE_RESISTENT,
									EEntityCharacteristicsTypes_ELECTRICITY_RESISTENT, EEntityCharacteristicsTypes_MAGNETIC };
	mTypeName = "MAGNETIC CRATE";

	mVulnerableToProjectiles = true;

	mHitboxEdges				= HitboxEdges(EEntityEdgeType_MAGNETIC, EEntityEdgeType_NON_EXISTENT, EEntityEdgeType_MOVEABLE, EEntityEdgeType_MOVEABLE);
	
	mImageOffsetX = 0;
	mImageOffsetY = 0;
	int frameRate	= 0;
	bool mustFinish = false;
	mHowToDetermineWidthHeight = EHowToDetermineWidthHeight_USE_WIDTH_AND_HEIGHT_INPUT;

	mHorizontalAnimationPresets.clear();
	std::vector <std::string> horizontalFileNames = { "Platforms/platformMetalCrate.bmp" };
	mHorizontalAnimationPresets = { AnimationPreset(EAnimationType_STATIONARY, horizontalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish) };
}

PArmoredCratePreset::PArmoredCratePreset() : PCratePreset()
{
	mEntityCharacterType      = EEntityCharacterTypes_P_ARMORED_CRATE;
	mEntityCharacteristicTypes = { EEntityCharacteristicsTypes_CRUSH_RESISTENT, EEntityCharacteristicsTypes_SPIKE_RESISTENT, EEntityCharacteristicsTypes_FIRE_RESISTENT };
	mTypeName = "ARMORED CRATE";

	mHitboxEdges = HitboxEdges(EEntityEdgeType_NEUTRAL, EEntityEdgeType_NON_EXISTENT, EEntityEdgeType_MOVEABLE, EEntityEdgeType_MOVEABLE);
	
	mVulnerableToProjectiles = false;

	mImageOffsetX = 0;
	mImageOffsetY = 0;
	int frameRate = 0;
	bool mustFinish = false;
	mHowToDetermineWidthHeight = EHowToDetermineWidthHeight_USE_WIDTH_AND_HEIGHT_INPUT;

	mHorizontalAnimationPresets.clear();
	std::vector <std::string> horizontalFileNames = { "Platforms/platformArmoredCrate.bmp" };
	mHorizontalAnimationPresets = { AnimationPreset(EAnimationType_STATIONARY, horizontalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish) };
}




PPressurePlatePreset::PPressurePlatePreset() : PPlatformPreset()
{
	mEntityCharacterType      = EEntityCharacterTypes_P_PRESSURE_PLATE;
	mTypeName = "PRESSURE PLATE";
	
	mHitboxEdges = HitboxEdges(EEntityEdgeType_WEIGHT_SENSITIVE, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);

	int  frameRate = 0;
	bool mustFinish = false;
	mPrintViaChunk = false;
	mHowToDetermineWidthHeight = EHowToDetermineWidthHeight_USE_WIDTH_AND_HEIGHT_INPUT;

	std::vector <std::string> horizontalFileNames = { "Platforms/platformPressurePlate.bmp" };
	mHorizontalAnimationPresets = { AnimationPreset(EAnimationType_STATIONARY, horizontalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish) };
}



PMovingPreset::PMovingPreset(int worldNumber) : PPlatformPreset()
{
	mEntityCharacterType      = EEntityCharacterTypes_P_MOVING_PLATFORM;
	mEntityType = EEntityType_NON_STATIC;
	mTypeName = "MOVING PLATFORM";

	mCharacterMode = ECharacterModes_MOVING;
	mMovementCodes = { EEntityMovements_FLY };
	mMovementPath = EEntityMovementPath_INVALID;

	mJumpDistance = 0;
	mMovementVect2 = Vect2(4, 5);

	mHitboxEdges = HitboxEdges(EEntityEdgeType_NEUTRAL, EEntityEdgeType_NON_EXISTENT, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);

	mHowToDetermineWidthHeight = EHowToDetermineWidthHeight_CHUNK_IMAGES;
	setUpAnimationPresets(EWorldType(worldNumber));
}

void PMovingPreset::setUpAnimationPresets(EWorldType worldType)
{
	int  frameRate = 0;
	bool mustFinish = false;

	std::vector <std::string> horizontalFileNames;
	std::vector <std::string> verticalFileNames;
	switch (worldType)
	{
	case EWorldType_EARTH:
		horizontalFileNames = { "Platforms/platformStandardEarth.bmp" };
		verticalFileNames = { "Platforms/platformStandardEarthV.bmp" };
		break;
	case EWorldType_FIRE:
		horizontalFileNames = { "Platforms/platformStandardFire.bmp" };
		verticalFileNames = { "Platforms/platformStandardFireV.bmp" };
		break;
	case EWorldType_WATER:
	default:
		horizontalFileNames = { "Platforms/platformStandardWater.bmp" };
		verticalFileNames = { "Platforms/platformStandardWaterV.bmp" };
		break;
	}
	
	mHorizontalAnimationPresets = { AnimationPreset(EAnimationType_STATIONARY, horizontalFileNames,	mImageOffsetX, mImageOffsetY, frameRate, mustFinish) };
	mVerticalAnimationPresets	= { AnimationPreset(EAnimationType_STATIONARY,   verticalFileNames,	mImageOffsetX, mImageOffsetY, frameRate, mustFinish) };
}



PAreaEffectPlatformPreset::PAreaEffectPlatformPreset() : PPlatformPreset()
{
	mEntitySubClassType		= EEntitySubClassTypes_AREA_EFFECT;
}


PWindGustPlatformPreset::PWindGustPlatformPreset() : PAreaEffectPlatformPreset()
{
	mEntityCharacterType    = EEntityCharacterTypes_P_WIND_GUST;
	mTypeName = "WIND GUST";
	
	mHitboxEdges			= HitboxEdges(EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);
	mAreaEffectMovement     = 5;

	setUpAnimationPresets();

	mWidth = -1;
	mHeight = -1;
}

void PWindGustPlatformPreset::setUpAnimationPresets()
{
	if (DEBUG)
	{
		int  frameRate = 0;
		bool mustFinish = false;

		std::vector <std::string> upAnimationFileNames = { "blue.bmp" };
		mUpAnimationPresets = { AnimationPreset(EAnimationType_STATIONARY,	upAnimationFileNames,		mImageOffsetX, mImageOffsetY, frameRate, mustFinish) };

		std::vector <std::string> downAnimationFileNames = { "blue.bmp" };
		mDownAnimationPresets = { AnimationPreset(EAnimationType_STATIONARY,  downAnimationFileNames,	mImageOffsetX, mImageOffsetY, frameRate, mustFinish) };

		std::vector <std::string> leftAnimationFileNames = { "blue.bmp" };
		mLeftAnimationPresets = { AnimationPreset(EAnimationType_STATIONARY,  leftAnimationFileNames,	mImageOffsetX, mImageOffsetY, frameRate, mustFinish) };

		std::vector <std::string> rightAnimationFileNames = { "blue.bmp" };
		mRightAnimationPresets = { AnimationPreset(EAnimationType_STATIONARY, rightAnimationFileNames,	mImageOffsetX, mImageOffsetY, frameRate, mustFinish) };


		frameRate = 10;
		mustFinish = true;

		std::vector <std::string> upAreaEffectFileNames = { "pink.bmp" };
		mAreaEffectUpAnimationPresets = { AnimationPreset(EAnimationType_STATIONARY,	upAreaEffectFileNames,		mImageOffsetX, mImageOffsetY, frameRate, mustFinish) };

		std::vector <std::string> downAreaEffectFileNames = { "pink.bmp" };
		mAreaEffectDownAnimationPresets = { AnimationPreset(EAnimationType_STATIONARY,	downAreaEffectFileNames,	mImageOffsetX, mImageOffsetY, frameRate, mustFinish) };

		std::vector <std::string> leftAreaEffectFileNames = { "pink.bmp" };
		mAreaEffectLeftAnimationPresets = { AnimationPreset(EAnimationType_STATIONARY,	leftAreaEffectFileNames,	mImageOffsetX, mImageOffsetY, frameRate, mustFinish) };

		std::vector <std::string> rightAreaEffectFileNames = { "pink.bmp" };
		mAreaEffectRightAnimationPresets = { AnimationPreset(EAnimationType_STATIONARY,	rightAreaEffectFileNames,	mImageOffsetX, mImageOffsetY, frameRate, mustFinish) };
	}
	
}
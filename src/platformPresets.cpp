#include "platformPresets.h"

PPlatformPreset::PPlatformPreset() : CCharacterPreset()
{
	mEntityClassType		 = EEntityClassTypes_PLATFORM;
	mEntityCharacteristicTypes.push_back(EEntityCharacteristicsTypes_INVINCIBLE);
	mVulnerableToProjectiles = false;
	mRideable                = true;

	mEntityType     = EEntityType_STATIC;
	mCharacterModes.push_back(ECharacterModes_STATIC);
	mMovementCodes  = { EEntityMovements_NONE };
	mMovementPath	= EEntityMovementPath_NONE;
	mMovementEffect = 0;

	mSpreadEdges			= false;
	mSpreadEdgesInterval    = 0;
	mImmuneToStatusEffects  = false;

	mCurDirection = EDirection_NONE;

	mMovementVect2 = Vect2(0, 0);

	mImageOffsetX = 0;
	mImageOffsetY = 0;

	mWidth  = -1;
	mHeight = -1;

	mExtraWidth = 0;
	mExtraHeight = 0;
}



PStandardPreset::PStandardPreset(int worldNumber) : PPlatformPreset()
{

	mEntityCharacterType      = EEntityCharacterTypes_P_STANDARD;
	mHitboxEdges = HitboxEdges(EEntityEdgeType_NEUTRAL, EEntityEdgeType_NON_EXISTENT, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);

	setUpAnimationPresets(EWorldType(worldNumber));

	mTypeName = "STANDARD";

	mWidth = -1;
	mHeight = -1;

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
		horizontalFileNames.push_back(	"Platforms/platformStandardEarth.bmp");
		verticalFileNames.push_back(	"Platforms/platformStandardEarthV.bmp");
		break;
	case EWorldType_FIRE:
		horizontalFileNames.push_back(	"Platforms/platformStandardFire.bmp");
		verticalFileNames.push_back(	"Platforms/platformStandardFireV.bmp");
		break;
	case EWorldType_WATER:
		horizontalFileNames.push_back(	"Platforms/platformStandardWater.bmp");
		verticalFileNames.push_back(	"Platforms/platformStandardWaterV.bmp");
		break;
	case EWorldType_AIR:
	default:
		horizontalFileNames.push_back(	"Platforms/platformStandardAir.bmp");
		verticalFileNames.push_back(	"Platforms/platformStandardAirV.bmp");
		break;
	}
	
	mHorizontalAnimationPresets.push_back(	AnimationPreset(EAnimationType_STATIONARY,		horizontalFileNames,	mImageOffsetX, mImageOffsetY, frameRate, mustFinish));
	mVerticalAnimationPresets.push_back(	AnimationPreset(EAnimationType_STATIONARY,		verticalFileNames,		mImageOffsetX, mImageOffsetY, frameRate, mustFinish));
}



PSolidPreset::PSolidPreset(int worldNumber) : PPlatformPreset()
{
	mEntityCharacterType = EEntityCharacterTypes_P_SOLID;
	mHitboxEdges = HitboxEdges(EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);

	mAnimationPresets.clear();

	setUpAnimationPresets(EWorldType(worldNumber));

	mTypeName = "SOLID";

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
		horizontalFileNames.push_back(	"Platforms/platformSolidEarth.bmp");
		verticalFileNames.push_back(	"Platforms/platformSolidEarthV.bmp");
		break;
	case EWorldType_FIRE:
		horizontalFileNames.push_back(	"Platforms/platformSolidFire.bmp");
		verticalFileNames.push_back(	"Platforms/platformSolidFireV.bmp");
		break;
	case EWorldType_WATER:
	default:
		horizontalFileNames.push_back(	"Platforms/platformSolidWater.bmp");
		verticalFileNames.push_back(	"Platforms/platformSolidWaterV.bmp");
		break;
	}

	mHorizontalAnimationPresets.push_back(	AnimationPreset(EAnimationType_STATIONARY,		horizontalFileNames,	mImageOffsetX, mImageOffsetY, frameRate, mustFinish));
	mVerticalAnimationPresets.push_back(	AnimationPreset(EAnimationType_STATIONARY,		verticalFileNames,		mImageOffsetX, mImageOffsetY, frameRate, mustFinish));
}



PStickyPreset::PStickyPreset() : PPlatformPreset()
{
	mEntityCharacterType      = EEntityCharacterTypes_P_STICKY;
	mHitboxEdges = HitboxEdges(EEntityEdgeType_STICKY, EEntityEdgeType_NON_EXISTENT, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);

	mAnimationPresets.clear();

	int  frameRate = 0;
	bool mustFinish = false;

	std::vector <std::string> horizontalFileNames;
	horizontalFileNames.push_back("Platforms/platformSticky.bmp");
	mHorizontalAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY, horizontalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish));

	mMovementEffect = -4;

	mTypeName = "STICKY";

}



PIcyPreset::PIcyPreset() : PPlatformPreset()
{
	mEntityCharacterType		= EEntityCharacterTypes_P_ICY;
	mHitboxEdges				= HitboxEdges(EEntityEdgeType_ICY, EEntityEdgeType_NON_EXISTENT, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);

	mAnimationPresets.clear();

	int  frameRate = 0;
	bool mustFinish = false;

	std::vector <std::string> horizontalFileNames;
	horizontalFileNames.push_back("Platforms/platformIcy.bmp");
	mHorizontalAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY, horizontalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish));

	mMovementEffect = 2;
	mEntityCharacteristicTypes.clear();
	mEntityCharacteristicTypes.push_back(EEntityCharacteristicsTypes_SLIPPERY);

	mTypeName = "ICY";
}



PMagneticPreset::PMagneticPreset() : PPlatformPreset()
{
	mEntityCharacterType		= EEntityCharacterTypes_P_MAGNETIC;
	mHitboxEdges				= HitboxEdges(EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);

	mAnimationPresets.clear();

	int  frameRate = 0;
	bool mustFinish = false;

	std::vector <std::string> horizontalFileNames;
	horizontalFileNames.push_back("Platforms/platformMetallic.bmp");
	mHorizontalAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY, horizontalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish));

	mEntityCharacteristicTypes.clear();
	mEntityCharacteristicTypes.push_back(EEntityCharacteristicsTypes_MAGNETIC);

	mTypeName = "METALLIC";

}



PCrumblingPreset::PCrumblingPreset() : PPlatformPreset()
{
	mEntityCharacterType = EEntityCharacterTypes_P_CRUMBLING;
	mHitboxEdges =		 HitboxEdges(EEntityEdgeType_CRUMBLING,    EEntityEdgeType_NON_EXISTENT, EEntityEdgeType_NEUTRAL,	   EEntityEdgeType_NEUTRAL);
	mHiddenHitboxEdges = HitboxEdges(EEntityEdgeType_NON_EXISTENT, EEntityEdgeType_NON_EXISTENT, EEntityEdgeType_NON_EXISTENT, EEntityEdgeType_NON_EXISTENT);

	int  frameRate  = 4;
	bool mustFinish = true;

	std::vector <std::string> horizontalFileNames = {"Platforms/platformCrumbling01.bmp", "Platforms/platformCrumbling02.bmp",
		"Platforms/platformCrumbling03.bmp", "Platforms/platformCrumbling04.bmp",
		"Platforms/platformCrumbling05.bmp", "Platforms/platformCrumbling06.bmp",
		"Platforms/platformCrumbling07.bmp", "Platforms/platformCrumbling08.bmp",
		"Platforms/platformCrumbling09.bmp", "Platforms/platformCrumbling10.bmp"};
	mHorizontalAnimationPresets.push_back(AnimationPreset(EAnimationType_PLAY, horizontalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish));

	mTypeName = "CRUMBLING";
}



PSpikePreset::PSpikePreset() : PPlatformPreset()
{
	mEntityCharacterType      = EEntityCharacterTypes_P_SPIKE;
	mHitboxEdges = HitboxEdges(EEntityEdgeType_SPIKEY, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);

	int frameRate = 0;
	bool mustFinish = false;
	mPrintViaChunk = false;
	mHowToDetermineWidthHeight = EHowToDetermineWidthHeight_SPLICE;

	mSplice = true;
	std::vector <std::string> horizontalFileNames;
	horizontalFileNames.push_back("Platforms/platformSpike1.bmp");
	horizontalFileNames.push_back("Platforms/platformSpike2.bmp");
	horizontalFileNames.push_back("Platforms/platformSpike3.bmp");
	mHorizontalAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY, horizontalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish));

	mTypeName = "SPIKE";
}



PLavaPreset::PLavaPreset() : PPlatformPreset()
{
	mEntityCharacterType      = EEntityCharacterTypes_P_LAVA;
	mHitboxEdges = HitboxEdges(EEntityEdgeType_BURNING, EEntityEdgeType_BURNING, EEntityEdgeType_BURNING, EEntityEdgeType_BURNING);

	mAnimationPresets.clear();

	int  frameRate = 0;
	bool mustFinish = false;

	std::vector <std::string> horizontalFileNames;
	horizontalFileNames.push_back("Platforms/platformLava.bmp");
	mHorizontalAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY, horizontalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish));

	std::vector <std::string> verticalFileNames;
	verticalFileNames.push_back("Platforms/platformLavaV.bmp");
	mVerticalAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY, verticalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish));

	mTypeName = "LAVA";
}



PElectricPreset::PElectricPreset() : PPlatformPreset()
{
	mEntityCharacterType      = EEntityCharacterTypes_P_ELECTRIC;
	mHitboxEdges = HitboxEdges(EEntityEdgeType_ELECTRIC, EEntityEdgeType_ELECTRIC, EEntityEdgeType_ELECTRIC, EEntityEdgeType_ELECTRIC);

	mSpreadEdges = true;
	mSpreadEdgesInterval = 15;

	mAnimationPresets.clear();

	int  frameRate = 0;
	bool mustFinish = false;

	std::vector <std::string> horizontalFileNames;
	horizontalFileNames.push_back("Platforms/platformElectric.bmp");
	mHorizontalAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY, horizontalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish));

	std::vector <std::string> verticalFileNames;
	verticalFileNames.push_back("Platforms/platformElectricV.bmp");
	mVerticalAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY, verticalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish));

	mTypeName = "ELECTRIC";
}



PBouncyPreset::PBouncyPreset() : PPlatformPreset()
{
	mEntityCharacterType      = EEntityCharacterTypes_P_BOUNCY;
	mHitboxEdges = HitboxEdges(EEntityEdgeType_BOUNCY, EEntityEdgeType_NON_EXISTENT, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);

	mAnimationPresets.clear();

	int  frameRate = 0;
	bool mustFinish = false;

	std::vector <std::string> horizontalFileNames;
	horizontalFileNames.push_back("Platforms/platformBouncy.bmp");
	mHorizontalAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY, horizontalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish));

	std::vector <std::string> verticalFileNames;
	verticalFileNames.push_back("Platforms/platformBouncyV.bmp");
	mVerticalAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY, verticalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish));

	mTypeName = "BOUNCY";
}



PWallJumpablePreset::PWallJumpablePreset() : PPlatformPreset()
{
	mEntityCharacterType      = EEntityCharacterTypes_P_WALL_JUMPABLE;
	mHitboxEdges = HitboxEdges(EEntityEdgeType_NEUTRAL, EEntityEdgeType_NON_EXISTENT, EEntityEdgeType_WALL_JUMPABLE, EEntityEdgeType_WALL_JUMPABLE);

	mAnimationPresets.clear();

	int  frameRate = 0;
	bool mustFinish = false;

	std::vector <std::string> horizontalFileNames;
	horizontalFileNames.push_back("Platforms/platformBouncy.bmp");
	mHorizontalAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY, horizontalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish));

	std::vector <std::string> verticalFileNames;
	verticalFileNames.push_back("Platforms/platformBouncyV.bmp");
	mVerticalAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY, verticalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish));

	mTypeName = "WALL JUMPABLE";
}



PGatePreset::PGatePreset(int worldNumber) : PPlatformPreset()
{
	mEntityCharacterType	= EEntityCharacterTypes_P_GATE;
	mHitboxEdges			= HitboxEdges(EEntityEdgeType_NEUTRAL,	   EEntityEdgeType_NEUTRAL,		 EEntityEdgeType_NEUTRAL,	   EEntityEdgeType_NEUTRAL);
	mHiddenHitboxEdges		= HitboxEdges(EEntityEdgeType_NON_EXISTENT, EEntityEdgeType_NON_EXISTENT, EEntityEdgeType_NON_EXISTENT, EEntityEdgeType_NON_EXISTENT);
	mPrintViaChunk			= false;

	mHowToDetermineWidthHeight = EHowToDetermineWidthHeight_USE_WIDTH_AND_HEIGHT_INPUT;

	setUpAnimationPresets(EWorldType(worldNumber));

	mTypeName = "GATE";
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
		verticalFileNames.push_back("Platforms/platformGateEarth.bmp");
		mExtraWidth = 35;
		break;
	case EWorldType_FIRE:
		verticalFileNames.push_back("Platforms/platformGateFire.bmp");
		break;
	case EWorldType_WATER:
	default:
		verticalFileNames.push_back("Platforms/platformGateWater.bmp");
		break;
	}

	mVerticalAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY,   verticalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish));
}


PTargetGatePreset::PTargetGatePreset(int worldNumber) : PGatePreset(worldNumber)
{
	mEntityCharacterType      = EEntityCharacterTypes_P_TARGET_GATE;

	setUpAnimationPresets(EWorldType(worldNumber));

	mTypeName = "TARGET GATE";
}

void PTargetGatePreset::setUpAnimationPresets(EWorldType worldType)
{
	int  frameRate = 0;
	bool mustFinish = false;

	mVerticalAnimationPresets.clear();
	mExtraWidth = 25;

	std::vector <std::string> verticalFileNames;
	switch (worldType)
	{
	case EWorldType_EARTH:
		verticalFileNames.push_back("Platforms/platformTargetGateEarth.bmp");
		mExtraWidth = 35;
		break;
	case EWorldType_FIRE:
		verticalFileNames.push_back("Platforms/platformTargetGateFire.bmp");
		break;
	case EWorldType_WATER:
		verticalFileNames.push_back("Platforms/platformTargetGateWater.bmp");
		break;
	}
	
	mVerticalAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY,   verticalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish));
}


PPressureOperatedGatePreset::PPressureOperatedGatePreset() : PGatePreset(0)
{
	mEntityCharacterType      = EEntityCharacterTypes_P_PRESSURE_OPERATED_GATE;

	mExtraWidth = 25;

	int frameRate = 0;
	bool mustFinish = false;
	mHowToDetermineWidthHeight = EHowToDetermineWidthHeight_USE_WIDTH_AND_HEIGHT_INPUT;

	mVerticalAnimationPresets.clear();
	std::vector <std::string> verticalFileNames;
	verticalFileNames.push_back("Platforms/platformPressureOperatedGate.bmp");
	mVerticalAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY, verticalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish));

	mTypeName = "PRESSURE OPERATED GATE";
}



PTargetPreset::PTargetPreset(int worldNumber) : PPlatformPreset()
{
	mEntityCharacterType      = EEntityCharacterTypes_P_TARGET;
	mHitboxEdges =		 HitboxEdges(EEntityEdgeType_NEUTRAL,	   EEntityEdgeType_NEUTRAL,      EEntityEdgeType_NEUTRAL,      EEntityEdgeType_NEUTRAL);
	mHiddenHitboxEdges = HitboxEdges(EEntityEdgeType_NON_EXISTENT, EEntityEdgeType_NON_EXISTENT, EEntityEdgeType_NON_EXISTENT, EEntityEdgeType_NON_EXISTENT);
	mVulnerableToProjectiles = true;
	mPrintViaChunk = false;

	mHowToDetermineWidthHeight = EHowToDetermineWidthHeight_GET_BEST_IMAGE_RATIO;

	mExtraWidth  = 10;
	mExtraHeight = 10;

	setUpAnimationPresets(EWorldType(worldNumber));

	mTypeName = "TARGET";
}

void PTargetPreset::setUpAnimationPresets(EWorldType worldType)
{
	int  frameRate = 0;
	bool mustFinish = false;

	std::vector <std::string> horizontalFileNames;
	switch (worldType)
	{
	case EWorldType_EARTH:
		horizontalFileNames.push_back("Platforms/platformTargetEarth.bmp");
		break;
	case EWorldType_FIRE:
		horizontalFileNames.push_back("Platforms/platformTargetFire.bmp");
		break;
	case EWorldType_WATER:
	default:
		horizontalFileNames.push_back("Platforms/platformTargetWater.bmp");
		break;
	}
	
	mHorizontalAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY,   horizontalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish));
}



PCratePreset::PCratePreset() : PPlatformPreset()
{
	mEntityCharacterType		= EEntityCharacterTypes_P_CRATE;
	mEntitySubClassType			= EEntitySubClassTypes_CRATE;
	mEntityCharacteristicTypes.clear();
	mEntityCharacteristicTypes.push_back(EEntityCharacteristicsTypes_CRUSH_RESISTENT);
	mEntityCharacteristicTypes.push_back(EEntityCharacteristicsTypes_SPIKE_RESISTENT);
	mEntityCharacteristicTypes.push_back(EEntityCharacteristicsTypes_ELECTRICITY_RESISTENT);
	mHitboxEdges				= HitboxEdges(EEntityEdgeType_NEUTRAL, EEntityEdgeType_NON_EXISTENT, EEntityEdgeType_MOVEABLE, EEntityEdgeType_MOVEABLE);
	mVulnerableToProjectiles	= true;
	mPrintViaChunk				= false;

	mSpreadEdges			= false;
	mSpreadEdgesInterval    = 0;
	mImmuneToStatusEffects  = false;

	mEntityType		= EEntityType_NON_STATIC;
	mCharacterModes.clear();
	mCharacterModes.push_back(ECharacterModes_MOVING);
	mMovementCodes	= { EEntityMovements_FALL };
	mMovementPath	= EEntityMovementPath_HORIZONTAL_CAN_FALL;
	mJumpDistance	= 75;
	mAutoMoveRule = EMovementAutoMoveRule_USE_CUR_DIRECTION;

	mCurDirection = EDirection_LEFT;

	mMovementVect2 = Vect2(7, 5);

	mImageOffsetX = 0;
	mImageOffsetY = 0;
	int frameRate	= 0;
	bool mustFinish = false;
	mHowToDetermineWidthHeight = EHowToDetermineWidthHeight_USE_WIDTH_AND_HEIGHT_INPUT;

	std::vector <std::string> horizontalFileNames;
	horizontalFileNames.push_back("Platforms/platformCrate.bmp");
	mHorizontalAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY, horizontalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish));

	mTypeName = "CRATE";
}


PMagneticCratePreset::PMagneticCratePreset() : PCratePreset()
{
	mEntityCharacterType      = EEntityCharacterTypes_P_MAGNETIC_CRATE;
	mEntityCharacteristicTypes.clear();
	mEntityCharacteristicTypes.push_back(EEntityCharacteristicsTypes_CRUSH_RESISTENT);
	mEntityCharacteristicTypes.push_back(EEntityCharacteristicsTypes_SPIKE_RESISTENT);
	mEntityCharacteristicTypes.push_back(EEntityCharacteristicsTypes_FIRE_RESISTENT);
	mEntityCharacteristicTypes.push_back(EEntityCharacteristicsTypes_ELECTRICITY_RESISTENT);
	mEntityCharacteristicTypes.push_back(EEntityCharacteristicsTypes_MAGNETIC);
	mHitboxEdges = HitboxEdges(EEntityEdgeType_MAGNETIC, EEntityEdgeType_NON_EXISTENT, EEntityEdgeType_MOVEABLE, EEntityEdgeType_MOVEABLE);
	mVulnerableToProjectiles = true;

	mImageOffsetX = 0;
	mImageOffsetY = 0;
	int frameRate	= 0;
	bool mustFinish = false;
	mHowToDetermineWidthHeight = EHowToDetermineWidthHeight_USE_WIDTH_AND_HEIGHT_INPUT;

	mHorizontalAnimationPresets.clear();
	std::vector <std::string> horizontalFileNames;
	horizontalFileNames.push_back("Platforms/platformMetalCrate.bmp");
	mHorizontalAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY, horizontalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish));

	mTypeName = "MAGNETIC CRATE";
}


PArmoredCratePreset::PArmoredCratePreset() : PCratePreset()
{
	mEntityCharacterType      = EEntityCharacterTypes_P_ARMORED_CRATE;
	mEntityCharacteristicTypes.clear();
	mEntityCharacteristicTypes.push_back(EEntityCharacteristicsTypes_CRUSH_RESISTENT);
	mEntityCharacteristicTypes.push_back(EEntityCharacteristicsTypes_SPIKE_RESISTENT);
	mEntityCharacteristicTypes.push_back(EEntityCharacteristicsTypes_FIRE_RESISTENT);
	mHitboxEdges = HitboxEdges(EEntityEdgeType_NEUTRAL, EEntityEdgeType_NON_EXISTENT, EEntityEdgeType_MOVEABLE, EEntityEdgeType_MOVEABLE);
	mVulnerableToProjectiles = false;

	mImageOffsetX = 0;
	mImageOffsetY = 0;
	int frameRate = 0;
	bool mustFinish = false;
	mHowToDetermineWidthHeight = EHowToDetermineWidthHeight_USE_WIDTH_AND_HEIGHT_INPUT;

	mHorizontalAnimationPresets.clear();
	std::vector <std::string> horizontalFileNames;
	horizontalFileNames.push_back("Platforms/platformArmoredCrate.bmp");
	mHorizontalAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY, horizontalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish));

	mTypeName = "ARMORED CRATE";
}




PPressurePlatePreset::PPressurePlatePreset() : PPlatformPreset()
{
	mEntityCharacterType      = EEntityCharacterTypes_P_PRESSURE_PLATE;
	mHitboxEdges = HitboxEdges(EEntityEdgeType_WEIGHT_SENSITIVE, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);


	int  frameRate = 0;
	bool mustFinish = false;
	mPrintViaChunk = false;
	mHowToDetermineWidthHeight = EHowToDetermineWidthHeight_USE_WIDTH_AND_HEIGHT_INPUT;

	std::vector <std::string> horizontalFileNames;
	horizontalFileNames.push_back("Platforms/platformPressurePlate.bmp");
	mHorizontalAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY, horizontalFileNames, mImageOffsetX, mImageOffsetY, frameRate, mustFinish));

	mTypeName = "PRESSURE PLATE";

	mWidth = -1;
	mHeight = -1;

}



PMovingPreset::PMovingPreset(int worldNumber) : PPlatformPreset()
{

	mEntityCharacterType      = EEntityCharacterTypes_P_MOVING_PLATFORM;
	mHitboxEdges = HitboxEdges(EEntityEdgeType_NEUTRAL, EEntityEdgeType_NON_EXISTENT, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);

	mEntityType    = EEntityType_NON_STATIC;
	mCharacterModes.clear();
	mCharacterModes.push_back(ECharacterModes_MOVING);
	mMovementCodes = { EEntityMovements_FLY };
	mMovementPath  = EEntityMovementPath_INVALID;
	mJumpDistance  = 0;

	mMovementVect2 = Vect2(4, 5);

	mHowToDetermineWidthHeight = EHowToDetermineWidthHeight_CHUNK_IMAGES;
	setUpAnimationPresets(EWorldType(worldNumber));

	mTypeName = "MOVING PLATFORM";

	mWidth = -1;
	mHeight = -1;

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
		horizontalFileNames.push_back("Platforms/platformStandardEarth.bmp");
		verticalFileNames.push_back("Platforms/platformStandardEarthV.bmp");
		break;
	case EWorldType_FIRE:
		horizontalFileNames.push_back("Platforms/platformStandardFire.bmp");
		verticalFileNames.push_back("Platforms/platformStandardFireV.bmp");
		break;
	case EWorldType_WATER:
	default:
		horizontalFileNames.push_back("Platforms/platformStandardWater.bmp");
		verticalFileNames.push_back("Platforms/platformStandardWaterV.bmp");
		break;
	}
	
	mHorizontalAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY, horizontalFileNames,	mImageOffsetX, mImageOffsetY, frameRate, mustFinish));
	mVerticalAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY,   verticalFileNames,		mImageOffsetX, mImageOffsetY, frameRate, mustFinish));
}



PAreaEffectPlatformPreset::PAreaEffectPlatformPreset() : PPlatformPreset()
{
	mEntitySubClassType		= EEntitySubClassTypes_AREA_EFFECT;
}


PWindGustPlatformPreset::PWindGustPlatformPreset() : PAreaEffectPlatformPreset()
{
	mEntityCharacterType    = EEntityCharacterTypes_P_WIND_GUST;
	mHitboxEdges			= HitboxEdges(EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);
	mAreaEffectMovement     = 5;

	setUpAnimationPresets();

	mTypeName = "WIND GUST";

	mWidth = -1;
	mHeight = -1;

}

void PWindGustPlatformPreset::setUpAnimationPresets()
{
	int  frameRate = 0;
	bool mustFinish = false;

	std::vector <std::string> upAnimationFileNames		= { "blue.bmp" };
	mUpAnimationPresets.push_back(	AnimationPreset(EAnimationType_STATIONARY,	upAnimationFileNames,		mImageOffsetX, mImageOffsetY, frameRate, mustFinish));

	std::vector <std::string> downAnimationFileNames	= { "blue.bmp" };
	mDownAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY,  downAnimationFileNames,		mImageOffsetX, mImageOffsetY, frameRate, mustFinish));

	std::vector <std::string> leftAnimationFileNames	= { "blue.bmp" };
	mLeftAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY,  leftAnimationFileNames,		mImageOffsetX, mImageOffsetY, frameRate, mustFinish));

	std::vector <std::string> rightAnimationFileNames	= { "blue.bmp" };
	mRightAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY, rightAnimationFileNames,	mImageOffsetX, mImageOffsetY, frameRate, mustFinish));


	frameRate = 10;
	mustFinish = true;

	std::vector <std::string> upAreaEffectFileNames		= { "blue.bmp" };
	mAreaEffectUpAnimationPresets.push_back(	AnimationPreset(EAnimationType_STATIONARY,	upAreaEffectFileNames,		mImageOffsetX, mImageOffsetY, frameRate, mustFinish));

	std::vector <std::string> downAreaEffectFileNames	= { "blue.bmp" };
	mAreaEffectDownAnimationPresets.push_back(	AnimationPreset(EAnimationType_STATIONARY,	downAreaEffectFileNames,	mImageOffsetX, mImageOffsetY, frameRate, mustFinish));

	std::vector <std::string> leftAreaEffectFileNames	= { "blue.bmp" };
	mAreaEffectLeftAnimationPresets.push_back(	AnimationPreset(EAnimationType_STATIONARY,	leftAreaEffectFileNames,	mImageOffsetX, mImageOffsetY, frameRate, mustFinish));

	std::vector <std::string> rightAreaEffectFileNames	= { "blue.bmp" };
	mAreaEffectRightAnimationPresets.push_back(	AnimationPreset(EAnimationType_STATIONARY,	rightAreaEffectFileNames,	mImageOffsetX, mImageOffsetY, frameRate, mustFinish));
}
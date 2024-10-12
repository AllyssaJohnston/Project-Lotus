#include "enemyPresets.h"

EEnemyPreset::EEnemyPreset() : CCharacterPreset()
{
	mEntityClassType          = EEntityClassTypes_ENEMY;
	mEntityType               = EEntityType_NON_STATIC;
	mUseMovementEffect		  = false;

	mSpreadEdges			  = false;
	mSpreadEdgesInterval      = 0;
	mImmuneToStatusEffects    = false;
}

//EARTH ENEMIES

//DRAGUAR BLOSSOM
ERatPreset::ERatPreset() : EEnemyPreset()
{
	mEntityCharacterType    = EEntityCharacterTypes_E_RAT;
	mHitboxEdges			= HitboxEdges(EEntityEdgeType_CRUSHABLE, EEntityEdgeType_HAZARDOUS, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);
	mCharacterModes.push_back(ECharacterModes_MOVING);
	mMovementCodes			= {EEntityMovements_WALK};
	mMovementPath           = EEntityMovementPath_HORIZONTAL_CAN_FALL;
	mJumpDistance           = 75;
	mWidth					= 60;
	mHeight					= 25;
	mImageOffsetX           = 0;
	mImageOffsetY           = 5;
	mCurDirection			= EDirection_LEFT;
	mMovementVect2			= Vect2(5, 5);
	mTypeName				= "RAT";
	mRideable               = false; 

	int maxImageWidth       = 90;
	int maxImageHeight      = 90;

	int  runFrameRate		= 15;
	bool runMustFinish		= false;
	std::vector <std::string> animationRunFileNames = { "EarthEnemies/DraguarBlossom/draguarBlossomAnimation01.bmp", "EarthEnemies/DraguarBlossom/draguarBlossomAnimation02.bmp",
		"EarthEnemies/DraguarBlossom/draguarBlossomAnimation03.bmp", "EarthEnemies/DraguarBlossom/draguarBlossomAnimation04.bmp",
		"EarthEnemies/DraguarBlossom/draguarBlossomAnimation05.bmp", "EarthEnemies/DraguarBlossom/draguarBlossomAnimation06.bmp",
		"EarthEnemies/DraguarBlossom/draguarBlossomAnimation07.bmp", "EarthEnemies/DraguarBlossom/draguarBlossomAnimation08.bmp",
		"EarthEnemies/DraguarBlossom/draguarBlossomAnimation09.bmp", "EarthEnemies/DraguarBlossom/draguarBlossomAnimation10.bmp",
		"EarthEnemies/DraguarBlossom/draguarBlossomAnimation11.bmp", "EarthEnemies/DraguarBlossom/draguarBlossomAnimation12.bmp"};
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_RUN, animationRunFileNames, maxImageWidth, maxImageHeight, mImageOffsetX, mImageOffsetY, runFrameRate, runMustFinish));
}

//SEEDLING
EFastRatPreset::EFastRatPreset() : EEnemyPreset()
{
	mEntityCharacterType    = EEntityCharacterTypes_E_FAST_RAT;
	mHitboxEdges			= HitboxEdges(EEntityEdgeType_CRUSHABLE, EEntityEdgeType_HAZARDOUS, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);
	mCharacterModes.push_back(ECharacterModes_MOVING);
	mMovementCodes			= { EEntityMovements_WALK };
	mMovementPath           = EEntityMovementPath_HORIZONTAL_CAN_FALL;
	mJumpDistance           = 75;
	mWidth					= 15;
	mHeight					= 40;
	mImageOffsetX           = 0;
	mImageOffsetY           = 5;
	mCurDirection			= EDirection_LEFT;
	mMovementVect2			= Vect2(6, 6);
	mTypeName				= "FAST RAT";
	mRideable               = false;

	int maxImageWidth       = 70;
	int maxImageHeight      = 70;

	int  runFrameRate		= 20;
	bool runMustFinish		= false;
	std::vector <std::string> animationRunFileNames = {"EarthEnemies/Seedling/seedlingAnimation02.bmp", "EarthEnemies/Seedling/seedlingAnimation01.bmp",
		"EarthEnemies/Seedling/seedlingAnimation02.bmp", "EarthEnemies/Seedling/seedlingAnimation03.bmp",
		"EarthEnemies/Seedling/seedlingAnimation02.bmp", "EarthEnemies/Seedling/seedlingAnimation04.bmp"};

	mAnimationPresets.push_back(AnimationPreset(EAnimationType_RUN, animationRunFileNames, maxImageWidth, maxImageHeight, mImageOffsetX, mImageOffsetY, runFrameRate, runMustFinish));
}

//SHROOM VIPER
EBouncyRatPreset::EBouncyRatPreset() : EEnemyPreset()
{
	mEntityCharacterType    = EEntityCharacterTypes_E_BOUNCY_RAT;
	mHitboxEdges			= HitboxEdges(EEntityEdgeType_BOUNCY, EEntityEdgeType_HAZARDOUS, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);
	mCharacterModes.push_back(ECharacterModes_MOVING);
	mMovementCodes			= { EEntityMovements_WALK };
	mMovementPath           = EEntityMovementPath_HORIZONTAL_CAN_FALL;
	mJumpDistance           = 75;
	mWidth					= 50;
	mHeight					= 25;
	mImageOffsetX           = 0;
	mImageOffsetY		    = 10;
	mCurDirection			= EDirection_LEFT;
	mMovementVect2			= Vect2(4, 4);
	mTypeName				= "BOUNCY RAT";
	mRideable               = false;

	int maxImageWidth       = 80;
	int maxImageHeight      = 80;

	int  runFrameRate		= 15;
	bool runMustFinish		= false;
	std::vector <std::string> animationRunFileNames = { "EarthEnemies/ShroomViper/shroomViperAnimation01.bmp", "EarthEnemies/ShroomViper/shroomViperAnimation02.bmp",
		"EarthEnemies/ShroomViper/shroomViperAnimation04.bmp" };
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_RUN, animationRunFileNames, maxImageWidth, maxImageHeight, mImageOffsetX, mImageOffsetY, runFrameRate, runMustFinish));
}

//FROCODILE
EFrogPreset::EFrogPreset() : EEnemyPreset()
{
	mEntityCharacterType    = EEntityCharacterTypes_E_FROG;
	mHitboxEdges			= HitboxEdges(EEntityEdgeType_CRUSHABLE, EEntityEdgeType_HAZARDOUS, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);
	mCharacterModes.push_back(ECharacterModes_MOVING);
	mMovementCodes			= { EEntityMovements_JUMP };
	mMovementPath           = EEntityMovementPath_HORIZONTAL_CAN_FALL;
	mWidth					= 60;
	mHeight					= 35;
	mImageOffsetX           = 0;
	mImageOffsetY		    = 10;
	mCurDirection			= EDirection_LEFT;
	mMovementVect2			= Vect2(5, 3);
	mJumpDistance			= 60;
	mTypeName				= "FROG";
	mRideable               = false;

	int maxImageWidth       = 100;
	int maxImageHeight      = 100;

	int  stationaryFrameRate = 0;
	bool stationaryMustFinish = false;
	std::vector <std::string> animationStationaryFileNames = { "EarthEnemies/Frocodile/frocodileAnimation01.bmp" };
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY, animationStationaryFileNames, maxImageWidth, maxImageHeight, mImageOffsetX, mImageOffsetY, stationaryFrameRate, stationaryMustFinish));


	int  jumpFrameRate = 10;
	bool jumpMustFinish = false;
	std::vector <std::string> animationJumpFileNames = { "EarthEnemies/Frocodile/frocodileAnimation02.bmp", "EarthEnemies/Frocodile/frocodileAnimation03.bmp" };
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_JUMP, animationJumpFileNames, maxImageWidth, maxImageHeight, mImageOffsetX, mImageOffsetY, jumpFrameRate, jumpMustFinish));

	int  fallFrameRate = 10;
	bool fallMustFinish = false;
	std::vector <std::string> animationFallFileNames = { "EarthEnemies/Frocodile/frocodileAnimation04.bmp", "EarthEnemies/Frocodile/frocodileAnimation05.bmp" };
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_FALL, animationFallFileNames, maxImageWidth, maxImageHeight, mImageOffsetX, mImageOffsetY, fallFrameRate, fallMustFinish));
}

//FROG SHROOM
EBouncyFrogPreset::EBouncyFrogPreset() : EEnemyPreset()
{
	mEntityCharacterType    = EEntityCharacterTypes_E_BOUNCY_FROG;
	mHitboxEdges			= HitboxEdges(EEntityEdgeType_BOUNCY, EEntityEdgeType_HAZARDOUS, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);
	mCharacterModes.push_back(ECharacterModes_MOVING);
	mMovementCodes			= { EEntityMovements_JUMP };
	mMovementPath           = EEntityMovementPath_HORIZONTAL_CAN_FALL;
	mWidth					= 60;
	mHeight					= 35;
	mImageOffsetX           = 0;
	mImageOffsetY		    = 10;
	mCurDirection			= EDirection_LEFT;
	mMovementVect2			= Vect2(5, 3);
	mJumpDistance			= 50;
	mTypeName				= "FROG";
	mRideable               = false;

	int maxImageWidth       = 100;
	int maxImageHeight      = 100;

	int  stationaryFrameRate = 0;
	bool stationaryMustFinish = false;
	std::vector <std::string> animationStationaryFileNames;
	animationStationaryFileNames.push_back("EarthEnemies/FrogShroom/frogShroomAnimation01.bmp");
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY, animationStationaryFileNames, maxImageWidth, maxImageHeight, mImageOffsetX, mImageOffsetY, stationaryFrameRate, stationaryMustFinish));


	int  jumpFrameRate = 12;
	bool jumpMustFinish = false;
	std::vector <std::string> animationJumpFileNames;
	animationJumpFileNames.push_back("EarthEnemies/FrogShroom/frogShroomAnimation02.bmp");
	animationJumpFileNames.push_back("EarthEnemies/FrogShroom/frogShroomAnimation03.bmp");
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_JUMP, animationJumpFileNames, maxImageWidth, maxImageHeight, mImageOffsetX, mImageOffsetY, jumpFrameRate, jumpMustFinish));

	int  fallFrameRate = 12;
	bool fallMustFinish = false;
	std::vector <std::string> animationFallFileNames;
	animationFallFileNames.push_back("EarthEnemies/FrogShroom/frogShroomAnimation04.bmp");
	animationFallFileNames.push_back("EarthEnemies/FrogShroom/frogShroomAnimation05.bmp");
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_FALL, animationFallFileNames, maxImageWidth, maxImageHeight, mImageOffsetX, mImageOffsetY, fallFrameRate, fallMustFinish));
}

//SPIDERIG
EHidingRatPreset::EHidingRatPreset() : EEnemyPreset()
{
	mEntityCharacterType    = EEntityCharacterTypes_E_HIDING_RAT;
	mHitboxEdges			= HitboxEdges(EEntityEdgeType_CRUSHABLE, EEntityEdgeType_HAZARDOUS, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);
	mCharacterModes.push_back(ECharacterModes_MOVING);
	mCharacterModes.push_back(ECharacterModes_STATIONARY);
	mMovementCodes			= { EEntityMovements_WALK };
	mMovementPath           = EEntityMovementPath_HORIZONTAL_CAN_FALL;
	mJumpDistance           = 75;
	mCharacterModeInterval  = 100;
	mWidth					= 60;
	mHeight					= 25;
	mImageOffsetX           = 0;
	mImageOffsetY           = 5;
	mCurDirection			= EDirection_LEFT;
	mMovementVect2			= Vect2(8, 8);
	mTypeName				= "HIDING RAT";
	mRideable               = false;

	int maxImageWidth       = 90;
	int maxImageHeight      = 90;

	int  stationaryFrameRate = 0;
	bool stationaryMustFinish = false;
	std::vector <std::string> animationStationaryFileNames;
	animationStationaryFileNames.push_back("EarthEnemies/Spiderig/spiderigAnimation04.bmp");
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY, animationStationaryFileNames, maxImageWidth, maxImageHeight, mImageOffsetX, mImageOffsetY, stationaryFrameRate, stationaryMustFinish));


	int  runFrameRate = 15;
	bool runMustFinish = false;
	std::vector <std::string> animationRunFileNames;
	animationRunFileNames.push_back("EarthEnemies/Spiderig/spiderigAnimation01.bmp");
	animationRunFileNames.push_back("EarthEnemies/Spiderig/spiderigAnimation02.bmp");
	animationRunFileNames.push_back("EarthEnemies/Spiderig/spiderigAnimation01.bmp");
	animationRunFileNames.push_back("EarthEnemies/Spiderig/spiderigAnimation03.bmp");
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_RUN, animationRunFileNames, maxImageWidth, maxImageHeight, mImageOffsetX, mImageOffsetY, runFrameRate, runMustFinish));
}


//FIRE ENEMIES

//FIRMUS
EFireRatPreset::EFireRatPreset() : EEnemyPreset()
{
	mEntityCharacterType    = EEntityCharacterTypes_E_FIRE_RAT;
	mHitboxEdges			= HitboxEdges(EEntityEdgeType_BURNING, EEntityEdgeType_HAZARDOUS, EEntityEdgeType_BURNING, EEntityEdgeType_BURNING);
	mEntityCharacteristicTypes.clear();
	mEntityCharacteristicTypes.push_back(EEntityCharacteristicsTypes_FIRE_RESISTENT);
	mCharacterModes.push_back(ECharacterModes_MOVING);
	mMovementCodes			= {EEntityMovements_WALK};
	mMovementPath           = EEntityMovementPath_HORIZONTAL_CAN_FALL;
	mJumpDistance           = 75;
	mWidth					= 60;
	mHeight					= 35;
	mImageOffsetX           = 0;
	mImageOffsetY           = 5;
	mCurDirection			= EDirection_LEFT;
	mMovementVect2			= Vect2(5, 5);
	mTypeName				= "FIRE RAT";
	mRideable               = false;

	int maxImageWidth       = 90;
	int maxImageHeight      = 90;

	int  runFrameRate = 22;
	bool runMustFinish = false;
	std::vector <std::string> animationRunFileNames;
	animationRunFileNames.push_back("FireEnemies/Firmus/firmusAnimation01.bmp");
	animationRunFileNames.push_back("FireEnemies/Firmus/firmusAnimation02.bmp");
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_RUN, animationRunFileNames, maxImageWidth, maxImageHeight, mImageOffsetX, mImageOffsetY, runFrameRate, runMustFinish));
}

//FIERLINE
EFireBatVPreset::EFireBatVPreset() : EEnemyPreset()
{
	mEntityCharacterType    = EEntityCharacterTypes_E_FIRE_BAT_V;
	mHitboxEdges			= HitboxEdges(EEntityEdgeType_BURNING, EEntityEdgeType_BURNING, EEntityEdgeType_BURNING, EEntityEdgeType_BURNING);
	mEntityCharacteristicTypes.clear();
	mEntityCharacteristicTypes.push_back(EEntityCharacteristicsTypes_FIRE_RESISTENT);
	mCharacterModes.push_back(ECharacterModes_MOVING);
	mMovementCodes			= {EEntityMovements_FLY};
	mMovementPath           = EEntityMovementPath_VERTICAL;
	mJumpDistance           = 75;
	mWidth					= 50;
	mHeight					= 50;
	mImageOffsetX           = 0;
	mImageOffsetY           = 0;
	mCurDirection			= EDirection_LEFT;
	mMovementVect2			= Vect2(0, 4);
	mTypeName				= "FIRE BAT V";
	mRideable               = false;

	int maxImageWidth       = 160;
	int maxImageHeight      = 160;

	int  flyFrameRate		= 15;
	bool flyMustFinish		= false;
	std::vector <std::string> animationFlyFileNames = {"FireEnemies/Fierline/fierlineAnimation01.bmp", "FireEnemies/Fierline/fierlineAnimation02.bmp",
		"FireEnemies/Fierline/fierlineAnimation03.bmp", "FireEnemies/Fierline/fierlineAnimation04.bmp",
		"FireEnemies/Fierline/fierlineAnimation05.bmp", "FireEnemies/Fierline/fierlineAnimation04.bmp",
		"FireEnemies/Fierline/fierlineAnimation03.bmp", "FireEnemies/Fierline/fierlineAnimation02.bmp" };
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_FLY, animationFlyFileNames, maxImageWidth, maxImageHeight, mImageOffsetX, mImageOffsetY, flyFrameRate, flyMustFinish));
}

//FIRIKIN
EFireFrogPreset::EFireFrogPreset() : EEnemyPreset()
{
	mEntityCharacterType    = EEntityCharacterTypes_E_FIRE_FROG;
	mHitboxEdges			= HitboxEdges(EEntityEdgeType_BURNING, EEntityEdgeType_HAZARDOUS, EEntityEdgeType_BURNING, EEntityEdgeType_BURNING);
	mEntityCharacteristicTypes.clear();
	mEntityCharacteristicTypes.push_back(EEntityCharacteristicsTypes_FIRE_RESISTENT);
	mCharacterModes.push_back(ECharacterModes_STATIONARY);
	mCharacterModes.push_back(ECharacterModes_MOVING);
	mMovementCodes			= {EEntityMovements_JUMP};
	mMovementPath           = EEntityMovementPath_HORIZONTAL_CAN_FALL;
	mJumpDistance           = 30;
	mCharacterModeInterval  = 100;
	mWidth					= 45;
	mHeight					= 40;
	mImageOffsetX           = 0;
	mImageOffsetY           = 5;
	mCurDirection			= EDirection_LEFT;
	mMovementVect2			= Vect2(6, 3);
	mTypeName				= "FIRE FROG";
	mRideable               = false;

	int maxImageWidth       = 120;
	int maxImageHeight      = 100;

	int  stationaryFrameRate = 0;
	bool stationaryMustFinish = false;
	std::vector <std::string> animationStationaryFileNames;
	animationStationaryFileNames.push_back("FireEnemies/Firikin/firikinAnimation01.bmp");
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY, animationStationaryFileNames, maxImageWidth, maxImageHeight, mImageOffsetX, mImageOffsetY, stationaryFrameRate, stationaryMustFinish));


	int  jumpFrameRate = 10;
	bool jumpMustFinish = false;
	std::vector <std::string> animationJumpFileNames;
	animationJumpFileNames.push_back("FireEnemies/Firikin/firikinAnimation02.bmp");
	animationJumpFileNames.push_back("FireEnemies/Firikin/firikinAnimation03.bmp");
	//animationJumpFileNames.push_back("FireEnemies/Firikin/firikinAnimation04.bmp");
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_JUMP, animationJumpFileNames, maxImageWidth, maxImageHeight, mImageOffsetX, mImageOffsetY, jumpFrameRate, jumpMustFinish));

	int  fallFrameRate = 0;
	bool fallMustFinish = false;
	std::vector <std::string> animationFallFileNames;
	animationFallFileNames.push_back("FireEnemies/Firikin/firikinAnimation05.bmp");
	//animationFallFileNames.push_back("FireEnemies/Firikin/firikinAnimation06.bmp");
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_FALL, animationFallFileNames, maxImageWidth, maxImageHeight, mImageOffsetX, mImageOffsetY, fallFrameRate, fallMustFinish));
}

//SCORPUS
ESnailPreset::ESnailPreset() : EEnemyPreset()
{
	mEntityCharacterType    = EEntityCharacterTypes_E_SNAIL;
	mHitboxEdges			= HitboxEdges(EEntityEdgeType_NEUTRAL, EEntityEdgeType_HAZARDOUS, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);
	mEntityCharacteristicTypes.clear();
	mEntityCharacteristicTypes.push_back(EEntityCharacteristicsTypes_CRUSH_RESISTENT);
	mEntityCharacteristicTypes.push_back(EEntityCharacteristicsTypes_SPIKE_RESISTENT);
	mEntityCharacteristicTypes.push_back(EEntityCharacteristicsTypes_FIRE_RESISTENT);
	mCharacterModes.push_back(ECharacterModes_MOVING);
	mMovementCodes			= {EEntityMovements_WALK};
	mMovementPath           = EEntityMovementPath_HORIZONTAL_CAN_FALL;
	mJumpDistance           = 75;
	mWidth					= 60;
	mHeight					= 35;
	mImageOffsetX           = 0;
	mImageOffsetY           = 5;
	mCurDirection			= EDirection_LEFT;
	mMovementVect2			= Vect2(6, 5);
	mTypeName				= "SNAIL";
	mRideable               = true;

	int maxImageWidth       = 160;
	int maxImageHeight      = 160;

	int  runFrameRate		= 15;
	bool runMustFinish		= false;
	std::vector <std::string> animationRunFileNames;
	animationRunFileNames.push_back("FireEnemies/Scorpus/scorpusAnimation01.bmp");
	animationRunFileNames.push_back("FireEnemies/Scorpus/scorpusAnimation02.bmp");
	animationRunFileNames.push_back("FireEnemies/Scorpus/scorpusAnimation03.bmp");
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_RUN, animationRunFileNames, maxImageWidth, maxImageHeight, mImageOffsetX, mImageOffsetY, runFrameRate, runMustFinish));
}

//GEM BUG
ESpikeySnailPreset::ESpikeySnailPreset() : EEnemyPreset()
{
	mEntityCharacterType    = EEntityCharacterTypes_E_SPIKEY_SNAIL;
	mHitboxEdges			= HitboxEdges(EEntityEdgeType_SPIKEY, EEntityEdgeType_HAZARDOUS, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);
	mEntityCharacteristicTypes.clear();
	mEntityCharacteristicTypes.push_back(EEntityCharacteristicsTypes_CRUSH_RESISTENT);
	mEntityCharacteristicTypes.push_back(EEntityCharacteristicsTypes_SPIKE_RESISTENT);
	mEntityCharacteristicTypes.push_back(EEntityCharacteristicsTypes_FIRE_RESISTENT);
	mCharacterModes.push_back(ECharacterModes_MOVING);
	mMovementCodes			= {EEntityMovements_WALK};
	mMovementPath           = EEntityMovementPath_HORIZONTAL_CAN_FALL;
	mJumpDistance           = 75;
	mWidth					= 60;
	mHeight					= 30;
	mImageOffsetX           = 0;
	mImageOffsetY           = 5;
	mCurDirection			= EDirection_LEFT;
	mMovementVect2			= Vect2(5, 5);
	mTypeName				= "SPIKEY SNAIL";
	mRideable               = true;

	int maxImageWidth       = 90;
	int maxImageHeight      = 90;

	int  runFrameRate		= 15;
	bool runMustFinish		= false;
	std::vector <std::string> animationRunFileNames;
	animationRunFileNames.push_back("FireEnemies/GemBug/gemBugAnimation01.bmp");
	animationRunFileNames.push_back("FireEnemies/GemBug/gemBugAnimation02.bmp");
	animationRunFileNames.push_back("FireEnemies/GemBug/gemBugAnimation03.bmp");
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_RUN, animationRunFileNames, maxImageWidth, maxImageHeight, mImageOffsetX, mImageOffsetY, runFrameRate, runMustFinish));
}

//GEM WALKER
ESpikeySnailChameleonPreset::ESpikeySnailChameleonPreset() : EEnemyPreset()
{
	mEntityCharacterType    = EEntityCharacterTypes_E_SPIKEY_SNAIL_CHAMELEON;
	mHitboxEdges			= HitboxEdges(EEntityEdgeType_SPIKEY, EEntityEdgeType_HAZARDOUS, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);
	mEntityCharacteristicTypes.clear();
	mEntityCharacteristicTypes.push_back(EEntityCharacteristicsTypes_CRUSH_RESISTENT);
	mEntityCharacteristicTypes.push_back(EEntityCharacteristicsTypes_SPIKE_RESISTENT);
	mEntityCharacteristicTypes.push_back(EEntityCharacteristicsTypes_FIRE_RESISTENT);
	mStages.push_back(ECharacterModes_STATIC);
	mStages.push_back(ECharacterModes_MOVING);
	mCharacterModes.push_back(mStages[0]);
	mMovementCodes			= {EEntityMovements_WALK};
	mMovementPath           = EEntityMovementPath_HORIZONTAL_CAN_FALL;
	mJumpDistance           = 75;
	mWidth					= 60;
	mHeight					= 60;
	mImageOffsetX           = 0;
	mImageOffsetY           = 5;
	mCurDirection			= EDirection_LEFT;
	mMovementVect2			= Vect2(4, 5);
	mTypeName				= "SPIKEY SNAIL CHAMELEON";
	mRideable               = true;

	int maxImageWidth       = 90;
	int maxImageHeight      = 90;

	int  stationaryFrameRate = 0;
	bool stationaryMustFinish = false;
	std::vector <std::string> animationStationaryFileNames;
	animationStationaryFileNames.push_back("FireEnemies/GemWalker/gemWalkerAnimation03.bmp");
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY, animationStationaryFileNames, maxImageWidth, maxImageHeight, mImageOffsetX, mImageOffsetY, stationaryFrameRate, stationaryMustFinish));


	int  runFrameRate = 15;
	bool runMustFinish = false;
	std::vector <std::string> animationRunFileNames;
	animationRunFileNames.push_back("FireEnemies/GemWalker/gemWalkerAnimation01.bmp");
	animationRunFileNames.push_back("FireEnemies/GemWalker/gemWalkerAnimation02.bmp");
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_RUN, animationRunFileNames, maxImageWidth, maxImageHeight, mImageOffsetX, mImageOffsetY, runFrameRate, runMustFinish));

	int  fallFrameRate = 15;
	bool fallMustFinish = false;
	std::vector <std::string> animationFallFileNames;
	animationFallFileNames.push_back("FireEnemies/GemWalker/gemWalkerAnimation02.bmp");
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_FALL, animationFallFileNames, maxImageWidth, maxImageHeight, mImageOffsetX, mImageOffsetY, fallFrameRate, fallMustFinish));
}



//WATER ENEMIES

//GLATA
EBatHPreset::EBatHPreset() : EEnemyPreset()
{
	mEntityCharacterType    = EEntityCharacterTypes_E_BAT_H;
	mHitboxEdges			= HitboxEdges(EEntityEdgeType_CRUSHABLE, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);
	mEntityCharacteristicTypes.clear();
	mEntityCharacteristicTypes.push_back(EEntityCharacteristicsTypes_ELECTRICITY_RESISTENT);
	mCharacterModes.push_back(ECharacterModes_MOVING);
	mMovementCodes			= {EEntityMovements_FLY};
	mMovementPath           = EEntityMovementPath_HORIZONTAL;
	mJumpDistance           = 75;
	mWidth					= 160;
	mHeight					= 30;
	mImageOffsetX           = 0;
	mImageOffsetY           = 0;
	mCurDirection			= EDirection_LEFT;
	mMovementVect2			= Vect2(4, 0);
	mTypeName				= "BAT H";
	mRideable               = false;

	int maxImageWidth       = 200;
	int maxImageHeight      = 130;

	int  flyFrameRate		= 15;
	bool flyMustFinish		= false;
	std::vector <std::string> animationFlyFileNames = {"WaterEnemies/Glata/GlataAnimation01.bmp", "WaterEnemies/Glata/GlataAnimation02.bmp", "WaterEnemies/Glata/GlataAnimation03.bmp"};
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_FLY, animationFlyFileNames, maxImageWidth, maxImageHeight, mImageOffsetX, mImageOffsetY, flyFrameRate, flyMustFinish));
}

//SERPHISH

EBatVPreset::EBatVPreset() : EEnemyPreset()
{
	mEntityCharacterType    = EEntityCharacterTypes_E_BAT_V;
	mHitboxEdges			= HitboxEdges(EEntityEdgeType_CRUSHABLE, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);
	mEntityCharacteristicTypes.clear();
	mEntityCharacteristicTypes.push_back(EEntityCharacteristicsTypes_ELECTRICITY_RESISTENT);
	mCharacterModes.push_back(ECharacterModes_MOVING);
	mMovementCodes			= {EEntityMovements_FLY};
	mMovementPath           = EEntityMovementPath_VERTICAL;
	mJumpDistance           = 75;
	mWidth					= 30;
	mHeight					= 140;
	mImageOffsetX           = 0;
	mImageOffsetY           = 0;
	mCurDirection			= EDirection_LEFT;
	mMovementVect2			= Vect2(0, 4);
	mTypeName				= "BAT V";
	mRideable               = false;

	int maxImageWidth       = 95;
	int maxImageHeight      = 160;

	int  flyFrameRate		= 15;
	bool flyMustFinish		= false;
	std::vector <std::string> animationFlyFileNames = {"WaterEnemies/Serphish/SerphishAnimation01.bmp", "WaterEnemies/Serphish/SerphishAnimation02.bmp"};
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_FLY, animationFlyFileNames, maxImageWidth, maxImageHeight, mImageOffsetX, mImageOffsetY, flyFrameRate, flyMustFinish));
}

//JELISH
EBatDPreset::EBatDPreset() : EEnemyPreset()
{
	mEntityCharacterType    = EEntityCharacterTypes_E_BAT_D;
	mHitboxEdges			= HitboxEdges(EEntityEdgeType_CRUSHABLE, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);
	mEntityCharacteristicTypes.clear();
	mEntityCharacteristicTypes.push_back(EEntityCharacteristicsTypes_ELECTRICITY_RESISTENT);
	mCharacterModes.push_back(ECharacterModes_MOVING);
	mMovementCodes			= {EEntityMovements_FLY};
	mMovementPath           = EEntityMovementPath_DIAGONAL;
	mJumpDistance           = 75;
	mWidth					= 30;
	mHeight					= 50;
	mImageOffsetX           = 0;
	mImageOffsetY           = 10;
	mCurDirection			= EDirection_LEFT;
	mMovementVect2			= Vect2(4, 4);
	mTypeName				= "BAT H";
	mRideable               = false;

	int maxImageWidth       = 80;
	int maxImageHeight      = 100;

	int  flyFrameRate		= 15;
	bool flyMustFinish		= false;
	std::vector <std::string> animationFlyFileNames = {"WaterEnemies/Jelish/JelishAnimation01.bmp", "WaterEnemies/Jelish/JelishAnimation02.bmp", "WaterEnemies/Jelish/JelishAnimation03.bmp"};
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_FLY, animationFlyFileNames, maxImageWidth, maxImageHeight, mImageOffsetX, mImageOffsetY, flyFrameRate, flyMustFinish));
}

//SHELL BEAST
EHidingSnailPreset::EHidingSnailPreset() : EEnemyPreset()
{
	mEntityCharacterType    = EEntityCharacterTypes_E_HIDING_SNAIL;
	mHitboxEdges			= HitboxEdges(EEntityEdgeType_NEUTRAL, EEntityEdgeType_HAZARDOUS, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);
	mEntityCharacteristicTypes.clear();
	mEntityCharacteristicTypes.push_back(EEntityCharacteristicsTypes_CRUSH_RESISTENT);
	mEntityCharacteristicTypes.push_back(EEntityCharacteristicsTypes_ELECTRICITY_RESISTENT);
	mCharacterModes.push_back(ECharacterModes_MOVING);
	mCharacterModes.push_back(ECharacterModes_STATIONARY);
	mMovementCodes			= {EEntityMovements_WALK};
	mMovementPath           = EEntityMovementPath_HORIZONTAL_CAN_FALL;
	mJumpDistance           = 75;
	mCharacterModeInterval  = 80;
	mWidth					= 80;
	mHeight					= 50;
	mImageOffsetX           = 0;
	mImageOffsetY           = 5;
	mCurDirection			= EDirection_LEFT;
	mMovementVect2			= Vect2(6, 5);
	mTypeName				= "HIDING SNAIL";
	mRideable               = true;

	int maxImageWidth       = 110;
	int maxImageHeight      = 110;

	int  stationaryFrameRate = 0;
	bool stationaryMustFinish = false;
	std::vector <std::string> animationStationaryFileNames;
	animationStationaryFileNames.push_back("WaterEnemies/ShellBeast/ShellBeastAnimation01.bmp");
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY, animationStationaryFileNames, maxImageWidth, maxImageHeight, mImageOffsetX, mImageOffsetY, stationaryFrameRate, stationaryMustFinish));

	maxImageWidth       = 180;
	maxImageHeight      = 170;

	int  runFrameRate = 15;
	bool runMustFinish = false;
	std::vector <std::string> animationRunFileNames;
	animationRunFileNames.push_back("WaterEnemies/ShellBeast/ShellBeastAnimation02.bmp");
	animationRunFileNames.push_back("WaterEnemies/ShellBeast/ShellBeastAnimation03.bmp");
	animationRunFileNames.push_back("WaterEnemies/ShellBeast/ShellBeastAnimation04.bmp");
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_RUN, animationRunFileNames, maxImageWidth, maxImageHeight, mImageOffsetX, mImageOffsetY, runFrameRate, runMustFinish));
}

//SPINE SHELL
EGrabbingSnailPreset::EGrabbingSnailPreset() : EEnemyPreset()
{
	mEntityCharacterType    = EEntityCharacterTypes_E_GRABBING_SNAIL;
	mHitboxEdges			= HitboxEdges(EEntityEdgeType_NEUTRAL, EEntityEdgeType_HAZARDOUS, EEntityEdgeType_GRABBING, EEntityEdgeType_GRABBING);
	mEntityCharacteristicTypes.clear();
	mEntityCharacteristicTypes.push_back(EEntityCharacteristicsTypes_CRUSH_RESISTENT);
	mEntityCharacteristicTypes.push_back(EEntityCharacteristicsTypes_ELECTRICITY_RESISTENT);
	mCharacterModes.push_back(ECharacterModes_MOVING);
	mMovementCodes			= {EEntityMovements_WALK};
	mMovementPath           = EEntityMovementPath_HORIZONTAL_CAN_FALL;
	mJumpDistance           = 75;
	mWidth					= 90;
	mHeight					= 30;
	mImageOffsetX           = 0;
	mImageOffsetY           = 5;
	mCurDirection			= EDirection_LEFT;
	mMovementVect2			= Vect2(3, 5);
	mTypeName				= "GRABBING SNAIL";
	mRideable               = true;
	mHasAttachmentPoint		= true;
	mAttachmentPoint		= Vect2(35, 0);

	int maxImageWidth       = 170;
	int maxImageHeight      = 100;

	int  runFrameRate = 12;
	bool runMustFinish = false;
	std::vector <std::string> animationRunFileNames;
	animationRunFileNames.push_back("WaterEnemies/SpineShell/SpineShellAnimation01.bmp");
	animationRunFileNames.push_back("WaterEnemies/SpineShell/SpineShellAnimation02.bmp");
	animationRunFileNames.push_back("WaterEnemies/SpineShell/SpineShellAnimation03.bmp");
	animationRunFileNames.push_back("WaterEnemies/SpineShell/SpineShellAnimation04.bmp");
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_RUN, animationRunFileNames, maxImageWidth, maxImageHeight, mImageOffsetX, mImageOffsetY, runFrameRate, runMustFinish));

	int  grabbingFrameRate = 8;
	bool grabbingMustFinish = true;
	std::vector <std::string> animationGrabbingFileNames;
	animationGrabbingFileNames.push_back("WaterEnemies/SpineShell/SpineShellAnimation01.bmp");
	animationGrabbingFileNames.push_back("WaterEnemies/SpineShell/SpineShellAnimation05.bmp");
	animationGrabbingFileNames.push_back("WaterEnemies/SpineShell/SpineShellAnimation06.bmp");
	animationGrabbingFileNames.push_back("WaterEnemies/SpineShell/SpineShellAnimation07.bmp");
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_GRABBING, animationGrabbingFileNames, maxImageWidth, maxImageHeight, mImageOffsetX, mImageOffsetY, grabbingFrameRate, grabbingMustFinish));
}


//AIR ENEMIES

//MOTHA
EWalkingFrogPreset::EWalkingFrogPreset() : EEnemyPreset()
{
	mEntityCharacterType    = EEntityCharacterTypes_E_WALKING_FROG;
	mHitboxEdges			= HitboxEdges(EEntityEdgeType_CRUSHABLE, EEntityEdgeType_HAZARDOUS, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);
	mCharacterModes.push_back(ECharacterModes_MOVING);
	mMovementCodes		    = { EEntityMovements_WALK, EEntityMovements_JUMP };
	mMovementPath           = EEntityMovementPath_HORIZONTAL_CAN_FALL;
	mMovementCodeInterval   = 80;
	mWidth					= 30;
	mHeight					= 20;
	mImageOffsetX           = 0;
	mImageOffsetY		    = 10;
	mCurDirection			= EDirection_LEFT;
	mMovementVect2			= Vect2(4, 4);
	mJumpDistance			= 60;
	mTypeName				= "WALKING FROG";
	mRideable               = false;

	int maxImageWidth       = 60;
	int maxImageHeight      = 60;

	int  stationaryFrameRate = 0;
	bool stationaryMustFinish = false;
	std::vector <std::string> animationStationaryFileNames = { "AirEnemies/Motha/mothaAnimation01.bmp" };
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY, animationStationaryFileNames, maxImageWidth, maxImageHeight, mImageOffsetX, mImageOffsetY, stationaryFrameRate, stationaryMustFinish));

	int  walkFrameRate = 10;
	bool walkMustFinish = false;
	std::vector <std::string> animationWalkFileNames = { "AirEnemies/Motha/mothaAnimation01.bmp", "AirEnemies/Motha/mothaAnimation02.bmp", "AirEnemies/Motha/mothaAnimation03.bmp" };
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_RUN, animationWalkFileNames, maxImageWidth, maxImageHeight, mImageOffsetX, mImageOffsetY, walkFrameRate, walkMustFinish));


	int  jumpFrameRate = 10;
	bool jumpMustFinish = false;
	//std::vector <std::string> animationJumpFileNames = { "AirEnemies/Motha/mothaAnimation04.bmp" };
	std::vector <std::string> animationJumpFileNames = { "AirEnemies/Motha/mothaAnimation05.bmp", "AirEnemies/Motha/mothaAnimation06.bmp" };
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_JUMP, animationJumpFileNames, maxImageWidth, maxImageHeight, mImageOffsetX, mImageOffsetY, jumpFrameRate, jumpMustFinish));

	int  fallFrameRate = 10;
	bool fallMustFinish = false;
	//std::vector <std::string> animationFallFileNames = { "AirEnemies/Motha/mothaAnimation07.bmp" };
	std::vector <std::string> animationFallFileNames = { "AirEnemies/Motha/mothaAnimation08.bmp", "AirEnemies/Motha/mothaAnimation09.bmp" };
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_FALL, animationFallFileNames, maxImageWidth, maxImageHeight, mImageOffsetX, mImageOffsetY, fallFrameRate, fallMustFinish));
}

//SWOOPER
ESlowBatHPreset::ESlowBatHPreset() : EEnemyPreset()
{
	mEntityCharacterType    = EEntityCharacterTypes_E_BAT_H;
	mHitboxEdges			= HitboxEdges(EEntityEdgeType_CRUSHABLE, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);
	mEntityCharacteristicTypes.clear();
	mEntityCharacteristicTypes.push_back(EEntityCharacteristicsTypes_ELECTRICITY_RESISTENT);
	mCharacterModes.push_back(ECharacterModes_MOVING);
	mMovementCodes			= {EEntityMovements_FLY};
	mMovementPath           = EEntityMovementPath_HORIZONTAL;
	mJumpDistance           = 75;
	mWidth					= 80;
	mHeight					= 20;
	mImageOffsetX           = 0;
	mImageOffsetY           = 0;
	mCurDirection			= EDirection_LEFT;
	mMovementVect2			= Vect2(2, 0);
	mTypeName				= "SLOW BAT H";
	mRideable               = false;

	int maxImageWidth       = 180;
	int maxImageHeight      = 180;

	int  flyFrameRate		= 8;
	bool flyMustFinish		= false;
	std::vector <std::string> animationFlyFileNames =	{"AirEnemies/Swooper/SwooperAnimation01.bmp", "AirEnemies/Swooper/SwooperAnimation02.bmp", "AirEnemies/Swooper/SwooperAnimation03.bmp",
		"AirEnemies/Swooper/SwooperAnimation04.bmp", "AirEnemies/Swooper/SwooperAnimation05.bmp", "AirEnemies/Swooper/SwooperAnimation06.bmp",
		"AirEnemies/Swooper/SwooperAnimation07.bmp", "AirEnemies/Swooper/SwooperAnimation08.bmp", "AirEnemies/Swooper/SwooperAnimation09.bmp",
		"AirEnemies/Swooper/SwooperAnimation10.bmp", "AirEnemies/Swooper/SwooperAnimation11.bmp", "AirEnemies/Swooper/SwooperAnimation12.bmp",
		"AirEnemies/Swooper/SwooperAnimation13.bmp", "AirEnemies/Swooper/SwooperAnimation12.bmp", "AirEnemies/Swooper/SwooperAnimation11.bmp",
		"AirEnemies/Swooper/SwooperAnimation10.bmp", "AirEnemies/Swooper/SwooperAnimation09.bmp", "AirEnemies/Swooper/SwooperAnimation08.bmp",
		"AirEnemies/Swooper/SwooperAnimation07.bmp", "AirEnemies/Swooper/SwooperAnimation06.bmp", "AirEnemies/Swooper/SwooperAnimation05.bmp",
		"AirEnemies/Swooper/SwooperAnimation04.bmp", "AirEnemies/Swooper/SwooperAnimation03.bmp", "AirEnemies/Swooper/SwooperAnimation02.bmp"};
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_FLY, animationFlyFileNames, maxImageWidth, maxImageHeight, mImageOffsetX, mImageOffsetY, flyFrameRate, flyMustFinish));
}

//FELIGA
EProjectileBatVPreset::EProjectileBatVPreset() : EEnemyPreset()
{
	mEntityCharacterType    = EEntityCharacterTypes_E_BAT_V;
	mHitboxEdges			= HitboxEdges(EEntityEdgeType_CRUSHABLE, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);
	mEntityCharacteristicTypes.clear();
	mEntityCharacteristicTypes.push_back(EEntityCharacteristicsTypes_ELECTRICITY_RESISTENT);
	mCharacterModes.push_back(ECharacterModes_MOVING);
	mMovementCodes			= {EEntityMovements_FLY};
	mMovementPath           = EEntityMovementPath_VERTICAL;
	mProjectilePath         = EEntityMovementPath_HORIZONTAL;
	mProjectileInterval     = 90;
	mJumpDistance           = 75;
	mWidth					= 45;
	mHeight					= 70;
	mImageOffsetX           = 0;
	mImageOffsetY           = 0;
	mCurDirection			= EDirection_LEFT;
	mMovementVect2			= Vect2(0, 4);
	mTypeName				= "PROJECTILE BAT V";
	mRideable               = false;

	int maxImageWidth       = 70;
	int maxImageHeight      = 250;

	int  flyFrameRate		= 15;
	bool flyMustFinish		= false;
	std::vector <std::string> animationFlyFileNames = {"AirEnemies/Feliga/FeligaAnimation01.bmp", "AirEnemies/Feliga/FeligaAnimation02.bmp", "AirEnemies/Feliga/FeligaAnimation03.bmp"};
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_FLY, animationFlyFileNames, maxImageWidth, maxImageHeight, mImageOffsetX, mImageOffsetY, flyFrameRate, flyMustFinish));

	int  projectileFrameRate  = 15;
	bool projectileMustFinish = true;
	std::vector <std::string> animationProjectileFileNames = {"AirEnemies/Feliga/FeligaAnimation04.bmp"};
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_SHOOTING_PROJECTILE, animationProjectileFileNames, maxImageWidth, maxImageHeight, mImageOffsetX, mImageOffsetY, projectileFrameRate, projectileMustFinish));

}

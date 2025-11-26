#include "playerPreset.h"

CPlayerPreset::CPlayerPreset() : CCharacterPreset()
{
	mHitboxEdges			= HitboxEdges(EEntityEdgeType_NEUTRAL, EEntityEdgeType_HAZARDOUS, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);
	mEntityType             = EEntityType_NON_STATIC;
	mCharacterModes.push_back(ECharacterModes_MOVING);

	mMovementCodes			= { EEntityMovements_INPUT };
	mMovementPath           = EEntityMovementPath_HORIZONTAL_CAN_FALL;

	mWidth					= 30;
	mHeight					= 70;

	mCurDirection			= EDirection_LEFT;

	mMovementVect2			= Vect2(8, 5);
	mJumpDistance		    = 75;
	mWallJumpDistanceX	    = 35;
	mWallJumpDistanceY	    = 75;

	mSpreadEdges			= false;
	mSpreadEdgesInterval    = 0;
	mImmuneToStatusEffects  = false;

	mTypeName				= "PLAYER";

	int imageOffsetX		= 0;
	int imageOffsetY		= 0;
	int maxImageWidth       = 125;
	int maxImageHeight      = 100;

	int  standardStandFrameRate		= 7;
	bool standardStandMustFinish		= false;
	std::vector <std::vector<std::string>> mStandOutfits;

	std::vector <std::string> standardAnimationStandFileNames;
	standardAnimationStandFileNames.push_back("Lotus/LotusStanding.bmp");

	std::vector <std::string> bWAnimationStandFileNames;
	bWAnimationStandFileNames.push_back("Lotus/LotusStandingBW.bmp");

	mStandOutfits.push_back(standardAnimationStandFileNames);
	mStandOutfits.push_back(bWAnimationStandFileNames);
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY, mStandOutfits, maxImageWidth, maxImageHeight, imageOffsetX, imageOffsetY, standardStandFrameRate, standardStandMustFinish));


	int  standardRunFrameRate = 7;
	bool standardRunMustFinish = false;
	std::vector <std::vector<std::string>> mRunOutfits;

	std::vector <std::string> standardAnimationRunFileNames;
	standardAnimationRunFileNames.push_back("Lotus/LotusWalk.bmp");

	std::vector <std::string> bWAnimationRunFileNames;
	bWAnimationRunFileNames.push_back("Lotus/LotusWalkBW.bmp");

	mRunOutfits.push_back(standardAnimationRunFileNames);
	mRunOutfits.push_back(bWAnimationRunFileNames);
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_RUN,  mRunOutfits,  maxImageWidth, maxImageHeight, imageOffsetX, imageOffsetY, standardRunFrameRate, standardRunMustFinish));



	int  standardJumpFrameRate = 12;
	bool standardJumpMustFinish = false;
	std::vector <std::vector<std::string>> mJumpOutfits;

	std::vector <std::string> standardAnimationJumpFileNames;
	standardAnimationJumpFileNames.push_back("Lotus/LotusJump.bmp");

	std::vector <std::string> bWAnimationJumpFileNames;
	bWAnimationJumpFileNames.push_back("Lotus/LotusJumpBW.bmp");

	mJumpOutfits.push_back(standardAnimationJumpFileNames);
	mJumpOutfits.push_back(bWAnimationJumpFileNames);
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_JUMP, mJumpOutfits, maxImageWidth, maxImageHeight, imageOffsetX, imageOffsetY, standardJumpFrameRate, standardJumpMustFinish));


	int  standardFallFrameRate = 12;
	bool standardFallMustFinish = false;
	std::vector <std::vector<std::string>> mFallOutfits;


	std::vector <std::string> standardAnimationFallFileNames;
	standardAnimationFallFileNames.push_back("Lotus/LotusFall.bmp");

	std::vector <std::string> bWAnimationFallFileNames;
	bWAnimationFallFileNames.push_back("Lotus/LotusFallBW.bmp");

	mFallOutfits.push_back(standardAnimationFallFileNames);
	mFallOutfits.push_back(bWAnimationFallFileNames);
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_FALL, mFallOutfits, maxImageWidth, maxImageHeight, imageOffsetX, imageOffsetY, standardFallFrameRate, standardFallMustFinish));


	int  standardWallGripFrameRate = 7;
	bool standardWallGripMustFinish = false;
	std::vector <std::vector<std::string>> mWallGripOutfits;

	std::vector <std::string> standardAnimationWallGripFileNames;
	standardAnimationWallGripFileNames.push_back("Lotus/LotusWallGrip.bmp");

	std::vector <std::string> bWAnimationWallGripFileNames;
	bWAnimationWallGripFileNames.push_back("Lotus/LotusWallGripBW.bmp");

	mWallGripOutfits.push_back(standardAnimationWallGripFileNames);
	mWallGripOutfits.push_back(bWAnimationWallGripFileNames);
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_WALL_GRIP, mWallGripOutfits, maxImageWidth, maxImageHeight, imageOffsetX, imageOffsetY, standardWallGripFrameRate, standardWallGripMustFinish));


}
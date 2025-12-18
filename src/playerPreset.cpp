#include "playerPreset.h"

CPlayerPreset::CPlayerPreset() : EntityPreset()
{
	mHitboxEdges			= HitboxEdges(EEntityEdgeType_NEUTRAL, EEntityEdgeType_HAZARDOUS, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);
	mEntityType             = EEntityType_NON_STATIC;
	mCharacterMode			= ECharacterModes_MOVING;

	mMovementCodes			= { EEntityMovements_INPUT };
	mMovementPath           = EEntityMovementPath_HORIZONTAL_CAN_FALL;

	mWidth					= 30;
	mHeight					= 70;

	mCurDirection			= EDirection_LEFT;

	mMovementVect2			= Vect2(8, 5);
	mJumpDistance		    = 75;

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
	std::vector <std::string> standardAnimationStandFileNames	= { "Lotus/LotusStanding.bmp" };
	std::vector <std::string> bWAnimationStandFileNames			= { "Lotus/LotusStandingBW.bmp" };
	std::vector <std::vector<std::string>> mStandOutfits = { standardAnimationStandFileNames, bWAnimationStandFileNames };
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY, mStandOutfits, maxImageWidth, maxImageHeight, imageOffsetX, imageOffsetY, standardStandFrameRate, standardStandMustFinish));


	int  standardRunFrameRate = 7;
	bool standardRunMustFinish = false;
	std::vector <std::string> standardAnimationRunFileNames = { "Lotus/LotusWalk.bmp" };
	std::vector <std::string> bWAnimationRunFileNames		= { "Lotus/LotusWalkBW.bmp" };
	std::vector <std::vector<std::string>> mRunOutfits = { standardAnimationRunFileNames, bWAnimationRunFileNames };
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_RUN,  mRunOutfits,  maxImageWidth, maxImageHeight, imageOffsetX, imageOffsetY, standardRunFrameRate, standardRunMustFinish));


	int  standardJumpFrameRate = 12;
	bool standardJumpMustFinish = false;
	std::vector <std::string> standardAnimationJumpFileNames	= { "Lotus/LotusJump.bmp" };
	std::vector <std::string> bWAnimationJumpFileNames			= { "Lotus/LotusJumpBW.bmp" };
	std::vector <std::vector<std::string>> mJumpOutfits = { standardAnimationJumpFileNames, bWAnimationJumpFileNames };
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_JUMP, mJumpOutfits, maxImageWidth, maxImageHeight, imageOffsetX, imageOffsetY, standardJumpFrameRate, standardJumpMustFinish));


	int  standardFallFrameRate = 12;
	bool standardFallMustFinish = false;
	std::vector <std::string> standardAnimationFallFileNames	= { "Lotus/LotusFall.bmp" };
	std::vector <std::string> bWAnimationFallFileNames			= { "Lotus/LotusFallBW.bmp" };
	std::vector <std::vector<std::string>> mFallOutfits = { standardAnimationFallFileNames, bWAnimationFallFileNames };
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_FALL, mFallOutfits, maxImageWidth, maxImageHeight, imageOffsetX, imageOffsetY, standardFallFrameRate, standardFallMustFinish));


	int  standardWallGripFrameRate = 7;
	bool standardWallGripMustFinish = false;
	std::vector <std::string> standardAnimationWallGripFileNames	= { "Lotus/LotusWallGrip.bmp" };
	std::vector <std::string> bWAnimationWallGripFileNames			= { "Lotus/LotusWallGripBW.bmp" };
	std::vector <std::vector<std::string>> mWallGripOutfits = { standardAnimationWallGripFileNames, bWAnimationWallGripFileNames };
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_WALL_GRIP, mWallGripOutfits, maxImageWidth, maxImageHeight, imageOffsetX, imageOffsetY, standardWallGripFrameRate, standardWallGripMustFinish));
}


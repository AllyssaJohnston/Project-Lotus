#include "collectiblePresets.h"

CCollectiblePreset::CCollectiblePreset() : EntityPreset()
{
	mEntityClassType		= EEntityClassTypes_COLLECTIBLE;
	mEntityCharacterType	= EEntityCharacterTypes_C_ENTITY;
	mEntityType				= EEntityType_STATIC;
	mTypeName				= "COLLECTIBLE";

	mCharacterMode  = ECharacterModes_STATIC;
	mMovementCodes  = { EEntityMovements_NONE };
	mMovementPath	= EEntityMovementPath_NONE;

	mCurDirection = EDirection_LEFT;

	mHitboxEdges = HitboxEdges(EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);

	mWidth	= -1;
	mHeight = -1;
}

CKeyPreset::CKeyPreset() : CCollectiblePreset()
{
	mEntityCharacterType	= EEntityCharacterTypes_C_KEY;
	mTypeName				= "KEY";
	
	mWidth	= 40;
	mHeight = 40;

	int maxImageWidth  = 50;
	int maxImageHeight = 50;

	int imageOffsetX = 0;
	int imageOffsetY = 0;

	int  stationaryFrameRate = 0;
	bool stationaryMustFinish = false;
	std::vector <std::string> animationStationaryFileNames = { "Collectibles/key.bmp" };
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY, animationStationaryFileNames, maxImageWidth, maxImageHeight, imageOffsetX, imageOffsetY, stationaryFrameRate, stationaryMustFinish));
}

CSavePointPreset::CSavePointPreset() : CCollectiblePreset()
{
	mEntityCharacterType	= EEntityCharacterTypes_C_SAVE_POINT;
	mTypeName				= "SAVE POINT";
	
	mWidth = 40;
	mHeight = 40;

	int maxImageWidth  = 60;
	int maxImageHeight = 60;

	int imageOffsetX = 0;
	int imageOffsetY = 0;

	int  stationaryFrameRate = 0;
	bool stationaryMustFinish = false;
	std::vector <std::string> animationStationaryFileNames = { "Collectibles/savePoint.bmp" };
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY, animationStationaryFileNames, maxImageWidth, maxImageHeight, imageOffsetX, imageOffsetY, stationaryFrameRate, stationaryMustFinish));
}

CLotusCollectiblePreset::CLotusCollectiblePreset() : CCollectiblePreset()
{
	mEntityCharacterType = EEntityCharacterTypes_C_LOTUS_COLLECTIBLE;
	mTypeName = "LOTUS COLLECTIBLE";
	
	mWidth = 30;
	mHeight = 30;

	int maxImageWidth  = 40;
	int maxImageHeight = 40;

	int imageOffsetX = 0;
	int imageOffsetY = 0;

	int  stationaryFrameRate = 0;
	bool stationaryMustFinish = false;
	std::vector <std::string> animationStationaryFileNames = { "Collectibles/lotusFlower.bmp" };
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY, animationStationaryFileNames, maxImageWidth, maxImageHeight, imageOffsetX, imageOffsetY, stationaryFrameRate, stationaryMustFinish));
}


CEndOfLevelPreset::CEndOfLevelPreset(bool vertical) : CCollectiblePreset()
{
	mEntityCharacterType	= EEntityCharacterTypes_C_END_OF_LEVEL;
	mTypeName				= "END OF LEVEL";
	
	mWidth = 40;
	mHeight = 40;

	int imageLongDim = 90;
	int imageShortDim = 60;

	int imageOffsetX = 0;
	int imageOffsetY = 10;

	int  stationaryFrameRate = 0;
	bool stationaryMustFinish = false;

	if (vertical)
	{
		std::vector <std::string> animationVFileNames = { "Collectibles/portalV.bmp" };
		mAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY, animationVFileNames, imageShortDim, imageLongDim, imageOffsetX, imageOffsetY, stationaryFrameRate, stationaryMustFinish));

	}
	else // horizontal
	{
		std::vector <std::string> animationHFileNames = { "Collectibles/portalH.bmp" };
		mAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY, animationHFileNames, imageLongDim, imageShortDim, imageOffsetX, imageOffsetY, stationaryFrameRate, stationaryMustFinish));
	}
}


CMiniGameLevelPreset::CMiniGameLevelPreset(EEntityCharacterTypes enemyType, LevelData givenNextLevelData) : CCollectiblePreset(), nextLevelData(givenNextLevelData)
{
	mEntityCharacterType = EEntityCharacterTypes_C_MINI_GAME_LEVEL;
	mTypeName = "MINI GAME LEVEL";
	
	mWidth = 40;
	mHeight = 40;

	setUpAnimationPresets(enemyType);
}

void CMiniGameLevelPreset::setUpAnimationPresets(EEntityCharacterTypes enemyType)
{
	int maxImageWidth = 130;
	int maxImageHeight = 130;
	
	int imageOffsetX = 0;
	int imageOffsetY = 10;

	int  frameRate = 14;
	bool mustFinish = false;

	std::vector <std::string> animationRunFileNames;
	switch (enemyType)
	{
	case EEntityCharacterTypes_E_RAT:
	default:
		mWidth	= 80;
		mHeight = 50;
		maxImageWidth	= 90;
		maxImageHeight	= 90;
		animationRunFileNames = {	"EarthEnemies/DraguarBlossom/draguarBlossomAnimationGhost01.bmp",  "EarthEnemies/DraguarBlossom/draguarBlossomAnimationGhost02.bmp",
									"EarthEnemies/DraguarBlossom/draguarBlossomAnimationGhost03.bmp",  "EarthEnemies/DraguarBlossom/draguarBlossomAnimationGhost04.bmp",
									"EarthEnemies/DraguarBlossom/draguarBlossomAnimationGhost05.bmp",  "EarthEnemies/DraguarBlossom/draguarBlossomAnimationGhost06.bmp",
									"EarthEnemies/DraguarBlossom/draguarBlossomAnimationGhost07.bmp",  "EarthEnemies/DraguarBlossom/draguarBlossomAnimationGhost08.bmp",
									"EarthEnemies/DraguarBlossom/draguarBlossomAnimationGhost09.bmp",  "EarthEnemies/DraguarBlossom/draguarBlossomAnimationGhost10.bmp",
									"EarthEnemies/DraguarBlossom/draguarBlossomAnimationGhost11.bmp",  "EarthEnemies/DraguarBlossom/draguarBlossomAnimationGhost12.bmp"};
		break;
	}
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_RUN, animationRunFileNames, maxImageWidth, maxImageHeight, imageOffsetX, imageOffsetY, frameRate, mustFinish));
}
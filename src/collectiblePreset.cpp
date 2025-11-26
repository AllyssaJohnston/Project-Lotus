#include "collectiblePresets.h"

CCollectiblePreset::CCollectiblePreset() : CCharacterPreset()
{
	mEntityClassType		= EEntityClassTypes_COLLECTIBLE;
	mEntityCharacterType	= EEntityCharacterTypes_C_ENTITY;
	mHitboxEdges			= HitboxEdges(EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);

	mEntityType		= EEntityType_STATIC;
	mCharacterModes.push_back(ECharacterModes_STATIC);
	mMovementCodes  = { EEntityMovements_NONE };
	mMovementPath	= EEntityMovementPath_NONE;

	mCurDirection = EDirection_LEFT;

	mWidth	= -1;
	mHeight = -1;

	mTypeName = "COLLECTIBLE";

}

CKeyPreset::CKeyPreset() : CCollectiblePreset()
{
	mEntityClassType		= EEntityClassTypes_COLLECTIBLE;
	mEntityCharacterType	= EEntityCharacterTypes_C_KEY;
	mHitboxEdges			= HitboxEdges(EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);

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

	mTypeName = "KEY";

}

CSavePointPreset::CSavePointPreset() : CCollectiblePreset()
{
	mEntityClassType	 = EEntityClassTypes_COLLECTIBLE;
	mEntityCharacterType = EEntityCharacterTypes_C_SAVE_POINT;
	mHitboxEdges = HitboxEdges(EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);

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

	mTypeName = "SAVE POINT";
}

CLotusCollectiblePreset::CLotusCollectiblePreset() : CCollectiblePreset()
{
	mEntityClassType	 = EEntityClassTypes_COLLECTIBLE;
	mEntityCharacterType = EEntityCharacterTypes_C_LOTUS_COLLECTIBLE;
	mHitboxEdges = HitboxEdges(EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);

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

	mTypeName = "LOTUS COLLECTIBLE";
}


CEndOfLevelPreset::CEndOfLevelPreset(int worldType) : CCollectiblePreset()
{
	mEntityClassType	 = EEntityClassTypes_COLLECTIBLE;
	mEntityCharacterType = EEntityCharacterTypes_C_END_OF_LEVEL;
	mHitboxEdges = HitboxEdges(EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);

	mWidth = 40;
	mHeight = 40;

	setUpAnimationPresets(worldType);

	mTypeName = "END OF LEVEL";
}

void CEndOfLevelPreset::setUpAnimationPresets(int worldType)
{
	int maxImageWidth  = 130;
	int maxImageHeight = 130;

	int imageOffsetX = 0;
	int imageOffsetY = 10;

	int  stationaryFrameRate = 0;
	bool stationaryMustFinish = false;

	std::vector <std::string> animationStationaryFileNames;
	switch (worldType)
	{
	case EWorldType_EARTH:
		animationStationaryFileNames = {"Collectibles/endOfLevelEarth.bmp"};
		break;
	case EWorldType_FIRE:
		animationStationaryFileNames = {"Collectibles/endOfLevelFire.bmp"};
		break;
	case EWorldType_WATER:
	default:
		animationStationaryFileNames = {"Collectibles/endOfLevelWater.bmp"};
		break;
	}
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_STATIONARY, animationStationaryFileNames, maxImageWidth, maxImageHeight, imageOffsetX, imageOffsetY, stationaryFrameRate, stationaryMustFinish));

}


CMiniGameLevelPreset::CMiniGameLevelPreset(int enemyType, LevelData givenNextLevelData) : CCollectiblePreset(), nextLevelData(givenNextLevelData)
{
	mEntityClassType = EEntityClassTypes_COLLECTIBLE;
	mEntityCharacterType = EEntityCharacterTypes_C_MINI_GAME_LEVEL;
	mHitboxEdges = HitboxEdges(EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL, EEntityEdgeType_NEUTRAL);

	mWidth = 40;
	mHeight = 40;

	setUpAnimationPresets(enemyType);

	mTypeName = "MINI GAME LEVEL";
}

void CMiniGameLevelPreset::setUpAnimationPresets(int enemyType)
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
		mWidth = 80;
		mHeight = 50;
		maxImageWidth = 90;
		maxImageHeight = 90;
		animationRunFileNames = {	"EarthEnemies/DraguarBlossom/draguarBlossomAnimationGhost01.bmp",  "EarthEnemies/DraguarBlossom/draguarBlossomAnimationGhost02.bmp",
									"EarthEnemies/DraguarBlossom/draguarBlossomAnimationGhost03.bmp",  "EarthEnemies/DraguarBlossom/draguarBlossomAnimationGhost04.bmp",
									"EarthEnemies/DraguarBlossom/draguarBlossomAnimationGhost05.bmp",  "EarthEnemies/DraguarBlossom/draguarBlossomAnimationGhost06.bmp",
									"EarthEnemies/DraguarBlossom/draguarBlossomAnimationGhost07.bmp",  "EarthEnemies/DraguarBlossom/draguarBlossomAnimationGhost08.bmp",
									"EarthEnemies/DraguarBlossom/draguarBlossomAnimationGhost09.bmp",  "EarthEnemies/DraguarBlossom/draguarBlossomAnimationGhost10.bmp",
									"EarthEnemies/DraguarBlossom/draguarBlossomAnimationGhost11.bmp",  "EarthEnemies/DraguarBlossom/draguarBlossomAnimationGhost12.bmp"};
		break;
	default:
		animationRunFileNames = { "UIElements/enemiesUI.bmp" }; // TODO replace
		break;
	}
	mAnimationPresets.push_back(AnimationPreset(EAnimationType_RUN, animationRunFileNames, maxImageWidth, maxImageHeight, imageOffsetX, imageOffsetY, frameRate, mustFinish));
}
#include "gameInstanceHelper.h"
#include "projectileHelper.h"
#include "projectilePresets.h"
#include "platformHelper.h"
#include "worldHelper.h"
#include "worldHelperClass.h"
#include "enemyHelper.h"
#include "collectibleHelper.h"
#include "menuHelper.h"
#include "movementJump.h"

GameInstance::GameInstance()
{
	mWorldData.createLevelChunk();
}

GameInstance::~GameInstance()
{
}

void GameInstance::preTick() 
{
	mGameStateManager.preTick();
}

void GameInstance::tick() 
{
	mGameStateManager.tick();
}

void GameInstance::postTick()
{
	mSettingsManager.mLastFrameTextIncrease = mSettingsManager.mTextIncrease;
	mGameStateManager.postTick();
}



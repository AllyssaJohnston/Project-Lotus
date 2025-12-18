#include "gameInstanceHelper.h"

GameInstance::GameInstance()
{
	mWorldData.createLevelChunk();
}

GameInstance::~GameInstance() { }

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



#include "miniGameLevelHelper.h"

MiniGameStage::MiniGameStage(int stageNumber) : mStageNumber(stageNumber) { ; }

void MiniGameStage::preTick()
{
    mGrid.preTick();
}

void MiniGameStage::resetStats()
{
    mCombatManager.resetStats();
}

MiniGameLevel::MiniGameLevel(int levelNumber, LevelData nextLevelData) : mLevelNumber(levelNumber), mNextLevelData(nextLevelData) { ; }

MiniGameLevel::~MiniGameLevel() 
{ 
    for (MiniGameStage* pStage : mpStages)
    {
        delete pStage;
    }
    mpStages.clear();
}


MiniGameWorld::MiniGameWorld(int worldNumber) : mWorldNumber(worldNumber) { ; }

MiniGameWorld::~MiniGameWorld() {

    for (MiniGameLevel* pLevel : mpLevels)
    {
        delete pLevel;
    }
    mpLevels.clear();
}
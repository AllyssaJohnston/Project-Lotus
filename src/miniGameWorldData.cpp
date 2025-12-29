#include "miniGameWorldData.h"

MiniGameWorldData::~MiniGameWorldData()
{
    for (MiniGameWorld* pWorld : mpMiniGameWorlds)
    {
        delete pWorld;
    }
    mpMiniGameWorlds.clear();
}

void MiniGameWorldData::resetStage() { mpMiniGameWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber]->mpStages[mCurStageNumber]->resetStats(); }

void MiniGameWorldData::setNextLevel(int worldNumber, int levelNumber, int stageNumber)
{
    mCurWorldNumber = worldNumber;
    mCurLevelNumber = levelNumber;
    mCurStageNumber = stageNumber;
    resetStage();
}

void MiniGameWorldData::goToNextLevel()
{
    if (mCurStageNumber + 1 < (int)mpMiniGameWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber]->mpStages.size())
    {
        // go to next stage
        mCurStageNumber++;
    }
    else if (mCurLevelNumber + 1 < (int)mpMiniGameWorlds[mCurWorldNumber]->mpLevels.size())
    {
        mCurStageNumber = 0;
        mCurLevelNumber++;
    }
    else if (mCurWorldNumber + 1 < (int)mpMiniGameWorlds.size())
    {
        mCurStageNumber = 0;
        mCurLevelNumber = 0;
        mCurWorldNumber++;
    }
    mpMiniGameWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber]->mpStages[mCurStageNumber]->resetStats();
}

bool MiniGameWorldData::onLastStage() const { return mCurStageNumber + 1 >= mpMiniGameWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber]->mpStages.size(); }

MiniGameLevel* MiniGameWorldData::getLevel() const { return mpMiniGameWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber]; }

MiniGameStage* MiniGameWorldData::getStage() const { return mpMiniGameWorlds[mCurWorldNumber]->mpLevels[mCurLevelNumber]->mpStages[mCurStageNumber]; }
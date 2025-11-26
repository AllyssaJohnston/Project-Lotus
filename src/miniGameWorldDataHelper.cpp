#include "miniGameWorldDataHelper.h"

MiniGameWorldData::~MiniGameWorldData()
{
    for (MiniGameLevel* level : mpMiniGameLevels)
    {
        delete level;
    }
}

void MiniGameWorldData::resetLevel() { mpMiniGameLevels[mCurMiniGameLevelNumber]->resetStats(); }

void MiniGameWorldData::setNextLevel(int levelNumber)
{
    mCurMiniGameLevelNumber = levelNumber;
    resetLevel();
}
#include "miniGameLevelHelper.h"

MiniGameLevel::MiniGameLevel(int levelNumber, LevelData nextLevelData) : mLevelNumber(levelNumber), mNextLevelData(nextLevelData) { ; }

void MiniGameLevel::preTick()
{
    mGrid.preTick();
}

void MiniGameLevel::resetStats()
{
    mCombatManager.resetStats();
}
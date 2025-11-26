#include "miniGameLevelHelper.h"

MiniGameLevel::MiniGameLevel(int levelNumber, Grid grid, CombatManager combatManager, LevelData nextLevelData) 
        : mLevelNumber(levelNumber), mGrid(grid), mCombatManager(combatManager), mMainGameNextLevelData(nextLevelData) {}

void MiniGameLevel::resetStats()
{
    mCombatManager.resetStats();
}
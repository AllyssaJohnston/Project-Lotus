#pragma once
#include "gridHelper.h"
#include "combatHelper.h"
#include "worldHelperClass.h"

class MiniGameLevel
{
public:
    int mLevelNumber;

    Grid mGrid;
    CombatManager mCombatManager;

    // the level in the main game to go back to after finishing some rounds of mini game levels
    LevelData mMainGameNextLevelData;

    MiniGameLevel(int levelNumber, Grid grid, CombatManager combatManager, LevelData nextLevelData);

    ~MiniGameLevel() { ; }

    void resetStats();
};
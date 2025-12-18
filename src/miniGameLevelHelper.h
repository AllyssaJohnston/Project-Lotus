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

    // either mini game level or main game level
    LevelData mNextLevelData;

    MiniGameLevel(int levelNumber, LevelData nextLevelData);

    ~MiniGameLevel() { ; }

    void preTick();

    void resetStats();
};
#pragma once
#include "grid.h"
#include "combatManager.h"
#include "worldHelpers.h"

class MiniGameStage
{
public:
    int mStageNumber;

    Grid mGrid;
    CombatManager mCombatManager;


    MiniGameStage(int stageNumber);

    void preTick();

    void resetStats();
};

class MiniGameLevel
{
public:
    int mLevelNumber;

    std::vector<MiniGameStage*> mpStages;

    // either mini game level or main game level
    LevelData mNextLevelData;

    MiniGameLevel(int levelNumber, LevelData nextLevelData);

    ~MiniGameLevel();
};

class MiniGameWorld
{
public:
    int mWorldNumber;

    std::vector<MiniGameLevel*> mpLevels;

    MiniGameWorld(int worldNumber);

    ~MiniGameWorld();
};
#pragma once
#include "gridHelper.h"
#include "combatHelper.h"
#include "worldHelperClass.h"

class MiniGameStage
{
public:
    int mStageNumber;

    Grid mGrid;
    CombatManager mCombatManager;


    MiniGameStage(int stageNumber);

    ~MiniGameStage() { ; }

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
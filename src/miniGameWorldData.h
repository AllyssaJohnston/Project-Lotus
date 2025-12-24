#pragma once
#include <vector>
#include "globals.h"
#include "miniGameLevel.h"
#include "worldHelpers.h"

struct MiniGameWorldData
{
    std::vector <MiniGameWorld*>	mpMiniGameWorlds;
#if DEMO == 0
    int                             mCurWorldNumber = 0;
    int								mCurLevelNumber = 0;
    int                             mCurStageNumber = 0;
#else
    int                             mCurWorldNumber = 0;
    int								mCurLevelNumber = 0;
    int                             mCurStageNumber = 0;
#endif

    int mCheatWorldNumber = 2;
    int mCheatLevelNumber = 0;
    int mCheatStageNumber = 0;

    bool                            mGoToNextLevel = false;
    LevelData*                      mpNextLevelData = nullptr;

    ~MiniGameWorldData();

    void resetStage();

    void setNextLevel(int worldNumber, int levelNumber, int stageNumber);

    void goToNextLevel();

    bool onLastStage() const;

    MiniGameLevel* getLevel() const;

    MiniGameStage* getStage() const;
};
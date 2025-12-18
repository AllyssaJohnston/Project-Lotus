#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include "globals.h"
#include "screenHelper.h"
#include "miniGameLevelHelper.h"
#include "worldHelperClass.h"

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
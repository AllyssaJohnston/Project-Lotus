#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include "globals.h"
#include "screenHelper.h"
#include "miniGameLevelHelper.h"
#include "worldHelperClass.h"

struct MiniGameWorldData
{
    std::vector <MiniGameLevel *>	mpMiniGameLevels;
#if DEMO == 0
    int								mCurMiniGameLevelNumber = 0;
#else
    int								mCurMiniGameLevelNumber = 0;
#endif
    bool                            mGoToNextLevel = false;
    LevelData*                      mpNextLevelData = nullptr;

    ~MiniGameWorldData();

    void resetLevel();

    void setNextLevel(int levelNumber);
};
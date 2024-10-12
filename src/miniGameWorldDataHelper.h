#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include "screenHelper.h"
#include "miniGameLevelHelper.h"

struct MiniGameWorldData
{
    std::vector <MiniGameLevel * >	mpMiniGameLevels;
    int								mCurMiniGameLevelNumber = 0;

    ~MiniGameWorldData();
};
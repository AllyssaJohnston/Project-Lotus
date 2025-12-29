#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include "helpers.h"


struct GameStateManagerData
{
    EGameState				    mCurStateEnum = EGameState_INVALID;
    EGameState				    mLastFrameStateEnum = EGameState_INVALID;
    bool                        mRunGame = true;
};

struct GameStateData
{
    EGameState mNextGameState = EGameState_INVALID;
    bool mCleanNextState = false;
};


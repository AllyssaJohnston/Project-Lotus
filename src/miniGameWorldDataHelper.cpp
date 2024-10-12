#include "miniGameWorldDataHelper.h"

MiniGameWorldData::~MiniGameWorldData()
{
    for (MiniGameLevel* level : mpMiniGameLevels)
    {
        delete level;
    }
}
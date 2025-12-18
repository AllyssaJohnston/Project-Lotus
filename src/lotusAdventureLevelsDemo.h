#pragma once
#include "globals.h"
#if DEMO != 0
#include "hitboxHelper.h"
#include "helperClass.h"
#include "textBoxHelperClass.h"
#include "platformPresets.h"
#include "platformHelper.h"
#include "enemyPresets.h"
#include "enemyHelper.h"
#include "collectiblePresets.h"
#include "collectibleHelper.h"
#include "worldDataHelper.h"
#include "worldHelper.h"
#include "miniGameWorldDataHelper.h"
#include "miniGameLevelHelper.h"


void createNames(Level* pCurLevel, int curLevelNumber);

void createLevels(WorldData& worldData, MiniGameWorldData& mgWorldData, ScreenObject& screen);


#endif


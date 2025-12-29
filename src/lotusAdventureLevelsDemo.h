#pragma once
#include "globals.h"
#if DEMO != 0
#include "hitbox.h"
#include "helpers.h"
#include "uiBoxHelpers.h"
#include "platformPresets.h"
#include "platform.h"
#include "enemyPresets.h"
#include "enemy.h"
#include "collectiblePresets.h"
#include "collectible.h"
#include "worldData.h"
#include "world.h"
#include "miniGameWorldData.h"
#include "miniGameLevel.h"
#include "screen.h"

void createNames(Level* pCurLevel);

void createMiniGameNames(MiniGameStage* pCurStage);


void createLevels(WorldData& worldData, MiniGameWorldData& mgWorldData, ScreenObject& screen);

#endif


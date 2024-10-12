#pragma once
#include "gameInstanceHelper.h"
#include "gameStateHelperClass.h"
#include "hitboxHelper.h"
#include "helperClass.h"
#include "miniGameLevelHelper.h"
#include "textBoxPresets.h"
#include "textBoxHelper.h"
#include "textBoxHelperClass.h"
#include "menuHelper.h"
#include "platformPresets.h"
#include "platformHelper.h"
#include "enemyPresets.h"
#include "enemyHelper.h"
#include "collectiblePresets.h"
#include "collectibleHelper.h"
#include "worldDataHelper.h"
#include "miniGameWorldDataHelper.h"
#include "worldHelper.h"

void createNames(Level* pCurLevel, int curLevelNumber);

void finishAllLevelsSetUp(GameInstance & gameInstance);

void createLevels(GameInstance & gameInstance);

void createMiniGameLevels(GameInstance& gameInstance);


void createMainGameMenu(GameInstance& gameInstance, const char * fontFileName);

void createMenus(GameInstance& gameInstance);

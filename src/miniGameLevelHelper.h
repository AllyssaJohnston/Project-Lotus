#pragma once
#include "gridHelper.h"
#include "combatHelper.h"

class MiniGameLevel
{
public:
    int mLevelNumber;

    Grid * mpGrid;
    CombatManager * mpCombatManager;

    MiniGameLevel(int levelNumber, Grid * pGrid, CombatManager * pCombatManager);

    ~MiniGameLevel();

    void resetStats();
};
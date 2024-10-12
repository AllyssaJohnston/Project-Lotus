#include "miniGameLevelHelper.h"

MiniGameLevel::MiniGameLevel(int levelNumber, Grid * pGrid, CombatManager * pCombatManager)
{
    mLevelNumber    = levelNumber;
    mpGrid          = pGrid;
    mpCombatManager = pCombatManager;
}

MiniGameLevel::~MiniGameLevel()
{
    if (mpGrid)
    {
        delete mpGrid;
        mpGrid = nullptr;
    }
    if (mpCombatManager)
    {
        delete mpCombatManager;
        mpCombatManager = nullptr;
    }
}

void MiniGameLevel::resetStats()
{
    mpCombatManager->resetStats();
}
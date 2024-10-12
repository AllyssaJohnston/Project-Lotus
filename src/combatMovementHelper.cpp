#include "combatMovementHelper.h"

CombatMovementManager::CombatMovementManager(Tile* pTile, CombatCharacterPreset* pPreset) : 
        mAttacks(pPreset->mAttacks), mMoveType(pPreset->mMoveType), 
        mpStartingTile(pTile), mpCurTile(pTile) {}

CombatMovementManager::~CombatMovementManager()
{
    mpStartingTile  = nullptr;
    mpCurTile       = nullptr;
}

void CombatMovementManager::CombatMovementManager::preTick()
{
    getMoveTiles();
}

void CombatMovementManager::resetStats()
{
    mpCurTile = mpStartingTile;
}

void CombatMovementManager::setCurTile(Tile* pTileInput)
{
    mpCurTile = pTileInput;
}

std::vector <TileCoords> CombatMovementManager::getMoveTiles() 
{
    mpMoveTileCoords.clear();
    mpMoveTileCoords = returnTileCoords(mpCurTile, mMoveType);
    return mpMoveTileCoords;
}

std::vector <Attack> CombatMovementManager::getAttacks() const
{
    return mAttacks;
}

Tile* CombatMovementManager::getCurTile()
{
    return mpCurTile;
}

EMiniGameCombatMoveAttackTypes CombatMovementManager::getMoveType() const
{
    return mMoveType;
}
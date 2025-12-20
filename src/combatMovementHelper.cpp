#include "combatMovementHelper.h"

CombatMovementManager::CombatMovementManager(Tile* pTile, const CombatCharacterPreset& preset) : mAttacks(preset.mAttacks), mMoveType(preset.mMoveType), 
        mpStartingTile(pTile), mpCurTile(pTile) {}

CombatMovementManager::~CombatMovementManager()
{
    mpStartingTile  = nullptr;
    mpCurTile       = nullptr;
}

void CombatMovementManager::CombatMovementManager::preTick()
{
    setMoveTiles();
}

void CombatMovementManager::resetStats()
{
    mpCurTile = mpStartingTile;
}

void CombatMovementManager::setCurTile(Tile* pTileInput)  { mpCurTile = pTileInput; }

std::vector <TileCoords> CombatMovementManager::getMoveTiles() const { return mMoveTileCoords; }

void CombatMovementManager::setMoveTiles() { mMoveTileCoords = returnTileCoords(*mpCurTile, mMoveType, EDirection_ALL); }

bool CombatMovementManager::isTileInMoveRange(const Tile& givenTile) const
{
    for (const TileCoords& curTileCoord : mMoveTileCoords)
    {
        if (givenTile.mRow == curTileCoord.mRow and givenTile.mCol == curTileCoord.mCol)
        {
            return true;
        }
    }
    return false;
}

std::vector <Attack>& CombatMovementManager::getAttacks() { return mAttacks; }

Tile* CombatMovementManager::getCurTile() const { return mpCurTile; }

EMiniGameCombatMoveAttackTypes CombatMovementManager::getMoveType() const { return mMoveType; }
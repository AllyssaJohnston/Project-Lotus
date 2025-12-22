#include "combatMovementHelper.h"

CombatMovementManager::CombatMovementManager(Tile& startingTile, const CombatCharacterPreset& preset) : mAttacks(preset.mAttacks), mMoveType(preset.mMoveType), 
        mStartingTile(startingTile), mpCurTile(&startingTile) {}

CombatMovementManager::~CombatMovementManager()
{
    mpCurTile       = nullptr;
}

void CombatMovementManager::CombatMovementManager::preTick()
{
    setMoveTiles();
}

void CombatMovementManager::resetStats()
{
    mpCurTile = &mStartingTile;
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
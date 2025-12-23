#include "combatMovementManager.h"

CombatMovementManager::CombatMovementManager(Tile& startingTile, const CombatCharacterPreset& preset) : mAttacks(preset.mAttacks), mType(preset.mMoveType), 
        mNum(preset.mMoveNum), mOut(preset.mMoveOut), mStartingTile(startingTile), mpCurTile(&startingTile) {;}

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

std::vector <TileCoords> CombatMovementManager::getMoveTileCoords() const { return mTileCoords; }

void CombatMovementManager::setMoveTiles() { mTileCoords = returnTileCoords(*mpCurTile, mType, mNum, mOut, EDirection_ALL); }

bool CombatMovementManager::isTileInMoveRange(const Tile& givenTile) const
{
    for (const TileCoords& curTileCoord : mTileCoords)
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

EMiniGameCombatMoveAttackTypes CombatMovementManager::getMoveType() const { return mType; }

int CombatMovementManager::getMoveNum() const { return mNum; }

int CombatMovementManager::getMoveOut() const { return mOut; }
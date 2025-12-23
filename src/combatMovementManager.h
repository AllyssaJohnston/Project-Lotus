#pragma once
#include <vector>
#include "combatAttack.h"
#include "combatHelpers.h"
#include "combatCharacterPresets.h"
#include "tileCoords.h"
#include "tile.h"
#include "tileHelpers.h"

class CombatMovementManager
{
private:
    std::vector <Attack>           mAttacks;
    EMiniGameCombatMoveAttackTypes mType = EMiniGameCombatMoveAttackTypes_INVALID;
    int                            mNum;
    int                            mOut;
    std::vector <TileCoords>       mTileCoords;

    Tile& mStartingTile;
    Tile* mpCurTile;

public:
    CombatMovementManager(Tile& startingTile, const CombatCharacterPreset& preset);

    ~CombatMovementManager();

    void preTick();

    void resetStats();

    void setCurTile(Tile* pTileInput);

    void setMoveTiles();

    std::vector <TileCoords> getMoveTileCoords() const;

    bool isTileInMoveRange(const Tile& givenTile) const;

    std::vector <Attack>& getAttacks();

    Tile* getCurTile() const;

    EMiniGameCombatMoveAttackTypes getMoveType() const;

    int getMoveNum() const;

    int getMoveOut() const;
};
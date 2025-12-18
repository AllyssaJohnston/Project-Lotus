#pragma once
#include <vector>
#include "combatAttackHelper.h"
#include "combatAttackHelperClass.h"
#include "combatHelperClass.h"
#include "combatCharacterPresets.h"
#include "tileCoordsHelper.h"

class CombatMovementManager
{
private:
    std::vector <Attack>           mAttacks;
    EMiniGameCombatMoveAttackTypes mMoveType = EMiniGameCombatMoveAttackTypes_INVALID;
    std::vector <TileCoords>       mMoveTileCoords;

    Tile* mpStartingTile    = nullptr;
    Tile* mpCurTile         = nullptr;

public:
    CombatMovementManager(Tile* pTile, const CombatCharacterPreset& preset);

    ~CombatMovementManager();

    void preTick();

    void resetStats();

    void setCurTile(Tile* pTileInput);

    void setMoveTiles();

    std::vector <TileCoords> getMoveTiles() const;

    bool isTileInMoveRange(const Tile& givenTile) const;

    std::vector <Attack>& getAttacks();

    Tile* getCurTile() const;

    EMiniGameCombatMoveAttackTypes getMoveType() const;
};
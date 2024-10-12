#pragma once
#include <vector>
#include "combatAttackHelper.h"
#include "combatAttackHelperClass.h"
#include "combatHelperClass.h"
#include "combatCharacterPresets.h"
#include "tileCoordsHelper.h"

std::vector <TileCoords> returnTileCoords(Tile* pGivenTile, EMiniGameCombatMoveAttackTypes moveAttackType);

class CombatMovementManager
{
private:
    std::vector <Attack>           mAttacks;
    EMiniGameCombatMoveAttackTypes mMoveType        = EMiniGameCombatMoveAttackTypes_INVALID;
	//TODO CHANGE NAME
    std::vector <TileCoords>       mpMoveTileCoords;

    Tile * mpStartingTile                           = nullptr;
    Tile * mpCurTile                                = nullptr;

public:

    CombatMovementManager() { }

    CombatMovementManager(Tile* pTile, CombatCharacterPreset* pPreset);

    ~CombatMovementManager();

    void preTick();

    void resetStats();

    void setCurTile(Tile* pTileInput);

    std::vector <TileCoords> getMoveTiles();

    std::vector <Attack> getAttacks() const;

    Tile* getCurTile();

    EMiniGameCombatMoveAttackTypes getMoveType() const;
};
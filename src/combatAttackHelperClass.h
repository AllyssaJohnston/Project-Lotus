#pragma once
#include "tileHelper.h"
#include "tileHelperClass.h"
#include "tileCoordsHelper.h"
#include "combatHelperClass.h"
#include "combatAttackHelper.h"

struct AttackTile
{
    Tile&  mTile;
    Attack& mAttack;

    AttackTile(Tile& tile, Attack& attack);
};

struct AttackAndCorrespondingTilesCoords
{
    std::vector <TileCoords> mTileCoords;
    Attack mAttack;

    AttackAndCorrespondingTilesCoords() {}
    AttackAndCorrespondingTilesCoords(std::vector <TileCoords>& tileCoords, const Attack& attack);
};

std::vector <TileCoords> returnTileCoords(const Tile& givenTile, EMiniGameCombatMoveAttackTypes moveAttackType, EDirection direction);


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

std::vector <TileCoords> returnTileCoords(const Tile& givenTile, EMiniGameCombatMoveAttackTypes moveAttackType, EDirection direction);


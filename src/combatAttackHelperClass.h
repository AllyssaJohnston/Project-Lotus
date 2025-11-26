#pragma once
#include "tileHelper.h"
#include "tileHelperClass.h"
#include "combatHelperClass.h"
#include "combatAttackHelper.h"

struct AttackTile
{
    Tile*  mpTile = nullptr;
    Attack mAttack;

    AttackTile(Tile* pTile, Attack& attack);

    ~AttackTile();

};

struct AttackAndListOfTileCoordsToCorrespondingTilesCoords
{
    std::vector <TileCoords> mTileCoords;
    Attack mAttack;

    AttackAndListOfTileCoordsToCorrespondingTilesCoords() {}
    AttackAndListOfTileCoordsToCorrespondingTilesCoords(std::vector <TileCoords> & tileCoords, Attack& attack);
};

AttackAndListOfTileCoordsToCorrespondingTilesCoords returnAttackTileCoordsBasedOnAttack(Tile* pGivenTile, Attack curAttack);

AttackAndListOfTileCoordsToCorrespondingTilesCoords returnAttackTileCoordsBasedOnAttackAndDirection(Tile* pGivenTile, Attack curAttack, EDirection givenDirection);

std::vector <TileCoords> returnTileCoords(Tile* pGivenTile, EMiniGameCombatMoveAttackTypes moveAttackType);

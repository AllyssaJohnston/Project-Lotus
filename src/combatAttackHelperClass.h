#pragma once
#include "tileHelper.h"
#include "tileHelperClass.h"
#include "combatHelperClass.h"
#include "combatAttackHelper.h"

class AttackTile
{
public:
    //Tiles are not transient, ptr to them.
    Tile*  mpTile = nullptr;
    //Attacks are transient, keep memory here.
    Attack mAttack;

    AttackTile() { }
    AttackTile(Tile* pTile, const Attack& attack);

    ~AttackTile();

};

class AttackAndListOfTileCoordsToCorrespondingTilesCoords
{
public:
    std::vector <TileCoords> mTileCoords;
    Attack mAttack;

    AttackAndListOfTileCoordsToCorrespondingTilesCoords() {}
    AttackAndListOfTileCoordsToCorrespondingTilesCoords(std::vector <TileCoords> & tileCoords, Attack& attack);
};

AttackAndListOfTileCoordsToCorrespondingTilesCoords returnAttackTileCoordsBasedOnAttack(Tile* pGivenTile, Attack& pCurAttack);

AttackAndListOfTileCoordsToCorrespondingTilesCoords returnAttackTileCoordsBasedOnAttackAndDirection(Tile* pGivenTile, Attack& pCurAttack, EDirection givenDirection);

std::vector <TileCoords> returnTileCoords(Tile* pGivenTile, EMiniGameCombatMoveAttackTypes moveAttackType);

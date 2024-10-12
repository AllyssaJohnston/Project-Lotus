#include "combatAttackHelperClass.h"

AttackTile::AttackTile(Tile* pTile, const Attack& attack) : mpTile(pTile), mAttack(attack) {}

AttackTile::~AttackTile()
{
    mpTile = nullptr;
}


AttackAndListOfTileCoordsToCorrespondingTilesCoords::AttackAndListOfTileCoordsToCorrespondingTilesCoords(std::vector <TileCoords>& tileCoords, Attack& attack)
        : mTileCoords(tileCoords), mAttack(attack) {}


std::vector <TileCoords> returnTileCoords(Tile* pGivenTile, EMiniGameCombatMoveAttackTypes moveAttackType)
{
    if (pGivenTile != nullptr)
    {
        switch (moveAttackType)
        {
        case EMiniGameCombatMoveAttackTypes_CROSS1:
            return getCross1Tiles(pGivenTile);
            break;
        case EMiniGameCombatMoveAttackTypes_CROSS2:
            return getCross2Tiles(pGivenTile);
            break;
        case EMiniGameCombatMoveAttackTypes_SQUARE1:
            return getSquare1Tiles(pGivenTile);
            break;
        case EMiniGameCombatMoveAttackTypes_SQUARE2:
            return getSquare2Tiles(pGivenTile);
            break;
        case EMiniGameCombatMoveAttackTypes_NONE:
            break;
        default:
            SDL_assert(false);
            break;
        }

    }
    //if else
    std::vector <TileCoords> tileCoords;
    return tileCoords;
}

AttackAndListOfTileCoordsToCorrespondingTilesCoords returnAttackTileCoordsBasedOnAttack(Tile* pGivenTile, Attack& curAttack)
{
    std::vector <TileCoords> tileCoordsList = returnTileCoords(pGivenTile, curAttack.mType);
    return AttackAndListOfTileCoordsToCorrespondingTilesCoords(tileCoordsList, curAttack);
}

AttackAndListOfTileCoordsToCorrespondingTilesCoords returnAttackTileCoordsBasedOnAttackAndDirection(Tile* pGivenTile, Attack& curAttack, EDirection givenDirection)
{
    std::vector <TileCoords> tileCoordsList;
    switch (curAttack.mType)
    {
    case EMiniGameCombatMoveAttackTypes_SQUARE1:
        switch (givenDirection)
        {
        case EDirection_LEFT:
            tileCoordsList = getSquare1LeftTiles(pGivenTile);
            break;
        case EDirection_RIGHT:
            tileCoordsList = getSquare1RightTiles(pGivenTile);
            break;
        case EDirection_UP:
            tileCoordsList = getSquare1UpTiles(pGivenTile);
            break;
        case EDirection_DOWN:
            tileCoordsList = getSquare1DownTiles(pGivenTile);
            break;
        default:
            SDL_assert(false);
            break;
        }
        break;
    case EMiniGameCombatMoveAttackTypes_SQUARE2:
        switch (givenDirection)
        {
        case EDirection_LEFT:
            tileCoordsList = getSquare2LeftTiles(pGivenTile);
            break;
        case EDirection_RIGHT:
            tileCoordsList = getSquare2RightTiles(pGivenTile);
            break;
        case EDirection_UP:
            tileCoordsList = getSquare2UpTiles(pGivenTile);
            break;
        case EDirection_DOWN:
            tileCoordsList = getSquare2DownTiles(pGivenTile);
            break;
        default:
            SDL_assert(false);
            break;
        }
        break;
    default:
        SDL_assert(false);
        break;
    }

    return AttackAndListOfTileCoordsToCorrespondingTilesCoords(tileCoordsList, curAttack);
}
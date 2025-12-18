#include "combatAttackHelperClass.h"

AttackTile::AttackTile(Tile& tile, Attack& attack) : mTile(tile), mAttack(attack) {}


AttackAndCorrespondingTilesCoords::AttackAndCorrespondingTilesCoords(std::vector <TileCoords>& tileCoords, const Attack& attack) : mTileCoords(tileCoords), mAttack(attack) {}

std::vector <TileCoords> returnTileCoords(const Tile& givenTile, EMiniGameCombatMoveAttackTypes moveAttackType)
{
    switch (moveAttackType)
    {
    case EMiniGameCombatMoveAttackTypes_CROSS1:
        return getCrossTiles(givenTile, 0, 1, EDirection_ALL);
    case EMiniGameCombatMoveAttackTypes_CROSS2:
        return getCrossTiles(givenTile, 0, 2, EDirection_ALL);
    case EMiniGameCombatMoveAttackTypes_CROSS3:
        return getCrossTiles(givenTile, 0, 3, EDirection_ALL);
    case EMiniGameCombatMoveAttackTypes_CROSS3_1UNIT_OUT:
        return getCrossTiles(givenTile, 1, 3, EDirection_ALL);
    case EMiniGameCombatMoveAttackTypes_CROSS4:
        return getCrossTiles(givenTile, 0, 4, EDirection_ALL);
    case EMiniGameCombatMoveAttackTypes_CROSS5:
        return getCrossTiles(givenTile, 0, 5, EDirection_ALL);
    case EMiniGameCombatMoveAttackTypes_SQUARE1:
        return getSquareTiles(givenTile, 0, 1, EDirection_ALL);
    case EMiniGameCombatMoveAttackTypes_SQUARE2:
        return getSquareTiles(givenTile, 0, 2, EDirection_ALL);
    case EMiniGameCombatMoveAttackTypes_NONE:
        break;
    default:
        SDL_assert(false);
        break;
    }
    return std::vector <TileCoords>();
}

AttackAndCorrespondingTilesCoords returnAttackTileCoordsBasedOnAttack(const Tile& givenTile, const Attack& curAttack)
{
    std::vector <TileCoords> tileCoordsList = returnTileCoords(givenTile, curAttack.mType);
    return AttackAndCorrespondingTilesCoords(tileCoordsList, curAttack);
}

AttackAndCorrespondingTilesCoords returnAttackTileCoordsBasedOnAttackAndDirection(const Tile& givenTile, const Attack& curAttack, EDirection givenDirection)
{
    std::vector <TileCoords> tileCoordsList;
    switch (curAttack.mType)
    {
    case EMiniGameCombatMoveAttackTypes_SQUARE1:
        tileCoordsList = getSquareTiles(givenTile, 0, 1, givenDirection);
        break;
    case EMiniGameCombatMoveAttackTypes_SQUARE2:
        tileCoordsList = getSquareTiles(givenTile, 0, 2, givenDirection);
        break;
    default:
        SDL_assert(false);
        break;
    }
    return AttackAndCorrespondingTilesCoords(tileCoordsList, curAttack);
}
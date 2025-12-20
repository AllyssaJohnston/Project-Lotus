#include "combatAttackHelperClass.h"

AttackTile::AttackTile(Tile& tile, Attack& attack) : mTile(tile), mAttack(attack) {}


AttackAndCorrespondingTilesCoords::AttackAndCorrespondingTilesCoords(std::vector <TileCoords>& tileCoords, const Attack& attack) : mTileCoords(tileCoords), mAttack(attack) {}

std::vector <TileCoords> returnTileCoords(const Tile& givenTile, EMiniGameCombatMoveAttackTypes moveAttackType, EDirection direction)
{
    switch (moveAttackType)
    {
    case EMiniGameCombatMoveAttackTypes_CROSS1:
        return getCrossTiles(givenTile, 0, 1, direction);
    case EMiniGameCombatMoveAttackTypes_CROSS2:
        return getCrossTiles(givenTile, 0, 2, direction);
    case EMiniGameCombatMoveAttackTypes_CROSS2_2UNIT_OUT:
        return getCrossTiles(givenTile, 2, 2, direction);
    case EMiniGameCombatMoveAttackTypes_CROSS3:
        return getCrossTiles(givenTile, 0, 3, direction);
    case EMiniGameCombatMoveAttackTypes_CROSS3_1UNIT_OUT:
        return getCrossTiles(givenTile, 1, 3, direction);
    case EMiniGameCombatMoveAttackTypes_CROSS4:
        return getCrossTiles(givenTile, 0, 4, direction);
    case EMiniGameCombatMoveAttackTypes_CROSS5:
        return getCrossTiles(givenTile, 0, 5, direction);
    case EMiniGameCombatMoveAttackTypes_SQUARE1:
        return getSquareTiles(givenTile, 0, 1, direction);
    case EMiniGameCombatMoveAttackTypes_SQUARE2:
        return getSquareTiles(givenTile, 0, 2, direction);
    case EMiniGameCombatMoveAttackTypes_WHOLE_GRID:
        return getGrid(30, 30);
    case EMiniGameCombatMoveAttackTypes_NONE:
        break;
    default:
        SDL_assert(false);
        break;
    }
    return std::vector <TileCoords>();
}

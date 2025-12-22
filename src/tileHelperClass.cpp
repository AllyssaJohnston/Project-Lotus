#include "tileHelperClass.h"
#include "tileHelper.h"
#include "tileCoordsHelper.h"
#include "helperClass.h"



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
    case EMiniGameCombatMoveAttackTypes_CHECKERBOARD2:
        return getCheckerboardTiles(givenTile, 0, 2, direction);
    case EMiniGameCombatMoveAttackTypes_WHOLE_GRID:
        SDL_assert(false);
    case EMiniGameCombatMoveAttackTypes_ONE_CHARACTER:
    case EMiniGameCombatMoveAttackTypes_ONE_PLAYER:
    case EMiniGameCombatMoveAttackTypes_ONE_ENEMY:
    case EMiniGameCombatMoveAttackTypes_NONE:
        break;
    default:
        SDL_assert(false);
        break;
    }
    return std::vector <TileCoords>();
}

std::vector<TileCoords> getSquareTiles(const Tile& centerTile, const int startOut, const int squareSize, const EDirection directions)
{
    const int& curRow = centerTile.mRow;
    const int& curCol = centerTile.mCol;
    std::vector <TileCoords> tileCoordsList;

    int rowStart    = -squareSize;
    int rowEnd      = squareSize;
    int colStart    = -squareSize;
    int colEnd      = squareSize;

    switch (directions)
    {
    case EDirection_LEFT:
        colEnd = startOut - 1;
        break;
    case EDirection_RIGHT:
        colStart = startOut + 1;
        break;
    case EDirection_UP:
        rowEnd = startOut - 1;
        break;
    case EDirection_DOWN:
        rowStart = startOut + 1;
        break;
    }
    
    for (int rowI = rowStart; rowI <= rowEnd; rowI++)
    {
        for (int colI = colStart; colI <= colEnd; colI++)
        {
            if (std::abs(rowI) < startOut && std::abs(colI) < startOut)
            {
                continue;
            }
            tileCoordsList.push_back(TileCoords(rowI + curRow, colI + curCol));
        }
    }
    return tileCoordsList;
}

std::vector<TileCoords> getCrossTiles(const Tile& centerTile, const int startOut, const int squareSize, const EDirection directions)
{
    const int& curRow = centerTile.mRow;
    const int& curCol = centerTile.mCol;
    std::vector <TileCoords> tileCoordsList;

    if (directions == EDirection_LEFT || directions == EDirection_ALL)
    {
        for (int i = startOut + 1; i <= startOut + squareSize; i++)
        {
            tileCoordsList.push_back(TileCoords(curRow, curCol - i));
        }
    }
    if (directions == EDirection_RIGHT || directions == EDirection_ALL)
    {
        for (int i = startOut + 1; i <= startOut + squareSize; i++)
        {
            tileCoordsList.push_back(TileCoords(curRow, curCol + i));
        }
    }
    if (directions == EDirection_UP || directions == EDirection_ALL)
    {
        for (int i = startOut + 1; i <= startOut + squareSize; i++)
        {
            tileCoordsList.push_back(TileCoords(curRow - i, curCol));
        }
    }
    if (directions == EDirection_DOWN || directions == EDirection_ALL)
    {
        for (int i = startOut + 1; i <= startOut + squareSize; i++)
        {
            tileCoordsList.push_back(TileCoords(curRow + i, curCol));
        }
    }
    return tileCoordsList;
}

std::vector<TileCoords> getCheckerboardTiles(const Tile& centerTile, const int startOut, const int squareSize, const EDirection directions)
{
    const int& curRow = centerTile.mRow;
    const int& curCol = centerTile.mCol;
    std::vector <TileCoords> tileCoordsList;

    int rowStart    = -squareSize;
    int rowEnd      = squareSize;
    int colStart    = -squareSize;
    int colEnd      = squareSize;

    switch (directions)
    {
    case EDirection_LEFT:
        colEnd = startOut - 1;
        break;
    case EDirection_RIGHT:
        colStart = startOut + 1;
        break;
    case EDirection_UP:
        rowEnd = startOut - 1;
        break;
    case EDirection_DOWN:
        rowStart = startOut + 1;
        break;
    }

    for (int rowI = rowStart; rowI <= rowEnd; rowI++)
    {
        for (int colI = colStart; colI <= colEnd; colI++)
        {
            if ((std::abs(rowI) < startOut && std::abs(colI) < startOut) ||  std::abs(rowI % 2) == std::abs(colI % 2))
            {
                continue;
            }
            tileCoordsList.push_back(TileCoords(rowI + curRow, colI + curCol));
        }
    }
    return tileCoordsList;
}

bool isPlayableTile(const Tile& givenTile)
{
    switch (givenTile.getType())
    {
    case EMiniGameCombatTileType_IMPASSABLE:
        break;
    case EMiniGameCombatTileType_TELEPORTER:
        //TODO
        break;
    default:
        return true;
        break;
    }
    return false;
}

float getDistanceBetweenTiles(const Tile& tile1, const Tile& tile2)
{
    int distanceRow = abs(tile1.mRow - tile2.mRow);
    int distanceCol = abs(tile1.mCol - tile2.mCol);
    return (float)sqrt(pow(distanceRow, 2) + pow(distanceCol, 2));
}

EDirection getDirectionBetweenTiles(const Tile& tile1, const Tile& tile2)
{
    if (tile1.mRow > tile2.mRow)
    {
        return EDirection_UP;
    }
    else if (tile1.mRow < tile2.mRow)
    {
        return EDirection_DOWN;
    }
    if (tile1.mCol > tile2.mCol)
    {
        return EDirection_LEFT;
    }
    else if (tile1.mCol < tile2.mCol)
    {
        return EDirection_RIGHT;
    }

    return EDirection_NONE;
}
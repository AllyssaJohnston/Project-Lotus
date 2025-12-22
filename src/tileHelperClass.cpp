#include "tileHelperClass.h"
#include "tileHelper.h"
#include "tileCoordsHelper.h"
#include "helperClass.h"


std::string createTileName(int row, int col) { return std::to_string(row) + " " + std::to_string(col); }


std::vector<TileCoords> getSquareTiles(const Tile& centerTile, int startOut, int squareSize, EDirection directions)
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

std::vector<TileCoords> getCrossTiles(const Tile& centerTile, int startOut, int squareSize, EDirection directions)
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

std::vector<TileCoords> getCheckerboardTiles(const Tile& centerTile, int startOut, int squareSize, EDirection directions)
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

std::vector<TileCoords> getGrid(int gridWidth, int gridHeight)
{
    std::vector <TileCoords> tileCoordsList;
    for (int i = 0; i < gridHeight; i++)
    {
        for (int j = 0; j < gridWidth; j++)
        {
            tileCoordsList.push_back(TileCoords(i, j));
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
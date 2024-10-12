#include "tileHelperClass.h"
#include "tileHelper.h"
#include "tileCoordsHelper.h"
#include "helperClass.h"


std::string createTileName(int row, int col)
{
    return std::to_string(row) + " " + std::to_string(col);
}

int getDistanceBetweenTiles(Tile* pTile1, Tile* pTile2)
{
    int distanceRow = abs(pTile1->mRow - pTile2->mRow);
    int distanceCol = abs(pTile1->mCol - pTile2->mCol);
    int distance = (int)sqrt( pow(distanceRow, 2) + pow(distanceCol, 2));
    return distance;
}

std::vector <TileCoords> getSquare1Tiles(Tile* pGivenTile)
{
    int curRow = pGivenTile->mRow;
    int curCol = pGivenTile->mCol;
    std::vector <TileCoords> tileCoordList;
    tileCoordList.reserve(8);
    tileCoordList.push_back(TileCoords(curRow - 1, curCol    ));
    tileCoordList.push_back(TileCoords(curRow - 1, curCol + 1));
    tileCoordList.push_back(TileCoords(curRow,     curCol + 1));
    tileCoordList.push_back(TileCoords(curRow + 1, curCol + 1));

    tileCoordList.push_back(TileCoords(curRow + 1, curCol    ));
    tileCoordList.push_back(TileCoords(curRow + 1, curCol - 1));
    tileCoordList.push_back(TileCoords(curRow,     curCol - 1));
    tileCoordList.push_back(TileCoords(curRow - 1, curCol - 1));
    return tileCoordList;
}

std::vector <TileCoords> getSquare1LeftTiles(Tile* pGivenTile) 
{
    int curRow = pGivenTile->mRow;
    int curCol = pGivenTile->mCol;
    std::vector <TileCoords> tileCoordList;
    tileCoordList.reserve(3);
    tileCoordList.push_back(TileCoords(curRow + 1, curCol - 1));
    tileCoordList.push_back(TileCoords(curRow,     curCol - 1));
    tileCoordList.push_back(TileCoords(curRow - 1, curCol - 1));
    return tileCoordList;
}

std::vector <TileCoords> getSquare1UpTiles(Tile* pGivenTile) 
{
    int curRow = pGivenTile->mRow;
    int curCol = pGivenTile->mCol;
    std::vector <TileCoords> tileCoordList;
    tileCoordList.reserve(3);
    tileCoordList.push_back(TileCoords(curRow - 1, curCol    ));
    tileCoordList.push_back(TileCoords(curRow - 1, curCol + 1));
    tileCoordList.push_back(TileCoords(curRow - 1, curCol - 1));
    return tileCoordList;
}

std::vector <TileCoords> getSquare1RightTiles(Tile* pGivenTile) 
{
    int curRow = pGivenTile->mRow;
    int curCol = pGivenTile->mCol;
    std::vector <TileCoords> tileCoordList;
    tileCoordList.reserve(3);
    tileCoordList.push_back(TileCoords(curRow - 1, curCol + 1));
    tileCoordList.push_back(TileCoords(curRow,     curCol + 1));
    tileCoordList.push_back(TileCoords(curRow + 1, curCol + 1));
    return tileCoordList;
}

std::vector <TileCoords> getSquare1DownTiles(Tile* pGivenTile) 
{
    int curRow = pGivenTile->mRow;
    int curCol = pGivenTile->mCol;
    std::vector <TileCoords> tileCoordList;
    tileCoordList.reserve(3);
    tileCoordList.push_back(TileCoords(curRow + 1, curCol + 1));
    tileCoordList.push_back(TileCoords(curRow + 1, curCol    ));
    tileCoordList.push_back(TileCoords(curRow + 1, curCol - 1));
    return tileCoordList;
}

std::vector <TileCoords> getSquare2Tiles(Tile* pGivenTile)
{
    int curRow = pGivenTile->mRow;
    int curCol = pGivenTile->mCol;
    std::vector <TileCoords> tileCoordList;
    tileCoordList.reserve(24);
    tileCoordList.push_back(TileCoords(curRow - 1, curCol    ));
    tileCoordList.push_back(TileCoords(curRow - 2, curCol    ));
    tileCoordList.push_back(TileCoords(curRow - 1, curCol + 1));
    tileCoordList.push_back(TileCoords(curRow - 1, curCol + 2));

    tileCoordList.push_back(TileCoords(curRow - 2, curCol + 2));
    tileCoordList.push_back(TileCoords(curRow - 2, curCol + 1));
    tileCoordList.push_back(TileCoords(curRow,     curCol + 1));
    tileCoordList.push_back(TileCoords(curRow,     curCol + 2));

    tileCoordList.push_back(TileCoords(curRow + 1, curCol + 1));
    tileCoordList.push_back(TileCoords(curRow + 1, curCol + 2));
    tileCoordList.push_back(TileCoords(curRow + 2, curCol + 2));
    tileCoordList.push_back(TileCoords(curRow + 2, curCol + 1));

    tileCoordList.push_back(TileCoords(curRow + 1, curCol    ));
    tileCoordList.push_back(TileCoords(curRow + 2, curCol    ));
    tileCoordList.push_back(TileCoords(curRow + 1, curCol - 1));
    tileCoordList.push_back(TileCoords(curRow + 1, curCol - 2));

    tileCoordList.push_back(TileCoords(curRow + 2, curCol - 2));
    tileCoordList.push_back(TileCoords(curRow + 2, curCol - 1));
    tileCoordList.push_back(TileCoords(curRow,     curCol - 1));
    tileCoordList.push_back(TileCoords(curRow,     curCol - 2));

    tileCoordList.push_back(TileCoords(curRow - 1, curCol - 1));
    tileCoordList.push_back(TileCoords(curRow - 1, curCol - 2));
    tileCoordList.push_back(TileCoords(curRow - 2, curCol - 2));
    tileCoordList.push_back(TileCoords(curRow - 2, curCol - 1));
    return tileCoordList;
}

std::vector <TileCoords> getSquare2LeftTiles(Tile* pGivenTile) 
{
    int curRow = pGivenTile->mRow;
    int curCol = pGivenTile->mCol;
    std::vector <TileCoords> tileCoordList;
    tileCoordList.reserve(10);
    tileCoordList.push_back(TileCoords(curRow + 1, curCol - 1));
    tileCoordList.push_back(TileCoords(curRow + 1, curCol - 2));
    tileCoordList.push_back(TileCoords(curRow + 2, curCol - 2));
    tileCoordList.push_back(TileCoords(curRow + 2, curCol - 1));

    tileCoordList.push_back(TileCoords(curRow,     curCol - 1));
    tileCoordList.push_back(TileCoords(curRow,     curCol - 2));
    tileCoordList.push_back(TileCoords(curRow - 1, curCol - 1));
    tileCoordList.push_back(TileCoords(curRow - 1, curCol - 2));

    tileCoordList.push_back(TileCoords(curRow - 2, curCol - 2));
    tileCoordList.push_back(TileCoords(curRow - 2, curCol - 1));
    return tileCoordList;
}

std::vector <TileCoords> getSquare2UpTiles(Tile* pGivenTile) 
{
    int curRow = pGivenTile->mRow;
    int curCol = pGivenTile->mCol;
    std::vector <TileCoords> tileCoordList;
    tileCoordList.reserve(10);
    tileCoordList.push_back(TileCoords(curRow - 1, curCol    ));
    tileCoordList.push_back(TileCoords(curRow - 2, curCol    ));
    tileCoordList.push_back(TileCoords(curRow - 1, curCol + 1));
    tileCoordList.push_back(TileCoords(curRow - 1, curCol + 2));

    tileCoordList.push_back(TileCoords(curRow - 2, curCol + 2));
    tileCoordList.push_back(TileCoords(curRow - 2, curCol + 1));
    tileCoordList.push_back(TileCoords(curRow - 1, curCol - 1));
    tileCoordList.push_back(TileCoords(curRow - 1, curCol - 2));

    tileCoordList.push_back(TileCoords(curRow - 2, curCol - 2));
    tileCoordList.push_back(TileCoords(curRow - 2, curCol - 1));
    return tileCoordList;
}

std::vector <TileCoords> getSquare2RightTiles(Tile* pGivenTile) 
{
    int curRow = pGivenTile->mRow;
    int curCol = pGivenTile->mCol;
    std::vector <TileCoords> tileCoordList;
    tileCoordList.reserve(10);
    tileCoordList.push_back(TileCoords(curRow - 1, curCol + 1));
    tileCoordList.push_back(TileCoords(curRow - 1, curCol + 2));
    tileCoordList.push_back(TileCoords(curRow - 2, curCol + 2));
    tileCoordList.push_back(TileCoords(curRow - 2, curCol + 1));

    tileCoordList.push_back(TileCoords(curRow,     curCol + 1));
    tileCoordList.push_back(TileCoords(curRow,     curCol + 2));
    tileCoordList.push_back(TileCoords(curRow + 1, curCol - 1));
    tileCoordList.push_back(TileCoords(curRow + 1, curCol - 2));

    tileCoordList.push_back(TileCoords(curRow + 2, curCol - 2));
    tileCoordList.push_back(TileCoords(curRow + 2, curCol - 1));
    return tileCoordList;
}

std::vector <TileCoords> getSquare2DownTiles(Tile* pGivenTile) 
{
    int curRow = pGivenTile->mRow;
    int curCol = pGivenTile->mCol;
    std::vector <TileCoords> tileCoordList;
    tileCoordList.reserve(10);
    tileCoordList.push_back(TileCoords(curRow + 1, curCol + 1));
    tileCoordList.push_back(TileCoords(curRow + 1, curCol + 2));
    tileCoordList.push_back(TileCoords(curRow + 2, curCol + 2));
    tileCoordList.push_back(TileCoords(curRow + 2, curCol + 1));

    tileCoordList.push_back(TileCoords(curRow + 1, curCol    ));
    tileCoordList.push_back(TileCoords(curRow + 2, curCol    ));
    tileCoordList.push_back(TileCoords(curRow + 1, curCol - 1));
    tileCoordList.push_back(TileCoords(curRow + 1, curCol - 2));

    tileCoordList.push_back(TileCoords(curRow + 2, curCol - 2));
    tileCoordList.push_back(TileCoords(curRow + 2, curCol - 1));
    return tileCoordList;
}

std::vector <TileCoords> getCross1Tiles(Tile* pGivenTile)
{
    int curRow = pGivenTile->mRow;
    int curCol = pGivenTile->mCol;
    std::vector <TileCoords> tileCoordList;
    tileCoordList.reserve(4);
    tileCoordList.push_back(TileCoords(curRow - 1, curCol    ));
    tileCoordList.push_back(TileCoords(curRow,     curCol + 1));
    tileCoordList.push_back(TileCoords(curRow + 1, curCol    ));
    tileCoordList.push_back(TileCoords(curRow,     curCol - 1));
    return tileCoordList;
}

std::vector <TileCoords> getCross2Tiles(Tile* pGivenTile)
{
    int curRow = pGivenTile->mRow;
    int curCol = pGivenTile->mCol;
    std::vector <TileCoords> tileCoordList;
    tileCoordList.reserve(8);
    tileCoordList.push_back(TileCoords(curRow - 1, curCol    ));
    tileCoordList.push_back(TileCoords(curRow - 2, curCol    ));
    tileCoordList.push_back(TileCoords(curRow,     curCol + 1));
    tileCoordList.push_back(TileCoords(curRow,     curCol + 2));

    tileCoordList.push_back(TileCoords(curRow + 1, curCol    ));
    tileCoordList.push_back(TileCoords(curRow + 2, curCol    ));
    tileCoordList.push_back(TileCoords(curRow,     curCol - 1));
    tileCoordList.push_back(TileCoords(curRow,     curCol - 2));
    return tileCoordList;
}

EDirection getDirectionBetweenTiles(Tile* tile1, Tile* tile2)
{
    if (tile1->mRow > tile2->mRow)
    {
        return EDirection_UP;
    }
    else if (tile1->mRow < tile2->mRow)
    {
        return EDirection_DOWN;
    }
    if (tile1->mCol > tile2->mCol)
    {
        return EDirection_LEFT;
    }
    else if (tile1->mCol < tile2->mCol)
    {
        return EDirection_RIGHT;
    }

    return EDirection_NONE;
}
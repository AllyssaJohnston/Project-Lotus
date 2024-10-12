#pragma once
#include "helperClass.h"
#include "tileHelper.h"
#include "tileCoordsHelper.h"
#include "combatHelperClass.h"

std::string createTileName(int row, int col);

int getDistanceBetweenTiles(Tile* pTile1, Tile* pTile2);

std::vector <TileCoords> getSquare1Tiles(Tile* pGivenTile);

std::vector <TileCoords> getSquare1LeftTiles(Tile* pGivenTile);

std::vector <TileCoords> getSquare1UpTiles(Tile* pGivenTile);

std::vector <TileCoords> getSquare1RightTiles(Tile* pGivenTile);

std::vector <TileCoords> getSquare1DownTiles(Tile* pGivenTile);

std::vector <TileCoords> getSquare2Tiles(Tile* pGivenTile);

std::vector <TileCoords> getSquare2LeftTiles(Tile* pGivenTile);

std::vector <TileCoords> getSquare2UpTiles(Tile* pGivenTile);

std::vector <TileCoords> getSquare2RightTiles(Tile* pGivenTile);

std::vector <TileCoords> getSquare2DownTiles(Tile* pGivenTile);

std::vector <TileCoords> getCross1Tiles(Tile* pGivenTile);

std::vector <TileCoords> getCross2Tiles(Tile* pGivenTile);

EDirection getDirectionBetweenTiles(Tile * tile1, Tile * tile2);
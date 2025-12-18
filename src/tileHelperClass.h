#pragma once
#include "helperClass.h"
#include "tileHelper.h"
#include "tileCoordsHelper.h"
#include "combatHelperClass.h"

std::string createTileName(int row, int col);

std::vector<TileCoords> getSquareTiles(const Tile& centerTile, int startOut, int numOut, EDirection directions);

std::vector<TileCoords> getCrossTiles(const Tile& centerTile, int startOut, int numOut, EDirection directions);

bool isPlayableTile(const Tile& gGivenTile);

int getDistanceBetweenTiles(const Tile& tile1, const Tile& tile2);

EDirection getDirectionBetweenTiles(const Tile& tile1, const Tile& tile2);
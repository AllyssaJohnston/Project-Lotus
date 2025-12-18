#pragma once

#include "tileCoordsHelper.h"

std::vector <TileCoords> removeDuplicateTiles(std::vector <TileCoords>& givenList);

bool inTileCoordList(const std::vector <TileCoords>& givenList, const TileCoords& curTileCoords);

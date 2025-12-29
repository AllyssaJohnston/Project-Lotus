#pragma once
#include "helpers.h"
#include "tile.h"
#include "tileCoords.h"
#include "combatHelpers.h"

std::vector <TileCoords> returnTileCoords(const Tile& givenTile, const EMiniGameCombatMoveAttackTypes moveAttackType, const int num, const int out, const EDirection direction);

std::vector<TileCoords> getSquareTiles(const Tile& centerTile, const int startOut, const int numOut, const EDirection directions);

std::vector<TileCoords> getCrossTiles(const Tile& centerTile, const int startOut, const int numOut, const EDirection directions);

std::vector<TileCoords> getCheckerboardTiles(const Tile& centerTile, const int startOut, const int numOut, const EDirection directions);

bool isPlayableTile(const Tile& gGivenTile);

float getDistanceBetweenTiles(const Tile& tile1, const Tile& tile2);

EDirection getDirectionBetweenTiles(const Tile& tile1, const Tile& tile2);


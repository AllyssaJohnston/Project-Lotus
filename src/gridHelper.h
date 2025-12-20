#pragma once

#include "styleHelper.h"
#include "hitboxHelper.h"
#include "tileHelper.h"
#include "tileHelperClass.h"

class Grid
{
public:
    const int           mNumRows = 16;
    const int           mNumCols = 16;

    const int           mStartX = 20;
    const int           mStartY = 375;
    const int           mTileWidth  = 25;
    const int           mTileHeight = 25;
    const int           mSpacing = 5;

    std::vector <Tile*> mpTiles;


    Grid();

    void preTick();

    int getIndex(int x, int y) const;

    Tile* getTileFromCoords(int mouseX, int mouseY) const;

    void createGrid();

    void resetTileColors();

    void resetAllTilesModes(EMiniGameCombatTileMode tileMode);

    bool isLegalCoords(int row, int col) const;

    void printGrid(SDL_Renderer* pRenderer, float gameScreenToGameLevelChunkRatio) const;
};

Tile* findTile(const Grid& grid, const TileCoords& tileCoords);

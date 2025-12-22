#pragma once

#include "styleHelper.h"
#include "hitboxHelper.h"
#include "tileHelper.h"
#include "tileHelperClass.h"

class Grid
{
private:
    Tile* mpMouseTile = nullptr; // tile under mouse pointer
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

    Tile* findTile(const TileCoords& tileCoords) const;

    void createGrid();

    void setMouseTile(int mouseX, int mouseY);

    void setMouseTileMode(EMiniGameCombatTileMode mode);

    Tile* getMouseTile() const;

    void resetTileColors();

    void resetAllTilesModes(EMiniGameCombatTileMode tileMode);

    bool isLegalCoords(int row, int col) const;

    void printGrid(SDL_Renderer* pRenderer, float gameScreenToGameLevelChunkRatio) const;
};



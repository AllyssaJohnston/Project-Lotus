#pragma once
#include "tile.h"
#include "tileHelpers.h"
#include "tileCoords.h"

class Grid
{
private:
    Tile* mpMouseTile = nullptr; // tile under mouse pointer
    bool mIsSetup = false;
public:
    int         mNumRows = -1;
    int         mNumCols = -1;

    const int   mStartX = 20;
    const int   mStartY = 375;
    const int   mTileWidth  = 25;
    const int   mTileHeight = 25;
    const int   mSpacing = 5;

    std::vector <Tile*> mpTiles;


    Grid() { ; }

    void createGrid(int numRows, int numCols);

    void preTick();

    int getIndex(int x, int y) const;

    Tile* findTile(const TileCoords& tileCoords) const;

   

    void setMouseTile(int mouseX, int mouseY);

    void setMouseTileMode(ECombatTileMode mode);

    Tile* getMouseTile() const;

    void resetTileColors();

    void resetAllTilesModes(ECombatTileMode tileMode);

    bool isLegalCoords(int row, int col) const;

    void printGrid(SDL_Renderer* pRenderer, float gameScreenToGameLevelChunkRatio) const;

    bool isSetUp() const;
};



#pragma once

#include "styleHelper.h"
#include "hitboxHelper.h"
#include "tileHelper.h"
#include "tileHelperClass.h"

class Grid
{
private:
    StyleManager        mStyleManager;
public:
    const int           mNumRows = 16;
    const int           mNumCols = 16;

    const int           mStartX = 40;
    const int           mStartY = 350;
    const int           mTileWidth  = 25;
    const int           mTileHeight = 25;
    const int           mSpacing = 5;

    std::vector <Tile*> mpTiles;


    Grid();

    void preTick();

    int getIndex(int x, int y) const;

    Tile* getTileFromCoords(int mouseX, int mouseY);

    void createGrid();

    void resetTileColors();

    void resetAllTilesModes(EMiniGameCombatTileMode tileMode);

    //void addEntityToTile(TileEntity & tileEntity)

    //void removeEntityFromTile(TileEntity & tileEntity)

    bool isLegalCoords(int row, int col) const;

    void printGrid(SDL_Renderer* pRenderer, float gameScreenToGameLevelChunkRatio) const;
};
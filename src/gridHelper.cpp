#include "gridHelper.h"

Grid::Grid() { createGrid(); }

void Grid::preTick()
{
    for (Tile* pTile : mpTiles)
    {
        pTile->preTick();
    }
    resetTileColors();
    resetAllTilesModes(EMiniGameCombatTileMode_NOT_SELECTED);
}

int Grid::getIndex(int x, int y) const { return x * (mNumCols) + y; }

Tile* Grid::getTileFromCoords(int mouseX, int mouseY) const
{

    int endX = mStartX + ((mTileWidth  + mSpacing) * mNumCols);
    int endY = mStartY + ((mTileHeight + mSpacing) * mNumRows);
    if ((mouseX < mStartX) or (mouseX >= endX))
    {
        return nullptr;
    }

    if ((mouseY < mStartY) or ( mouseY >= endY )) 
    {
        return nullptr;
    }

    int row = (mouseY - mStartY) / (mTileHeight + mSpacing);
    int col = (mouseX - mStartX) / (mTileWidth  + mSpacing);

    return mpTiles[getIndex(row, col)];
}

void Grid::createGrid()
{
    int curY = mStartY;
    int curX = 0;
    RegularTilePreset preset = RegularTilePreset();
    for (int row = 0; row < mNumRows; row++)
    {
        curX = mStartX;
        for (int col = 0; col < mNumCols; col++)
        {
            mpTiles.push_back(new Tile(createTileName(row, col), row, col, CoordsX1Y1WidthHeight(curX, curY, mTileWidth, mTileHeight), preset));
            curX += mTileWidth + mSpacing;
        }
        curY += mTileHeight + mSpacing;
    }
}

void Grid::resetTileColors()
{
    for (Tile* pTile : mpTiles)
    {
        pTile->resetCurColor();
    }
}

void Grid::resetAllTilesModes(EMiniGameCombatTileMode tileMode)
{
    for (Tile* pTile : mpTiles)
    {
        if (pTile->getLastFrameMode() == EMiniGameCombatTileMode_SELECTED)
        {
            pTile->setMode(tileMode);
        }
    }
}

bool Grid::isLegalCoords(int row, int col) const { return (row >= 0 and row < mNumRows and col >= 0 and col < mNumCols); }

void Grid::printGrid(SDL_Renderer* pRenderer, float gameScreenToGameLevelChunkRatio) const
{
    for (Tile* pCurTile : mpTiles)
    {
        SDL_Color& curColor = pCurTile->mCurColor;
        SDL_FRect curTileRect {float(pCurTile->mCoords.mX1) * gameScreenToGameLevelChunkRatio, float(pCurTile->mCoords.mY1) * gameScreenToGameLevelChunkRatio, float(pCurTile->mCoords.mWidth) * gameScreenToGameLevelChunkRatio, float(pCurTile->mCoords.mHeight) * gameScreenToGameLevelChunkRatio};
        SDL_SetRenderDrawColor(pRenderer, curColor.r, curColor.g, curColor.b, curColor.a);
        SDL_RenderFillRect(pRenderer, &curTileRect);
    }
}

Tile* findTile(const Grid& grid, const TileCoords& tileCoords)
{
    if (grid.isLegalCoords(tileCoords.mRow, tileCoords.mCol))
    {
        return grid.mpTiles[grid.getIndex(tileCoords.mRow, tileCoords.mCol)];
    }
    return nullptr;
}
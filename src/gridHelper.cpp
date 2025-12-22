#include "gridHelper.h"

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

Tile* Grid::findTile(const TileCoords& tileCoords) const
{
    if (isLegalCoords(tileCoords.mRow, tileCoords.mCol))
    {
        return mpTiles[getIndex(tileCoords.mRow, tileCoords.mCol)];
    }
    return nullptr;
}

void Grid::createGrid(int numRows, int numCols)
{
    mNumRows = numRows;
    mNumCols = numCols;
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
    mIsSetup = true;
}

void Grid::setMouseTile(int mouseX, int mouseY)
{
    int endX = mStartX + ((mTileWidth + mSpacing) * mNumCols);
    int endY = mStartY + ((mTileHeight + mSpacing) * mNumRows);
    if ((mouseX < mStartX) or (mouseX >= endX) || (mouseY < mStartY) or (mouseY >= endY))
    {
        // out of range
        mpMouseTile = nullptr;
        return;
    }

    int row = (mouseY - mStartY) / (mTileHeight + mSpacing);
    int col = (mouseX - mStartX) / (mTileWidth + mSpacing);

    mpMouseTile = mpTiles[getIndex(row, col)];
}

void Grid::setMouseTileMode(EMiniGameCombatTileMode mode)
{
    if (mpMouseTile != nullptr)
    {
        mpMouseTile->setMode(mode);
    }
}

Tile* Grid::getMouseTile() const { return mpMouseTile; }

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

bool Grid::isSetUp() const { return mIsSetup; }

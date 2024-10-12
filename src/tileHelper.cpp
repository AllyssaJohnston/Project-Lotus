#include "tileHelper.h"

Tile::Tile(std::string name, int row, int col, CoordsX1Y1WidthHeight coords, TilePreset* preset)
{
    mName    = name;
    mType    = preset->mType;
    mRow     = row;
    mCol     = col;
    mCoords  = coords;
    mpPreset = preset;
    mRegularColor = preset->mRegularColor;
    mCurColor     = preset->mRegularColor;
}

void Tile::preTick()
{
    mLastFrameMode = mCurMode;
}

void Tile::setMode(EMiniGameCombatTileMode newMode)
{
    mCurMode = newMode;
}

EMiniGameCombatTileMode Tile::getLastFrameMode() const
{
    return mLastFrameMode;
}

EMiniGameCombatTileMode Tile::getMode() const
{
    return mCurMode;
}

void Tile::setCurColor(SDL_Color* newColor)
{
    mCurColor = *newColor;
}

void Tile::resetCurColor()
{
    mCurColor = mRegularColor;
}

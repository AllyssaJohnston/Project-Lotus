#include "tile.h"

Tile::Tile(const std::string name, int row, int col, const CoordsX1Y1WidthHeight coords, const TilePreset& preset)
    : mName(name), mType(preset.mType), mRow(row), mCol(col), mCoords(coords),
    mRegularColor(preset.mRegularColor), mCurColor(preset.mRegularColor) { ; }

void Tile::preTick()
{
    mLastFrameMode = mCurMode;
}

void Tile::setMode(EMiniGameCombatTileMode newMode) { mCurMode = newMode; }

EMiniGameCombatTileMode Tile::getLastFrameMode() const { return mLastFrameMode; }

EMiniGameCombatTileMode Tile::getMode() const { return mCurMode; }

void Tile::setType(const TilePreset& preset)
{
    mRegularColor = preset.mRegularColor;
    mCurColor = preset.mRegularColor;
    mType = preset.mType;
}

void Tile::setType(EMiniGameCombatTileType type, const SDL_Color& regularColor) 
{
    mRegularColor = regularColor;
    mCurColor = regularColor;
    mType = type;
}

EMiniGameCombatTileType Tile::getType() const { return mType; }

void Tile::setCurColor(const SDL_Color& newColor) { mCurColor = newColor; }

void Tile::resetCurColor() { mCurColor = mRegularColor; }

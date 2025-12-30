#pragma once
#include <SDL3/SDL.h>
#include <string>
#include "hitbox.h"
#include "tilePresets.h"
#include "combatHelpers.h"

class Tile
{
private:
    SDL_Color               mRegularColor;
    ECombatTileMode mLastFrameMode  = ECombatTileMode_NOT_SELECTED;
    ECombatTileMode mCurMode        = ECombatTileMode_NOT_SELECTED;
    ECombatTileType mType           = ECombatTileType_INVALID;

public:
    std::string             mName;
   
    int                     mRow    = -1;
    int                     mCol    = -1;
    CoordsX1Y1WidthHeight   mCoords;

    SDL_Color               mCurColor;

    Tile(const std::string name, int row, int col, const CoordsX1Y1WidthHeight coords, const TilePreset& preset);

    void preTick();

    void setMode(ECombatTileMode newMode);

    ECombatTileMode getLastFrameMode() const;

    ECombatTileMode getMode() const;

    void setType(const TilePreset& preset);
    void setType(ECombatTileType type, const SDL_Color& regularColor);

    ECombatTileType getType() const;

    void setCurColor(const SDL_Color& newColor);

    void resetCurColor();
};
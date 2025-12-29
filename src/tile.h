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
    EMiniGameCombatTileMode mLastFrameMode  = EMiniGameCombatTileMode_NOT_SELECTED;
    EMiniGameCombatTileMode mCurMode        = EMiniGameCombatTileMode_NOT_SELECTED;
    EMiniGameCombatTileType mType           = EMiniGameCombatTileType_INVALID;

public:
    std::string             mName;
   
    int                     mRow    = -1;
    int                     mCol    = -1;
    CoordsX1Y1WidthHeight   mCoords;

    SDL_Color               mCurColor;

    Tile(const std::string name, int row, int col, const CoordsX1Y1WidthHeight coords, const TilePreset& preset);

    void preTick();

    void setMode(EMiniGameCombatTileMode newMode);

    EMiniGameCombatTileMode getLastFrameMode() const;

    EMiniGameCombatTileMode getMode() const;

    void setType(const TilePreset& preset);
    void setType(EMiniGameCombatTileType type, const SDL_Color& regularColor);

    EMiniGameCombatTileType getType() const;

    void setCurColor(const SDL_Color& newColor);

    void resetCurColor();
};
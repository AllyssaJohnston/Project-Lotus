#pragma once
#include <SDL3/SDL.h>
#include <string>
#include "hitboxHelper.h"
#include "tilePresets.h"
#include "combatHelperClass.h"

class Tile
{
private:
    TilePreset * mpPreset                   = nullptr;
    SDL_Color    mRegularColor;
    EMiniGameCombatTileMode mLastFrameMode  = EMiniGameCombatTileMode_NOT_SELECTED;
    EMiniGameCombatTileMode mCurMode        = EMiniGameCombatTileMode_NOT_SELECTED;

    //CombatEntity * mCurEntity;

public:
    std::string             mName;
    EMiniGameCombatTileType mType   = EMiniGameCombatTileType_INVALID;
    int                     mRow    = -1;
    int                     mCol    = -1;
    CoordsX1Y1WidthHeight   mCoords;

    SDL_Color               mCurColor;

    Tile(std::string name, int row, int col, CoordsX1Y1WidthHeight coords, TilePreset* preset);

    void preTick();

    void setMode(EMiniGameCombatTileMode newMode);

    EMiniGameCombatTileMode getLastFrameMode() const;

    EMiniGameCombatTileMode getMode() const;

    void setCurColor(SDL_Color* newColor);

    void resetCurColor();

    //void setCurEntity(CombatEntity & newEntity) 

};
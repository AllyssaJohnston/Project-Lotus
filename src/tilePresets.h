#pragma once
#include <SDL3/SDL.h>
#include "combatHelperClass.h"
#include "styleHelper.h"
#include "tileCoordsHelper.h"

class TilePreset
{
public:
    EMiniGameCombatTileType mType           = EMiniGameCombatTileType_INVALID;
    SDL_Color               mRegularColor   = SDL_Color();

    
    TilePreset() {}
};

class RegularTilePreset : public TilePreset
{
public:
    RegularTilePreset();
};

class ImpassableTilePreset : public TilePreset
{
public:
    ImpassableTilePreset();
};

class TeleporterTilePreset : public TilePreset
{
public:
    TileCoords * mpTileToMoveTo;
    TeleporterTilePreset(TileCoords * pTileToMoveTo);
};
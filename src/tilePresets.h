#pragma once
#include <SDL3/SDL.h>
#include "combatHelperClass.h"
#include "styleHelper.h"
#include "tileCoordsHelper.h"

struct TilePreset
{
    EMiniGameCombatTileType mType           = EMiniGameCombatTileType_INVALID;
    SDL_Color               mRegularColor   = SDL_Color();

    TilePreset() {}
};

struct RegularTilePreset : public TilePreset { RegularTilePreset(); };

struct ImpassableTilePreset : public TilePreset { ImpassableTilePreset(); };

struct TeleporterTilePreset : public TilePreset
{
    TileCoords* mpTileToMoveTo;
    TeleporterTilePreset(TileCoords* pTileToMoveTo);
};
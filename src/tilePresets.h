#pragma once
#include <SDL3/SDL.h>
#include "combatHelpers.h"
#include "styleManager.h"
#include "tileCoords.h"

struct TilePreset
{
    ECombatTileType mType           = ECombatTileType_INVALID;
    SDL_Color       mRegularColor   = SDL_Color();

    TilePreset() { ; }
};

struct RegularTilePreset : public TilePreset { RegularTilePreset(); };

struct ImpassableTilePreset : public TilePreset { ImpassableTilePreset(); };

struct TeleporterTilePreset : public TilePreset
{
    TileCoords* mpTileToMoveTo;
    TeleporterTilePreset(TileCoords* pTileToMoveTo);
};
#include "tilePresets.h"

RegularTilePreset::RegularTilePreset() : TilePreset()
{
    mType           = EMiniGameCombatTileType_NORMAL;
    mRegularColor   = StyleManager::oliveGreen;
}

ImpassableTilePreset::ImpassableTilePreset() : TilePreset()
{
    mType           = EMiniGameCombatTileType_IMPASSABLE;
    mRegularColor   = StyleManager::black;
}

TeleporterTilePreset::TeleporterTilePreset(TileCoords * pTileToMoveTo) : TilePreset()
{
    mType = EMiniGameCombatTileType_TELEPORTER;
    mRegularColor = StyleManager::navy;
    mpTileToMoveTo = pTileToMoveTo;
}
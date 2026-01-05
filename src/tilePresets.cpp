#include "tilePresets.h"

RegularTilePreset::RegularTilePreset() : TilePreset()
{
    mType           = ECombatTileType_NORMAL;
    mRegularColor   = StyleManager::oliveGreen;
}

ImpassableTilePreset::ImpassableTilePreset() : TilePreset()
{
    mType           = ECombatTileType_IMPASSABLE;
    mRegularColor   = StyleManager::gray;
}

TeleporterTilePreset::TeleporterTilePreset(TileCoords* pTileToMoveTo) : TilePreset()
{
    mType = ECombatTileType_TELEPORTER;
    mRegularColor = StyleManager::navy;
    mpTileToMoveTo = pTileToMoveTo;
}
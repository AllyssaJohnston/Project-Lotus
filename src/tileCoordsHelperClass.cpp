#include "tileCoordsHelperClass.h"

std::vector <TileCoords> removeDuplicateTiles(std::vector <TileCoords>& givenList)
{
	std::vector <TileCoords> uniqueList;
	for (TileCoords& tileCoord : givenList)
	{
		if (!inTileCoordList(uniqueList, tileCoord))
		{
			uniqueList.push_back(tileCoord);
		}
	}
	return uniqueList;
}

bool inTileCoordList(const std::vector <TileCoords>& givenList, const TileCoords& givenTileCoords)
{
	for (const TileCoords& curTileCoord : givenList)
	{
		if ((curTileCoord.mRow == givenTileCoords.mRow) and (curTileCoord.mCol == givenTileCoords.mCol))
		{
			return true;
		}
	}
	return false;
}

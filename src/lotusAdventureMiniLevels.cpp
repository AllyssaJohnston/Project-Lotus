#include "lotusAdventureMiniLevels.h"

void createMiniGameLevels(MiniGameWorldData& worldData)
{
	bool build = true;

	// LEVEL 0
	if (build)
	{
		Grid* pGrid = new Grid();
		CombatManager* pCombatManager = new CombatManager();
		int levelNumber = 0;
		
		pCombatManager->mpAllCombatCharacters.push_back(new CombatCharacter("LOTUS", pGrid->mpTiles[pGrid->getIndex(0, 8)], new PlayerCombatPreset()));
		pCombatManager->mpAllCombatCharacters.push_back(new CombatCharacter("DRAGAUR", pGrid->mpTiles[pGrid->getIndex(2, 5)], new RatCombatPreset()));

		worldData.mpMiniGameLevels.push_back(new MiniGameLevel(levelNumber, *pGrid, *pCombatManager, LevelData(levelNumber + 1)));
	}

	// LEVEL 1
	if (build)
	{
		Grid* pGrid = new Grid();
		CombatManager* pCombatManager = new CombatManager();
		int levelNumber = 1;
		
		pCombatManager->mpAllCombatCharacters.push_back(new CombatCharacter("LOTUS", pGrid->mpTiles[pGrid->getIndex(1, 1)], new PlayerCombatPreset()));
		pCombatManager->mpAllCombatCharacters.push_back(new CombatCharacter("DRAGAUR", pGrid->mpTiles[pGrid->getIndex(1, 6)], new RatCombatPreset()));

		pGrid->mpTiles[1][3].mType = EMiniGameCombatTileType_IMPASSABLE;
		pGrid->mpTiles[3][4].mType = EMiniGameCombatTileType_IMPASSABLE;

		worldData.mpMiniGameLevels.push_back(new MiniGameLevel(levelNumber, *pGrid, *pCombatManager, LevelData(levelNumber + 1)));
	}

	// LEVEL 2
	if (build)
	{
		Grid* pGrid = new Grid();
		CombatManager* pCombatManager = new CombatManager();
		int levelNumber = 2;

		pCombatManager->mpAllCombatCharacters.push_back(new CombatCharacter("LOTUS",		pGrid->mpTiles[pGrid->getIndex(1, 1)], new PlayerCombatPreset()));
		pCombatManager->mpAllCombatCharacters.push_back(new CombatCharacter("DRAGAUR 1",	pGrid->mpTiles[pGrid->getIndex(1, 6)], new RatCombatPreset()));
		pCombatManager->mpAllCombatCharacters.push_back(new CombatCharacter("DRAGAUR 2",	pGrid->mpTiles[pGrid->getIndex(4, 1)], new RatCombatPreset()));

		pGrid->mpTiles[2][3].mType = EMiniGameCombatTileType_IMPASSABLE;
		pGrid->mpTiles[3][4].mType = EMiniGameCombatTileType_IMPASSABLE;
		pGrid->mpTiles[5][6].mType = EMiniGameCombatTileType_IMPASSABLE;

		worldData.mpMiniGameLevels.push_back(new MiniGameLevel(levelNumber, *pGrid, *pCombatManager, LevelData(GO_TO_MAIN_WORLD, GO_TO_MAIN_WORLD)));
	}

	// LEVEL 3
	if (build)
	{
		Grid* pGrid = new Grid();
		CombatManager* pCombatManager = new CombatManager();
		int levelNumber = 3;

		pCombatManager->mpAllCombatCharacters.push_back(new CombatCharacter("LOTUS",		pGrid->mpTiles[pGrid->getIndex(1, 1)], new PlayerCombatPreset()));
		pCombatManager->mpAllCombatCharacters.push_back(new CombatCharacter("DRAGAUR",		pGrid->mpTiles[pGrid->getIndex(1, 6)], new RatCombatPreset()));
		pCombatManager->mpAllCombatCharacters.push_back(new CombatCharacter("FROCODILE",	pGrid->mpTiles[pGrid->getIndex(4, 1)], new FrogCombatPreset()));

		pGrid->mpTiles[2][3].mType = EMiniGameCombatTileType_IMPASSABLE;
		pGrid->mpTiles[3][4].mType = EMiniGameCombatTileType_IMPASSABLE;
		pGrid->mpTiles[5][6].mType = EMiniGameCombatTileType_IMPASSABLE;

		worldData.mpMiniGameLevels.push_back(new MiniGameLevel(levelNumber, *pGrid, *pCombatManager, LevelData(GO_TO_MAIN_WORLD, GO_TO_MAIN_WORLD)));
	}
}
#include "lotusAdventureMiniLevels.h"

void createMiniGameLevels(MiniGameWorldData& worldData)
{
	bool build = true;
	PlayerCombatPreset	playerPreset	= PlayerCombatPreset();;
	RatCombatPreset		ratPreset		= RatCombatPreset();
	FrogCombatPreset	frogPreset		= FrogCombatPreset();

	ImpassableTilePreset impassiblePreset = ImpassableTilePreset();

	// LEVEL 0
	if (build)
	{
		int levelNumber = 0;
		MiniGameLevel* pLevel = new MiniGameLevel(levelNumber, LevelData(levelNumber + 1));
		CombatManager& combatManager = pLevel->mCombatManager;
		Grid& grid = pLevel->mGrid;
		
		combatManager.mpAllCombatCharacters.push_back(new CombatCharacter("LOTUS",		grid.mpTiles[grid.getIndex(0, 8)], playerPreset));
		combatManager.mpAllCombatCharacters.push_back(new CombatCharacter("DRAGAUR",	grid.mpTiles[grid.getIndex(2, 5)], ratPreset));

		worldData.mpMiniGameLevels.push_back(pLevel);
		pLevel = nullptr;
	}

	// LEVEL 1
	if (build)
	{
		int levelNumber = 1;
		MiniGameLevel* pLevel = new MiniGameLevel(levelNumber, LevelData(levelNumber + 1));
		CombatManager& combatManager = pLevel->mCombatManager;
		Grid& grid = pLevel->mGrid;
		
		
		combatManager.mpAllCombatCharacters.push_back(new CombatCharacter("LOTUS",		grid.mpTiles[grid.getIndex(1, 1)], playerPreset));
		combatManager.mpAllCombatCharacters.push_back(new CombatCharacter("DRAGAUR",	grid.mpTiles[grid.getIndex(1, 6)], ratPreset));

		grid.mpTiles[grid.getIndex(1, 3)]->setType(impassiblePreset);
		grid.mpTiles[grid.getIndex(3, 4)]->setType(impassiblePreset);

		worldData.mpMiniGameLevels.push_back(pLevel);
		pLevel = nullptr;
	}

	// LEVEL 2
	if (build)
	{
		int levelNumber = 2;
		MiniGameLevel* pLevel = new MiniGameLevel(levelNumber, LevelData(GO_TO_MAIN_WORLD, GO_TO_MAIN_WORLD));
		CombatManager& combatManager = pLevel->mCombatManager;
		Grid& grid = pLevel->mGrid;

		combatManager.mpAllCombatCharacters.push_back(new CombatCharacter("LOTUS",		grid.mpTiles[grid.getIndex(1, 1)], playerPreset));
		combatManager.mpAllCombatCharacters.push_back(new CombatCharacter("DRAGAUR 1",	grid.mpTiles[grid.getIndex(1, 6)], ratPreset));
		combatManager.mpAllCombatCharacters.push_back(new CombatCharacter("DRAGAUR 2",	grid.mpTiles[grid.getIndex(4, 1)], ratPreset));

		grid.mpTiles[grid.getIndex(2, 3)]->setType(impassiblePreset);
		grid.mpTiles[grid.getIndex(3, 4)]->setType(impassiblePreset);
		grid.mpTiles[grid.getIndex(5, 6)]->setType(impassiblePreset);

		worldData.mpMiniGameLevels.push_back(pLevel);
		pLevel = nullptr;
	}

	// LEVEL 3
	if (build)
	{
		int levelNumber = 3;
		MiniGameLevel* pLevel = new MiniGameLevel(levelNumber, LevelData(GO_TO_MAIN_WORLD, GO_TO_MAIN_WORLD));
		CombatManager& combatManager = pLevel->mCombatManager;
		Grid& grid = pLevel->mGrid;

		combatManager.mpAllCombatCharacters.push_back(new CombatCharacter("LOTUS",		grid.mpTiles[grid.getIndex(1, 1)], playerPreset));
		combatManager.mpAllCombatCharacters.push_back(new CombatCharacter("DRAGAUR",	grid.mpTiles[grid.getIndex(1, 6)], ratPreset));
		combatManager.mpAllCombatCharacters.push_back(new CombatCharacter("FROCODILE",	grid.mpTiles[grid.getIndex(4, 1)], frogPreset));

		grid.mpTiles[2][3].setType(impassiblePreset);
		grid.mpTiles[3][4].setType(impassiblePreset);
		grid.mpTiles[5][6].setType(impassiblePreset);

		worldData.mpMiniGameLevels.push_back(pLevel);
		pLevel = nullptr;
	}
}
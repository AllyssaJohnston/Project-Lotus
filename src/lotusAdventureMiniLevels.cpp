#include "lotusAdventureMiniLevels.h"

void createMiniGameLevels(MiniGameWorldData& worldData)
{
	bool build = true;
	PlayerCombatPreset	playerPreset	= PlayerCombatPreset();;
	RatCombatPreset		ratPreset		= RatCombatPreset();
	FrogCombatPreset	frogPreset		= FrogCombatPreset();

	ImpassableTilePreset impassiblePreset = ImpassableTilePreset();

	int worldNumber = 0;
	int levelNumber = 0;
	int stageNumber = 0;

	MiniGameWorld* pCurWorld = nullptr;
	MiniGameLevel* pCurLevel = nullptr;
	MiniGameStage* pCurStage = nullptr;

	// WORLD 0
	worldData.mpMiniGameWorlds.push_back(new MiniGameWorld(worldNumber));
	pCurWorld = worldData.mpMiniGameWorlds[worldNumber];

	// LEVEL 0
	pCurWorld->mpLevels.push_back(new MiniGameLevel(levelNumber, LevelData(GO_TO_MAIN_WORLD, GO_TO_MAIN_WORLD)));
	pCurLevel = pCurWorld->mpLevels[levelNumber];

	// STAGE 0
	if (build)
	{
		stageNumber = 0;
		pCurLevel->mpStages.push_back(new MiniGameStage(stageNumber));
		pCurStage = pCurLevel->mpStages[stageNumber];
		CombatManager& combatManager = pCurStage->mCombatManager;
		Grid& grid = pCurStage->mGrid;
		
		combatManager.mpAllCombatCharacters.push_back(new CombatCharacter("LOTUS",		grid.mpTiles[grid.getIndex(0, 8)], playerPreset));
		combatManager.mpAllCombatCharacters.push_back(new CombatCharacter("DRAGAUR",	grid.mpTiles[grid.getIndex(2, 5)], ratPreset));
	}

	// STAGE 1
	if (build)
	{
		stageNumber = 1;
		pCurLevel->mpStages.push_back(new MiniGameStage(stageNumber));
		pCurStage = pCurLevel->mpStages[stageNumber];
		CombatManager& combatManager = pCurStage->mCombatManager;
		Grid& grid = pCurStage->mGrid;
		
		
		combatManager.mpAllCombatCharacters.push_back(new CombatCharacter("LOTUS",		grid.mpTiles[grid.getIndex(1, 1)], playerPreset));
		combatManager.mpAllCombatCharacters.push_back(new CombatCharacter("DRAGAUR",	grid.mpTiles[grid.getIndex(1, 6)], ratPreset));

		grid.mpTiles[grid.getIndex(1, 3)]->setType(impassiblePreset);
		grid.mpTiles[grid.getIndex(3, 4)]->setType(impassiblePreset);
	}

	// STAGE 2
	if (build)
	{
		stageNumber = 2;
		pCurLevel->mpStages.push_back(new MiniGameStage(stageNumber));
		pCurStage = pCurLevel->mpStages[stageNumber];
		CombatManager& combatManager = pCurStage->mCombatManager;
		Grid& grid = pCurStage->mGrid;

		combatManager.mpAllCombatCharacters.push_back(new CombatCharacter("LOTUS",		grid.mpTiles[grid.getIndex(1, 1)], playerPreset));
		combatManager.mpAllCombatCharacters.push_back(new CombatCharacter("DRAGAUR 1",	grid.mpTiles[grid.getIndex(1, 6)], ratPreset));
		combatManager.mpAllCombatCharacters.push_back(new CombatCharacter("DRAGAUR 2",	grid.mpTiles[grid.getIndex(4, 1)], ratPreset));

		grid.mpTiles[grid.getIndex(2, 3)]->setType(impassiblePreset);
		grid.mpTiles[grid.getIndex(3, 4)]->setType(impassiblePreset);
		grid.mpTiles[grid.getIndex(5, 6)]->setType(impassiblePreset);
	}



	// LEVEL 1
	levelNumber = 1;
	pCurWorld->mpLevels.push_back(new MiniGameLevel(levelNumber, LevelData(GO_TO_MAIN_WORLD, GO_TO_MAIN_WORLD)));
	pCurLevel = pCurWorld->mpLevels[levelNumber];

	// STAGE 0
	if (build)
	{
		stageNumber = 0;
		pCurLevel->mpStages.push_back(new MiniGameStage(stageNumber));
		pCurStage = pCurLevel->mpStages[stageNumber];
		CombatManager& combatManager = pCurStage->mCombatManager;
		Grid& grid = pCurStage->mGrid;

		combatManager.mpAllCombatCharacters.push_back(new CombatCharacter("LOTUS",		grid.mpTiles[grid.getIndex(1, 1)], playerPreset));
		combatManager.mpAllCombatCharacters.push_back(new CombatCharacter("DRAGAUR",	grid.mpTiles[grid.getIndex(1, 6)], ratPreset));
		combatManager.mpAllCombatCharacters.push_back(new CombatCharacter("FROCODILE",	grid.mpTiles[grid.getIndex(4, 1)], frogPreset));

		grid.mpTiles[2][3].setType(impassiblePreset);
		grid.mpTiles[3][4].setType(impassiblePreset);
		grid.mpTiles[5][6].setType(impassiblePreset);
	}
}
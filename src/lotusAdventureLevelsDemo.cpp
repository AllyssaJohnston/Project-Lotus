#include "lotusAdventureLevelsDemo.h"

void createNames(Level* pCurLevel, int curLevelNumber)
{
	for (int count = 0; count < pCurLevel->mpPlatforms.size(); count++)
	{
		pCurLevel->mpPlatforms[count]->mName = pCurLevel->mpPlatforms[count]->getPreset()->mTypeName + " " + std::to_string(curLevelNumber) + " - " + std::to_string(count);
	}
	for (int count = 0; count < pCurLevel->mpAllNonStaticPlatforms.size(); count++)
	{
		pCurLevel->mpAllNonStaticPlatforms[count]->mName = pCurLevel->mpAllNonStaticPlatforms[count]->getPreset()->mTypeName + " " + std::to_string(curLevelNumber) + " - " + std::to_string(count);
	}
	for (int count = 0; count < pCurLevel->mpAllEnemies.size(); count++)
	{
		pCurLevel->mpAllEnemies[count]->mName = pCurLevel->mpAllEnemies[count]->getPreset()->mTypeName + " " + std::to_string(curLevelNumber) + " - " + std::to_string(count);
	}
	for (int count = 0; count < pCurLevel->mpAllCollectibles.size(); count++)
	{
		pCurLevel->mpAllCollectibles[count]->mName = pCurLevel->mpAllCollectibles[count]->mpsgPreset->mTypeName + " " + std::to_string(curLevelNumber) + " - " + std::to_string(count);
	}
}

void finishAllLevelsSetUp(GameInstance & gameInstance)
{
	for (World* pWorld : gameInstance.mWorldData.mpWorlds)
	{
		for (int countLevel = 0; countLevel < pWorld->mpLevels.size(); countLevel++)
		{
			Level* pCurLevel = pWorld->mpLevels[countLevel];
			pCurLevel->setUp(gameInstance.mScreen.mpRenderer);
			createNames(pCurLevel, countLevel);
		}
	}
}

void createLevels(GameInstance & gameInstance) 
{
	//GENERAL PLATFORMS
	static PSpikePreset spikePreset;
	static PLavaPreset  lavaPreset;


	int curWorldNumber;
	int curLevelNumber;
	Vect2 playerStartingPosition;
	World* curWorld;
	Level* curLevel;

	//No functionality - used to create if statements so that I can collapse level building code
	bool build = true;

	//WORLD 0 EARTH
	curWorldNumber = 0;
	WorldData& worldData = gameInstance.mWorldData;
	worldData.mpWorlds.push_back(new World(curWorldNumber));
	curWorld = worldData.mpWorlds[curWorldNumber];

	//EARTH PLATFORMS
	static PStandardPreset		preset			    = PStandardPreset(curWorldNumber);
	static PSolidPreset			solidPreset			= PSolidPreset(curWorldNumber);
	static PBouncyPreset		bouncyPreset;
	static PWallJumpablePreset	wallJumpablePreset;
	static PCratePreset			cratePreset;
	static PGatePreset			gatePreset		    = PGatePreset(curWorldNumber);
	static PTargetGatePreset	targetGatePreset    = PTargetGatePreset(curWorldNumber);
	static PTargetPreset		targetPreset		= PTargetPreset(curWorldNumber);

	//EARTH ENEMIES
	static ERatPreset			ratPreset;
	static EFastRatPreset		fastRatPreset;
	static EBouncyRatPreset		bouncyRatPreset;
	static EHidingRatPreset		hidingRatPreset;
	static EFrogPreset			frogPreset;
	static EBouncyFrogPreset    bouncyFrogPreset;

	static CEndOfLevelPreset    endOfLevelPreset = CEndOfLevelPreset(curWorldNumber);
	static CKeyPreset			keyPreset;

	//Level info
	bool doubleJumpAllowed				= false;
	bool throwProjectileAllowed			= false;
	bool throwDownwardProjectileAllowed = false;
	bool slashAllowed					= false;
	bool mustKillAllEnemies             = false;
	LevelInfo levelInfo =  LevelInfo(doubleJumpAllowed, throwProjectileAllowed, throwDownwardProjectileAllowed, slashAllowed, mustKillAllEnemies);

	int levelBlockX = 0;
	int levelBlockY = 0;

	//LEVEL 0
	if (build) 
	{
		curLevelNumber = 0;
		playerStartingPosition = Vect2(100, 1100);
		worldData.mpWorlds[curWorldNumber]->mpLevels.push_back(new Level(curLevelNumber, playerStartingPosition, levelInfo));
		curLevel = worldData.mpWorlds[curWorldNumber]->mpLevels[curLevelNumber];
		curLevel->mArtFileName = "EarthWorld/earthWorldLevel0Background.bmp";


		//LEVEL BLOCK - JUMPING
		levelBlockX = 0;
		levelBlockY = 825;

		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(0,    25,    25,   800,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(0,    1050,  800,  825,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(1050, 1975,  800,  825,  levelBlockX, levelBlockY), &spikePreset				));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(1975, 4000,  800,  825,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(0,    3975,  0,    25,   levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(3975, 4150,  0,    25,   levelBlockX, levelBlockY), &solidPreset 				));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(3975, 4000,  25,   700,  levelBlockX, levelBlockY), &wallJumpablePreset	    ));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(4125, 4150,  200,  825,  levelBlockX, levelBlockY), &wallJumpablePreset 	    ));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(4125, 4150,  25,   200,  levelBlockX, levelBlockY), &gatePreset 				));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(25,   275,   500,  525,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(100,  300,   325,  350,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(200,  475,   650,  675,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(550,  700,   575,  600,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(800,  950,   575,  600,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(1100, 1200,  575,  600,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(1500, 1650,  575,  600,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(2050, 2175,  575,  600,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(2200, 2375,  675,  700,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(2350, 2375,  475,  800,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(2225, 2500,  450,  475,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(2575, 2700,  550,  575,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(2425, 2550,  675,  700,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(2575, 2800,  325,  350,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(3300, 3500,  450,  475,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(3350, 3375,  475,  800,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(3575, 3700,  550,  575,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(3425, 3525,  675,  700,  levelBlockX, levelBlockY), &preset 					));

		curLevel->mpAllCollectibles.push_back(new SavePoint(Vect2(3800 + levelBlockX, 700 + levelBlockY)));

		SDL_Color curColor = {255, 230, 0};
		curLevel->mpBackgroundEffects.push_back(new CircleEffect(Vect2(1050 + levelBlockX, 100 + levelBlockY), curColor, 25, Vect2(800 + levelBlockX, 25 + levelBlockY), Vect2(1300 + levelBlockX, 300 + levelBlockY)));
		curLevel->mpBackgroundEffects.push_back(new CircleEffect(Vect2(1450 + levelBlockX, 120 + levelBlockY), curColor, 25, Vect2(800 + levelBlockX, 25 + levelBlockY), Vect2(1350 + levelBlockX, 300 + levelBlockY)));


		//LEVEL BLOCK - CRATES and SPIKES
		levelBlockX = 1800;
		levelBlockY = 1650;

		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(0,    1725,  800,  825,  levelBlockX, levelBlockY), &preset 			));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(1725, 1850,  810,  825,  levelBlockX, levelBlockY), &spikePreset 	));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(1850, 2350,  800,  825,  levelBlockX, levelBlockY), &preset 			));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(0,    25,    0,    800,  levelBlockX, levelBlockY), &preset 			));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(2175, 2200,  0,    625,  levelBlockX, levelBlockY), &wallJumpablePreset));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(2325, 2350,  0,    800,  levelBlockX, levelBlockY), &wallJumpablePreset));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(2175, 2250,  725,  800,  levelBlockX, levelBlockY), &cratePreset		));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(1900, 1925,  600,  800,  levelBlockX, levelBlockY), &preset 			));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(1800, 2025,  575,  600,  levelBlockX, levelBlockY), &preset 			));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(1500, 1750,  475,  500,  levelBlockX, levelBlockY), &preset 			));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(950,  1425,  425,  450,  levelBlockX, levelBlockY), &preset 			));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(575,  675,   325,  425,  levelBlockX, levelBlockY), &cratePreset 	));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(550,  800,   425,  450,  levelBlockX, levelBlockY), &preset 			));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(200,  450,   550,  575,  levelBlockX, levelBlockY), &preset 			));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(475,  750,   675,  700,  levelBlockX, levelBlockY), &preset 			));

		curLevel->mpAllCollectibles.push_back(new Collectible(Vect2(100 + levelBlockX, 350 + levelBlockY), &keyPreset));




		//LEVEL BLOCK - CRATES
		levelBlockX = 3500;
		levelBlockY = 0;

		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(0,      2300,  0,    25,   levelBlockX, levelBlockY), &solidPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(650,    2300,  1025, 1050, levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(0,        25,  0,    825,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(1300,   1525,  875,  900,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(1125,   1225,  750,  775,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(1275,   1750,  625,  650,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(1800,   2200,  725,  750,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(2025,   2050,  200,  600,  levelBlockX, levelBlockY), &wallJumpablePreset		));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(2175,   2200,  25,   725,  levelBlockX, levelBlockY), &wallJumpablePreset		));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(1400,   1425,  100,  275,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(1025,   2025,  275,  300,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(325,    1225,  425,  450,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(1225,   1250,  375,  450,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(1800,   1875,  200,  275,  levelBlockX, levelBlockY), &cratePreset 			));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(1200,   1275,  200,  275,  levelBlockX, levelBlockY), &cratePreset 			));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(2275,   2300,  875, 1025,  levelBlockX, levelBlockY), &preset 					));

		curLevel->mpAllCollectibles.push_back(new SavePoint(Vect2(1900 + levelBlockX, 950 + levelBlockY)));


		//LEVEL BLOCK - SPIKES
		levelBlockX = 5800;
		levelBlockY = 0;

		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(   0, 625,     0,   25, levelBlockX, levelBlockY), &solidPreset 				));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2( -25,  75,   850,  875, levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(  50,  75,   300,  850, levelBlockX, levelBlockY), &wallJumpablePreset 		));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(  75, 100,   300, 1725, levelBlockX, levelBlockY), &lavaPreset 				));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(   0, 200,   275,  300, levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2( 250, 400,   200,  225, levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2( 450, 600,   400,  425, levelBlockX, levelBlockY), &spikePreset 				));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2( 250, 400,   700,  725, levelBlockX, levelBlockY), &spikePreset 				));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2( 450, 600,   900,  925, levelBlockX, levelBlockY), &spikePreset 				));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2( 150, 300,  1100, 1125, levelBlockX, levelBlockY), &spikePreset 				));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2( 350, 600,  1400, 1425, levelBlockX, levelBlockY), &spikePreset 				));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2( 100, 275,  1700, 1725, levelBlockX, levelBlockY), &spikePreset 				));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2( 600, 625,     0, 1725, levelBlockX, levelBlockY), &lavaPreset 				));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2( 600, 625,  1725, 2000, levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(-800, 625,  2000, 2025, levelBlockX, levelBlockY), &preset 					));



		//LEVEL BLOCK - Sword
		levelBlockX = 4150;
		levelBlockY = 1050;

		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(0,    1650,  0,    25,    levelBlockX, levelBlockY), &solidPreset 				));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(0,    600,   950,  975,   levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(1500, 1525,  975,  1425,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(650,  800,   1075, 1100,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(400,  425,   1150, 1200,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(300,  1500,  1200, 1225,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(0,    1300,  1400, 1425,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(100,  250,   1275, 1300,  levelBlockX, levelBlockY), &preset 					));

		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(100 + levelBlockX, 900  + levelBlockY), &ratPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(600 + levelBlockX, 1150 + levelBlockY), &ratPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(900 + levelBlockX, 1150 + levelBlockY), &ratPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(200 + levelBlockX, 1300 + levelBlockY), &ratPreset));

		curLevel->mpAllCollectibles.push_back(new SavePoint(Vect2(1700 + levelBlockX, 850 + levelBlockY)));


		//LEVEL BLOCK - Jump Challenge
		levelBlockX = 2200;
		levelBlockY = 2475;

		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(1800, 3475,     950, 975, levelBlockX, levelBlockY), &preset					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(0,      25,     0,   575, levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(3450, 3475,     0,   950, levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2000, 3450,     575, 600, levelBlockX, levelBlockY), &preset 					));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(3000, 3075,     500, 575, levelBlockX, levelBlockY), &cratePreset 				));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(2400, 2425,     775, 950, levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(2775, 2800,     775, 950, levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(1975, 2000,     575, 800, levelBlockX, levelBlockY), &wallJumpablePreset		));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(1800, 1825,     575, 950, levelBlockX, levelBlockY), &wallJumpablePreset		));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(0,    625,      575, 600, levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(625,  900,      575, 600, levelBlockX, levelBlockY), &spikePreset 				));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(925,  1200,     575, 600, levelBlockX, levelBlockY), &spikePreset 				));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(1225, 1500,     575, 600, levelBlockX, levelBlockY), &spikePreset 				));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(1525, 1550,     575, 600, levelBlockX, levelBlockY), &preset		 			));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(1550, 1650,     575, 600, levelBlockX, levelBlockY), &bouncyPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(1625, 1800,     575, 600, levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(1475, 1550,     400, 425, levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(1500, 1525,     425, 600, levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(1200, 1225,     400, 600, levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(900,  925,      400, 600, levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(600,  625,      300, 575, levelBlockX, levelBlockY), &wallJumpablePreset		));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(300,  325,      200, 575, levelBlockX, levelBlockY), &wallJumpablePreset		));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(25,   150,      300, 325, levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	      CoordsX1X2Y1Y2(3200, 3225,     600, 800, levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Gate(		      CoordsX1X2Y1Y2(3200, 3225,     800, 950, levelBlockX, levelBlockY), &gatePreset				));

		curLevel->mpAllCollectibles.push_back(new Collectible(Vect2(100  + levelBlockX,  250 + levelBlockY), &keyPreset));
		curLevel->mpAllCollectibles.push_back(new SavePoint(  Vect2(3300 + levelBlockX,  475 + levelBlockY)));
		curLevel->mpAllCollectibles.push_back(new EndOfLevelCollectible(Vect2(3325 + levelBlockX, 850 + levelBlockY), &endOfLevelPreset));
	}

	//LEVEL 1 
	if (build)
	{
		curLevelNumber = 1;
		playerStartingPosition = Vect2(2030, 725);
		worldData.mpWorlds[curWorldNumber]->mpLevels.push_back(new Level(curLevelNumber, playerStartingPosition, levelInfo));
		curLevel = worldData.mpWorlds[curWorldNumber]->mpLevels[curLevelNumber];
		curLevel->mArtFileName = "EarthWorld/earthWorldLevel1Background.bmp";

		//LEVEL BLOCK - 1 SECRET
		levelBlockX = 0;
		levelBlockY = 0;
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1050, 1700,  2000,    2025,   levelBlockX, levelBlockY), &preset 			 ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1500, 1700,  2000,    2025,   levelBlockX, levelBlockY), &bouncyPreset		 ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1800, 2000,  1825,    1850,   levelBlockX, levelBlockY), &preset 			 ));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(1800, 1875,  1675,    1750,   levelBlockX, levelBlockY), &cratePreset 		 ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1800, 1825,  1225,    1750,   levelBlockX, levelBlockY), &preset 			 ));

		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1075, 1400,  1600,    1625,   levelBlockX, levelBlockY), &preset 			 ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1400, 1700,  1600,    1625,   levelBlockX, levelBlockY), &bouncyPreset		 ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1075, 1250,  1425,    1450,   levelBlockX, levelBlockY), &preset 			 ));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(1125, 1200,  1350,    1425,   levelBlockX, levelBlockY), &cratePreset 		 ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1225, 1250,  400,     1350,   levelBlockX, levelBlockY), &preset 		 	 ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1050, 1225,  975,     1000,   levelBlockX, levelBlockY), &solidPreset 		 ));

		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1500, 1700,  1000,    1025,   levelBlockX, levelBlockY), &preset 		 	 ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1500, 1525,  700,     1000,   levelBlockX, levelBlockY), &wallJumpablePreset));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1675, 1700,  600,     1225,   levelBlockX, levelBlockY), &wallJumpablePreset));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1700, 1825,  1200,    1225,   levelBlockX, levelBlockY), &solidPreset	 	 ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1250, 1500,  900,     925,    levelBlockX, levelBlockY), &preset 		 	 ));

		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1050, 1075,  1000,    2000,   levelBlockX, levelBlockY), &preset 		 	 ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1225, 2025,  375,     400,    levelBlockX, levelBlockY), &solidPreset 		 ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2225, 2250,  1975,    2000,   levelBlockX, levelBlockY), &preset 		 	 ));

		curLevel->mpAllCollectibles.push_back(new SavePoint(	   Vect2(1925 + levelBlockX, 1950 + levelBlockY)));
		curLevel->mpAllCollectibles.push_back(new LotusCollectible(Vect2(1350 + levelBlockX, 800  + levelBlockY)));


		//LEVEL BLOCK - 2
		levelBlockX = 2000;
		levelBlockY = 0;

		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    2500,  700,    725,   levelBlockX, levelBlockY), &solidPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    25,    400,    1850,  levelBlockX, levelBlockY), &preset 			    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(25,   100,   1000,   1025,  levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(25,   500,   1200,   1225,  levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(500,  800,   1225,   1250,  levelBlockX, levelBlockY), &bouncyPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(800,  1300,  1200,   1225,  levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(200,  225,   725,    900,   levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(200,  225,   900,    1075,  levelBlockX, levelBlockY), &lavaPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(300,  1000,  950,    975,   levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(500,  525,   975,    1125,  levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1100, 1125,  725,    1075,  levelBlockX, levelBlockY), &lavaPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1300, 1500,  1225,   1250,  levelBlockX, levelBlockY), &bouncyPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1500, 2000,  1200,   1225,  levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1200, 1500,  950,    975,   levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1300, 1325,  975,    1125,  levelBlockX, levelBlockY), &preset					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1600, 1625,  725,    1075,  levelBlockX, levelBlockY), &lavaPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1975, 2000,  1225,   1500,  levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2000, 2300,  1475,   1500,  levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2300, 2450,  1475,   1500,  levelBlockX, levelBlockY), &bouncyPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2200, 2850,  1200,   1225,  levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2500, 2525,  1000,   1200,  levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2525, 3000,  1000,   1025,  levelBlockX, levelBlockY), &solidPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2500, 2700,  1475,   1500,  levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2775, 3000,  1475,   1500,  levelBlockX, levelBlockY), &bouncyPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(3000, 3025,  125,    2025,  levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2500, 3000,  1650,   1675,  levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(-300, 650,   2000,   2025,  levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(650,  675,   2010,   2025,  levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(675,  1050,  2000,   2025,  levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1050, 1075,  2010,   2025,  levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1075, 1425,  2000,   2025,  levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1425, 1450,  2010,   2025,  levelBlockX, levelBlockY), &preset		 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1450, 1700,  2000,   2025,  levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1700, 2000,  2000,   2025,  levelBlockX, levelBlockY), &bouncyPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2000, 3000,  2000,   2025,  levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2500, 2525,  1675,   1850,  levelBlockX, levelBlockY), &preset					));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(2500, 2525,  1850,   2000,  levelBlockX, levelBlockY), &gatePreset				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2050, 2300,  1750,   1775,  levelBlockX, levelBlockY), &bouncyPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1400, 2050,  1750,   1775,  levelBlockX, levelBlockY), &preset	 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1400, 1425,  1550,   1775,  levelBlockX, levelBlockY), &wallJumpablePreset 	));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1550, 1575,  1450,   1675,  levelBlockX, levelBlockY), &wallJumpablePreset 	));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1125, 1400,  1700,   1725,  levelBlockX, levelBlockY), &preset	 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1100, 1125,  1500,   1725,  levelBlockX, levelBlockY), &wallJumpablePreset 	));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(800,  1100,  1500,   1525,  levelBlockX, levelBlockY), &preset		 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(25,   400,   1500,   1525,  levelBlockX, levelBlockY), &preset		 			));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(325,  400,   1425,   1500,  levelBlockX, levelBlockY), &cratePreset	 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(300,  600,   1725,   1750,  levelBlockX, levelBlockY), &bouncyPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2550, 2800,  850,    875,   levelBlockX, levelBlockY), &preset		 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2350, 2375,  625,    700,   levelBlockX, levelBlockY), &preset					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2200, 2325,  675,    700,   levelBlockX, levelBlockY), &spikePreset			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1700, 1725,  125,    700,   levelBlockX, levelBlockY), &preset		 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1700, 3025,  100,    125,   levelBlockX, levelBlockY), &solidPreset		 	));

		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(400  + levelBlockX, 1150 + levelBlockY), &ratPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(1100 + levelBlockX, 1150 + levelBlockY), &ratPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(1600 + levelBlockX, 1150 + levelBlockY), &fastRatPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(2800 + levelBlockX, 1600 + levelBlockY), &fastRatPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(2800 + levelBlockX, 1975 + levelBlockY), &ratPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(1200 + levelBlockX, 1975 + levelBlockY), &ratPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(900  + levelBlockX, 1525 + levelBlockY), &fastRatPreset	 ));

		curLevel->mpAllCollectibles.push_back(new Collectible(			Vect2(2600 + levelBlockX, 1150 + levelBlockY), &keyPreset));
		curLevel->mpAllCollectibles.push_back(new EndOfLevelCollectible(Vect2(1800 + levelBlockX, 550  + levelBlockY), &endOfLevelPreset));
	}

	//LEVEL 2
	if (build)
	{
		curLevelNumber = 2;
		playerStartingPosition = Vect2(75, 275);
		worldData.mpWorlds[curWorldNumber]->mpLevels.push_back(new Level(curLevelNumber, playerStartingPosition, levelInfo));
		curLevel = worldData.mpWorlds[curWorldNumber]->mpLevels[curLevelNumber];
		curLevel->mArtFileName = "EarthWorld/earthWorldLevel2Background.bmp";


		//LEVEL BLOCK 1
		levelBlockX = 0;
		levelBlockY = 0;

		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    2000,  800,   825,   levelBlockX, levelBlockY), &solidPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    3200,  0,     25,    levelBlockX, levelBlockY), &solidPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    25,    25,    450,   levelBlockX, levelBlockY), &preset		 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    25,    450,   800,   levelBlockX, levelBlockY), &wallJumpablePreset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(125,  150,   450,   600,   levelBlockX, levelBlockY), &wallJumpablePreset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(25,   350,   425,   450,   levelBlockX, levelBlockY), &preset		 			));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(125,  325,   225,   425,   levelBlockX, levelBlockY), &cratePreset				));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(100,  150,   760,   800,   levelBlockX, levelBlockY), &cratePreset				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1100, 1300,  575,   600,   levelBlockX, levelBlockY), &preset		 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(750,  950,   500,   525,   levelBlockX, levelBlockY), &preset		 			));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(800,  950,   350,   500,   levelBlockX, levelBlockY), &cratePreset				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1950, 1975,  550,   800,   levelBlockX, levelBlockY), &preset	 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1975, 2000,  550,   1400,  levelBlockX, levelBlockY), &wallJumpablePreset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1975, 2000,  25,    400,   levelBlockX, levelBlockY), &wallJumpablePreset 		));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(2150, 2175,  25,    200,   levelBlockX, levelBlockY), &gatePreset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2150, 2175,  200,   1500,  levelBlockX, levelBlockY), &wallJumpablePreset 		));

		curLevel->mpAllCollectibles.push_back(new SavePoint(Vect2(  1975 + levelBlockX,  500 + levelBlockY)));


		//LEVEL BLOCK 2
		levelBlockX = 0;
		levelBlockY = 825;

		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    25,    0,     800,   levelBlockX, levelBlockY), &preset	 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(250,  375,   625,   650,   levelBlockX, levelBlockY), &preset	 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(25,   200,   500,   525,   levelBlockX, levelBlockY), &preset	 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    525,   800,   825,   levelBlockX, levelBlockY), &preset	 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(525,  675,   800,   825,   levelBlockX, levelBlockY), &bouncyPreset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(675,  3200,  800,   825,   levelBlockX, levelBlockY), &preset	 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1825, 1850,  400,   800,   levelBlockX, levelBlockY), &wallJumpablePreset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1800, 1825,  400,   800,   levelBlockX, levelBlockY), &preset	 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(450,  475,   300,   575,   levelBlockX, levelBlockY), &wallJumpablePreset		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(475,  500,   325,   575,   levelBlockX, levelBlockY), &preset					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(500,  700,   325,   350,   levelBlockX, levelBlockY), &lavaPreset				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(475,  1300,  300,   325,   levelBlockX, levelBlockY), &preset	 				));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(850,  900,   250,   300,   levelBlockX, levelBlockY), &cratePreset	 			));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(925,  1000,  225,   300,   levelBlockX, levelBlockY), &cratePreset	 			));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(1025, 1125,  200,   300,   levelBlockX, levelBlockY), &cratePreset	 			));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(1150, 1275,  175,   300,   levelBlockX, levelBlockY), &cratePreset	 			));


		curLevel->mpAllCollectibles.push_back(new Collectible(Vect2(     100 + levelBlockX,  400 + levelBlockY), &keyPreset));
		curLevel->mpAllCollectibles.push_back(new LotusCollectible(Vect2(575 + levelBlockX,  400 + levelBlockY)));


		//LEVEL BLOCK 3
		levelBlockX = 2175;
		levelBlockY = 0;

		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    25,   225,   1500,  levelBlockX, levelBlockY), &preset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1000, 1025, 25,    1200,  levelBlockX, levelBlockY), &preset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1000, 1025, 1425,  1625,  levelBlockX, levelBlockY), &preset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1000, 1200, 1400,  1425,  levelBlockX, levelBlockY), &preset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1175, 1200, 1225,  1400,  levelBlockX, levelBlockY), &preset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    150,  200,   225,   levelBlockX, levelBlockY), &preset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(25,   400,  400,   425,   levelBlockX, levelBlockY), &preset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(435,  800,  400,   425,   levelBlockX, levelBlockY), &preset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(835,  1000, 400,   425,   levelBlockX, levelBlockY), &preset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(25,   100,  575,   600,   levelBlockX, levelBlockY), &preset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(135,  500,  575,   600,   levelBlockX, levelBlockY), &preset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(535,  900,  575,   600,   levelBlockX, levelBlockY), &preset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(935,  1000, 575,   600,   levelBlockX, levelBlockY), &preset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(25,   400,  800,   825,   levelBlockX, levelBlockY), &preset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(435,  800,  800,   825,   levelBlockX, levelBlockY), &preset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(835,  1000, 800,   825,   levelBlockX, levelBlockY), &preset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(25,   100,  975,   1000,  levelBlockX, levelBlockY), &preset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(135,  500,  975,   1000,  levelBlockX, levelBlockY), &preset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(535,  900,  975,   1000,  levelBlockX, levelBlockY), &preset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(935,  1000, 975,   1000,  levelBlockX, levelBlockY), &preset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(25,   765,  1200,  1225,  levelBlockX, levelBlockY), &solidPreset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(800,  1200, 1200,  1225,  levelBlockX, levelBlockY), &solidPreset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(650,  850,  1400,  1425,  levelBlockX, levelBlockY), &preset 			));

		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(300 + levelBlockX, 350 + levelBlockY), &frogPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(800 + levelBlockX, 750 + levelBlockY), &frogPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(400 + levelBlockX,1150 + levelBlockY), &frogPreset	 ));

		curLevel->mpAllCollectibles.push_back(new SavePoint(Vect2(  100 + levelBlockX,  150 + levelBlockY)));
		curLevel->mpAllCollectibles.push_back(new EndOfLevelCollectible(Vect2(1100 + levelBlockX, 1300 + levelBlockY), &endOfLevelPreset));
	}

	//LEVEL 3
	if (build) 
	{
		curLevelNumber = 3;
		playerStartingPosition = Vect2(100, 275);
		worldData.mpWorlds[curWorldNumber]->mpLevels.push_back(new Level(curLevelNumber, playerStartingPosition, levelInfo));
		curLevel = worldData.mpWorlds[curWorldNumber]->mpLevels[curLevelNumber];
		curLevel->mArtFileName = "EarthWorld/earthWorldLevel3Background.bmp";

		//LEVEL BLOCK - BOUNCY
		levelBlockX = 0;
		levelBlockY = 0;

		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    3000,  0,     25,   levelBlockX, levelBlockY), &solidPreset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(25,   1350,  1000, 1025,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    25,    0,    1025,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1350, 1875,  1010, 1025,  levelBlockX, levelBlockY), &spikePreset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1875, 2475,  1000, 1025,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(25,   225,   700,  725,   levelBlockX, levelBlockY), &solidPreset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(225,  650,   700,  725,   levelBlockX, levelBlockY), &bouncyPreset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(650,  750,   700,  725,   levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(200,  225,   25,   525,   levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(350,  750,   500,  525,   levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(750,  775,   300,  875,   levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(450,  475,   25,   400,   levelBlockX, levelBlockY), &preset 					));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(375,  475,   400,  500,   levelBlockX, levelBlockY), &cratePreset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(350,  750,   500,  525,   levelBlockX, levelBlockY), &preset 					));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(800,  925,   875,  1000,  levelBlockX, levelBlockY), &cratePreset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(350,  750,   500,  525,   levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(775,  900,   850,  875,   levelBlockX, levelBlockY), &bouncyPreset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(900,  925,   800,  875,   levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(900,  925,   25,   625,   levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(925,  1950,  500,  525,   levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2100, 2125,  25,   875,   levelBlockX, levelBlockY), &wallJumpablePreset 		));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(1400, 1475,  425,  500,   levelBlockX, levelBlockY), &cratePreset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(925,  1200,  250,  275,   levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2450, 2475,  825,  1000,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2450, 2475,  0,    625,   levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Gate(		  	  CoordsX1X2Y1Y2(2450, 2475,  625,  800,   levelBlockX, levelBlockY), &gatePreset 				));

		curLevel->mpAllCollectibles.push_back(new Collectible(	   Vect2(250  + levelBlockX, 150 + levelBlockY), &keyPreset));
		curLevel->mpAllCollectibles.push_back(new Collectible(	   Vect2(700  + levelBlockX, 650 + levelBlockY), &keyPreset));
		curLevel->mpAllCollectibles.push_back(new Collectible(	   Vect2(950  + levelBlockX, 450 + levelBlockY), &keyPreset));
		curLevel->mpAllCollectibles.push_back(new LotusCollectible(Vect2(1300 + levelBlockX, 150 + levelBlockY)));
		curLevel->mpAllCollectibles.push_back(new SavePoint(	   Vect2(2300 + levelBlockX, 900 + levelBlockY)));



		//LEVEL BLOCK - GATES
		levelBlockX = 2475;
		levelBlockY = 600;

		curLevel->mpPlatforms.push_back(new Platform(	   CoordsX1X2Y1Y2(0,    800,  0,    25,   levelBlockX, levelBlockY), &solidPreset 				));
		curLevel->mpPlatforms.push_back(new Platform(	   CoordsX1X2Y1Y2(-25,  650,  200,  225,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Gate(		   CoordsX1X2Y1Y2(200,  225,  25,   200,  levelBlockX, levelBlockY), &gatePreset 				));
		curLevel->mpPlatforms.push_back(new Gate(		   CoordsX1X2Y1Y2(400,  425,  25,   200,  levelBlockX, levelBlockY), &gatePreset 				));
		curLevel->mpPlatforms.push_back(new Platform(	   CoordsX1X2Y1Y2(775,  800,  0,    650,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	   CoordsX1X2Y1Y2(400,  425,  225,  450,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	   CoordsX1X2Y1Y2(250,  400,  425,  450,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	   CoordsX1X2Y1Y2(175,  250,  425,  450,  levelBlockX, levelBlockY), &lavaPreset 				));
		curLevel->mpPlatforms.push_back(new Platform(	   CoordsX1X2Y1Y2(175,  200,  450,  650,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(	   CoordsX1X2Y1Y2(200,  775,  625,  650,  levelBlockX, levelBlockY), &bouncyPreset 				));



		//LEVEL BLOCK - FROCODILE
		levelBlockX = 1000;
		levelBlockY = 1025;

		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1400,   1675,  200,  225,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1200,   1225,  0,    50,   levelBlockX, levelBlockY), &lavaPreset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1350,   1375,  150,  200,  levelBlockX, levelBlockY), &lavaPreset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1200,   1375,  200,  225,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(700,    1475,  500,  525,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1450,   1475,  225,  350,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(1450,   1475,  350,  500,  levelBlockX, levelBlockY), &gatePreset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(700,    725,   300,  500,  levelBlockX, levelBlockY), &wallJumpablePreset      ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(400,    700,   300,  325,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,      25,    0,    1300, levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(25,     700,   500,  525,  levelBlockX, levelBlockY), &spikePreset 			));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(500,    575,   225,  300,  levelBlockX, levelBlockY), &cratePreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1450,   1475,  525,  700,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1475,   1700,  675,  700,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1700,   1725,  500,  700,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1900,   1925,  225,  1300, levelBlockX, levelBlockY), &preset 					));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(1500,   1575,  1000, 1075, levelBlockX, levelBlockY), &cratePreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(200,    1900,  1075, 1100, levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(200,    225,   525,  900,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(200,    225,   900,  1075, levelBlockX, levelBlockY), &gatePreset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,      1925,  1275, 1300, levelBlockX, levelBlockY), &preset 					));

		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(800   + levelBlockX,  250 + levelBlockY), &frogPreset			 ));

		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(900   + levelBlockX,  825 + levelBlockY), &frogPreset			 ));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(1300  + levelBlockX,  825 + levelBlockY), &frogPreset			 ));

		curLevel->mpAllCollectibles.push_back(new Collectible(Vect2(350 + levelBlockX,  200 + levelBlockY), &keyPreset));
		curLevel->mpAllCollectibles.push_back(new Collectible(Vect2(600 + levelBlockX,  800 + levelBlockY), &keyPreset));
		curLevel->mpAllCollectibles.push_back(new SavePoint(Vect2( 1600 + levelBlockX,  400 + levelBlockY)));
		curLevel->mpAllCollectibles.push_back(new EndOfLevelCollectible(Vect2(1800 + levelBlockX, 1200 + levelBlockY), &endOfLevelPreset));
	}

	//LEVEL 4
	if (build)
	{
		curLevelNumber = 4;
		playerStartingPosition = Vect2(100, 2600);
		worldData.mpWorlds[curWorldNumber]->mpLevels.push_back(new Level(curLevelNumber, playerStartingPosition, levelInfo));
		curLevel = worldData.mpWorlds[curWorldNumber]->mpLevels[curLevelNumber];
		curLevel->mArtFileName = "EarthWorld/earthWorldLevel4Background.bmp";


		//LEVEL BLOCK - 1
		int levelBlockX = 0;
		int levelBlockY = 0;

		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(25,   800,    3000,  3025,  levelBlockX, levelBlockY), &preset 			    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(800,  815,    3010,  3025,  levelBlockX, levelBlockY), &preset 			    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(815,  1750,   3000,  3025,  levelBlockX, levelBlockY), &preset 			    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1750, 2400,   3000,  3025,  levelBlockX, levelBlockY), &spikePreset 		    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2400, 3400,   3000,  3025,  levelBlockX, levelBlockY), &preset		 		    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    3425,   1175,  1200,  levelBlockX, levelBlockY), &solidPreset 		    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    25,     1200,  3025,  levelBlockX, levelBlockY), &preset 			    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(25,   200,    2825,  2850,  levelBlockX, levelBlockY), &preset 			    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(650,  800,    2750,  2775,  levelBlockX, levelBlockY), &preset 			    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1100, 1500,   2750,  2775,  levelBlockX, levelBlockY), &preset 			    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1200, 1400,   2550,  2575,  levelBlockX, levelBlockY), &preset 			    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(700,  1000,   2450,  2475,  levelBlockX, levelBlockY), &preset 			    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(400,  600,    2350,  2375,  levelBlockX, levelBlockY), &preset 			    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(800,  1100,   2250,  2275,  levelBlockX, levelBlockY), &preset 			    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1100, 1200,   2250,  2275,  levelBlockX, levelBlockY), &bouncyPreset 		    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1050, 1250,   2000,  2025,  levelBlockX, levelBlockY), &preset 			    ));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(700,  775,    1725,  1800,  levelBlockX, levelBlockY), &cratePreset 		    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(500,  950,    1800,  1825,  levelBlockX, levelBlockY), &preset 			    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1250, 1450,   2050,  2075,  levelBlockX, levelBlockY), &preset 			    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1200, 1700,   1825,  1850,  levelBlockX, levelBlockY), &preset 			    ));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(800,  875,    1475,  1550,  levelBlockX, levelBlockY), &cratePreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(775,  1000,   1550,  1575,  levelBlockX, levelBlockY), &preset 			    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1000, 1025,   1200,  1575,  levelBlockX, levelBlockY), &preset 			    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(600,  625,    1200,  1575,  levelBlockX, levelBlockY), &preset 			    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1700, 1725,   1625,  3000,  levelBlockX, levelBlockY), &preset 			    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1725, 2100,   1725,  1750,  levelBlockX, levelBlockY), &preset 			    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2000, 2400,   1900,  1925,  levelBlockX, levelBlockY), &bouncyPreset 		    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1725, 2000,   2150,  2175,  levelBlockX, levelBlockY), &spikePreset 		    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2000, 2200,   2150,  2175,  levelBlockX, levelBlockY), &bouncyPreset 		    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1725, 2200,   2175,  2200,  levelBlockX, levelBlockY), &solidPreset 		    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2400, 2425,   1200,  2100,  levelBlockX, levelBlockY), &preset 			    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2400, 2425,   2325,  3000,  levelBlockX, levelBlockY), &wallJumpablePreset     ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2400, 2800,   2300,  2325,  levelBlockX, levelBlockY), &preset 			    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1725, 1750,   2200,  3000,  levelBlockX, levelBlockY), &wallJumpablePreset     ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2100, 2125,   2200,  2700,  levelBlockX, levelBlockY), &lavaPreset 		    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(3000, 3300,   2300,  2325,  levelBlockX, levelBlockY), &preset 			    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2900, 3400,   2100,  2125,  levelBlockX, levelBlockY), &preset 			    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(3300, 3400,   1975,  2000,  levelBlockX, levelBlockY), &preset 			    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(3200, 3300,   1800,  1825,  levelBlockX, levelBlockY), &preset 			    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(3300, 3400,   1675,  1700,  levelBlockX, levelBlockY), &preset 			    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2425, 2800,   1800,  1825,  levelBlockX, levelBlockY), &preset 			    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2800, 2900,   1800,  1825,  levelBlockX, levelBlockY), &spikePreset 		    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2700, 3200,   1525,  1550,  levelBlockX, levelBlockY), &preset 			    ));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(2900, 3025,   1400,  1525,  levelBlockX, levelBlockY), &cratePreset 			));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(2600, 2725,   1675,  1800,  levelBlockX, levelBlockY), &cratePreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(3175, 3200,   1200,  1425,  levelBlockX, levelBlockY), &preset 			    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(3400, 3425,   1200,  3025,  levelBlockX, levelBlockY), &preset 			    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2425, 2450,   2325,  3000,  levelBlockX, levelBlockY), &preset 			    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2625, 2650,   2325,  2800,  levelBlockX, levelBlockY), &preset 			    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2450, 2625,   2700,  2725,  levelBlockX, levelBlockY), &preset 			    ));


		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(600   + levelBlockX,  2950 + levelBlockY), &bouncyRatPreset			 ));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(1200  + levelBlockX,  2700 + levelBlockY), &bouncyRatPreset			 ));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(800   + levelBlockX,  2400 + levelBlockY), &ratPreset					 ));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(900   + levelBlockX,  2200 + levelBlockY), &fastRatPreset				 ));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(1300  + levelBlockX,  2000 + levelBlockY), &bouncyRatPreset			 ));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(3200  + levelBlockX,  2250 + levelBlockY), &bouncyRatPreset			 ));

		curLevel->mpAllCollectibles.push_back(new LotusCollectible(Vect2(400  + levelBlockX, 1475 + levelBlockY)));
		curLevel->mpAllCollectibles.push_back(new SavePoint(	   Vect2(1700 + levelBlockX, 1500 + levelBlockY)));
		curLevel->mpAllCollectibles.push_back(new LotusCollectible(Vect2(1900 + levelBlockX, 2300 + levelBlockY)));
		curLevel->mpAllCollectibles.push_back(new SavePoint(	   Vect2(2500 + levelBlockX, 2200 + levelBlockY)));

		curLevel->mpAllCollectibles.push_back(new EndOfLevelCollectible(Vect2(2525 + levelBlockX, 2500 + levelBlockY), &endOfLevelPreset));
	}

	//LEVEL 5
	if (build)
	{
		curLevelNumber = 5;
		playerStartingPosition = Vect2(100, 100);
		worldData.mpWorlds[curWorldNumber]->mpLevels.push_back(new Level(curLevelNumber, playerStartingPosition, levelInfo));
		curLevel = worldData.mpWorlds[curWorldNumber]->mpLevels[curLevelNumber];
		curLevel->mArtFileName = "EarthWorld/earthWorldLevel5Background.bmp";


		//LEVEL BLOCK - 1
		int levelBlockX = 0;
		int levelBlockY = 0;

		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    575,   1100,  1125,  levelBlockX, levelBlockY), &preset 			    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(575,  1000,  1100,  1125,  levelBlockX, levelBlockY), &solidPreset 	    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    2125,  0,     25,    levelBlockX, levelBlockY), &solidPreset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    25,    0,     2550,  levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(25,   1000,  500,   525,   levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(200,  225,   25,    200,   levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(100,  500,   200,   225,   levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(975,  1000,  250,   500,   levelBlockX, levelBlockY), &wallJumpablePreset 	));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(200,  450,   825,   850,   levelBlockX, levelBlockY), &preset	 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(600,  975,   825,   850,   levelBlockX, levelBlockY), &preset	 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(950,  975,   850,   1100,  levelBlockX, levelBlockY), &solidPreset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(975,  1000,  675,   1750,  levelBlockX, levelBlockY), &wallJumpablePreset 	));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1125, 1150,  25,    1400,  levelBlockX, levelBlockY), &wallJumpablePreset 	));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(1125, 1150,  1400,  1550,  levelBlockX, levelBlockY), &gatePreset		 	));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1125, 1150,  1550,  1600,  levelBlockX, levelBlockY), &wallJumpablePreset 	));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(975,  1150,  1550,  1575,  levelBlockX, levelBlockY), &preset			 	));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1125, 1150,  1725,  1900,  levelBlockX, levelBlockY), &wallJumpablePreset 	));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1275, 1300,  525,   800,   levelBlockX, levelBlockY), &wallJumpablePreset	));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1275, 1300,  900,   1900,  levelBlockX, levelBlockY), &wallJumpablePreset	));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    150,   1900,  1925,  levelBlockX, levelBlockY), &preset	 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(150,  350,   1900,  1925,  levelBlockX, levelBlockY), &bouncyPreset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(350,  1325,  1900,  1925,  levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(825,  850,   1400,  1900,  levelBlockX, levelBlockY), &wallJumpablePreset 	));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(575,  600,   1125,  1800,  levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(100,  450,   1650,  1675,  levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1125, 1825,  500,   525,   levelBlockX, levelBlockY), &preset			 	));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1375, 1675,  250,   275,   levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1300, 1325,  925,   2225,  levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1300, 1825,  900,   925,   levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1625, 1825,  1150,  1175,  levelBlockX, levelBlockY), &bouncyPreset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1325, 1525,  1300,  1325,  levelBlockX, levelBlockY), &bouncyPreset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1625, 1825,  1450,  1475,  levelBlockX, levelBlockY), &bouncyPreset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1325, 1525,  1600,  1625,  levelBlockX, levelBlockY), &bouncyPreset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1625, 1825,  1750,  1775,  levelBlockX, levelBlockY), &bouncyPreset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1325, 1525,  1900,  1925,  levelBlockX, levelBlockY), &bouncyPreset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1625, 1825,  2050,  2075,  levelBlockX, levelBlockY), &bouncyPreset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1325, 1525,  2200,  2225,  levelBlockX, levelBlockY), &bouncyPreset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1625, 1825,  2350,  2375,  levelBlockX, levelBlockY), &bouncyPreset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1825, 1850,  500,   2375,  levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    1925,  2525,  2550,  levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1925, 2100,  2525,  2550,  levelBlockX, levelBlockY), &bouncyPreset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1000, 1025,  2200,  2525,  levelBlockX, levelBlockY), &wallJumpablePreset 	));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(500,  700,   2300,  2325,  levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(700,  725,   1925,  2325,  levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2100, 2125,  25,    775,   levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2100, 2525,  775,   800,   levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2100, 2525,  975,   1000,  levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2100, 2125,  1000,  2200,  levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2100, 2125,  2400,  2550,  levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2100, 2600,  2375,  2400,  levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2100, 2600,  2175,  2200,  levelBlockX, levelBlockY), &solidPreset			));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(2100, 2125,  2200,  2375,  levelBlockX, levelBlockY), &gatePreset			));
		curLevel->mpPlatforms.push_back(new Gate(		      CoordsX1X2Y1Y2(2200, 2225,  2200,  2375,  levelBlockX, levelBlockY), &gatePreset			));
		curLevel->mpPlatforms.push_back(new Gate(		      CoordsX1X2Y1Y2(2300, 2325,  2200,  2375,  levelBlockX, levelBlockY), &gatePreset			));
		curLevel->mpPlatforms.push_back(new Gate(		      CoordsX1X2Y1Y2(2400, 2425,  2200,  2375,  levelBlockX, levelBlockY), &gatePreset			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2575, 2600,  2200,  2375,  levelBlockX, levelBlockY), &preset				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2500, 2525,  1000,  1125,  levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2525, 2925,  1100,  1125,  levelBlockX, levelBlockY), &spikePreset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2925, 2950,  600,   1125,  levelBlockX, levelBlockY), &wallJumpablePreset  ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2925, 2950,  400,   600,   levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2500, 2525,  100,   800,   levelBlockX, levelBlockY), &wallJumpablePreset  ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2825, 3525,  375,   400,   levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2500, 4025,  75,    100,   levelBlockX, levelBlockY), &solidPreset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2950, 2975,  400,   1125,  levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(3100, 4000,  700,   725,   levelBlockX, levelBlockY), &bouncyPreset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(3400, 3425,  400,   475,   levelBlockX, levelBlockY), &lavaPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(3400, 3425,  625,   700,   levelBlockX, levelBlockY), &lavaPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(3200, 3225,  400,   475,   levelBlockX, levelBlockY), &lavaPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(3200, 3225,  625,   700,   levelBlockX, levelBlockY), &lavaPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2975, 3150,  950,   975,   levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(3150, 3275,  950,   975,   levelBlockX, levelBlockY), &bouncyPreset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(3275, 3350,  950,   975,   levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(3350, 3475,  950,   975,   levelBlockX, levelBlockY), &bouncyPreset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(3475, 3700,  950,   975,   levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(4000, 4025,  100,   1300,  levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(3500, 3900,  1275,  1300,  levelBlockX, levelBlockY), &bouncyPreset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(3900, 4000,  1275,  1300,  levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(3500, 3525,  1125,  1275,  levelBlockX, levelBlockY), &lavaPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(3700, 3725,  1125,  1275,  levelBlockX, levelBlockY), &lavaPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2300, 3500,  1600,  1625,  levelBlockX, levelBlockY), &bouncyPreset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(3500, 3525,  1300,  1625,  levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(3325, 3350,  975,   1300,  levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(3300, 3350,  1425,  1450,  levelBlockX, levelBlockY), &lavaPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(3150, 3200,  1425,  1450,  levelBlockX, levelBlockY), &lavaPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(3000, 3050,  1425,  1450,  levelBlockX, levelBlockY), &lavaPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2850, 2900,  1425,  1450,  levelBlockX, levelBlockY), &lavaPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2700, 2750,  1425,  1450,  levelBlockX, levelBlockY), &lavaPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2300, 2325,  1000,  1600,  levelBlockX, levelBlockY), &wallJumpablePreset 	));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2475, 2500,  1000,  1400,  levelBlockX, levelBlockY), &wallJumpablePreset 	));

		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(650  + levelBlockX, 450  + levelBlockY), &bouncyFrogPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(500  + levelBlockX, 1050 + levelBlockY), &bouncyFrogPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(300  + levelBlockX, 1575 + levelBlockY), &bouncyRatPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(1600 + levelBlockX, 400  + levelBlockY), &bouncyRatPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(800  + levelBlockX, 2450 + levelBlockY), &bouncyRatPreset	 ));

		curLevel->mpAllCollectibles.push_back(new Collectible(Vect2(325 + levelBlockX,  150  + levelBlockY), &keyPreset));
		curLevel->mpAllCollectibles.push_back(new Collectible(Vect2(100 + levelBlockX,  900  + levelBlockY), &keyPreset));
		curLevel->mpAllCollectibles.push_back(new Collectible(Vect2(225 + levelBlockX,  1375 + levelBlockY), &keyPreset));
		curLevel->mpAllCollectibles.push_back(new Collectible(Vect2(1500+ levelBlockX,  150  + levelBlockY), &keyPreset));
		curLevel->mpAllCollectibles.push_back(new Collectible(Vect2(550 + levelBlockX,  2200 + levelBlockY), &keyPreset));
		curLevel->mpAllCollectibles.push_back(new SavePoint(	   Vect2(2300	   + levelBlockX, 875  + levelBlockY)));
		curLevel->mpAllCollectibles.push_back(new LotusCollectible(Vect2(2375	   + levelBlockX, 1300 + levelBlockY)));
		curLevel->mpAllCollectibles.push_back(new EndOfLevelCollectible(Vect2(2500 + levelBlockX, 2300 + levelBlockY), &endOfLevelPreset));
	}

	//LEVEL 6
	if (build)
	{
		curLevelNumber = 6;
		playerStartingPosition = Vect2(100, 275);
		worldData.mpWorlds[curWorldNumber]->mpLevels.push_back(new Level(curLevelNumber, playerStartingPosition, levelInfo));
		curLevel = worldData.mpWorlds[curWorldNumber]->mpLevels[curLevelNumber];
		curLevel->mArtFileName = "EarthWorld/earthWorldLevel6Background.bmp";

		//LEVEL BLOCK 1
		levelBlockX = 0;
		levelBlockY = 0;

		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    1075,  700,   725,   levelBlockX, levelBlockY), &preset				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1100, 1400,  700,   725,   levelBlockX, levelBlockY), &bouncyPreset		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    25,    25,    700,   levelBlockX, levelBlockY), &preset				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    2825,  0,     25,    levelBlockX, levelBlockY), &solidPreset			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(875,  900,   25,    550,   levelBlockX, levelBlockY), &preset				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1000, 1100,  525,   550,   levelBlockX, levelBlockY), &preset				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(900,  1000,  400,   425,   levelBlockX, levelBlockY), &preset				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1000, 1100,  275,   300,   levelBlockX, levelBlockY), &preset				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1075, 1100,  200,   725,   levelBlockX, levelBlockY), &preset				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1400, 1425,  25,    150,   levelBlockX, levelBlockY), &preset				));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(1400, 1425,  150,   300,   levelBlockX, levelBlockY), &gatePreset			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1400, 1425,  300,   1200,  levelBlockX, levelBlockY), &preset				));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(1400, 1425,  1200,  1350,  levelBlockX, levelBlockY), &gatePreset			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(25,   300,   525,   550,   levelBlockX, levelBlockY), &preset				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1700, 1900,  400,   425,   levelBlockX, levelBlockY), &bouncyPreset		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1425, 1625,  600,   625,   levelBlockX, levelBlockY), &bouncyPreset		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1700, 1900,  800,   825,   levelBlockX, levelBlockY), &bouncyPreset		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1425, 1625,  1000,  1025,  levelBlockX, levelBlockY), &bouncyPreset		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1700, 1900,  1200,  1225,  levelBlockX, levelBlockY), &bouncyPreset		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1900, 2000,  200,   225,   levelBlockX, levelBlockY), &preset				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1900, 1925,  225,   1350,  levelBlockX, levelBlockY), &preset				));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(1900, 1925,  25,    200,   levelBlockX, levelBlockY), &gatePreset			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1200, 1925,  1350,  1375,  levelBlockX, levelBlockY), &preset				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1925, 2325,  1350,  1375,  levelBlockX, levelBlockY), &bouncyPreset		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1100, 1200,  1350,  1375,  levelBlockX, levelBlockY), &bouncyPreset		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(475,  1100,  1350,  1375,  levelBlockX, levelBlockY), &preset				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1225, 1400,  1100,  1125,  levelBlockX, levelBlockY), &spikePreset			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1200, 1225,  925,   1200,  levelBlockX, levelBlockY), &wallJumpablePreset	));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(600,  1225,  900,   925,   levelBlockX, levelBlockY), &preset				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(975,  1000,  1150,  1350,  levelBlockX, levelBlockY), &preset				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(500,  700,   1100,  1125,  levelBlockX, levelBlockY), &preset				));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(625,  700,   925,   1000,  levelBlockX, levelBlockY), &cratePreset			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(475,  500,   750,   1350,  levelBlockX, levelBlockY), &preset				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(475,  1400,  725,   750,   levelBlockX, levelBlockY), &solidPreset			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1925, 2290,  400,   425,   levelBlockX, levelBlockY), &bouncyPreset		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1960, 2325,  750,   775,   levelBlockX, levelBlockY), &bouncyPreset		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1925, 2095,  1100,  1125,  levelBlockX, levelBlockY), &bouncyPreset		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2140, 2325,  1100,  1125,  levelBlockX, levelBlockY), &bouncyPreset		));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(2325, 2350,  25,    200,   levelBlockX, levelBlockY), &gatePreset			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2325, 2500,  200,   225,   levelBlockX, levelBlockY), &preset				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2325, 2350,  225,   1375,  levelBlockX, levelBlockY), &preset				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2350, 2800,  1350,  1375,  levelBlockX, levelBlockY), &preset				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2350, 2600,  600,   625,   levelBlockX, levelBlockY), &spikePreset			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2400, 2800,  900,   925,   levelBlockX, levelBlockY), &spikePreset			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2350, 2500,  1150,  1175,  levelBlockX, levelBlockY), &spikePreset			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2800, 2825,  25,    1375,  levelBlockX, levelBlockY), &preset				));

		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(500  + levelBlockX, 650  + levelBlockY), &bouncyFrogPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(1300 + levelBlockX, 650  + levelBlockY), &bouncyFrogPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(1800 + levelBlockX, 350  + levelBlockY), &bouncyFrogPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(1600 + levelBlockX, 550  + levelBlockY), &bouncyFrogPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(1800 + levelBlockX, 750  + levelBlockY), &bouncyFrogPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(1600 + levelBlockX, 950  + levelBlockY), &bouncyFrogPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(2000 + levelBlockX, 350  + levelBlockY), &bouncyFrogPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(2000 + levelBlockX, 725  + levelBlockY), &bouncyFrogPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(2200 + levelBlockX, 1050 + levelBlockY), &bouncyFrogPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(2200 + levelBlockX, 1300 + levelBlockY), &bouncyFrogPreset));

		curLevel->mpAllCollectibles.push_back(new Collectible(	   Vect2(600  + levelBlockX, 350  + levelBlockY), &keyPreset));
		curLevel->mpAllCollectibles.push_back(new Collectible(	   Vect2(1825 + levelBlockX, 900  + levelBlockY), &keyPreset));
		curLevel->mpAllCollectibles.push_back(new Collectible(	   Vect2(600  + levelBlockX, 1200 + levelBlockY), &keyPreset));
		curLevel->mpAllCollectibles.push_back(new Collectible(	   Vect2(2000 + levelBlockX, 1200 + levelBlockY), &keyPreset));
		curLevel->mpAllCollectibles.push_back(new SavePoint(	   Vect2(1300 + levelBlockX, 1225 + levelBlockY)));
		curLevel->mpAllCollectibles.push_back(new SavePoint(	   Vect2(1950 + levelBlockX, 125  + levelBlockY)));
		curLevel->mpAllCollectibles.push_back(new SavePoint(	   Vect2(2400 + levelBlockX, 125  + levelBlockY)));
		curLevel->mpAllCollectibles.push_back(new LotusCollectible(Vect2(1300 + levelBlockX, 1060 + levelBlockY)));

		curLevel->mpAllCollectibles.push_back(new EndOfLevelCollectible(Vect2(2700 + levelBlockX, 1200 + levelBlockY), &endOfLevelPreset));

	}

	//LEVEL 7
	if (build)
	{
		curLevelNumber = 7;
		playerStartingPosition = Vect2(1100, 275);
		worldData.mpWorlds[curWorldNumber]->mpLevels.push_back(new Level(curLevelNumber, playerStartingPosition, levelInfo));
		curLevel = worldData.mpWorlds[curWorldNumber]->mpLevels[curLevelNumber];
		curLevel->mArtFileName = "EarthWorld/earthWorldLevel7Background.bmp";

		//LEVEL BLOCK 1
		levelBlockX = 1000;
		levelBlockY = 0;

		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    1825,  600,   625,   levelBlockX, levelBlockY), &solidPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    2525,  0,     25,    levelBlockX, levelBlockY), &solidPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    25,    0,     1000,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    25,    1125,  1525,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(25,   150,   350,   375,   levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1000, 1025,  375,   600,   levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(850,  1175,  350,   375,   levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1400, 1425,  25,    400,   levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1800, 1825,  375,   825,   levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1650, 2275,  350,   375,   levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2500, 2525,  25,    1725,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2175, 2500,  575,   600,   levelBlockX, levelBlockY), &spikePreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1825, 2250,  800,   825,   levelBlockX, levelBlockY), &spikePreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(300,  1300,  1100,  1125,  levelBlockX, levelBlockY), &spikePreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(250,  275,   625,   950,   levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1300, 2525,  1100,  1125,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1000, 1400,  925,   950,   levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(400,  800,   925,   950,   levelBlockX, levelBlockY), &preset 					));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(100,  225,   1150,  1225,  levelBlockX, levelBlockY), &cratePreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(25,   500,   1500,  1525,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(25,   350,   1325,  1350,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(475,  500,   1525,  1725,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(500,  700,   1700,  1725,  levelBlockX, levelBlockY), &spikePreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(700,  725,   1525,  1725,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(550,  575,   1125,  1400,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(700,  1100,  1500,  1525,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1300, 1900,  1500,  1525,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2100, 2500,  1500,  1525,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(725,  2500,  1700,  1725,  levelBlockX, levelBlockY), &spikePreset 			));

		curLevel->mpAllEnemies.push_back(new Enemy(Vect2( 600 + levelBlockX,  525 + levelBlockY), &bouncyFrogPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(1400 + levelBlockX,  525 + levelBlockY), &bouncyFrogPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(1800 + levelBlockX,  300 + levelBlockY), &fastRatPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(1300 + levelBlockX,  850 + levelBlockY), &ratPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2( 500 + levelBlockX,  850 + levelBlockY), &ratPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(1700 + levelBlockX, 1425 + levelBlockY), &fastRatPreset));

		curLevel->mpAllCollectibles.push_back(new SavePoint(Vect2(  1900 + levelBlockX, 250  + levelBlockY)));
		curLevel->mpAllCollectibles.push_back(new SavePoint(Vect2(375    + levelBlockX, 1400 + levelBlockY)));
		curLevel->mpAllCollectibles.push_back(new SavePoint(Vect2(400    + levelBlockX, 750  + levelBlockY)));

		curLevel->mpAllCollectibles.push_back(new EndOfLevelCollectible(Vect2(2425 + levelBlockX, 1400 + levelBlockY), &endOfLevelPreset));

		//LEVEL BLOCK 2
		levelBlockX = 0;
		levelBlockY = 0;

		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(650,  1000,  1150,  1175,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(925,  1000,  1075,  1150,  levelBlockX, levelBlockY), &cratePreset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(850,  900,   980,   1000,  levelBlockX, levelBlockY), &spikePreset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(675,  775,   980,   1000,  levelBlockX, levelBlockY), &spikePreset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(900,  1000,  975,   1000,  levelBlockX, levelBlockY), &solidPreset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(650,  675,   675,   1150,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(675,  1000,  825,   850,   levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(650,  1000,  650,   675,   levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(325,  1000,  400,   425,   levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(325,  350,   425,   1700,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(325,  775,   1700,  1725,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(450,  650,   700,   725,   levelBlockX, levelBlockY), &spikePreset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(350,  550,   900,   925,   levelBlockX, levelBlockY), &spikePreset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(450,  650,   1100,  1125,  levelBlockX, levelBlockY), &spikePreset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(750,  775,   1175,  1300,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(750,  775,   1400,  1700,  levelBlockX, levelBlockY), &wallJumpablePreset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(775,  1000,  1275,  1300,  levelBlockX, levelBlockY), &preset	 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(775,  1000,  1500,  1525,  levelBlockX, levelBlockY), &bouncyPreset 				));

		curLevel->mpAllCollectibles.push_back(new LotusCollectible(Vect2(850	   + levelBlockX, 1200 + levelBlockY)));
	}

	//LEVEL 8
	if (build)
	{
		curLevelNumber = 8;
		playerStartingPosition = Vect2(100, 1200);
		worldData.mpWorlds[curWorldNumber]->mpLevels.push_back(new Level(curLevelNumber, playerStartingPosition, levelInfo));
		curLevel = worldData.mpWorlds[curWorldNumber]->mpLevels[curLevelNumber];
		curLevel->mArtFileName = "EarthWorld/earthWorldLevel8Background.bmp";


		//LEVEL BLOCK FROG SHROOM
		levelBlockX = 0;
		levelBlockY = 525;

		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    25,    0,      800,   levelBlockX, levelBlockY), &preset 						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    550,   800,    825,   levelBlockX, levelBlockY), &preset 						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(375,  525,   400,    425,   levelBlockX, levelBlockY), &preset 						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(375,  400,   425,    700,   levelBlockX, levelBlockY), &wallJumpablePreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(525,  550,   400,    650,   levelBlockX, levelBlockY), &wallJumpablePreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(525,  550,   650,    800,   levelBlockX, levelBlockY), &preset 						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(550,  800,   800,    825,   levelBlockX, levelBlockY), &bouncyPreset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(400,  825,   250,    275,   levelBlockX, levelBlockY), &solidPreset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(800,  825,   0,      250,   levelBlockX, levelBlockY), &wallJumpablePreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(800,  825,   400,    825,   levelBlockX, levelBlockY), &preset 						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    800,   0,      25,    levelBlockX, levelBlockY), &preset 						));

		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(700 + levelBlockX,   700 + levelBlockY), &bouncyFrogPreset	 ));

		//LEVEL BLOCK FROCODILE
		int levelBlockX = 800;
		int levelBlockY = 100;

		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(-800, -775,    0,      425,   levelBlockX, levelBlockY), &preset 						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(-775, 1100,    0,      25,    levelBlockX, levelBlockY), &solidPreset					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    1125,    825,    850,   levelBlockX, levelBlockY), &preset 						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    25,      25,     250,   levelBlockX, levelBlockY), &wallJumpablePreset			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(150,  175,     625,    825,   levelBlockX, levelBlockY), &wallJumpablePreset			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(175,  200,     625,    825,   levelBlockX, levelBlockY), &preset 						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(150,  175,     25,     500,   levelBlockX, levelBlockY), &wallJumpablePreset			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(175,  200,     25,     500,   levelBlockX, levelBlockY), &preset 						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(350,  450,     650,    675,   levelBlockX, levelBlockY), &preset 						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(525,  625,     525,    550,   levelBlockX, levelBlockY), &preset 						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(725,  825,     450,    475,   levelBlockX, levelBlockY), &preset 						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(200,  650,     325,    350,   levelBlockX, levelBlockY), &preset 						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(625,  650,     25,     250,   levelBlockX, levelBlockY), &preset 						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1100, 1125,    -75,    700,   levelBlockX, levelBlockY), &preset 						));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(1100, 1125,    700,    825,   levelBlockX, levelBlockY), &gatePreset 					));

		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(-500 + levelBlockX, 100 + levelBlockY), &bouncyRatPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2( 800 + levelBlockX, 775 + levelBlockY), &frogPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2( 200 + levelBlockX, 250 + levelBlockY), &frogPreset));

		curLevel->mpAllCollectibles.push_back(new Collectible(Vect2(-600 + levelBlockX,   150 + levelBlockY), &keyPreset));
		curLevel->mpAllCollectibles.push_back(new SavePoint(Vect2(    75 + levelBlockX,   700 + levelBlockY)));


		//LEVEL BLOCK CRATE
		levelBlockX = 1925;
		levelBlockY = 0;

		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(-25,  1400,    0,        25,    levelBlockX, levelBlockY), &solidPreset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(675,  700,     450,      775,   levelBlockX, levelBlockY), &preset 						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    800,     925,      950,   levelBlockX, levelBlockY), &preset 						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(800,  900,     925,      950,   levelBlockX, levelBlockY), &bouncyPreset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(200,  675,     700,      725,   levelBlockX, levelBlockY), &preset 						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(700,  800,     750,      775,   levelBlockX, levelBlockY), &preset 						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(800,  900,     750,      775,   levelBlockX, levelBlockY), &bouncyPreset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    500,     475,      500,   levelBlockX, levelBlockY), &preset 						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(700,  800,     575,      600,   levelBlockX, levelBlockY), &preset 						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(800,  900,     575,      600,   levelBlockX, levelBlockY), &bouncyPreset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(675,  925,     425,      450,   levelBlockX, levelBlockY), &preset 						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    300,     300,      325,   levelBlockX, levelBlockY), &preset 						));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(200,  275,     225,      300,   levelBlockX, levelBlockY), &cratePreset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(900,  925,     450,      950,   levelBlockX, levelBlockY), &preset 						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(925,  950,     25,       300,   levelBlockX, levelBlockY), &wallJumpablePreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(925,  950,     425,      1600,  levelBlockX, levelBlockY), &wallJumpablePreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1075, 1400,    150,      175,   levelBlockX, levelBlockY), &preset 						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1375, 1400,    25,       150,   levelBlockX, levelBlockY), &preset 						));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(1225, 1250,    25,       150,   levelBlockX, levelBlockY), &gatePreset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1075, 1100,    175,      1500,  levelBlockX, levelBlockY), &wallJumpablePreset 			));

		curLevel->mpAllCollectibles.push_back(new SavePoint(Vect2(200 + levelBlockX,   825 + levelBlockY)));
		curLevel->mpAllCollectibles.push_back(new EndOfLevelCollectible(Vect2(1300 + levelBlockX, 50 + levelBlockY), &endOfLevelPreset));

		//LEVEL BLOCK SPIKES
		levelBlockX = 800;
		levelBlockY = 950;

		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(25,   2050,    0,          25,     levelBlockX, levelBlockY), &solidPreset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    1600,    800,        825,    levelBlockX, levelBlockY), &lavaPreset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1600, 2425,    800,        825,    levelBlockX, levelBlockY), &solidPreset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2400, 2425,    550,        800,    levelBlockX, levelBlockY), &preset 						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2225, 2425,    525,        550,    levelBlockX, levelBlockY), &solidPreset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1600, 1625,    650,        800,    levelBlockX, levelBlockY), &preset 						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1600, 1625,    25,         425,    levelBlockX, levelBlockY), &solidPreset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2175, 2275,    675,        700,    levelBlockX, levelBlockY), &preset 						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1400, 1625,    625,        650,    levelBlockX, levelBlockY), &preset 						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1350, 1400,    625,        650,    levelBlockX, levelBlockY), &lavaPreset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1000, 1125,    525,        550,    levelBlockX, levelBlockY), &preset 						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1125, 1225,    525,        550,    levelBlockX, levelBlockY), &lavaPreset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1225, 1300,    525,        550,    levelBlockX, levelBlockY), &preset 						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(600,  775,     425,        450,    levelBlockX, levelBlockY), &preset 						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(775,  825,     425,        450,    levelBlockX, levelBlockY), &lavaPreset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(825,  900,     425,        450,    levelBlockX, levelBlockY), &preset 						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(300,  500,     350,        375,    levelBlockX, levelBlockY), &preset 						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(200,  350,     225,        250,    levelBlockX, levelBlockY), &lavaPreset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(550,  650,     225,        250,    levelBlockX, levelBlockY), &preset 						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(750,  775,     150,        275,    levelBlockX, levelBlockY), &preset 						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(725,  750,     200,        250,    levelBlockX, levelBlockY), &lavaPreset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(950,  1050,    225,        250,    levelBlockX, levelBlockY), &preset 						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1050, 1250,    225,        250,    levelBlockX, levelBlockY), &spikePreset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1250, 1450,    225,        250,    levelBlockX, levelBlockY), &preset 						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1400, 1575,    400,        425,    levelBlockX, levelBlockY), &lavaPreset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    25,      400,        800,    levelBlockX, levelBlockY), &preset 						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1725, 1975,    525,        550,    levelBlockX, levelBlockY), &preset 						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(475,  500,     375,        800,    levelBlockX, levelBlockY), &wallJumpablePreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(300,  325,     375,        500,    levelBlockX, levelBlockY), &wallJumpablePreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(300,  320,     500,        600,    levelBlockX, levelBlockY), &lavaPreset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(200,  300,     475,        500,    levelBlockX, levelBlockY), &lavaPreset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(75,   150,     600,        625,    levelBlockX, levelBlockY), &lavaPreset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(275,  350,     750,        775,    levelBlockX, levelBlockY), &bouncyPreset 				));

		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(1800 + levelBlockX,   700 + levelBlockY), &frogPreset	 ));

		curLevel->mpAllCollectibles.push_back(new SavePoint(Vect2(  1800 + levelBlockX,  325 + levelBlockY)));
		curLevel->mpAllCollectibles.push_back(new Collectible(Vect2(1500 + levelBlockX,  225 + levelBlockY), &keyPreset));
		curLevel->mpAllCollectibles.push_back(new LotusCollectible(Vect2(375 + levelBlockX, 425 + levelBlockY)));
	}

	//LEVEL 9
	if (build)
	{
		curLevelNumber = 9;
		playerStartingPosition = Vect2(50, 275);
		worldData.mpWorlds[curWorldNumber]->mpLevels.push_back(new Level(curLevelNumber, playerStartingPosition, levelInfo));
		curLevel = worldData.mpWorlds[curWorldNumber]->mpLevels[curLevelNumber];
		curLevel->mArtFileName = "EarthWorld/earthWorldLevel9Background.bmp";


		//LEVEL BLOCK 1
		levelBlockX = 0;
		levelBlockY = 0;

		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    2225,  0,     25,    levelBlockX, levelBlockY), &solidPreset				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(25,   1000,  800,   825,   levelBlockX, levelBlockY), &solidPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1000, 1900,  800,   825,   levelBlockX, levelBlockY), &spikePreset				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    25,    25,    1725,  levelBlockX, levelBlockY), &preset					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(300,  500,   650,   675,   levelBlockX, levelBlockY), &preset					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(25,   325,   525,   550,   levelBlockX, levelBlockY), &preset					));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(100,  200,   425,   525,   levelBlockX, levelBlockY), &cratePreset				));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(200,  300,   425,   525,   levelBlockX, levelBlockY), &cratePreset				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(500,  700,   425,   450,   levelBlockX, levelBlockY), &preset					));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(550,  650,   325,   425,   levelBlockX, levelBlockY), &cratePreset				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(700,  725,   25,    450,   levelBlockX, levelBlockY), &preset					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1100, 1400,  575,   600,   levelBlockX, levelBlockY), &preset					));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(1175, 1275,  475,   575,   levelBlockX, levelBlockY), &cratePreset				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1400, 1425,  25,    600,   levelBlockX, levelBlockY), &preset					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2200, 2225,  25,    1425,  levelBlockX, levelBlockY), &preset					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(25,   150,   1400,  1425,  levelBlockX, levelBlockY), &preset					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(200,  2200,  1400,  1425,  levelBlockX, levelBlockY), &preset					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1875, 1900,  825,   1200,  levelBlockX, levelBlockY), &preset					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1700, 1725,  1000,  1400,  levelBlockX, levelBlockY), &wallJumpablePreset		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1500, 1525,  1250,  1400,  levelBlockX, levelBlockY), &preset					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1300, 1325,  1250,  1400,  levelBlockX, levelBlockY), &preset					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(300,  400,   1250,  1275,  levelBlockX, levelBlockY), &preset					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(25,   100,   1125,  1150,  levelBlockX, levelBlockY), &preset					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(135,  300,   1125,  1150,  levelBlockX, levelBlockY), &preset					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(335,  500,   1125,  1150,  levelBlockX, levelBlockY), &preset					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(535,  700,   1125,  1150,  levelBlockX, levelBlockY), &preset					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(735,  900,   1125,  1150,  levelBlockX, levelBlockY), &preset					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(875,  900,   825,   1150,  levelBlockX, levelBlockY), &preset					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(25,   800,   1700,  1725,  levelBlockX, levelBlockY), &preset					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(800,  1000,  1700,  1725,  levelBlockX, levelBlockY), &bouncyPreset			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1000, 2000,  1700,  1725,  levelBlockX, levelBlockY), &preset					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1000, 1025,  1425,  1450,  levelBlockX, levelBlockY), &preset					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1000, 1025,  1575,  1700,  levelBlockX, levelBlockY), &preset					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1400, 1425,  1525,  1700,  levelBlockX, levelBlockY), &preset					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1800, 1825,  1425,  1575,  levelBlockX, levelBlockY), &preset					));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(1800, 1825,  1575,  1700,  levelBlockX, levelBlockY), &gatePreset				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2000, 2025,  1425,  1725,  levelBlockX, levelBlockY), &preset					));

		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(1400 + levelBlockX, 1200 + levelBlockY), &frogPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(500  + levelBlockX, 1075 + levelBlockY), &frogPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(200  + levelBlockX, 1200 + levelBlockY), &frogPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(1200 + levelBlockX, 1600 + levelBlockY), &bouncyRatPreset));

		curLevel->mpAllCollectibles.push_back(new Collectible(Vect2(     50   + levelBlockX,  1050 + levelBlockY), &keyPreset));
		curLevel->mpAllCollectibles.push_back(new SavePoint(Vect2(		 2100 + levelBlockX,  1325 + levelBlockY)));
		curLevel->mpAllCollectibles.push_back(new LotusCollectible(Vect2(1800 + levelBlockX,  450  + levelBlockY)));
		curLevel->mpAllCollectibles.push_back(new EndOfLevelCollectible(Vect2(1900 + levelBlockX, 1600 + levelBlockY), &endOfLevelPreset));

	}

	throwProjectileAllowed			= true;
	levelInfo =  LevelInfo(doubleJumpAllowed, throwProjectileAllowed, throwDownwardProjectileAllowed, slashAllowed, mustKillAllEnemies);

	//LEVEL 10
	if (build)
	{
		curLevelNumber = 10;
		playerStartingPosition = Vect2(150, 275);
		worldData.mpWorlds[curWorldNumber]->mpLevels.push_back(new Level(curLevelNumber, playerStartingPosition, levelInfo));
		curLevel = worldData.mpWorlds[curWorldNumber]->mpLevels[curLevelNumber];
		curLevel->mArtFileName = "EarthWorld/earthWorldLevel10Background.bmp";


		//LEVEL BLOCK 1
		levelBlockX = 0;
		levelBlockY = 0;

		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(25,   1325,  500,   525,   levelBlockX, levelBlockY), &solidPreset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    25,    25,    1525,  levelBlockX, levelBlockY), &preset	 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1300, 1325,  25,    375,   levelBlockX, levelBlockY), &preset	 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    1325,  0,     25,    levelBlockX, levelBlockY), &solidPreset 				));
		curLevel->mpPlatforms.push_back(new Target(			  CoordsX1X2Y1Y2(1100, 1150,  400,   450,   levelBlockX, levelBlockY), &targetPreset 				));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(1300, 1325,  375,   500,   levelBlockX, levelBlockY), &targetGatePreset 			));

		//LEVEL BLOCK 2
		levelBlockX = 1325;
		levelBlockY = 0;

		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    1025,  500,   525,   levelBlockX, levelBlockY), &solidPreset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1000, 1025,  25,    350,   levelBlockX, levelBlockY), &preset	 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    1025,  0,     25,    levelBlockX, levelBlockY), &solidPreset 				));
		curLevel->mpPlatforms.push_back(new Target(			  CoordsX1X2Y1Y2(800,  850,   300,   350,   levelBlockX, levelBlockY), &targetPreset 				));
		curLevel->mpPlatforms.push_back(new Target(			  CoordsX1X2Y1Y2(800,  850,   400,   450,   levelBlockX, levelBlockY), &targetPreset 				));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(1000, 1025,  350,   500,   levelBlockX, levelBlockY), &targetGatePreset 			));

		//LEVEL BLOCK 3
		levelBlockX = 2350;
		levelBlockY = 325;

		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    700,   0,    25,   levelBlockX, levelBlockY), &solidPreset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    500,   175,  200,  levelBlockX, levelBlockY), &solidPreset 				));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(300,  325,   25,   175,  levelBlockX, levelBlockY), &targetGatePreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(675,  700,   25,   700,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(475,  500,   200,  500,  levelBlockX, levelBlockY), &preset 					));

		curLevel->mpAllCollectibles.push_back(new SavePoint(Vect2(  375 + levelBlockX,  100 + levelBlockY)));

		//LEVEL BLOCK 4
		levelBlockX = 1100;
		levelBlockY = 525;

		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    25,    0,     350,   levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(0,    25,    375,   500,   levelBlockX, levelBlockY), &targetGatePreset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    1350,  500,   525,   levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1350, 1550,  500,   525,   levelBlockX, levelBlockY), &bouncyPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1550, 1950,  500,   525,   levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Target(			  CoordsX1X2Y1Y2(900,  950,   200,   250,   levelBlockX, levelBlockY), &targetPreset 			));
		curLevel->mpPlatforms.push_back(new Target(			  CoordsX1X2Y1Y2(900,  950,   300,   350,   levelBlockX, levelBlockY), &targetPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(875,  900,   0,     325,   levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(875,  900,   350,   500,   levelBlockX, levelBlockY), &targetGatePreset 		));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(775,  800,   350,   500,   levelBlockX, levelBlockY), &targetGatePreset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(775,  900,   325,   350,   levelBlockX, levelBlockY), &solidPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(300,  425,   250,   275,   levelBlockX, levelBlockY), &bouncyPreset 			));
		curLevel->mpPlatforms.push_back(new Target(			  CoordsX1X2Y1Y2(450,  485,   75,    105,   levelBlockX, levelBlockY), &targetPreset 			));
		curLevel->mpPlatforms.push_back(new Target(			  CoordsX1X2Y1Y2(100,  135,   100,   135,   levelBlockX, levelBlockY), &targetPreset 			));
		curLevel->mpPlatforms.push_back(new Target(			  CoordsX1X2Y1Y2(225,  260,   375,   410,   levelBlockX, levelBlockY), &targetPreset 			));

		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(400 + levelBlockX,   450 + levelBlockY), &bouncyFrogPreset	 ));

		curLevel->mpAllCollectibles.push_back(new SavePoint(Vect2(  950 + levelBlockX,  400 + levelBlockY)));

		//LEVEL BLOCK 5
		levelBlockX = 0;
		levelBlockY = 525;

		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,      1100,    1000,    1025,  levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(300,    325,     350,     525,   levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(325,    525,     500,     525,   levelBlockX, levelBlockY), &bouncyPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(525,    1100,    500,     525,   levelBlockX, levelBlockY), &preset 				));
		//curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,      25,      0,       1000,  levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(1000,   1025,    375,     500,   levelBlockX, levelBlockY), &targetGatePreset 		));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(900,    925,     375,     500,   levelBlockX, levelBlockY), &targetGatePreset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(900,    1125,    350,     375,   levelBlockX, levelBlockY), &solidPreset 			));
		curLevel->mpPlatforms.push_back(new Target(			  CoordsX1X2Y1Y2(1000,   1050,    200,     250,   levelBlockX, levelBlockY), &targetPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(850,    1000,    875,     900,   levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1075,   1100,    525,     800,   levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1075,   1100,    825,     850,   levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(1075,   1100,    850,     1000,  levelBlockX, levelBlockY), &targetGatePreset 		));

		curLevel->mpAllCollectibles.push_back(new SavePoint(Vect2(  800 + levelBlockX,  400 + levelBlockY)));


		//LEVEL BLOCK 6
		levelBlockX = 1100;
		levelBlockY = 1025;

		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,      900,     500,    525,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(900,    1000,    500,    525,  levelBlockX, levelBlockY), &bouncyPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1000,   1425,    500,    525,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,      325,     325,    350,  levelBlockX, levelBlockY), &solidPreset 			));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(100,    125,     350,    500,  levelBlockX, levelBlockY), &targetGatePreset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(300,    325,     25,     325,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Target(			  CoordsX1X2Y1Y2(150,    200,     275,    325,  levelBlockX, levelBlockY), &targetPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(325,    550,     300,    325,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(550,    575,     25,     225,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(575,    800,     200,    225,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1100,   1200,    375,    400,  levelBlockX, levelBlockY), &bouncyPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1400,   1425,    25,     250,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1400,   1425,    325,    500,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1400,   1700,    300,    325,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1700,   1725,    25,     325,  levelBlockX, levelBlockY), &preset 					));

		curLevel->mpAllEnemies.push_back(new Enemy(Vect2( 250 + levelBlockX, 250 + levelBlockY), &ratPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2( 400 + levelBlockX, 250 + levelBlockY), &ratPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2( 700 + levelBlockX, 150 + levelBlockY), &ratPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(1500 + levelBlockX, 250 + levelBlockY), &ratPreset	));

		curLevel->mpAllCollectibles.push_back(new EndOfLevelCollectible(Vect2(1300 + levelBlockX, 400 + levelBlockY), &endOfLevelPreset));
	}

	//LEVEL 11
	if (build)
	{
		curLevelNumber = 11;
		playerStartingPosition = Vect2(150, 1800);
		worldData.mpWorlds[curWorldNumber]->mpLevels.push_back(new Level(curLevelNumber, playerStartingPosition, levelInfo));
		curLevel = worldData.mpWorlds[curWorldNumber]->mpLevels[curLevelNumber];
		curLevel->mArtFileName = "EarthWorld/earthWorldLevel11Background.bmp";

		//LEVEL BLOCK 1
		levelBlockX = 0;
		levelBlockY = 1225;

		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    200,   775,   800,   levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(25,   1400,  800,   825,   levelBlockX, levelBlockY), &solidPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    25,    25,    1825,  levelBlockX, levelBlockY), &preset					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    1400,  0,     25,    levelBlockX, levelBlockY), &solidPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1375, 1400,  25,    650,   levelBlockX, levelBlockY), &preset					));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(1375, 1400,  650,   800,   levelBlockX, levelBlockY), &targetGatePreset		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(300,  700,   550,   575,   levelBlockX, levelBlockY), &preset	 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(800,  900,   425,   450,   levelBlockX, levelBlockY), &bouncyPreset	 		));
		curLevel->mpPlatforms.push_back(new Target(			  CoordsX1X2Y1Y2(1300, 1350,  275,   325,   levelBlockX, levelBlockY), &targetPreset	 		));
		curLevel->mpPlatforms.push_back(new Target(			  CoordsX1X2Y1Y2(1275, 1350,  400,   475,   levelBlockX, levelBlockY), &targetPreset			));
		curLevel->mpPlatforms.push_back(new Target(			  CoordsX1X2Y1Y2(1300, 1350,  525,   575,   levelBlockX, levelBlockY), &targetPreset	 		));

		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(800 + levelBlockX, 750 + levelBlockY), &bouncyRatPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(500 + levelBlockX, 500 + levelBlockY), &ratPreset		 ));

		//LEVEL BLOCK 2
		levelBlockX = 1400;
		levelBlockY = 1225;

		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(-5,   325,   625,   650,   levelBlockX, levelBlockY), &solidPreset 			));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(100,  125,   650,   800,   levelBlockX, levelBlockY), &targetGatePreset		));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(300,  325,   650,   800,   levelBlockX, levelBlockY), &targetGatePreset		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    1100,  800,   825,   levelBlockX, levelBlockY), &solidPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    75,    0,     25,    levelBlockX, levelBlockY), &solidPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(300,  500,   0,     25,    levelBlockX, levelBlockY), &preset	 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(500,  1525,  0,     25,    levelBlockX, levelBlockY), &solidPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    150,   450,   475,   levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1100, 1125,  350,   1150,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(1100, 1125,  1150,  1300,  levelBlockX, levelBlockY), &gatePreset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(250,  700,   325,   350,   levelBlockX, levelBlockY), &preset					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(800,  825,   25,    325,   levelBlockX, levelBlockY), &lavaPreset				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(625,  950,   450,   475,   levelBlockX, levelBlockY), &preset					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(850,  1200,  325,   350,   levelBlockX, levelBlockY), &preset					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1500, 1525,  25,    1300,  levelBlockX, levelBlockY), &lavaPreset				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1125, 1300,  450,   475,   levelBlockX, levelBlockY), &lavaPreset				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1300, 1500,  700,   725,   levelBlockX, levelBlockY), &lavaPreset				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1125, 1350,  1000,  1025,  levelBlockX, levelBlockY), &lavaPreset				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1450, 1500,  1000,  1025,  levelBlockX, levelBlockY), &lavaPreset				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(200,  400,   150,   175,   levelBlockX, levelBlockY), &preset	 				));

		curLevel->mpAllEnemies.push_back(new Enemy(Vect2( 500 + levelBlockX,  750 + levelBlockY), &bouncyFrogPreset	 ));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(1100 + levelBlockX,  275 + levelBlockY), &fastRatPreset	 ));

		curLevel->mpAllCollectibles.push_back(new SavePoint(  Vect2(1125 + levelBlockX, 150 + levelBlockY)));
		curLevel->mpAllCollectibles.push_back(new Collectible(Vect2(1200 + levelBlockX, 850 + levelBlockY), &keyPreset));

		//LEVEL BLOCK 3
		levelBlockX = 1000;
		levelBlockY = 0;

		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(100,  125,  425,  1225,   levelBlockX, levelBlockY), &wallJumpablePreset				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(425,  450,  625,  1225,   levelBlockX, levelBlockY), &wallJumpablePreset				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(450,  475,  625,  1050,   levelBlockX, levelBlockY), &preset							));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(450,  475,  200,  425,    levelBlockX, levelBlockY), &wallJumpablePreset				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(450,  475,  1050, 1225,   levelBlockX, levelBlockY), &preset							));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(475,  650,  1050, 1075,   levelBlockX, levelBlockY), &preset							));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(775,  1025, 925,  950,    levelBlockX, levelBlockY), &bouncyPreset						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1025, 1200, 925,  950,    levelBlockX, levelBlockY), &preset							));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(900,  925,  950,  1225,   levelBlockX, levelBlockY), &preset							));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(925,  1400, 1200, 1225,   levelBlockX, levelBlockY), &bouncyPreset						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1400, 1800, 1025, 1050,   levelBlockX, levelBlockY), &preset							));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(1600, 1675, 950,  1025,   levelBlockX, levelBlockY), &cratePreset 						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1800, 1825, 225,  1225,   levelBlockX, levelBlockY), &preset							));
		curLevel->mpPlatforms.push_back(new Target(			  CoordsX1X2Y1Y2(1725, 1775, 575,  625,    levelBlockX, levelBlockY), &targetPreset						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1025, 1200, 500,  525,    levelBlockX, levelBlockY), &lavaPreset						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(425,  650,  600,  625,    levelBlockX, levelBlockY), &preset							));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(450,  475,  450,  600,    levelBlockX, levelBlockY), &targetGatePreset					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(100,  475,  425,  450,    levelBlockX, levelBlockY), &solidPreset						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(450,  1800, 175,  200,    levelBlockX, levelBlockY), &solidPreset						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(125,  225,  600,  625,    levelBlockX, levelBlockY), &lavaPreset						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(300,  375,  900,  925,    levelBlockX, levelBlockY), &lavaPreset						));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(125,  225,  1100, 1125,   levelBlockX, levelBlockY), &lavaPreset						));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(1600, 1625, 1050, 1225,   levelBlockX, levelBlockY), &gatePreset						));

		curLevel->mpAllCollectibles.push_back(new SavePoint(  Vect2(	 525  + levelBlockX,		 950  + levelBlockY)));
		curLevel->mpAllCollectibles.push_back(new LotusCollectible(Vect2(1700 + levelBlockX,		 1100 + levelBlockY)));
		curLevel->mpAllCollectibles.push_back(new Collectible(Vect2(     300  + levelBlockX,		 1100 + levelBlockY), &keyPreset));

		//LEVEL BLOCK 4
		levelBlockX = 0;
		levelBlockY = 2050;

		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(300,  500,    485,   500,   levelBlockX, levelBlockY), &spikePreset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(500,  1400,   475,   500,   levelBlockX, levelBlockY), &solidPreset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1600, 2925,   475,   500,   levelBlockX, levelBlockY), &solidPreset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1375, 1400,   500,   600,   levelBlockX, levelBlockY), &preset	 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1600, 1625,   500,   600,   levelBlockX, levelBlockY), &preset	 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1400, 1600,   575,   600,   levelBlockX, levelBlockY), &lavaPreset					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2000, 2025,   0,     375,   levelBlockX, levelBlockY), &preset 					));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(2025, 2150,   350,   475,   levelBlockX, levelBlockY), &cratePreset 				));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(1700, 1750,   425,   475,   levelBlockX, levelBlockY), &cratePreset 				));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(1150, 1225,   400,   475,   levelBlockX, levelBlockY), &cratePreset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(275,  300,    300,   500,   levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    950,    1000,  1025,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(400,  425,    500,   825,   levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(400,  425,    825,   1000,  levelBlockX, levelBlockY), &gatePreset 				));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(500,  525,    825,   1000,  levelBlockX, levelBlockY), &gatePreset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(400,  525,    800,   825,   levelBlockX, levelBlockY), &solidPreset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(525,  700,    800,   825,   levelBlockX, levelBlockY), &preset	 					));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(500,  600,    700,   800,   levelBlockX, levelBlockY), &cratePreset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(950,  1050,   1000,  1025,  levelBlockX, levelBlockY), &bouncyPreset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1050, 1375,   1000,  1025,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(825,  1250,   775,   800,   levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1450, 1700,   850,   875,   levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1700, 1725,   700,   875,   levelBlockX, levelBlockY), &preset 					));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(1475, 1550,   775,   850,   levelBlockX, levelBlockY), &cratePreset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1300, 1325,   1025,  1325,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1325, 1800,   1300,  1325,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1800, 1825,   1025,  1325,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1800, 2600,   1000,  1025,  levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Target(			  CoordsX1X2Y1Y2(1325, 1375,   1025,  1075,  levelBlockX, levelBlockY), &targetPreset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2300, 2325,   500,   850,   levelBlockX, levelBlockY), &preset 					));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(2300, 2325,   850,   1000,  levelBlockX, levelBlockY), &targetGatePreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2575, 2600,   500,   1000,  levelBlockX, levelBlockY), &preset 					));

		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(1500 + levelBlockX,   1250 + levelBlockY), &ratPreset		 ));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(2100 + levelBlockX,   950 +  levelBlockY), &fastRatPreset	 ));

		curLevel->mpAllCollectibles.push_back(new SavePoint(			Vect2(1900 + levelBlockX, 375 + levelBlockY)));
		curLevel->mpAllCollectibles.push_back(new SavePoint(			Vect2(600  + levelBlockX, 900 + levelBlockY)));
		curLevel->mpAllCollectibles.push_back(new Collectible(			Vect2(900  + levelBlockX, 200 + levelBlockY), &keyPreset));
		curLevel->mpAllCollectibles.push_back(new Collectible(			Vect2(100  + levelBlockX, 400 + levelBlockY), &keyPreset));
		curLevel->mpAllCollectibles.push_back(new EndOfLevelCollectible(Vect2(2425 + levelBlockX, 800 + levelBlockY), &endOfLevelPreset));
	}

	//LEVEL 12
	if (build)
	{
		curLevelNumber = 12;
		playerStartingPosition = Vect2(300, 1100);
		worldData.mpWorlds[curWorldNumber]->mpLevels.push_back(new Level(curLevelNumber, playerStartingPosition, levelInfo));
		curLevel = worldData.mpWorlds[curWorldNumber]->mpLevels[curLevelNumber];
		curLevel->mArtFileName = "EarthWorld/earthWorldLevel12Background.bmp";

		//LEVEL BLOCK 1
		levelBlockX = 0;
		levelBlockY = 825;

		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    1500,   0,     25,    levelBlockX, levelBlockY), &solidPreset 	));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1500, 1900,   0,     25,    levelBlockX, levelBlockY), &lavaPreset 	));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1900, 2050,   0,     25,    levelBlockX, levelBlockY), &preset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2050, 2100,   0,     25,    levelBlockX, levelBlockY), &lavaPreset 	));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2200, 3100,   0,     25,    levelBlockX, levelBlockY), &solidPreset 	));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    25,     25,    800,   levelBlockX, levelBlockY), &preset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(875,  900,    25,    650,   levelBlockX, levelBlockY), &preset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,    950,    800,   825,   levelBlockX, levelBlockY), &preset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(950,  1050,   810,   825,   levelBlockX, levelBlockY), &spikePreset 	));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1050, 1100,   800,   825,   levelBlockX, levelBlockY), &preset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1100, 1400,   800,   825,   levelBlockX, levelBlockY), &bouncyPreset	));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1400, 2900,   800,   825,   levelBlockX, levelBlockY), &preset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(3050, 3075,   775,   850,   levelBlockX, levelBlockY), &preset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(200,  700,    475,   500,   levelBlockX, levelBlockY), &preset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(750,  875,    625,   650,   levelBlockX, levelBlockY), &preset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(900,  1200,   575,   600,   levelBlockX, levelBlockY), &preset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1175, 1475,   400,   425,   levelBlockX, levelBlockY), &bouncyPreset	));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1475, 1500,   -500,  425,   levelBlockX, levelBlockY), &preset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2200, 2225,   -500,  300,   levelBlockX, levelBlockY), &preset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(900,  1050,   300,   325,   levelBlockX, levelBlockY), &solidPreset 	));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(975,  1075,   200,   300,   levelBlockX, levelBlockY), &cratePreset 	));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1800, 1825,   625,   800,   levelBlockX, levelBlockY), &preset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1800, 2300,   600,   625,   levelBlockX, levelBlockY), &preset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2600, 3075,   600,   625,   levelBlockX, levelBlockY), &preset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2200, 2500,   450,   475,   levelBlockX, levelBlockY), &preset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2675, 2700,   250,   600,   levelBlockX, levelBlockY), &preset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2700, 2800,   450,   475,   levelBlockX, levelBlockY), &preset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2850, 2950,   325,   350,   levelBlockX, levelBlockY), &preset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(3075, 3100,   25,    1100,  levelBlockX, levelBlockY), &preset 		));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(2875, 2900,   625,   800,   levelBlockX, levelBlockY), &gatePreset 	));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(2300, 2375,   725,   800,   levelBlockX, levelBlockY), &cratePreset 	));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2200, 3100,   1100,  1125,  levelBlockX, levelBlockY), &preset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1900, 2200,   1110,  1125,  levelBlockX, levelBlockY), &lavaPreset 	));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(800,  1900,   1100,  1125,  levelBlockX, levelBlockY), &preset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1575, 1600,   825,   990,   levelBlockX, levelBlockY), &lavaPreset 	));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1375, 1400,   825,   990,   levelBlockX, levelBlockY), &lavaPreset 	));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1175, 1200,   825,   990,   levelBlockX, levelBlockY), &lavaPreset 	));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(600,  1200,   1275,  1300,  levelBlockX, levelBlockY), &preset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(600,  625,    825,   1275,  levelBlockX, levelBlockY), &preset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1175, 1200,   1125,  1275,  levelBlockX, levelBlockY), &preset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1700, 2000,   425,   450,   levelBlockX, levelBlockY), &preset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1500, 1700,   250,   275,   levelBlockX, levelBlockY), &preset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1775, 1800,   150,   275,   levelBlockX, levelBlockY), &lavaPreset		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1800, 1900,   150,   175,   levelBlockX, levelBlockY), &preset 		));

		curLevel->mpAllEnemies.push_back(new Enemy(Vect2( 500 + levelBlockX,  750 + levelBlockY), &hidingRatPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(1000 + levelBlockX,  525 + levelBlockY), &hidingRatPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(2000 + levelBlockX,  550 + levelBlockY), &hidingRatPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(2400 + levelBlockX,  400 + levelBlockY), &hidingRatPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(2800 + levelBlockX,  675 + levelBlockY), &bouncyRatPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(2900 + levelBlockX, 1000 + levelBlockY), &hidingRatPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(1700 + levelBlockX, 1000 + levelBlockY), &hidingRatPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(900  + levelBlockX, 1000 + levelBlockY), &hidingRatPreset));

		curLevel->mpAllCollectibles.push_back(new Collectible(Vect2(3000 + levelBlockX,   350 + levelBlockY), &keyPreset));
		curLevel->mpAllCollectibles.push_back(new SavePoint(Vect2(  2900 + levelBlockX,   400 + levelBlockY)));
		curLevel->mpAllCollectibles.push_back(new SavePoint(Vect2(  1600 + levelBlockX,   150 + levelBlockY)));
		curLevel->mpAllCollectibles.push_back(new EndOfLevelCollectible(Vect2(1000 + levelBlockX, 1150 + levelBlockY), &endOfLevelPreset));


		//LEVEL BLOCK 2
		levelBlockX = 0;
		levelBlockY = 0;

		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1800,    1825,   750,     825,    levelBlockX, levelBlockY), &lavaPreset 	));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1700,    1800,   800,     825,    levelBlockX, levelBlockY), &preset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1500,    1700,   625,     650,    levelBlockX, levelBlockY), &preset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1700,    1750,   630,     650,    levelBlockX, levelBlockY), &lavaPreset 	));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1850,    1875,   575,     600,    levelBlockX, levelBlockY), &lavaPreset 	));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1875,    2200,   575,     600,    levelBlockX, levelBlockY), &preset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1475,    2225,   300,     325,    levelBlockX, levelBlockY), &solidPreset 	));

		curLevel->mpAllCollectibles.push_back(new LotusCollectible(Vect2(2000 + levelBlockX,		 475 + levelBlockY)));
	}

	//LEVEL 13
	if (build) 
	{
		curLevelNumber = 13;
		playerStartingPosition = Vect2(1400, 800);
		worldData.mpWorlds[curWorldNumber]->mpLevels.push_back(new Level(curLevelNumber, playerStartingPosition, levelInfo));
		curLevel = worldData.mpWorlds[curWorldNumber]->mpLevels[curLevelNumber];
		curLevel->mArtFileName = "EarthWorld/earthWorldLevel13Background.bmp";


		//LEVEL BLOCK 1
		levelBlockX = 0;
		levelBlockY = 0;

		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,       3175,   0,      25,      levelBlockX, levelBlockY), &solidPreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,       875,    1000,   1025,    levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,       25,     25,     1000,    levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(25,      525,    500,    525,     levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(25,      525,    500,    525,     levelBlockX, levelBlockY), &preset 				));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(300,     390,    410,    500,     levelBlockX, levelBlockY), &cratePreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(100,     450,    300,    325,     levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(375,     575,    175,    200,     levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(525,     550,    500,    850,     levelBlockX, levelBlockY), &wallJumpablePreset 	));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(675,     700,    25,     425,     levelBlockX, levelBlockY), &wallJumpablePreset 	));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(645,     700,    425,    450,     levelBlockX, levelBlockY), &preset			 	));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(675,     700,    450,    700,     levelBlockX, levelBlockY), &wallJumpablePreset 	));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(675,     700,    800,    1000,    levelBlockX, levelBlockY), &wallJumpablePreset 	));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(825,     850,    500,    850,     levelBlockX, levelBlockY), &wallJumpablePreset 	));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(700,     1300,   475,    500,     levelBlockX, levelBlockY), &preset 				));
		curLevel->mpAllNonStaticPlatforms.push_back(new Crate(CoordsX1X2Y1Y2(1300,    1375,   775,    850,     levelBlockX, levelBlockY), &cratePreset 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1275,    1300,   300,    475,     levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1275,    1350,   275,    300,     levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1075,    1600,   1000,   1025,    levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1200,    1400,   850,    875,     levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(25,      200,    750,    775,     levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(300,     525,    700,    725,     levelBlockX, levelBlockY), &preset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(400,     425,    525,    625,     levelBlockX, levelBlockY), &lavaPreset 			));


		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(300 + levelBlockX, 250 + levelBlockY), &ratPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(400 + levelBlockX, 125 + levelBlockY), &ratPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(800 + levelBlockX, 425 + levelBlockY), &bouncyFrogPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(200 + levelBlockX, 950 + levelBlockY), &hidingRatPreset	 ));

		curLevel->mpAllCollectibles.push_back(new Collectible(Vect2( 450 + levelBlockX, 550 + levelBlockY), &keyPreset));
		curLevel->mpAllCollectibles.push_back(new Collectible(Vect2( 775 + levelBlockX, 100 + levelBlockY), &keyPreset));
		curLevel->mpAllCollectibles.push_back(new SavePoint(Vect2(660  + levelBlockX,   725 + levelBlockY)));
		curLevel->mpAllCollectibles.push_back(new SavePoint(Vect2(1275 + levelBlockX,   175 + levelBlockY)));

		//LEVEL BLOCK 2
		levelBlockX = 1600;
		levelBlockY = 0;

		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,       25,     25,     800,     levelBlockX, levelBlockY), &preset	 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,       200,    1000,   1025,    levelBlockX, levelBlockY), &preset	 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(200,     300,    1000,   1025,    levelBlockX, levelBlockY), &bouncyPreset	 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(300,     450,    1010,   1025,    levelBlockX, levelBlockY), &spikePreset	 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(450,     1425,   1000,   1025,    levelBlockX, levelBlockY), &preset	 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1460,    1575,   1000,   1025,    levelBlockX, levelBlockY), &preset	 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(100,     125,    725,    800,     levelBlockX, levelBlockY), &lavaPreset	 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(125,     225,    750,    775,     levelBlockX, levelBlockY), &bouncyPreset	 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(260,     350,    750,    775,     levelBlockX, levelBlockY), &lavaPreset	 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(75,      150,    500,    525,     levelBlockX, levelBlockY), &lavaPreset	 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(250,     350,    550,    575,     levelBlockX, levelBlockY), &bouncyPreset	 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(250,     450,    400,    425,     levelBlockX, levelBlockY), &preset	 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(225,     250,    375,    475,     levelBlockX, levelBlockY), &lavaPreset	 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(450,     475,    410,    525,     levelBlockX, levelBlockY), &lavaPreset	 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(250,     450,    400,    425,     levelBlockX, levelBlockY), &preset	 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(25,      200,    275,    300,     levelBlockX, levelBlockY), &preset	 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(625,     700,    400,    425,     levelBlockX, levelBlockY), &spikePreset	 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(425,     600,    625,    650,     levelBlockX, levelBlockY), &spikePreset	 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(550,     700,    850,    875,     levelBlockX, levelBlockY), &spikePreset	 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(700,     725,    25,     900,     levelBlockX, levelBlockY), &preset	 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1000,    1025,   800,    1000,    levelBlockX, levelBlockY), &wallJumpablePreset   ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1000,    1025,   600,    675,     levelBlockX, levelBlockY), &preset			    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(725,     1200,   575,    600,     levelBlockX, levelBlockY), &preset			    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1075,    1200,   350,    375,     levelBlockX, levelBlockY), &preset			    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1200,    1225,   350,    600,     levelBlockX, levelBlockY), &wallJumpablePreset   ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1400,    1425,   600,    900,     levelBlockX, levelBlockY), &wallJumpablePreset   ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1550,    1575,   25,     400,     levelBlockX, levelBlockY), &preset			    ));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1550,    1575,   400,    1000,    levelBlockX, levelBlockY), &wallJumpablePreset   ));

		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(1200 + levelBlockX, 950 + levelBlockY), &frogPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(1000 + levelBlockX, 550 + levelBlockY), &frogPreset));

		curLevel->mpAllCollectibles.push_back(new Collectible(Vect2(100  + levelBlockX,  175 + levelBlockY), &keyPreset));
		curLevel->mpAllCollectibles.push_back(new Collectible(Vect2(800  + levelBlockX,  475 + levelBlockY), &keyPreset));
		curLevel->mpAllCollectibles.push_back(new SavePoint(Vect2(  400  + levelBlockX,  300 + levelBlockY)));
		curLevel->mpAllCollectibles.push_back(new SavePoint(Vect2(  850  + levelBlockX,  900 + levelBlockY)));
		curLevel->mpAllCollectibles.push_back(new SavePoint(Vect2(  1100 + levelBlockX,  200 + levelBlockY)));

		//LEVEL BLOCK 3
		int levelBlockX = 0;
		int levelBlockY = 1025;

		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,       875,    800,       825,      levelBlockX, levelBlockY), &preset		 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,       25,     25,        800,      levelBlockX, levelBlockY), &preset		 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(0,       875,    0,         25,       levelBlockX, levelBlockY), &solidPreset	 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(775,     800,    25,        700,      levelBlockX, levelBlockY), &preset		 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(225,     250,    25,        200,      levelBlockX, levelBlockY), &preset		 			));
		curLevel->mpPlatforms.push_back(new Target(			  CoordsX1X2Y1Y2(50,      100,    200,       250,      levelBlockX, levelBlockY), &targetPreset		 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(225,     250,    250,       300,      levelBlockX, levelBlockY), &preset		 			));
		curLevel->mpPlatforms.push_back(new Target(			  CoordsX1X2Y1Y2(50,      100,    300,       350,      levelBlockX, levelBlockY), &targetPreset		 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(225,     250,    350,       400,      levelBlockX, levelBlockY), &preset		 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(225,     250,    400,       600,      levelBlockX, levelBlockY), &wallJumpablePreset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(225,     240,    600,       700,      levelBlockX, levelBlockY), &lavaPreset 				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(225,     250,    700,       800,      levelBlockX, levelBlockY), &wallJumpablePreset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(375,     400,    400,       450,      levelBlockX, levelBlockY), &wallJumpablePreset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(385,     400,    450,       550,      levelBlockX, levelBlockY), &lavaPreset		 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(375,     400,    550,       800,      levelBlockX, levelBlockY), &wallJumpablePreset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(375,     600,    400,       425,      levelBlockX, levelBlockY), &preset		 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(600,     625,    400,       550,      levelBlockX, levelBlockY), &wallJumpablePreset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(600,     615,    550,       650,      levelBlockX, levelBlockY), &lavaPreset		 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(600,     625,    650,       800,      levelBlockX, levelBlockY), &wallJumpablePreset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(750,     775,    200,       400,      levelBlockX, levelBlockY), &wallJumpablePreset 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(760,     775,    400,       500,      levelBlockX, levelBlockY), &lavaPreset		 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(750,     775,    500,       700,      levelBlockX, levelBlockY), &wallJumpablePreset 		));

		curLevel->mpAllCollectibles.push_back(new Collectible(Vect2(300 + levelBlockX, 725 + levelBlockY), &keyPreset));
		curLevel->mpAllCollectibles.push_back(new SavePoint(  Vect2(675 + levelBlockX, 725 + levelBlockY)));

		//LEVEL BLOCK 4
		levelBlockX = 875;
		levelBlockY = 1025;

		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(-200,   -175,    825,       1000,     levelBlockX, levelBlockY), &preset		 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(100,     1700,   800,       825,      levelBlockX, levelBlockY), &preset		 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(700,     1700,   775,       800,      levelBlockX, levelBlockY), &spikePreset	 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(-200,    1800,   975,       1000,     levelBlockX, levelBlockY), &preset		 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1800,    2000,   975,       1000,     levelBlockX, levelBlockY), &bouncyPreset		 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2000,    2100,   975,       1000,     levelBlockX, levelBlockY), &preset		 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2100,    2125,   400,       1000,     levelBlockX, levelBlockY), &preset		 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(-25,     1375,   275,       300,      levelBlockX, levelBlockY), &preset		 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(750,     900,    125,       150,      levelBlockX, levelBlockY), &preset		 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(900,     925,    0,         150,      levelBlockX, levelBlockY), &preset		 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(925,     1025,   0,         15,       levelBlockX, levelBlockY), &solidPreset		 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1200,    1225,   65,        275,      levelBlockX, levelBlockY), &preset		 			));
		curLevel->mpPlatforms.push_back(new Target(			  CoordsX1X2Y1Y2(1300,    1325,   25,        50,       levelBlockX, levelBlockY), &targetPreset		 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1350,    1375,   0,         275,      levelBlockX, levelBlockY), &preset		 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(-75,     25,     450,       475,      levelBlockX, levelBlockY), &preset		 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(-75,     75,     625,       650,      levelBlockX, levelBlockY), &preset		 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(175,     475,    625,       650,      levelBlockX, levelBlockY), &preset		 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(475,     575,    625,       650,      levelBlockX, levelBlockY), &bouncyPreset		 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(575,     675,    625,       650,      levelBlockX, levelBlockY), &preset		 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(675,     700,    425,       800,      levelBlockX, levelBlockY), &preset		 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(700,     1000,   525,       550,      levelBlockX, levelBlockY), &preset		 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1150,    1250,   650,       675,      levelBlockX, levelBlockY), &bouncyPreset	 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1400,    2100,   525,       550,      levelBlockX, levelBlockY), &preset		 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1500,    1525,   400,       475,      levelBlockX, levelBlockY), &preset		 			));
		curLevel->mpPlatforms.push_back(new Target(			  CoordsX1X2Y1Y2(1900,    1925,   475,       500,      levelBlockX, levelBlockY), &targetPreset		 		));
		curLevel->mpPlatforms.push_back(new Target(			  CoordsX1X2Y1Y2(1950,    1975,   475,       500,      levelBlockX, levelBlockY), &targetPreset		 		));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1500,    1525,   500,       550,      levelBlockX, levelBlockY), &preset		 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1500,    2300,   375,       400,      levelBlockX, levelBlockY), &preset		 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1550,    1650,   175,       200,      levelBlockX, levelBlockY), &preset		 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1900,    2000,   150,       175,      levelBlockX, levelBlockY), &preset		 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2050,    2275,   225,       250,      levelBlockX, levelBlockY), &preset		 			));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(300,     325,    825,       975,      levelBlockX, levelBlockY), &gatePreset				));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(400,     425,    825,       975,      levelBlockX, levelBlockY), &targetGatePreset			));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(500,     525,    825,       975,      levelBlockX, levelBlockY), &gatePreset				));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(600,     625,    825,       975,      levelBlockX, levelBlockY), &targetGatePreset			));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(700,     725,    825,       975,      levelBlockX, levelBlockY), &gatePreset				));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(800,     825,    825,       975,      levelBlockX, levelBlockY), &targetGatePreset			));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(900,     925,    825,       975,      levelBlockX, levelBlockY), &gatePreset				));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(1000,    1025,   825,       975,      levelBlockX, levelBlockY), &targetGatePreset			));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(1100,    1125,   825,       975,      levelBlockX, levelBlockY), &gatePreset				));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(1200,    1225,   825,       975,      levelBlockX, levelBlockY), &targetGatePreset			));
		curLevel->mpPlatforms.push_back(new Gate(			  CoordsX1X2Y1Y2(1300,    1325,   825,       975,      levelBlockX, levelBlockY), &gatePreset				));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(1675,    1700,   550,       825,      levelBlockX, levelBlockY), &preset		 			));
		curLevel->mpPlatforms.push_back(new Platform(		  CoordsX1X2Y1Y2(2275,    2300,   0,         375,      levelBlockX, levelBlockY), &preset		 			));

		curLevel->mpAllEnemies.push_back(new Enemy(Vect2( 800 + levelBlockX, 325 + levelBlockY), &fastRatPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(1600 + levelBlockX, 325 + levelBlockY), &bouncyRatPreset));
		curLevel->mpAllEnemies.push_back(new Enemy(Vect2(2000 + levelBlockX, 325 + levelBlockY), &fastRatPreset));

		curLevel->mpAllCollectibles.push_back(new Collectible(			Vect2(1600 + levelBlockX,  10 + levelBlockY), &keyPreset));
		curLevel->mpAllCollectibles.push_back(new SavePoint(			Vect2(2200 + levelBlockX, 100 + levelBlockY)));
		curLevel->mpAllCollectibles.push_back(new EndOfLevelCollectible(Vect2(1900 + levelBlockX, 750 + levelBlockY), &endOfLevelPreset));
	}

	finishAllLevelsSetUp(gameInstance);
}

void createMiniGameLevels(GameInstance& gameInstance)
{

}



void createMainGameMenu(GameInstance& gameInstance, const char * fontFileName)
{
	SDL_Renderer * pRenderer     = gameInstance.mScreen.mpRenderer;
	MenuManager  * pMenuManager  = &gameInstance.mMenuManager;
	StyleManager   styleManager  = gameInstance.mStyleManager;

	SDL_Color pink     = styleManager.pink;
	SDL_Color white    = styleManager.white;
	SDL_Color teal     = styleManager.teal;
	SDL_Color hintBlue = styleManager.hintBlue;
	SDL_Color clear    = styleManager.clear;

	int levelChunkWidth  = gameInstance.mScreen.mGameLevelChunkWidth;
	int levelChunkHeight = gameInstance.mScreen.mGameLevelChunkHeight;

	//MAIN GAME MENU PAGE
	MenuPage * mainGameMenuPage = new MenuPage();

	mainGameMenuPage->mpImageBoxes.push_back(new ImageBox(DontAutoShowImageBoxPreset(ETextBoxID_TAKE_DAMAGE_SCREEN), ImageBoxPositionInfo(Vect2(0, 0), 0, ETextBoxPositionAlign_LEFT, 1600, 900), "UIElements/takeDamageScreen.bmp"));

	//UPPER HUD
	int maxWidth  = 400;
	int maxHeight = 100;
	BlockAlignElementsVertically*   upperHUDBlock = new BlockAlignElementsVertically(  Hitbox(CoordsX1Y1WidthHeight(0,  0,    maxWidth, 450)), EDirection_RIGHT, EDirection_DOWN, Edges());
	BlockAlignElementsHorizontally* uLine1        = new BlockAlignElementsHorizontally(										  maxWidth, maxHeight,  ETextBoxPositionAlign_CENTER, EDirection_RIGHT, Edges(10, 0, 0, 0));
	ImageBox* hudIcon							  = new ImageBox(ImageBoxPreset(),																			   ImageBoxPositionInfo(0, ETextBoxPositionAlign_LEFT, 80, 80, Edges(0, 0, 0,  0)), "UIElements/HUDUI.bmp");
	TextBox*  curLevelBox						  = new TextBox( GameStatBoxPreset(EGameStatBoxValueToDisplay_CUR_LEVEL_NUMBER), ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(    ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_CENTER, maxWidth, maxHeight, Edges(0,  0,  0,   0)),  fontFileName,  16,   16, TextBoxColorInfo(hintBlue));
	uLine1->mpCurBoxes.push_back(hudIcon);
	uLine1->mpCurBoxes.push_back(curLevelBox);
	BlockAlignElementsHorizontally* uLine2        = new BlockAlignElementsHorizontally(										  maxWidth, maxHeight,  ETextBoxPositionAlign_CENTER, EDirection_RIGHT, Edges(10, 0, 5,  0));
	BlockAlignElementsVertically*   uLine3	      = new BlockAlignElementsVertically(								   	      maxWidth, maxHeight,  ETextBoxPositionAlign_CENTER, EDirection_DOWN,  Edges(0,  0, 0,  0));
	ImageBox* keyIcon                             = new ImageBox(ImageBoxPreset(),																			 ImageBoxPositionInfo(0, ETextBoxPositionAlign_CENTER, 20, 20, Edges(0, 0, 0, 0)), "UIElements/keyUI.bmp");
	ImageBox* targetIcon	                      = new ImageBox(ImageBoxPreset(),																			 ImageBoxPositionInfo(0, ETextBoxPositionAlign_CENTER, 20, 20, Edges(3, 0, 0, 0)), "UIElements/targetUI.bmp");
	ImageBox* collectiblesIcon					  = new ImageBox(ImageBoxPreset(),																			 ImageBoxPositionInfo(0, ETextBoxPositionAlign_CENTER, 20, 20, Edges(3, 0, 0, 0)), "UIElements/collectibleUI.bmp");
	ImageBox* enemiesIcon						  = new ImageBox(ImageBoxPreset(),																			 ImageBoxPositionInfo(0, ETextBoxPositionAlign_CENTER, 20, 20, Edges(3, 0, 0, 0)), "UIElements/enemiesUI.bmp");
	uLine3->mpCurBoxes.push_back(keyIcon);
	uLine3->mpCurBoxes.push_back(targetIcon);
	uLine3->mpCurBoxes.push_back(collectiblesIcon);
	uLine3->mpCurBoxes.push_back(enemiesIcon);

	BlockAlignElementsVertically*   uLine4	      = new BlockAlignElementsVertically(								   	      maxWidth, maxHeight, ETextBoxPositionAlign_LEFT, EDirection_DOWN,    Edges(0,  0, 8, 0));
	TextBox*  curKeysBox                          = new TextBox( GameStatBoxPreset(EGameStatBoxValueToDisplay_CUR_KEYS),		 ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(    ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_CENTER, maxWidth, maxHeight, Edges(3,  0, 0,  0)),  fontFileName,  12,   12, TextBoxColorInfo(hintBlue));
	TextBox*  curTargetsBox						  = new TextBox( GameStatBoxPreset(EGameStatBoxValueToDisplay_CUR_TARGETS),	     ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(    ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_CENTER, maxWidth, maxHeight, Edges(5,  0, 0,  0)),  fontFileName,  12,   12, TextBoxColorInfo(hintBlue));
	TextBox*  curCollectiblesBox				  = new TextBox( GameStatBoxPreset(EGameStatBoxValueToDisplay_CUR_COLLECTIBLES), ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(    ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_CENTER, maxWidth, maxHeight, Edges(6,  0, 0,  0)),  fontFileName,  12,   12, TextBoxColorInfo(hintBlue));
	TextBox*  curEnemiesLeftBox					  = new TextBox( GameStatBoxPreset(EGameStatBoxValueToDisplay_CUR_ENEMIES_LEFT), ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(    ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_CENTER, maxWidth, maxHeight, Edges(9,  0, 0,  0)),  fontFileName,  12,   12, TextBoxColorInfo(hintBlue));
	uLine4->mpCurBoxes.push_back(curKeysBox);
	uLine4->mpCurBoxes.push_back(curTargetsBox);
	uLine4->mpCurBoxes.push_back(curCollectiblesBox);
	uLine4->mpCurBoxes.push_back(curEnemiesLeftBox);

	upperHUDBlock->mpSubBlocks.push_back(uLine1);
	upperHUDBlock->mpSubBlocks.push_back(uLine2);
	uLine2->mpSubBlocks.push_back(uLine3);
	uLine2->mpSubBlocks.push_back(uLine4);

	mainGameMenuPage->mpBlocks.push_back(upperHUDBlock);
	mainGameMenuPage->mpImageBoxes.push_back(hudIcon);
	mainGameMenuPage->mpAllDisplayOnlyTextBoxes.push_back(curLevelBox);
	mainGameMenuPage->mpImageBoxes.push_back(keyIcon);
	mainGameMenuPage->mpAllDisplayOnlyTextBoxes.push_back(curKeysBox);
	mainGameMenuPage->mpImageBoxes.push_back(targetIcon);
	mainGameMenuPage->mpAllDisplayOnlyTextBoxes.push_back(curTargetsBox);
	mainGameMenuPage->mpImageBoxes.push_back(collectiblesIcon);
	mainGameMenuPage->mpAllDisplayOnlyTextBoxes.push_back(curCollectiblesBox);
	mainGameMenuPage->mpImageBoxes.push_back(enemiesIcon);
	mainGameMenuPage->mpAllDisplayOnlyTextBoxes.push_back(curEnemiesLeftBox);

	//LOWER LEFT HUD
	BlockAlignElementsVertically*   lowerLeftHUDBlock = new BlockAlignElementsVertically(  Hitbox(CoordsX1Y1WidthHeight(0,   900, maxWidth, 450)), EDirection_RIGHT, EDirection_UP, Edges(), "head block");
	BlockAlignElementsHorizontally* llLine1	          = new BlockAlignElementsHorizontally(										  maxWidth, maxHeight,   ETextBoxPositionAlign_CENTER, EDirection_RIGHT, Edges(0,  10, 0, 0), "slash block");
	TextBox* slashKeyBox				= new TextBox( GameStatBoxPreset(EGameStatBoxValueToDisplay_SLASH_KEY),				    ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(     ETextBoxPositionAlign_LEFT,   ETextBoxTextAlign_CENTER, 100,  60, Edges(0,  0, 10, 0)), fontFileName, 15, 15, TextBoxColorInfo(hintBlue));          
	ImageBox* slashIcon					= new ImageBox(DontAutoShowImageBoxPreset(ETextBoxID_SLASH_UI),														  ImageBoxPositionInfo(0,  ETextBoxPositionAlign_CENTER,						   40,   25, Edges(0,  0, 30, 0)), "UIElements/slashUI.bmp");
	llLine1->mpCurBoxes.push_back(slashKeyBox);
	llLine1->mpCurBoxes.push_back(slashIcon);
	BlockAlignElementsHorizontally* llLine2	          = new BlockAlignElementsHorizontally(										  maxWidth, maxHeight,   ETextBoxPositionAlign_LEFT,   EDirection_RIGHT, Edges(0,  2,  0, 0), "double jump block");
	ImageBox* doubleJumpIcon			= new ImageBox(DontAutoShowImageBoxPreset(ETextBoxID_DOUBLE_JUMP_UI),												  ImageBoxPositionInfo(0,  ETextBoxPositionAlign_CENTER,						   40,   30, Edges(0,  0, 53, 0)), "UIElements/doubleJumpUI.bmp");
	llLine2->mpCurBoxes.push_back(doubleJumpIcon);
	BlockAlignElementsHorizontally* llLine3	          = new BlockAlignElementsHorizontally(										  maxWidth, maxHeight,   ETextBoxPositionAlign_CENTER, EDirection_RIGHT, Edges(0,  10, 0, 0), "projectile block");
	BlockAlignElementsVertically*   llLine4	          = new BlockAlignElementsVertically(										  maxWidth, maxHeight,   ETextBoxPositionAlign_CENTER, EDirection_DOWN,  Edges(20, 0,  0, 0), "projectile keys block");
	TextBox* projectileHorizontalKeyBox = new TextBox( GameStatBoxPreset(EGameStatBoxValueToDisplay_PROJECTILE_HORIZONTAL_KEY), ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(     ETextBoxPositionAlign_LEFT,   ETextBoxTextAlign_CENTER, 100,  60, Edges(0,  0, 10, 0)), fontFileName, 15, 15, TextBoxColorInfo(hintBlue));
	TextBox* projectileVerticalKeyBox   = new TextBox( GameStatBoxPreset(EGameStatBoxValueToDisplay_PROJECTILE_VERTICAL_KEY),   ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(     ETextBoxPositionAlign_LEFT,   ETextBoxTextAlign_CENTER, 100,  60, Edges(10, 0, 10, 0)), fontFileName, 15, 15, TextBoxColorInfo(hintBlue));
	llLine4->mpCurBoxes.push_back(projectileHorizontalKeyBox);
	llLine4->mpCurBoxes.push_back(projectileVerticalKeyBox);
	BlockAlignElementsHorizontally* llLine5	          = new BlockAlignElementsHorizontally(										  maxWidth, maxHeight,   ETextBoxPositionAlign_LEFT,   EDirection_RIGHT, Edges(0,  0,  0, 0), "projectile icons block");
	ImageBox* projectileIcon1           = new ImageBox(DontAutoShowImageBoxPreset(ETextBoxID_PROJECTILE_UI),												  ImageBoxPositionInfo(30, ETextBoxPositionAlign_CENTER,						   80,   90, Edges(0,  0, 40, 0)), "UIElements/swordUI.bmp");
	ImageBox* projectileIcon2           = new ImageBox(DontAutoShowImageBoxPreset(ETextBoxID_PROJECTILE_UI),												  ImageBoxPositionInfo(30, ETextBoxPositionAlign_CENTER,						   80,   90, Edges(0,  0, 22, 0)), "UIElements/swordUI.bmp");
	ImageBox* projectileIcon3           = new ImageBox(DontAutoShowImageBoxPreset(ETextBoxID_PROJECTILE_UI),												  ImageBoxPositionInfo(30, ETextBoxPositionAlign_CENTER,						   80,   90, Edges(0,  0, 22, 0)), "UIElements/swordUI.bmp");
	ImageBox* projectileIcon4           = new ImageBox(DontAutoShowImageBoxPreset(ETextBoxID_PROJECTILE_UI),												  ImageBoxPositionInfo(30, ETextBoxPositionAlign_CENTER,						   80,   90, Edges(0,  0, 22, 0)), "UIElements/swordUI.bmp");
	ImageBox* projectileIcon5           = new ImageBox(DontAutoShowImageBoxPreset(ETextBoxID_PROJECTILE_UI),												  ImageBoxPositionInfo(30, ETextBoxPositionAlign_CENTER,						   80,   90, Edges(0,  0, 22, 0)), "UIElements/swordUI.bmp");
	ImageBox* projectileIcon6           = new ImageBox(DontAutoShowImageBoxPreset(ETextBoxID_PROJECTILE_UI),												  ImageBoxPositionInfo(30, ETextBoxPositionAlign_CENTER,						   80,   90, Edges(0,  0, 22, 0)), "UIElements/swordUI.bmp");
	ImageBox* projectileIcon7           = new ImageBox(DontAutoShowImageBoxPreset(ETextBoxID_PROJECTILE_UI),												  ImageBoxPositionInfo(30, ETextBoxPositionAlign_CENTER,						   80,   90, Edges(0,  0, 22, 0)), "UIElements/swordUI.bmp");
	llLine5->mpCurBoxes.push_back(projectileIcon1);
	llLine5->mpCurBoxes.push_back(projectileIcon2);
	llLine5->mpCurBoxes.push_back(projectileIcon3);
	llLine5->mpCurBoxes.push_back(projectileIcon4);
	llLine5->mpCurBoxes.push_back(projectileIcon5);
	llLine5->mpCurBoxes.push_back(projectileIcon6);
	llLine5->mpCurBoxes.push_back(projectileIcon7);
	llLine3->mpSubBlocks.push_back(llLine4);
	llLine3->mpSubBlocks.push_back(llLine5);

	lowerLeftHUDBlock->mpSubBlocks.push_back(llLine1);
	lowerLeftHUDBlock->mpSubBlocks.push_back(llLine2);
	lowerLeftHUDBlock->mpSubBlocks.push_back(llLine3);

	mainGameMenuPage->mpBlocks.push_back(lowerLeftHUDBlock);
	mainGameMenuPage->mpAllDisplayOnlyTextBoxes.push_back(slashKeyBox);
	mainGameMenuPage->mpImageBoxes.push_back(slashIcon);
	mainGameMenuPage->mpImageBoxes.push_back(doubleJumpIcon);
	mainGameMenuPage->mpAllDisplayOnlyTextBoxes.push_back(projectileHorizontalKeyBox);
	mainGameMenuPage->mpAllDisplayOnlyTextBoxes.push_back(projectileVerticalKeyBox);
	mainGameMenuPage->mpImageBoxes.push_back(projectileIcon1);
	mainGameMenuPage->mpImageBoxes.push_back(projectileIcon2);
	mainGameMenuPage->mpImageBoxes.push_back(projectileIcon3);
	mainGameMenuPage->mpImageBoxes.push_back(projectileIcon4);
	mainGameMenuPage->mpImageBoxes.push_back(projectileIcon5);
	mainGameMenuPage->mpImageBoxes.push_back(projectileIcon6);
	mainGameMenuPage->mpImageBoxes.push_back(projectileIcon7);

	//LOWER RIGHT HUD
	BlockAlignElementsHorizontally* lowerRightHUDBlock   = new BlockAlignElementsHorizontally(Hitbox(CoordsX1Y1WidthHeight(1600, 900,   maxWidth, 450)), EDirection_LEFT,  EDirection_UP, Edges(0, 10, 0, 0), "head block");

	//Movement
	BlockAlignElementsVertically*   lrLine1	             = new BlockAlignElementsVertically(										   maxWidth, maxHeight, ETextBoxPositionAlign_CENTER, EDirection_DOWN,  Edges(0, 0,  0,  10), "Movement Controls Block");
	BlockAlignElementsHorizontally* lrLine2              = new BlockAlignElementsHorizontally(										   maxWidth, maxHeight, ETextBoxPositionAlign_CENTER, EDirection_RIGHT, Edges(0, 0,  32, 0),  "Up Movement Controls Block");
	ImageBox*						upMovementIcon       = new ImageBox(ImageBoxPreset(),																				ImageBoxPositionInfo(0, ETextBoxPositionAlign_CENTER,							20,   20, Edges(0, 0, 0, 0)), "UIElements/upUI.bmp");
	lrLine2->mpCurBoxes.push_back(upMovementIcon);
	BlockAlignElementsHorizontally* lrLine3              = new BlockAlignElementsHorizontally(										   maxWidth, maxHeight, ETextBoxPositionAlign_CENTER, EDirection_RIGHT, Edges(2, 0,  15,  0),  "Left Right Movement Controls");
	ImageBox*						leftMovementIcon     = new ImageBox(ImageBoxPreset(),																				ImageBoxPositionInfo(0, ETextBoxPositionAlign_CENTER,							20,   20, Edges(0, 0, 0,  0)), "UIElements/leftUI.bmp");
	ImageBox*						rightMovementIcon    = new ImageBox(ImageBoxPreset(),																				ImageBoxPositionInfo(0, ETextBoxPositionAlign_CENTER,							20,   20, Edges(0, 0, 22, 0)), "UIElements/rightUI.bmp");
	lrLine3->mpCurBoxes.push_back(leftMovementIcon);
	lrLine3->mpCurBoxes.push_back(rightMovementIcon);
	BlockAlignElementsVertically*   lrLine4              = new BlockAlignElementsVertically(						   			       maxWidth, maxHeight, ETextBoxPositionAlign_CENTER, EDirection_DOWN,  Edges(3, 0,  0,  0),  "Movement Keys Block");
	TextBox*                        leftMovementControl  = new TextBox( GameStatBoxPreset(EGameStatBoxValueToDisplay_MOVEMENT_LEFT_KEY),	  ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(	ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER,	300,  60, Edges(0, 0, 0, 0)), fontFileName, 12, 12, TextBoxColorInfo(hintBlue));
	TextBox*                        upMovementControl    = new TextBox( GameStatBoxPreset(EGameStatBoxValueToDisplay_MOVEMENT_UP_KEY),	      ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(	ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER,	300,  60, Edges(0, 0, 0, 0)), fontFileName, 12, 12, TextBoxColorInfo(hintBlue));
	TextBox*                        rightMovementControl = new TextBox( GameStatBoxPreset(EGameStatBoxValueToDisplay_MOVEMENT_RIGHT_KEY),	  ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(	ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER,	300,  60, Edges(0, 0, 0, 0)), fontFileName, 12, 12, TextBoxColorInfo(hintBlue));
	lrLine4->mpCurBoxes.push_back(leftMovementControl);
	lrLine4->mpCurBoxes.push_back(upMovementControl);
	lrLine4->mpCurBoxes.push_back(rightMovementControl);
	lrLine1->mpSubBlocks.push_back(lrLine2);
	lrLine1->mpSubBlocks.push_back(lrLine3);
	lrLine1->mpSubBlocks.push_back(lrLine4);

	//resetLevel
	BlockAlignElementsVertically*   lrLine5	             = new BlockAlignElementsVertically(										   maxWidth, maxHeight, ETextBoxPositionAlign_CENTER, EDirection_DOWN,  Edges(15, 0,  0,  10), "Reset Level Block");
	ImageBox*						resetLevelIcon       = new ImageBox(ImageBoxPreset(),																				    ImageBoxPositionInfo(0, ETextBoxPositionAlign_CENTER,							20,   20, Edges(0, 0, 0, 0)), "UIElements/resetLevelUI.bmp");
	TextBox*                        resetLevelKey        = new TextBox( GameStatBoxPreset(EGameStatBoxValueToDisplay_LEVEL_RESET_KEY),        ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(    ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER,	100,  60, Edges(0, 0, 0, 0)), fontFileName, 12, 12, TextBoxColorInfo(hintBlue));
	lrLine5->mpCurBoxes.push_back(resetLevelIcon);
	lrLine5->mpCurBoxes.push_back(resetLevelKey);

	//resetCheckpoint
	BlockAlignElementsVertically*   lrLine6	             = new BlockAlignElementsVertically(										   maxWidth, maxHeight, ETextBoxPositionAlign_CENTER, EDirection_DOWN,  Edges(15, 0,  0,  10), "Reset Checkpoint Block");
	ImageBox*						resetCheckpointIcon  = new ImageBox(ImageBoxPreset(),																				    ImageBoxPositionInfo(0, ETextBoxPositionAlign_CENTER,							20,   20, Edges(0, 0, 0, 0)), "UIElements/resetCheckpointUI.bmp");
	TextBox*                        resetCheckpointKey   = new TextBox( GameStatBoxPreset(EGameStatBoxValueToDisplay_CHECKPOINT_RESET_KEY),   ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(    ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER,	100,  60, Edges(0, 0, 0, 0)), fontFileName, 12, 12, TextBoxColorInfo(hintBlue));
	lrLine6->mpCurBoxes.push_back(resetCheckpointIcon);
	lrLine6->mpCurBoxes.push_back(resetCheckpointKey);

	lowerRightHUDBlock->mpSubBlocks.push_back(lrLine1);
	lowerRightHUDBlock->mpSubBlocks.push_back(lrLine5);
	lowerRightHUDBlock->mpSubBlocks.push_back(lrLine6);

	mainGameMenuPage->mpBlocks.push_back(lowerRightHUDBlock);
	mainGameMenuPage->mpImageBoxes.push_back(upMovementIcon);
	mainGameMenuPage->mpImageBoxes.push_back(leftMovementIcon);
	mainGameMenuPage->mpImageBoxes.push_back(rightMovementIcon);
	mainGameMenuPage->mpAllDisplayOnlyTextBoxes.push_back(leftMovementControl);
	mainGameMenuPage->mpAllDisplayOnlyTextBoxes.push_back(upMovementControl);
	mainGameMenuPage->mpAllDisplayOnlyTextBoxes.push_back(rightMovementControl);
	mainGameMenuPage->mpImageBoxes.push_back(resetLevelIcon);
	mainGameMenuPage->mpAllDisplayOnlyTextBoxes.push_back(resetLevelKey);
	mainGameMenuPage->mpImageBoxes.push_back(resetCheckpointIcon);
	mainGameMenuPage->mpAllDisplayOnlyTextBoxes.push_back(resetCheckpointKey);

	pMenuManager->mpMenuPages.push_back(mainGameMenuPage);
}


void createMenus(GameInstance& gameInstance)
{
	SDL_Renderer * pRenderer     = gameInstance.mScreen.mpRenderer;
	MenuManager  * pMenuManager  = &(gameInstance.mMenuManager);
	StyleManager   styleManager  = gameInstance.mStyleManager;

	const char * fontFileName = styleManager.robotoFontName;

	SDL_Color pink     = styleManager.pink;
	SDL_Color white    = styleManager.white;
	SDL_Color teal     = styleManager.teal;
	SDL_Color hintBlue = styleManager.hintBlue;
	SDL_Color clear    = styleManager.clear;

	int levelChunkWidth  = gameInstance.mScreen.mGameLevelChunkWidth;
	int levelChunkHeight = gameInstance.mScreen.mGameLevelChunkHeight;



	//MAIN MENU PAGE
	MenuPage * mainMenuPage = new MenuPage();

	//															 dataStorage,					          textBoxFunction,	                   positionInfo,																											fileName,			  standardSize, selectedSize,   colorInfo)
	mainMenuPage->mpAllSelectableTextBoxes.push_back(new TextBox(StandardTextBoxPreset("PLAY GAME"),      ETextBoxFunction_PLAY_GAME_BOX,      TextBoxPositionInfo(Vect2(levelChunkWidth / 2, 200), ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, 800, 200),  fontFileName,		  60,			80,				TextBoxColorInfo(pink, white, teal, teal)));
	mainMenuPage->mpAllSelectableTextBoxes.push_back(new TextBox(StandardTextBoxPreset("PLAY MINI GAME"), ETextBoxFunction_PLAY_MINI_GAME_BOX, TextBoxPositionInfo(Vect2(levelChunkWidth / 2, 325), ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, 800, 200),  fontFileName,		  60,			80,				TextBoxColorInfo(pink, white, teal, teal)));

	pMenuManager->mpMenuPages.push_back(mainMenuPage);

	createMainGameMenu(gameInstance, fontFileName);


	//MINI GAME MENU PAGE
	MenuPage * miniGameMenuPage = new MenuPage();

	//int curX = 1400;

	//miniGameMenuPage->mpAllSelectableTextBoxes.push_back(new TextBox(StandardTextBoxPreset("PLAY GAME"), ETextBoxFunction_PLAY_GAME_BOX,       TextBoxPositionInfo(Vect2(curX, 50),  ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, 300, 60), fontFileName, 20, 35, TextBoxColorInfo(pink, white, teal, teal)));
	//miniGameMenuPage->mpAllSelectableTextBoxes.push_back(new TextBox(StandardTextBoxPreset("MAIN MENU"), ETextBoxFunction_GO_TO_MAIN_MENU_BOX, TextBoxPositionInfo(Vect2(curX, 100), ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, 300, 60), fontFileName, 20, 35, TextBoxColorInfo(pink, white, teal, teal)));

	//curX     = 100;
	//for (int count = 0; count < gameInstance.mMiniGameWorldData.mpMiniGameLevels[0]->mpCombatManager->mpAllCombatCharacters.size(); count++)
	//{
	//	miniGameMenuPage->mpAllDisplayOnlyTextBoxes.push_back(new TextBox(MiniGameCharacterBoxPreset(count, ECharacterStatBoxValueToDisplay_CHARACTER_NAME),    ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(Vect2(curX,  90),   ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, 300,  60), fontFileName, 20, 20, TextBoxColorInfo(pink, white, teal, teal)));
	//	miniGameMenuPage->mpAllDisplayOnlyTextBoxes.push_back(new TextBox(MiniGameCharacterBoxPreset(count, ECharacterStatBoxValueToDisplay_CHARACTER_HEALTH),  ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(Vect2(curX,  150),  ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, 300,  60), fontFileName, 15, 15, TextBoxColorInfo(pink, white, teal, teal)));
	//	miniGameMenuPage->mpAllDisplayOnlyTextBoxes.push_back(new TextBox(MiniGameCharacterBoxPreset(count, ECharacterStatBoxValueToDisplay_CHARACTER_DEFENSE), ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(Vect2(curX,  200),  ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, 300,  60), fontFileName, 15, 15, TextBoxColorInfo(pink, white, teal, teal)));
	//	miniGameMenuPage->mpAllDisplayOnlyTextBoxes.push_back(new TextBox(MiniGameCharacterBoxPreset(count, ECharacterStatBoxValueToDisplay_CHARACTER_STUN),    ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(Vect2(curX,  250),  ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, 300,  60), fontFileName, 15, 15, TextBoxColorInfo(pink, white, teal, teal)));
	//	curX += 250;
	//}

	//std::vector <EMiniGameState> miniGameStateWhenToShowList;

	//curX = 1100;

	////CHARACTER
	////SELECT TILE TO MOVE TO
	////MOVE TYPE
	////MOVE DIAGRAM
	//miniGameStateWhenToShowList.clear();
	//miniGameStateWhenToShowList.push_back(EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT);
	//miniGameMenuPage->mpAllDisplayOnlyTextBoxes.push_back(new TextBox(MiniGamePlayerBoxPreset(ECharacterStatBoxValueToDisplay_CHARACTER_NAME,      miniGameStateWhenToShowList), ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(Vect2(curX,  400), ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER,  500,  50), fontFileName, 20, 20, TextBoxColorInfo(pink, white, teal, teal)));
	//miniGameMenuPage->mpAllDisplayOnlyTextBoxes.push_back(new TextBox(MiniGameBoxPreset("Select Tile to Move To",					    		   miniGameStateWhenToShowList), ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(Vect2(curX,  450), ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER,  500,  50), fontFileName, 25, 25, TextBoxColorInfo(pink, white, teal, teal)));
	//miniGameMenuPage->mpAllDisplayOnlyTextBoxes.push_back(new TextBox(MiniGamePlayerBoxPreset(ECharacterStatBoxValueToDisplay_CHARACTER_MOVE_TYPE, miniGameStateWhenToShowList), ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(Vect2(curX,  500), ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER,  500,  50), fontFileName, 20, 20, TextBoxColorInfo(pink, white, teal, teal)));

	////CHARACTER
	////CHOOSE ACTION
	////ATTACK
	////DEFEND
	////PASS
	//miniGameStateWhenToShowList.clear();
	//miniGameStateWhenToShowList.push_back(EMiniGameState_PLAYER_WAIT_FOR_ACTION_INPUT);
	//miniGameMenuPage->mpAllDisplayOnlyTextBoxes.push_back(new TextBox(MiniGamePlayerBoxPreset(ECharacterStatBoxValueToDisplay_CHARACTER_NAME,       miniGameStateWhenToShowList), ETextBoxFunction_NO_FUNCTION,						   TextBoxPositionInfo(Vect2(curX, 400), ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, 500, 50), fontFileName, 20, 20, TextBoxColorInfo(pink, white, teal, teal)));
	//miniGameMenuPage->mpAllSelectableTextBoxes.push_back( new TextBox(MiniGameBoxPreset("ATTACK",													miniGameStateWhenToShowList), ETextBoxFunction_ATTACK_CUR_COMBAT_CHARACTER_BOX,	   TextBoxPositionInfo(Vect2(curX, 450), ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, 500, 50), fontFileName, 25, 30, TextBoxColorInfo(pink, white, teal, teal)));
	//miniGameMenuPage->mpAllSelectableTextBoxes.push_back( new TextBox(MiniGameBoxPreset("DEFEND",													miniGameStateWhenToShowList), ETextBoxFunction_DEFEND_CUR_COMBAT_CHARACTER_BOX,	   TextBoxPositionInfo(Vect2(curX, 500), ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, 500, 50), fontFileName, 25, 30, TextBoxColorInfo(pink, white, teal, teal)));
	//miniGameMenuPage->mpAllSelectableTextBoxes.push_back( new TextBox(MiniGameBoxPreset("PASS TURN",												miniGameStateWhenToShowList), ETextBoxFunction_PASS_CUR_COMBAT_CHARACTER_TURN_BOX, TextBoxPositionInfo(Vect2(curX, 550), ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, 500, 50), fontFileName, 25, 30, TextBoxColorInfo(pink, white, teal, teal)));

	////CHARACTER
	////CHOOSE ATTACK
	////OPTION 1 NAME
	////OPTION 1 TYPE
	////OPTION 1 DAMAGE
	////OPTION 1 SPECIAL EFFECTS / NOTES
	////OPTION 2 NAME
	////OPTION 2 TYPE
	////OPTION 2 DAMAGE
	////OPTION 2 SPECIAL EFFECTS / NOTES
	//miniGameStateWhenToShowList.clear();
	//miniGameStateWhenToShowList.push_back(EMiniGameState_PLAYER_WAIT_FOR_ATTACK_INPUT);
	//miniGameMenuPage->mpAllDisplayOnlyTextBoxes.push_back( new TextBox(MiniGamePlayerBoxPreset(ECharacterStatBoxValueToDisplay_CHARACTER_NAME,										miniGameStateWhenToShowList),  ETextBoxFunction_NO_FUNCTION,			TextBoxPositionInfo(Vect2(curX,  400), ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, 500,  50), fontFileName, 20, 20, TextBoxColorInfo(pink, white, teal, teal)));
	//miniGameMenuPage->mpAllDisplayOnlyTextBoxes.push_back( new TextBox(MiniGameBoxPreset("Choose Attack",																			miniGameStateWhenToShowList),  ETextBoxFunction_NO_FUNCTION,		    TextBoxPositionInfo(Vect2(curX,  450), ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, 500,  50), fontFileName, 35, 35, TextBoxColorInfo(pink, white, teal, teal)));
	//miniGameMenuPage->mpAllSelectableTextBoxes.push_back(  new TextBox(MiniGamePlayerBoxPreset(ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_1_NAME,						miniGameStateWhenToShowList),  ETextBoxFunction_ATTACK_STYLE_1_BOX,		TextBoxPositionInfo(Vect2(curX,  500), ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, 500,  50), fontFileName, 25, 30, TextBoxColorInfo(pink, white, teal, teal)));
	//miniGameMenuPage->mpAllDisplayOnlyTextBoxes.push_back( new TextBox(MiniGamePlayerBoxPreset(ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_1_TYPE,						miniGameStateWhenToShowList),  ETextBoxFunction_NO_FUNCTION,			TextBoxPositionInfo(Vect2(curX,  550), ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, 500,  50), fontFileName, 18, 18, TextBoxColorInfo(pink, white, teal, teal)));
	//miniGameMenuPage->mpAllDisplayOnlyTextBoxes.push_back( new TextBox(MiniGamePlayerBoxPreset(ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_1_DAMAGE,					miniGameStateWhenToShowList),  ETextBoxFunction_NO_FUNCTION,	        TextBoxPositionInfo(Vect2(curX,  600), ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, 500,  50), fontFileName, 18, 18, TextBoxColorInfo(pink, white, teal, teal)));
	//miniGameMenuPage->mpAllDisplayOnlyTextBoxes.push_back( new TextBox(MiniGamePlayerBoxPreset(ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_1_SPECIAL_EFFECTS_AND_NOTES,	miniGameStateWhenToShowList),  ETextBoxFunction_NO_FUNCTION,		    TextBoxPositionInfo(Vect2(curX,  650), ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, 500,  50), fontFileName, 18, 18, TextBoxColorInfo(pink, white, teal, teal)));
	//miniGameMenuPage->mpAllSelectableTextBoxes.push_back(  new TextBox(MiniGamePlayerBoxPreset(ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_2_NAME,						miniGameStateWhenToShowList),  ETextBoxFunction_ATTACK_STYLE_2_BOX,		TextBoxPositionInfo(Vect2(curX,  700), ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, 500,  50), fontFileName, 25, 30, TextBoxColorInfo(pink, white, teal, teal)));
	//miniGameMenuPage->mpAllDisplayOnlyTextBoxes.push_back( new TextBox(MiniGamePlayerBoxPreset(ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_2_TYPE,						miniGameStateWhenToShowList),  ETextBoxFunction_NO_FUNCTION,		    TextBoxPositionInfo(Vect2(curX,  750), ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, 500,  50), fontFileName, 18, 18, TextBoxColorInfo(pink, white, teal, teal)));
	//miniGameMenuPage->mpAllDisplayOnlyTextBoxes.push_back( new TextBox(MiniGamePlayerBoxPreset(ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_2_DAMAGE,					miniGameStateWhenToShowList),  ETextBoxFunction_NO_FUNCTION,		    TextBoxPositionInfo(Vect2(curX,  800), ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, 500,  50), fontFileName, 18, 18, TextBoxColorInfo(pink, white, teal, teal)));
	//miniGameMenuPage->mpAllDisplayOnlyTextBoxes.push_back( new TextBox(MiniGamePlayerBoxPreset(ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_2_SPECIAL_EFFECTS_AND_NOTES,	miniGameStateWhenToShowList),  ETextBoxFunction_NO_FUNCTION,		    TextBoxPositionInfo(Vect2(curX,  850), ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, 500,  50), fontFileName, 18, 18, TextBoxColorInfo(pink, white, teal, teal)));


	////CHARACTER
	////CHOOSE ATTACK DIRECTION
	////CUR ATTACK NAME
	////LEFT + DIAGRAM
	////RIGHT + DIAGRAM
	////UP + DIAGRAM
	////DOWN + DIAGRAM
	//miniGameStateWhenToShowList.clear();
	//miniGameStateWhenToShowList.push_back(EMiniGameState_PLAYER_WAIT_FOR_ATTACK_SUB_INPUT);
	//miniGameMenuPage->mpAllDisplayOnlyTextBoxes.push_back(new TextBox(MiniGamePlayerBoxPreset(ECharacterStatBoxValueToDisplay_CHARACTER_NAME,									   miniGameStateWhenToShowList),  ETextBoxFunction_NO_FUNCTION,				   TextBoxPositionInfo(Vect2(curX,      400), ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER,	500,  50), fontFileName, 20, 35, TextBoxColorInfo(pink, white, teal, teal)));
	//miniGameMenuPage->mpAllDisplayOnlyTextBoxes.push_back(new TextBox(MiniGameBoxPreset("Choose Attack Direction",																   miniGameStateWhenToShowList),  ETextBoxFunction_NO_FUNCTION,				   TextBoxPositionInfo(Vect2(curX,      450), ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, 	500,  50), fontFileName, 20, 35, TextBoxColorInfo(pink, white, teal, teal)));
	//miniGameMenuPage->mpAllDisplayOnlyTextBoxes.push_back(new TextBox(MiniGamePlayerBoxPreset(ECharacterStatBoxValueToDisplay_CHARACTER_CUR_ATTACK_NAME,						   miniGameStateWhenToShowList),  ETextBoxFunction_NO_FUNCTION,				   TextBoxPositionInfo(Vect2(curX,      500), ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER,	500,  50), fontFileName, 20, 35, TextBoxColorInfo(pink, white, teal, teal)));
	//miniGameMenuPage->mpAllSelectableTextBoxes.push_back( new TextBox(MiniGameBoxPreset("LEFT",																					   miniGameStateWhenToShowList),  ETextBoxFunction_ATTACK_DIRECTION_LEFT_BOX,  TextBoxPositionInfo(Vect2(curX - 75, 600), ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER,	500,  50), fontFileName, 20, 35, TextBoxColorInfo(pink, white, teal, teal)));
	//miniGameMenuPage->mpAllSelectableTextBoxes.push_back( new TextBox(MiniGameBoxPreset("RIGHT",																				   miniGameStateWhenToShowList),  ETextBoxFunction_ATTACK_DIRECTION_RIGHT_BOX, TextBoxPositionInfo(Vect2(curX + 75, 600), ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER,	500,  50), fontFileName, 20, 35, TextBoxColorInfo(pink, white, teal, teal)));
	//miniGameMenuPage->mpAllSelectableTextBoxes.push_back( new TextBox(MiniGameBoxPreset("UP",																					   miniGameStateWhenToShowList),  ETextBoxFunction_ATTACK_DIRECTION_UP_BOX,    TextBoxPositionInfo(Vect2(curX - 75, 650), ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER,	500,  50), fontFileName, 20, 35, TextBoxColorInfo(pink, white, teal, teal)));
	//miniGameMenuPage->mpAllSelectableTextBoxes.push_back( new TextBox(MiniGameBoxPreset("DOWN",																					   miniGameStateWhenToShowList),  ETextBoxFunction_ATTACK_DIRECTION_DOWN_BOX,  TextBoxPositionInfo(Vect2(curX + 75, 650), ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER,	500,  50), fontFileName, 20, 35, TextBoxColorInfo(pink, white, teal, teal)));

	////CHARACTER
	////SELECT TILE(S) TO ATTACK
	//miniGameStateWhenToShowList.clear();
	//miniGameStateWhenToShowList.push_back(EMiniGameState_PLAYER_TAKE_ACTION_ATTACK);
	//miniGameMenuPage->mpAllDisplayOnlyTextBoxes.push_back(new TextBox(MiniGamePlayerBoxPreset(ECharacterStatBoxValueToDisplay_CHARACTER_NAME,									   miniGameStateWhenToShowList),  ETextBoxFunction_NO_FUNCTION,				   TextBoxPositionInfo(Vect2(curX,      400), ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, 500,  50), fontFileName, 20, 35, TextBoxColorInfo(pink, white, teal, teal)));
	//miniGameMenuPage->mpAllDisplayOnlyTextBoxes.push_back(new TextBox(MiniGameBoxPreset("Select a Tile to Attack",																   miniGameStateWhenToShowList),  ETextBoxFunction_NO_FUNCTION,				   TextBoxPositionInfo(Vect2(curX,      450), ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, 500,  50), fontFileName, 20, 35, TextBoxColorInfo(pink, white, teal, teal)));
	//miniGameMenuPage->mpAllDisplayOnlyTextBoxes.push_back(new TextBox(MiniGamePlayerBoxPreset(ECharacterStatBoxValueToDisplay_CHARACTER_CUR_ATTACK_NAME,						   miniGameStateWhenToShowList),  ETextBoxFunction_NO_FUNCTION,				   TextBoxPositionInfo(Vect2(curX,      500), ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, 500,  50), fontFileName, 20, 35, TextBoxColorInfo(pink, white, teal, teal)));

	//pMenuManager->mpMenuPages.push_back(miniGameMenuPage);


	//SETTINGS
	MenuPage * settingsPage = new MenuPage();

	//															 dataStorage,					          textBoxFunction,	                   positionInfo,																											fileName,			  standardSize, selectedSize,   colorInfo)
	settingsPage->mpAllDisplayOnlyTextBoxes.push_back(new TextBox(StandardTextBoxPreset("SETTINGS"),      ETextBoxFunction_NO_FUNCTION,        TextBoxPositionInfo(Vect2(levelChunkWidth / 2, 200), ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, 1600, 200), fontFileName,		  60,			80,				TextBoxColorInfo(white, teal)));
	settingsPage->mpAllDisplayOnlyTextBoxes.push_back(new TextBox(StandardTextBoxPreset("Text Size"),     ETextBoxFunction_NO_FUNCTION,        TextBoxPositionInfo(Vect2(levelChunkWidth / 2, 200), ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, 1600, 200), fontFileName,		  60,			80,				TextBoxColorInfo(white, teal)));


	pMenuManager->mpMenuPages.push_back(settingsPage);





	pMenuManager->mpCurMenuPage = pMenuManager->mpMenuPages[EMenuPageType_MAIN_GAME_MENU];


	TextBox* testBox = new TextBox(StandardTextBoxPreset("A"), ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(Vect2(levelChunkWidth / 2, 200), ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, 1600, 200), fontFileName, 12, 12, TextBoxColorInfo(white));
	int textIncrease = 4;
	testBox->updateMessage(pRenderer, "A", 0);
	testBox->setBoxGroupMaxFontSize(testBox->getMaxFontSizeGivenText());
	testBox->updateTexture(pRenderer);

	pMenuManager->setAllTextBoxTextures(pRenderer, gameInstance.mSettingsManager.mTextIncrease);
	mainMenuPage->setCurTextBox(mainMenuPage->mpAllSelectableTextBoxes[0]);
	//miniGameMenuPage->setCurTextBox(miniGameMenuPage->mpAllSelectableTextBoxes[0]);
	pMenuManager->setUpBlocks();
}

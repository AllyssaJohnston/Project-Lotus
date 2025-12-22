#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <thread>
#include <chrono>

#include "globals.h"
#include "playerHelper.h"
#include "imageHelper.h"
#include "platformHelper.h"
#include "screenHelper.h"
#include "gameInstanceHelper.h"

#if DEMO == 0
#include "lotusAdventureLevels.h"
#else 
#include "lotusAdventureLevelsDemo.h"
#endif

#include "lotusAdventureMenus.h"
#include "hitboxHelper.h"
#include "worldHelper.h"
#include "gameStateHelper.h"

GameInstance gameInstance	= {};
Player& player				= gameInstance.mWorldData.mPlayer;
ScreenObject& screen		= gameInstance.mScreen;
LevelChunk& curLevelChunk	= gameInstance.mWorldData.mCurLevelChunk;

void setUpAllTextures()
{
	//Player
	if (DEMO == 0)
	{
		player.setHitboxTexture(AssetManager::getTextureFromSurface(screen.mpRenderer, player.getImageObjectHitbox().getSurface()));
	}
	player.setUpAllTextures(screen.mpRenderer);

	// Levels
	for (World* pWorld : gameInstance.mWorldData.mpWorlds)
	{
		for (Level* pLevel : pWorld->mpLevels)
		{
			//Platforms
			for (Platform* pPlatform : pLevel->mpPlatforms)
			{
				pPlatform->mAnimationManager.setUpAllTextures(screen.mpRenderer);
			}
			for (Platform* pPlatform : pLevel->mpAllNonStaticPlatforms)
			{
				pPlatform->mAnimationManager.setUpAllTextures(screen.mpRenderer);
			}
			for (AreaEffectPlatform* pPlatform : pLevel->mpAreaEffectPlatforms)
			{
				pPlatform->mAnimationManager.setUpAllTextures(screen.mpRenderer);
				pPlatform->mAreaEffectAnimationManager.setUpAllTextures(screen.mpRenderer);
			}

			//enemies
			for (Enemy* pEnemy : pLevel->mpAllEnemies)
			{
				pEnemy->mAnimationManager.setUpAllTextures(screen.mpRenderer);
				if (DEMO == 0)
				{
					pEnemy->setHitboxTexture(AssetManager::getTextureFromSurface(screen.mpRenderer, pEnemy->getImageObjectHitbox().getSurface()));

				}
			}

			//collectibles
			for (Collectible* pCollectible : pLevel->mpAllCollectibles)
			{
				pCollectible->mAnimationManager.setUpAllTextures(screen.mpRenderer);
			}
		}
	}

	//MiniGame Levels
	for (MiniGameWorld* pWorld : gameInstance.mMiniGameWorldData.mpMiniGameWorlds)
	{
		for (MiniGameLevel* pLevel : pWorld->mpLevels)
		{
			for (MiniGameStage* pStage : pLevel->mpStages)
			{
				//Characters
				for (CombatCharacter* pCharacter : pStage->mCombatManager.mpAllCombatCharacters)
				{
					pCharacter->mModel.setUpTexture(screen.mpRenderer);
				}
			}
		}
	}

	MenuManager& menuManager = gameInstance.mMenuManager;
	for (MenuPage* pMenuPage : menuManager.mpMenuPages)
	{
		for (UIBlock* pBlock : pMenuPage->mpBlocks)
		{
			pBlock->setAllTextures(screen.mpRenderer);
		}
	}
}

int main(int argc, char* args[]) 
{

	std::string basePath = SDL_GetBasePath();
	bool runGame = true;

	if (TTF_Init() < 0)
	{  
		SDL_assert(false); //Can't start without UI
		return -1;
	}

	createLevels(gameInstance.mWorldData, gameInstance.mMiniGameWorldData, gameInstance.mScreen); // PLATFORMING + MINI GAME
	setUpFontSizeChart(gameInstance.mFontSizeChart, gameInstance.mScreen.mpRenderer);
	createMenus(gameInstance.mMenuManager, gameInstance.mScreen, gameInstance.mMiniGameWorldData);
	setUpAllTextures();

	player.getMovementManager().setStartPosition(gameInstance.mWorldData.mpWorlds[gameInstance.mWorldData.mCurWorldNumber]->mpLevels[gameInstance.mWorldData.mCurLevelNumber]->mPlayerStartingPosition);
	gameInstance.mWorldData.resetStats();
	


	static std::chrono::time_point<std::chrono::steady_clock> frameStart;
	static std::chrono::time_point<std::chrono::steady_clock> frameEnd;
	
	while (gameInstance.mGameStateManager.mData.mRunGame)
	{
		frameStart = std::chrono::high_resolution_clock::now();
		gameInstance.preTick();

		SDL_RenderClear(gameInstance.mScreen.mpRenderer);
		gameInstance.tick();

		SDL_RenderPresent(gameInstance.mScreen.mpRenderer);

		gameInstance.postTick();

		frameEnd = std::chrono::high_resolution_clock::now();

		const auto deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(frameEnd - frameStart);
		if ((int)deltaTime.count() / (1000 * 1000) < 16)
		{
			//We running faster than 60fps, sleep till we hit 16ms.
			int sleepTime = 16 - (int)deltaTime.count() / (1000 * 1000);
			std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
		}
	}

	TTF_Quit();
	return 0;
}

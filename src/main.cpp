#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <thread>
#include <chrono>

#include "playerHelper.h"
#include "imageHelper.h"
#include "platformHelper.h"
#include "screenHelper.h"
#include "gameInstanceHelper.h"
#include "lotusAdventureLevelsDemo.h"
#include "hitboxHelper.h"
#include "worldHelper.h"
#include "gameStateHelper.h"

GameInstance gameInstance = {};
Player & player           = gameInstance.mWorldData.mPlayer;
ScreenObject & screen     = gameInstance.mScreen;
LevelChunk& curLevelChunk = gameInstance.mWorldData.mCurLevelChunk;

void setUpAllTextures()
{
	//Player
	player.setHitboxTexture(AssetManager::getTextureFromSurface(screen.mpRenderer, player.getImageObjectHitbox().getSurface()));

	player.setUpAllTextures(screen.mpRenderer);

	for (int countWorld = 0; countWorld < gameInstance.mWorldData.mpWorlds.size(); countWorld++)
	{
		for (int countLevel = 0; countLevel < gameInstance.mWorldData.mpWorlds[countWorld]->mpLevels.size(); countLevel++)
		{
			//Platforms
			Level* curLevel = gameInstance.mWorldData.mpWorlds[countWorld]->mpLevels[countLevel];
			for (int count = 0; count < curLevel->mpPlatforms.size(); count++)
			{
				Platform& curPlatform = *(curLevel->mpPlatforms[count]);
				curPlatform.mAnimationManager.setUpAllTextures(screen.mpRenderer);
			}
			for (int count = 0; count < curLevel->mpAllNonStaticPlatforms.size(); count++)
			{
				Platform& curPlatform = *(curLevel->mpAllNonStaticPlatforms[count]);
				curPlatform.mAnimationManager.setUpAllTextures(screen.mpRenderer);
			}
			for (int count = 0; count < curLevel->mpAreaEffectPlatforms.size(); count++)
			{
				AreaEffectPlatform& curPlatform = *(curLevel->mpAreaEffectPlatforms[count]);
				curPlatform.mAnimationManager.setUpAllTextures(screen.mpRenderer);
				curPlatform.mAreaEffectAnimationManager.setUpAllTextures(screen.mpRenderer);
			}

			//enemies
			for (int count = 0; count < curLevel->mpAllEnemies.size(); count++)
			{
				Enemy& curEnemy = *(curLevel->mpAllEnemies[count]);
				curEnemy.mAnimationManager.setUpAllTextures(screen.mpRenderer);
				curEnemy.setHitboxTexture(AssetManager::getTextureFromSurface(screen.mpRenderer, curEnemy.getImageObjectHitbox().getSurface()));
			}

			//collectibles
			for (int count = 0; count < curLevel->mpAllCollectibles.size(); count++)
			{
				Collectible& curCollectible = *(curLevel->mpAllCollectibles[count]);
				curCollectible.mAnimationManager.setUpAllTextures(screen.mpRenderer);
			}
		}
	}

	MenuManager& menuManager = gameInstance.mMenuManager;
	for (int countMenuPage = 0; countMenuPage < menuManager.mpMenuPages.size(); countMenuPage++)
	{
		MenuPage* pCurMenuPage = menuManager.mpMenuPages[countMenuPage];
		for (int countBox = 0; countBox < pCurMenuPage->mpImageBoxes.size(); countBox++)
		{
			ImageBox* pImageBox = pCurMenuPage->mpImageBoxes[countBox];
			pImageBox->mImageObject.setUpTexture(screen.mpRenderer);
		}
	}
}

int main(int argc, char* args[]) 
{

	std::string basePath = SDL_GetBasePath();
	bool runGame		 = true;

	if (TTF_Init() < 0)
	{  
		SDL_assert(false); 		//Can't start without UI
		return -1;
	}

	createLevels(gameInstance);
	createMiniGameLevels(gameInstance);
	createMenus(gameInstance);
	setUpAllTextures();

	player.getMovementManager().setStartPosition(&gameInstance.mWorldData.mpWorlds[gameInstance.mWorldData.mCurWorldNumber]->mpLevels[gameInstance.mWorldData.mCurLevelNumber]->mPlayerStartingPosition);
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

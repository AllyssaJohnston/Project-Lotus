#pragma once
#include <SDL3/SDL.h>
#include "gameState.h"
#include "gameStateHelpers.h"
#include "helpers.h"
#include "miniGameWorldData.h"
#include "miniGameLevel.h"
#include "miniGameState.h"
#include "miniGameStateManager.h"
#include "worldData.h"
#include "collisionManager.h"
#include "settingsManager.h"
#include "slashManager.h"
#include "screen.h"
#include "menuManager.h"




class GameStateManager
{
public: 

    GameStateData               mGameStateData;
	std::vector <GameState*>    mStates;
	GameState*				    mpCurState;
    GameStateManagerData        mData;
    WorldData&                  mWorldData;

    MiniGameStateManager&       mMiniGameStateManager;

    GameStateManager(KeyboardData& keyboardData, WorldData& worldData, MenuManager& menuManager, 
            SettingsManager& settingsManager, CollisionManager& collisionManager, SlashManager& slashManager, 
            MiniGameStateManager& miniGameStateManager);
    ~GameStateManager();

    void preTick();

	void tick();

	void updateCurState(EGameState newStateEnum);

    void postTick();

private:
    void switchToMiniGame();
};






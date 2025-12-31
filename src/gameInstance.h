#pragma once
#include <vector>
#include "screen.h"
#include "collisionManager.h"
#include "styleManager.h"
#include "settingsManager.h"
#include "slashManager.h"
#include "menuDataController.h"
#include "menuManager.h"
#include "worldData.h"
#include "miniGameWorldData.h"
#include "gameStateManager.h"
#include "gameStateHelpers.h"
#include "miniGameStateManager.h"


struct GameInstance 
{

	ScreenObject					mScreen;

	// MAIN GAME
	CollisionManager				mCollisionManager   = CollisionManager();
	SlashManager					mSlashManager		= SlashManager();
	WorldData						mWorldData			= WorldData(mScreen, mSlashManager, mCollisionManager);
	

	// INPUT
	KeyboardData					mKeyboardData	= KeyboardData();

	// MINI GAME
	MiniGameWorldData				mMiniGameWorldData;
	MiniGameStateManager			mMiniGameStateManager = MiniGameStateManager(mKeyboardData, mMiniGameWorldData);

	// MANAGERS
	SettingsManager					mSettingsManager;
	FontSizeChart					mFontSizeChart;

	MenuManager						mMenuManager = MenuManager(mScreen, mSettingsManager, mFontSizeChart);
	MenuDataController				mMenuDataController = MenuDataController(mMenuManager, mWorldData, mMiniGameStateManager.mData, mMiniGameWorldData,
																			 mScreen, mSettingsManager, mFontSizeChart, mKeyboardData);

	GameStateManager				mGameStateManager = GameStateManager(mKeyboardData, mWorldData, mMenuManager, mMenuDataController, 
																		mSettingsManager, mCollisionManager, mSlashManager,
																		mMiniGameStateManager);
	
	GameInstance();
	~GameInstance() { ; }

	void preTick();
	void tick();
	void postTick();
};
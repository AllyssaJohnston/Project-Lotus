#pragma once
#include <vector>
#include "screenHelper.h"
#include "collisionHelper.h"
#include "styleHelper.h"
#include "settingsHelper.h"
#include "slashHelper.h"
#include "menuHelper.h"
#include "worldDataHelper.h"
#include "miniGameWorldDataHelper.h"
#include "gameStateHelper.h"
#include "gameStateHelperClass.h"


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

	GameStateManager				mGameStateManager = GameStateManager(mKeyboardData, mWorldData, mMenuManager, 
																		mSettingsManager, mCollisionManager, mSlashManager,
																		mMiniGameStateManager);
	MenuManager						mMenuManager	  = MenuManager(mScreen, mWorldData, mSettingsManager, mFontSizeChart, mMiniGameStateManager.mData, mMiniGameWorldData); 

	GameInstance();
	~GameInstance();

	void preTick();
	void tick();
	void postTick();
};
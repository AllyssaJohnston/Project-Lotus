#pragma once
#include <vector>
#include "screenHelper.h"
#include "damageHelper.h"
#include "collisionHelper.h"
#include "styleHelper.h"
#include "settingsHelper.h"
#include "slashHelper.h"
#include "menuHelper.h"
#include "worldDataHelper.h"
#include "miniGameWorldDataHelper.h"
#include "gameStateHelper.h"
#include "gameStateHelperClass.h"


class GameInstance 
{
public:

	ScreenObject					mScreen;

	//MAIN GAME
	WorldData						mWorldData		= WorldData(mScreen);
	DamageManager					mDamageManager;
	CollisionManager				mCollisionManager;
	
	SlashManager					mSlashManager   = SlashManager(mWorldData.mPlayer.getMovementManager());

	//INPUT
	KeyboardData					mKeyboardData	= KeyboardData();

	//MINI GAME
	MiniGameWorldData				mMiniGameWorldData;
	MiniGameStateManager			mMiniGameStateManager = MiniGameStateManager(mKeyboardData, mMiniGameWorldData);

	//MANAGERS
	StyleManager					mStyleManager;
	SettingsManager					mSettingsManager;
	FontSizeChart					mFontSizeChart;

	GameStateManager				mGameStateManager = GameStateManager(mKeyboardData, mWorldData, mMenuManager, 
																		mSettingsManager, mCollisionManager, mDamageManager, mSlashManager,
																		mStyleManager, mMiniGameStateManager);
	MenuManager						mMenuManager	  = MenuManager(mScreen, mWorldData, mSettingsManager, mFontSizeChart, mMiniGameStateManager.mData, mMiniGameWorldData); 

	//bool							mDebugMode	= true;

	GameInstance();
	
	~GameInstance();

	void preTick();

	void tick();

	void postTick();
};
#include "gameStateManager.h"

extern const std::map<const SDL_Keycode, const EKeyboardInput> SDLKToKeyboardMap;
extern const std::map<const EKeyboardInput, const std::string> keyboardToStringMap;
extern const int numEventsToGrab;

// GAME STATE MANAGER
GameStateManager::GameStateManager(KeyboardData& keyboardData, WorldData& worldData, MenuManager& menuManager, SettingsManager& settingsManager, 
	CollisionManager& collisionManager, SlashManager& slashManager, MiniGameStateManager& miniGameStateManager) : mMiniGameStateManager(miniGameStateManager), mWorldData(worldData)
{
	mGameStateData = GameStateData();
	mStates.push_back(new GameStatePlay(mGameStateData, keyboardData, worldData, menuManager, settingsManager, collisionManager, slashManager));
	mStates.push_back(new GameStatePlayMiniGame(mGameStateData, keyboardData, miniGameStateManager, menuManager, worldData.mScreen, settingsManager));
	mStates.push_back(new GameStateMenu(mGameStateData, keyboardData, menuManager, settingsManager, worldData));
	mData.mCurStateEnum			= EGameState_PLAY;
	mData.mLastFrameStateEnum	= EGameState_INVALID;
	mpCurState = mStates[mData.mCurStateEnum];
}

GameStateManager::~GameStateManager()
{
	for (GameState* iter : mStates)
	{
		delete iter;
	}
	mStates.clear();
	mpCurState		= nullptr;
}

void GameStateManager::preTick()
{
	mData.mLastFrameStateEnum = mData.mCurStateEnum;
	mpCurState->preTick();
}

void GameStateManager::tick() { mpCurState->tick(mData, mMiniGameStateManager); }

void GameStateManager::updateCurState(EGameState nextStateEnum)
{
	EGameState previousState = mData.mCurStateEnum;
	mData.mCurStateEnum = nextStateEnum;
	mpCurState = mStates[nextStateEnum];
	switch (nextStateEnum) 
	{
	case EGameState_PLAY:
		mpCurState->mMenuManager.setCurMenuPage(mpCurState->mMenuManager.mpMenuPages[int(EMenuPageType_MAIN_GAME_MENU)]);
		
		if (previousState == EGameState_PLAY_MINI_GAME)
		{
			MiniGameWorldData& miniWorldData = ((GameStatePlayMiniGame*)mStates[EGameState_PLAY_MINI_GAME])->mMiniGameStateManager.mWorldData;
			if (miniWorldData.mGoToNextLevel)
			{
				((GameStatePlay*)mpCurState)->mWorldData.setNextLevel(miniWorldData.mpNextLevelData->mWorldNumber, miniWorldData.mpNextLevelData->mLevelNumber);
				miniWorldData.mGoToNextLevel = false;
				miniWorldData.mpNextLevelData = nullptr;
			}
		}
		if (mGameStateData.mCleanNextState)
		{
			((GameStatePlay*)mpCurState)->mWorldData.resetStats();
			mGameStateData.mCleanNextState;
		}
		break;
	case EGameState_PLAY_MINI_GAME:
		
		
		if (previousState == EGameState_PLAY)
		{
			WorldData& worldData = ((GameStatePlay*)mStates[EGameState_PLAY])->mWorldData;
			if (mWorldData.mGoToNextLevel)
			{
				((GameStatePlayMiniGame*)mpCurState)->mMiniGameStateManager.mWorldData.setNextLevel(worldData.mpNextLevelData->mWorldNumber, worldData.mpNextLevelData->mLevelNumber, worldData.mpNextLevelData->mStageNumber);
				switchToMiniGame();
				((GameStatePlayMiniGame*)mpCurState)->mMiniGameStateManager.start();	
			}
			mWorldData.mGoToNextLevel = false;
			mWorldData.mpNextLevelData = nullptr;
		}
		else if (mGameStateData.mCleanNextState)
		{
			switchToMiniGame();
			((GameStatePlayMiniGame*)mpCurState)->mMiniGameStateManager.start();
			mGameStateData.mCleanNextState = false;
		}
		else
		{
			switchToMiniGame();
		}
		break;
	case EGameState_MENU:
		mpCurState->mMenuManager.setCurMenuPage(mpCurState->mMenuManager.mpMenuPages[int(EMenuPageType_MAIN_MENU)]);
		break;
	default:
		break;
	}
	mGameStateData.mCleanNextState = false;
}

void GameStateManager::postTick()
{
	mpCurState->postTick(mData, mMiniGameStateManager);
	if (mGameStateData.mNextGameState != EGameState_INVALID)
	{
		if (mGameStateData.mNextGameState != mData.mCurStateEnum) 
		{
			updateCurState(mGameStateData.mNextGameState);
		}
		
		mGameStateData.mNextGameState = EGameState_INVALID;
	}
	if (mMiniGameStateManager.mData.mCurStateEnum == EMiniGameState_BUILD_NEXT_LEVEL)
	{
		MiniGameWorldData& worldData = mMiniGameStateManager.mWorldData;
		worldData.goToNextLevel();
		switchToMiniGame();
		mMiniGameStateManager.updateCurState(EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT);
	}
}

void GameStateManager::switchToMiniGame()
{
	mpCurState->mMenuManager.setCurMenuPage(mpCurState->mMenuManager.mpMenuPages[int(EMenuPageType_MINI_GAME_MENU)]);
	createMiniGameCharacterStatsMenu(mpCurState->mMenuManager, mpCurState->mScreen, mMiniGameStateManager.mWorldData);
	createMiniGameCharacterAttackPanel(mpCurState->mMenuManager, mpCurState->mScreen, mMiniGameStateManager.mWorldData);
	createMiniGameCharacterSelectionMenu(mpCurState->mMenuManager, mpCurState->mScreen, mMiniGameStateManager.mWorldData);
	((GameStatePlayMiniGame*)mpCurState)->setUp();
	((GameStatePlayMiniGame*)mpCurState)->mMiniGameStateManager.start();
}





#pragma once
#include "gameStateHelperClass.h"
#include "hitboxHelper.h"
#include "helperClass.h"
#include "menuHelper.h"
#include "textBoxHelper.h"
#include "miniGameWorldDataHelper.h"
#include "miniGameLevelHelper.h"
#include "miniGameStateHelper.h"
#include "worldDataHelper.h"
#include "settingsHelper.h"
#include "slashHelper.h"
#include "styleHelper.h"
#include "screenHelper.h"
#include "movementHelperClass.h"


class Tile;
class CombatCharacter;
class Grid;
struct TileCoords;



class GameState
{
public:
    std::vector <KeyData> eventVect;
    KeyboardData&    mKeyboardData;
   
    MenuManager&     mMenuManager;
   
    GameStateData&   mGameStateData;
    
    SettingsManager& mSettingsManager;
    ScreenObject&    mScreen;

    int mTicksSinceInput = 0;
    int mTicksBeforeUseInput = 10;

    Vect2 mousePos;

	GameState(GameStateData& gameStateData, KeyboardData& keyboardData, MenuManager& menuManager,
        SettingsManager& settingsManager, ScreenObject& screen);
    virtual ~GameState() { ; }

    virtual void preTick();

    virtual void tick(GameStateManagerData& gameStateManagerData,  MiniGameStateManager& miniGameStateManager) {;}

	void getInput();

    virtual void useInput(GameStateManagerData& gameStateManagerData);

    void useMouseCursor();

    virtual void render(EGameState curState){;}

    virtual void postTick(GameStateManagerData& gameStateManagerData,  MiniGameStateManager& miniGameStateManager){;}

    virtual void takeMenuAction(MiniGameStateManager& miniGameStateManager);
};

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


class GameStatePlay : public GameState
{
public:
    WorldData&          mWorldData;
    CollisionManager&   mCollisionManager;
    SlashManager&       mSlashManager;

	GameStatePlay(GameStateData& gameStateData, KeyboardData& keyboardData, WorldData& worldData, 
            MenuManager& menuManager, SettingsManager& settingsManager, CollisionManager& collisionManager, 
            SlashManager& slashManager);

    ~GameStatePlay() { ; }

	void tick(GameStateManagerData& gameStateManagerData, MiniGameStateManager& miniGameStateManager) override;

	void useInput(GameStateManagerData& gameStateManagerData) override;

	void render(EGameState curState) override;

    void takeMenuAction(MiniGameStateManager& miniGameStateManager) override { ; }

};

class GameStatePlayMiniGame : public GameState
{
public:
    MiniGameStateManager& mMiniGameStateManager;

    GameStatePlayMiniGame(GameStateData& gameStateData, KeyboardData& keyboardData,
            MiniGameStateManager& miniGameStateManager, MenuManager& menuManager, ScreenObject& screen, 
            SettingsManager& settingsManager);

    ~GameStatePlayMiniGame() { ; }

    void setUp();

    void tick(GameStateManagerData& gameStateManagerData, MiniGameStateManager& miniGameStateManager) override;

    void render(EGameState curState) override;

private:
    int mTicks = 0;
};

class GameStateMenu : public GameState
{
public:
	GameStateMenu(GameStateData& gameStateData, KeyboardData& keyboardData, 
            MenuManager& menuManager, SettingsManager& settingsManager,
            WorldData& worldData);
    
    ~GameStateMenu() { ; }

	void tick(GameStateManagerData& gameStateManagerData, MiniGameStateManager& miniGameStateManager) override;

	void render(EGameState curState) override;

private:
    WorldData& mWorldData;
};



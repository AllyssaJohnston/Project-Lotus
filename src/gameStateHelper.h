#pragma once
#include "hitboxHelper.h"
#include "combatAttackHelper.h"
#include "combatAttackHelperClass.h"
#include "helperClass.h"
#include "menuHelper.h"
#include "textBoxHelper.h"
#include "settingsHelper.h"
#include "miniGameLevelHelper.h"
#include "gameStateHelperClass.h"
#include "worldDataHelper.h"
#include "slashHelper.h"
#include "miniGameWorldDataHelper.h"
#include "movementHelperClass.h"
#include "lotusAdventureMenus.h"
#include "miniGameStateHelper.h"

class Tile;
class CombatCharacter;
class Grid;
class TileCoords;





class GameState
{
public:
    std::vector <KeyData> eventVect;
    KeyboardData&    mKeyboardData;
   
    MenuManager&     mMenuManager;
   
    GameStateData&   mGameStateData;
    
    SettingsManager& mSettingsManager;
    StyleManager&    mStyleManager;
    ScreenObject&    mScreen;

    int mTicksSinceInput = 0;
    int mTicksBeforeUseInput = 10;

    Vect2 mousePos;

	GameState(GameStateData& gameStateData, KeyboardData& keyboardData, MenuManager& menuManager,
        SettingsManager& settingsManager, StyleManager& styleManager, ScreenObject& screen);
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
	GameState *				    mpCurState;
    GameStateManagerData        mData;
    WorldData& mWorldData;

    MiniGameStateManager&       mMiniGameStateManager;

    GameStateManager(KeyboardData& keyboardData, WorldData& worldData, MenuManager& menuManager, 
            SettingsManager& settingsManager, CollisionManager& collisionManager, DamageManager& damageManager, 
            SlashManager& slashManager, StyleManager& styleManager, MiniGameStateManager& miniGameStateManager);
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
    DamageManager&      mDamageManager;
    SlashManager&       mSlashManager;

	GameStatePlay(GameStateData& gameStateData, KeyboardData& keyboardData, WorldData& worldData, 
            MenuManager& menuManager, SettingsManager& settingsManager, CollisionManager& collisionManager, 
            DamageManager& damageManager, SlashManager& slashManager, StyleManager& styleManager);

    ~GameStatePlay() { ; }

	void tick(GameStateManagerData& gameStateManagerData, MiniGameStateManager& miniGameStateManager) override;

	void useInput(GameStateManagerData& gameStateManagerData) override;

	void render(EGameState curState) override;

    void takeMenuAction(MiniGameStateManager& miniGameStateManager) override { ; }

};

class GameStatePlayMiniGame : public GameState
{
    int mTicks = 0;
public:
    MiniGameStateManager& mMiniGameStateManager;

    GameStatePlayMiniGame(GameStateData& gameStateData, KeyboardData& keyboardData,
            MiniGameStateManager& miniGameStateManager, MenuManager& menuManager, ScreenObject& screen, 
            SettingsManager& settingsManager, StyleManager& styleManager);

    ~GameStatePlayMiniGame() { ; }

    void setUp();

    void tick(GameStateManagerData& gameStateManagerData, MiniGameStateManager& miniGameStateManager) override;

    void render(EGameState curState) override;
};

class GameStateMenu : public GameState
{
public:
	GameStateMenu(GameStateData& gameStateData, KeyboardData& keyboardData, 
            MenuManager& menuManager, SettingsManager& settingsManager, StyleManager& styleManager,
            WorldData& worldData);
    
    ~GameStateMenu() { ; }

	void tick(GameStateManagerData& gameStateManagerData, MiniGameStateManager& miniGameStateManager) override;

	void render(EGameState curState) override;

private:
    WorldData& mWorldData;
};



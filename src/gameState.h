#pragma once
#include "gameStateHelpers.h"
#include "menuManager.h"
#include "menuDataController.h"
#include "miniGameStateManager.h"

class GameState
{
public:
    std::vector <KeyData> eventVect;
    KeyboardData& mKeyboardData;

    MenuManager& mMenuManager;
    MenuDataController& mMenuDataController;

    GameStateData& mGameStateData;

    SettingsManager& mSettingsManager;
    ScreenObject& mScreen;

    int mTicksSinceInput = 0;
    int mTicksBeforeUseInput = 10;

    Vect2 mousePos;

    GameState(GameStateData& gameStateData, KeyboardData& keyboardData, MenuManager& menuManager, MenuDataController& menuController, SettingsManager& settingsManager, ScreenObject& screen);

    virtual ~GameState() { ; }

    virtual void preTick();

    virtual void tick(GameStateManagerData& gameStateManagerData, MiniGameStateManager& miniGameStateManager) { ; }

    void getInput();

    virtual void useInput(GameStateManagerData& gameStateManagerData);

    void useMouseCursor();

    virtual void render(EGameState curState) { ; }

    virtual void postTick(GameStateManagerData& gameStateManagerData, MiniGameStateManager& miniGameStateManager) { ; }

    virtual void takeMenuAction(MiniGameStateManager& miniGameStateManager);
};

class GameStatePlay : public GameState
{
public:
    WorldData& mWorldData;
    CollisionManager& mCollisionManager;
    SlashManager& mSlashManager;

    GameStatePlay(GameStateData& gameStateData, KeyboardData& keyboardData, WorldData& worldData,
        MenuManager& menuManager, MenuDataController& menuController, SettingsManager& settingsManager, CollisionManager& collisionManager,
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
        MiniGameStateManager& miniGameStateManager, MenuManager& menuManager, MenuDataController& menuController, ScreenObject& screen,
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
        MenuManager& menuManager, MenuDataController& menuController, SettingsManager& settingsManager,
        WorldData& worldData);

    ~GameStateMenu() { ; }

    void tick(GameStateManagerData& gameStateManagerData, MiniGameStateManager& miniGameStateManager) override;

    void render(EGameState curState) override;

};
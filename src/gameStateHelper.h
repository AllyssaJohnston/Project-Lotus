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

class Tile;
class CombatCharacter;
class Grid;
class TileCoords;


Tile* findTile(Grid* pGrid, TileCoords* pTileCoords);

bool isPlayableTile(Tile* pGivenTile);

bool validAttackTile(MiniGameStateManagerData& stateManagerData, Tile* pGivenTile, CombatCharacter* pGivenCharacter);

void attemptAttackMultipleTiles(MiniGameStateManagerData& stateManagerData, std::vector <Tile*> tilesToAttack, CombatCharacter* pGivenCharacter);

std::vector <Tile*> returnListWithoutTilesWithCharacters(CombatManager* pCombatManager, CombatCharacter* pGivenCharacter, std::vector <Tile*> listOfTiles);

std::vector <TileDistance> returnListOfTileDistances(std::vector <CombatCharacter*> pCurCombatCharacters, std::vector <Tile*> pMoveTiles, CombatCharacter* pCurEnemy);


class MiniGameState
{
public:
    MouseData*          mpMouseData = nullptr;
    MiniGameStateData*  mpData      = nullptr; 

    MiniGameState(MouseData* pMouseData, MiniGameStateData* pData);

    ~MiniGameState();

    virtual void tick(MiniGameStateManagerData& stateManagerData) { ; }

    virtual void setCharacter(CombatCharacter* pCharacter);

    void highlightTile(Grid* pGrid, Vect2 pos);

    void useMouseInput(EMiniGameState curStateEnum, ScreenObject& screenObject, Grid* pGrid);

    virtual void selectTile(Grid* pGrid, Vect2 pos) { ; }
};

class MiniGameStateManager
{
public:
    std::vector <MiniGameState *> mpStates;
    MiniGameState *				  mpCurState;
    MiniGameStateManagerData      mData;

    MiniGameStateManager(MouseData* pMouseData, MiniGameWorldData& miniGameWorldData);

    ~MiniGameStateManager();

    void preTick();

    void tick();
   
    void postTick();

    void updateCurState(EMiniGameState newStateEnum);

};

class GameState
{
public:
    KeyboardData*    mpKeyboardData     = nullptr;
    MouseData*       mpMouseData        = nullptr;
   
    MenuManager*     mpMenuManager      = nullptr;
   
    GameStateData*   mpGameStateData    = nullptr;
    
    SettingsManager* mpSettingsManager  = nullptr;
    StyleManager*    mpStyleManager     = nullptr;
    ScreenObject*    mpScreen           = nullptr;

	GameState();
    virtual ~GameState();

    virtual void preTick(GameStateManagerData& gameStateManagerData,  MiniGameStateManager& miniGameStateManager,
        WorldData& worldData){;}

    virtual void tick(GameStateManagerData& gameStateManagerData,  MiniGameStateManager& miniGameStateManager,
        WorldData& worldData) {;}

	void getInput();

    void useInput(GameStateManagerData& gameStateManagerData);

    virtual void useMouseCursor(){;}

    virtual void render(GameStateManagerData& gameStateManagerData, MiniGameStateManagerData& miniGameStateManagerData,
         WorldData& worldData){;}

    virtual void postTick(GameStateManagerData& gameStateManagerData,  MiniGameStateManager& miniGameStateManager,
        WorldData& worldData){;}

};

class GameStateManager
{
public: 

    GameStateData*              mpGameStateData;
	std::vector <GameState*>    mStates;
	GameState *				    mpCurState;
    GameStateManagerData        mData;

    MiniGameStateManager&       mMiniGameStateManager;
    WorldData&                  mWorldData;

    GameStateManager(KeyboardData& keyboardData, MouseData& mouseData, WorldData& worldData, MenuManager& menuManager, 
            SettingsManager& settingsManager, CollisionManager& collisionManager, DamageManager& damageManager, 
            SlashManager& slashManager, StyleManager& styleManager, MiniGameStateManager& miniGameStateManager);
    ~GameStateManager();

    void preTick();

	void tick();

	void updateCurState(EGameState newStateEnum);

    void postTick();
};



class MiniGamePlayerWaitForMoveInput : public MiniGameState
{
public:
    MiniGamePlayerWaitForMoveInput(MouseData* pMouseData, MiniGameStateData* pData);

    ~MiniGamePlayerWaitForMoveInput();

    void selectTile(Grid* pGrid, Vect2 pos);

    void moveToTile(Tile* pGivenTile, CombatCharacter* pGivenCharacter);

    void postTick(Tile* pNewTile);
};

class MiniGamePlayerMoveCharacter : public MiniGameState
{
public:
    MiniGamePlayerMoveCharacter(MouseData* pMouseData, MiniGameStateData* pData);

    ~MiniGamePlayerMoveCharacter();

    void tick(MiniGameStateManagerData& stateManagerData);

    void postTick();
};

class MiniGamePlayerWaitForActionInput : public MiniGameState
{
public:
    MiniGamePlayerWaitForActionInput(MouseData* pMouseData, MiniGameStateData* pData);

    ~MiniGamePlayerWaitForActionInput();

    void postTick(MiniGameWorldData& worldData, EMiniGameState nextStateEnum);
};

class MiniGamePlayerWaitForAttackInput : public MiniGameState
{
public:
    MiniGamePlayerWaitForAttackInput(MouseData* pMouseData, MiniGameStateData* pData);

    ~MiniGamePlayerWaitForAttackInput();

    void postTick(const Attack& pAttack);
};

class MiniGamePlayerWaitForAttackSubInput : public MiniGameState
{
public:
    MiniGamePlayerWaitForAttackSubInput(MouseData* pMouseData, MiniGameStateData* pData);

    ~MiniGamePlayerWaitForAttackSubInput();

    void postTick(EDirection curAttackDirection);
};

class MiniGamePlayerCompleteDirectionalAttack : public MiniGameState
{
public:
    MiniGamePlayerCompleteDirectionalAttack(MouseData* pMouseData, MiniGameStateData* pData);

    ~MiniGamePlayerCompleteDirectionalAttack();

    void tick(MiniGameStateManagerData& stateManagerData);

    void attackTiles(MiniGameStateManagerData& stateManagerData);

    //attackCharacterChanges
    void postTick(MiniGameWorldData& worldData);
};

class MiniGamePlayerTakeActionAttack : public MiniGameState
{
public:
    MiniGamePlayerTakeActionAttack(MouseData* pMouseData, MiniGameStateData* pData);

    ~MiniGamePlayerTakeActionAttack();

    void selectTile(MiniGameStateManagerData& stateManagerData, Grid* pGrid, Vect2 pos);

    void postTick(MiniGameWorldData& worldData);
};

class MiniGamePlayerTakeActionDefend : public MiniGameState
{
public:
    MiniGamePlayerTakeActionDefend(MouseData* pMouseData, MiniGameStateData* pData);

    ~MiniGamePlayerTakeActionDefend();

    void tick(MiniGameStateManagerData& stateManagerData);

    //defense change
    void postTick(MiniGameWorldData& worldData);
};

class MiniGameEnemyMoveCharacter : public MiniGameState
{
public:
    MiniGameEnemyMoveCharacter(MouseData* pMouseData, MiniGameStateData* pData);

    ~MiniGameEnemyMoveCharacter();

    void tick(MiniGameStateManagerData& stateManagerData);

    void decideTileToMoveTo(MiniGameStateManagerData& stateManagerData);

    void postTick();
};

class MiniGameEnemyTakeAction : public MiniGameState
{
public:
    MiniGameEnemyTakeAction(MouseData* pMouseData, MiniGameStateData* pData);

    ~MiniGameEnemyTakeAction();

    void tick(MiniGameStateManagerData& stateManagerData);

    bool shouldAttack(MiniGameWorldData& worldData);

    bool shouldDefend(MiniGameWorldData& worldData);

    void performAttack(MiniGameStateManagerData& stateManagerData);

    void postTick(MiniGameWorldData& worldData);
};

class MiniGameBuffer : public MiniGameState
{
public:
    MiniGameBuffer(MouseData* pMouseData, MiniGameStateData* pData);
    
    ~MiniGameBuffer();

    void tick(MiniGameStateManagerData& stateManagerData);

    void postTick();
};




class GameStatePlay : public GameState
{
public:
    WorldData&          mWorldData;
    CollisionManager&   mCollisionManager;
    DamageManager&      mDamageManager;
    SlashManager&       mSlashManager;

	GameStatePlay(GameStateData& gameStateData, KeyboardData& keyboardData, MouseData& mouseData, WorldData& worldData, 
            MenuManager& menuManager, SettingsManager& settingsManager, CollisionManager& collisionManager, 
            DamageManager& damageManager, SlashManager& slashManager, StyleManager& styleManager);

    ~GameStatePlay();

	void tick(GameStateManagerData& gameStateManagerData, MiniGameStateManager& miniGameStateManager,
        WorldData& worldData);

	void useInput(GameStateManagerData& gameStateManagerData);

    void useMouseCursor();

	void render(GameStateManagerData& gameStateManagerData, MiniGameStateManagerData& miniGameStateManagerData,
        WorldData& worldData);

};

class GameStatePlayMiniGame : public GameState
{
public:
    MiniGameStateManager& mMiniGameStateManager;

    GameStatePlayMiniGame(GameStateData& gameStateData, KeyboardData& keyboardData, MouseData& mouseData, 
            MiniGameStateManager& miniGameStateManager, MenuManager& menuManager, ScreenObject& screen, 
            SettingsManager& settingsManager, StyleManager& styleManager);

    ~GameStatePlayMiniGame();

    void tick(GameStateManagerData& gameStateManagerData, MiniGameStateManager& miniGameStateManager,
        WorldData& worldData);

    void render(GameStateManagerData& gameStateManagerData, MiniGameStateManagerData& miniGameStateManagerData,
        WorldData& worldData);

    void useMouseCursor();
};

class GameStateMenu : public GameState
{
public:
    int mTicksSinceInput = 0;
    int mTicksBeforeUseInput = 10;

	GameStateMenu(GameStateData& gameStateData, KeyboardData& keyboardData, MouseData& mouseData, 
            MenuManager& menuManager, ScreenObject& screen, SettingsManager& settingsManager, StyleManager& styleManager);
    
    ~GameStateMenu();

	void tick(GameStateManagerData& gameStateManagerData, MiniGameStateManager& miniGameStateManager,
        WorldData& worldData);

	void useInput(GameStateManagerData& gameStateManagerData);

    void useMouseCursor();

	void takeMenuAction(MiniGameStateManager& miniGameStateManager);

	void render(GameStateManagerData& gameStateManagerData, MiniGameStateManagerData& miniGameStateManagerData,
        WorldData& worldData);
};



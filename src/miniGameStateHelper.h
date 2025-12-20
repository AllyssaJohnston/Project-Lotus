#pragma once
#include "hitboxHelper.h"
#include "combatAttackHelper.h"
#include "combatAttackHelperClass.h"
#include "tileCoordsHelper.h"
#include "tileCoordsHelperClass.h"
#include "helperClass.h"
#include "menuHelper.h"
#include "settingsHelper.h"
#include "miniGameLevelHelper.h"
#include "miniGameStateHelperClass.h"
#include "miniGameWorldDataHelper.h"
#include "lotusAdventureMenus.h"

class Tile;
class CombatCharacter;
class Grid;
struct TileCoords;


struct MiniGameState
{
    MiniGameStateData&  mData; 
    MiniGameWorldData&  mWorldData;
    KeyboardData&       mKeyboardData;

    MiniGameState(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData);
    ~MiniGameState() { ; }

    virtual void tick() { ; }

    void highlightTile(const Vect2 pos);

    void useMouseInput(EMiniGameState curStateEnum, ScreenObject& screenObject);

    virtual void selectTile(const Vect2 pos) { ; }
};

struct MiniGamePlayerWaitForMoveInput : public MiniGameState
{
    MiniGamePlayerWaitForMoveInput(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData);

    void selectTile(const Vect2 pos) override;
    void moveToTile(Tile& givenTile);

    void postTick(Tile& tileToMoveTo);
};


struct MiniGamePlayerMoveCharacter : public MiniGameState
{
    MiniGamePlayerMoveCharacter(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData);

    void tick() override;
    void postTick();
};

struct MiniGamePlayerWaitForActionInput : public MiniGameState
{
    MiniGamePlayerWaitForActionInput(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData);

    void postTick(EMiniGameState nextStateEnum);
};

struct MiniGamePlayerWaitForAttackInput : public MiniGameState
{
    MiniGamePlayerWaitForAttackInput(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData);

    void postTick(Attack attack);
};

struct MiniGamePlayerWaitForAttackSubInput : public MiniGameState
{
    MiniGamePlayerWaitForAttackSubInput(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData);

    void postTick(EDirection curAttackDirection);
};

struct MiniGamePlayerCompleteDirectionalAttack : public MiniGameState
{
    MiniGamePlayerCompleteDirectionalAttack(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData);

    void tick() override;

    void attackTiles();

    void postTick();
};

struct MiniGamePlayerTakeActionAttack : public MiniGameState
{
    MiniGamePlayerTakeActionAttack(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData);

    void selectTile(const Vect2 pos) override;

    void postTick();
};

struct MiniGamePlayerTakeActionDefend : public MiniGameState
{
    MiniGamePlayerTakeActionDefend(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData);

    void tick() override;

    void postTick();
};

struct MiniGameEnemyMoveCharacter : public MiniGameState
{
    MiniGameEnemyMoveCharacter(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData);

    void tick() override;

    void decideTileToMoveTo();

    void postTick();
};

struct MiniGameEnemyTakeAction : public MiniGameState
{
    MiniGameEnemyTakeAction(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData);

    void tick() override;

    bool shouldAttack();
    bool shouldDefend();

    void performAttack();

    void postTick();
};

struct MiniGameCharacterStunned : public MiniGameState
{
    MiniGameCharacterStunned(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData);

    void tick() override;
    void postTick();
};

struct MiniGameBuffer : public MiniGameState
{
    MiniGameBuffer(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData);

    void tick() override;
    void postTick();
};

struct MiniGameBuildNextLevel : public MiniGameState { MiniGameBuildNextLevel(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData); };

struct MiniGameExit : public MiniGameState { MiniGameExit(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData); };



class MiniGameStateManager
{
public:
    std::vector <MiniGameState *> mpStates;
    MiniGameState* mpCurState               = nullptr;
    MiniGameStateManagerData mData          = MiniGameStateManagerData();
    MiniGameWorldData& mWorldData;
    bool setUp                              = false;

    MiniGameStateManager(KeyboardData& keyboardData, MiniGameWorldData& miniGameWorldData);

    ~MiniGameStateManager();

    void start();

    void preTick();
    virtual void tick();
    virtual void postTick();

    void printBoard(ScreenObject& screenObject, const StyleManager& styleManager);

    void printCharacters(ScreenObject& screenObject, const StyleManager& styleManager);

    void updateTileColors(const StyleManager& styleManager);

    void updateCurState(EMiniGameState state);

    void createDebugLog();
};


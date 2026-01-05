#pragma once
#include "hitbox.h"
#include "combatAttack.h"
#include "tileCoords.h"
#include "helpers.h"
#include "miniGameLevel.h"
#include "miniGameStateHelpers.h"
#include "miniGameWorldData.h"
#include "screen.h"


struct MiniGameState
{
    MiniGameStateData&  mData; 
    MiniGameWorldData&  mWorldData;
    KeyboardData&       mKeyboardData;

    MiniGameState(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData);
    ~MiniGameState() { ; }

    virtual void tick() { ; }

    void highlightTile(const Vect2 pos);

    void useInput(EMiniGameState curStateEnum, ScreenObject& screenObject);

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

struct MiniGamePlayerWaitForAttackOptionInput : public MiniGameState
{
    MiniGamePlayerWaitForAttackOptionInput(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData);

    void postTick(Attack& attack);
};

struct MiniGamePlayerWaitForAttackDirectionInput : public MiniGameState
{
    MiniGamePlayerWaitForAttackDirectionInput(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData);

    void postTick(EDirection curAttackDirection);
};

struct MiniGamePlayerWaitForAttackTileInput : public MiniGameState
{
    MiniGamePlayerWaitForAttackTileInput(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData);

    void selectTile(const Vect2 pos) override;

    void postTick();
};

struct MiniGamePlayerWaitForAttackCharacterInput : public MiniGameState
{
    MiniGamePlayerWaitForAttackCharacterInput(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData);

    void postTick(CombatCharacter* pTarget);
};

struct MiniGamePlayerCompleteActionAttack : public MiniGameState
{
    MiniGamePlayerCompleteActionAttack(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData);
    
    void tick() override;

    void attackTiles();

    void postTick();
};

struct MiniGamePlayerCompleteActionDefend : public MiniGameState
{
    MiniGamePlayerCompleteActionDefend(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData);

    void tick() override;

    void postTick();
};

struct MiniGamePlayerCompleteActionHeal : public MiniGameState
{
    MiniGamePlayerCompleteActionHeal(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData);

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
    bool shouldHeal();

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

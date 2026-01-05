#pragma once
#include <vector>
#include <stack>
#include "algorithm"
#include "miniGameState.h"
#include "combatManager.h"
#include "helpers.h"
#include "miniGameLevel.h"
#include "miniGameStateHelpers.h"
#include "miniGameWorldData.h"
#include "lotusAdventureMenus.h"


class MiniGameStateManager
{
public:
    std::vector <MiniGameState*> mpStates;
    MiniGameState* mpCurState = nullptr;
    MiniGameStateManagerData mData = MiniGameStateManagerData();
    MiniGameWorldData& mWorldData;
    bool setUp = false;

    MiniGameStateManager(KeyboardData& keyboardData, MiniGameWorldData& miniGameWorldData);

    ~MiniGameStateManager();

    void start();

    void preTick();
    virtual void tick();
    virtual void postTick();

    void undo();

    void printBoard(ScreenObject& screenObject);

    void printCharacters(ScreenObject& screenObject);

    void updateTileColors();

    void colorTile(Tile& tile, const ECombatActionType tileType);

    void updateCurState(EMiniGameState state);

    void createDebugLog();
};


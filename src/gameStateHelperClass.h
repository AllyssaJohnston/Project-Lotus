#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include "tileHelper.h"
#include "tileCoordsHelper.h"
#include "combatCharacterHelper.h"
#include "miniGameLevelHelper.h"
#include "screenHelper.h"
#include "combatAttackHelperClass.h"
#include "helperClass.h"
#include "worldDataHelper.h"
#include "miniGameWorldDataHelper.h"

class TileDistance
{
public:
    Tile* mpTile;
    CombatCharacter* mpCharacter1;
    CombatCharacter* mpCharacter2;
    float            mDistance;

    TileDistance(Tile* pTile, CombatCharacter* pCharacter1, CombatCharacter* pCharacter2, float distance)
    {
        mpTile       = pTile;
        mpCharacter1 = pCharacter1;
        mpCharacter2 = pCharacter2;
        mDistance    = distance;
    }

    ~TileDistance()
    {
        mpTile = nullptr;
        mpCharacter1 = nullptr;
        mpCharacter2 = nullptr;
    }
};

struct MiniGameStateData
{
    CombatCharacter *   mpCharacter             = nullptr;
    Tile *              mpTileToMoveTo          = nullptr;
    Tile *              mpTileToAttack          = nullptr;
    std::vector <Tile*> mpTilesToAttack;
    Attack              mCurAttack              = Attack();
    EDirection          mCurAttackDirection     = EDirection_INVALID;
    bool                mGoingToAttack          = false;
    EMiniGameState      mNextMiniGameState      = EMiniGameState_INVALID;
    EMiniGameState      mPostBufferGameState    = EMiniGameState_INVALID;

    int                 mTicks                  = 0;
    int                 mTicksBeforeAction      = 40;

    bool                mTickYet                = false;

    ~MiniGameStateData();
};

struct MiniGameStateManagerData
{
    EMiniGameState                mLastFrameStateEnum = EMiniGameState_INVALID;
    EMiniGameState		          mCurStateEnum       = EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT;
    MiniGameStateData *           mpData = new MiniGameStateData();;
    MiniGameWorldData *           mpMiniGameWorldData;

    ~MiniGameStateManagerData();

    void printBoard(ScreenObject& screenObject, StyleManager& styleManager);

    void printCharacters(ScreenObject& screenObject, StyleManager& styleManager);

    void updateTileColors(StyleManager& styleManager);
    
};

struct GameStateManagerData
{
    EGameState				    mCurStateEnum;
    EGameState				    mLastFrameStateEnum;
    bool                        mRunGame = true;
};

struct GameStateData
{
    EGameState mNextGameState = EGameState_INVALID;
};


std::vector <TileCoords> removeDuplicateTiles(std::vector <TileCoords> givenList);

bool inTileCoordList(std::vector <TileCoords> givenList, TileCoords* curTileCoords);

SDL_Color blendColors(SDL_Color* color1, SDL_Color* color2, float blendPercent);

bool checkIfTileInCharacterMoveRange(Tile* pGivenTile, CombatCharacter* pGivenCharacter);

//void renderCircleGradient(GameInstance& gameInstance, SDL_Color color, Vect2 center, int radius);

std::vector <AttackTile> returnAttackTileCoordsWithPlayersOnThem(MiniGameWorldData& worldData, Tile* pReferenceTile, CombatCharacter* pCharacter);
#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <stack>
#include "tileHelper.h"
#include "tileCoordsHelper.h"
#include "combatCharacterHelper.h"
#include "combatAttackHelperClass.h"
#include "miniGameLevelHelper.h"
#include "miniGameWorldDataHelper.h"

struct CombatCharacterSnapShot
{
    CombatCharacter* mpCharacter = nullptr;
    bool mAmAlive;
    int mCurAttackDamage = -1;
    int mCurHealthCapacity = -1;
    int mCurHealth = -1;

    int mCurDefenseCapacity = -1;
    int mCurDefense = 0;

    int mTurnsToPass = 0;

    Tile* mpTile = nullptr;

    CombatCharacterSnapShot(CombatCharacter* pCharacter);

    ~CombatCharacterSnapShot();
};

struct TileDistance
{
    Tile* mpTile;
    CombatCharacter* mpCharacter1;
    CombatCharacter* mpCharacter2;
    float            mDistance;

    TileDistance(Tile* pTile, CombatCharacter* pCharacter1, CombatCharacter* pCharacter2, float distance)
    {
        mpTile = pTile;
        mpCharacter1 = pCharacter1;
        mpCharacter2 = pCharacter2;
        mDistance = distance;
    }

    ~TileDistance()
    {
        mpTile = nullptr;
        mpCharacter1 = nullptr;
        mpCharacter2 = nullptr;
    }
};

class MiniGameStateData
{
private:
    CombatCharacter* mpCharacter = nullptr;
    int              mCharacterIndex = -1;
public:
    Tile* mpTileToMoveTo = nullptr;
    Tile* mpTileLastMovedTo = nullptr;

    bool mGoingToAttack = false;
    Tile* mpTileToAttack = nullptr;
    std::vector <Tile*> mpTilesToAttack;
    Attack* mpCurAttack = nullptr;
    EDirection          mCurAttackDirection = EDirection_INVALID;

    bool                mAttacked = false;
    bool                mDefended = false;
    std::string         mDebugLine = " ";

    EMiniGameState      mNextMiniGameState = EMiniGameState_INVALID;
    EMiniGameState      mPostBufferGameState = EMiniGameState_INVALID;

    int                 mTicks = 0;
    int                 mTicksBeforeAction = 40;
    bool                mTickYet = false;

    bool                mForcePrintLog = false;

    ~MiniGameStateData();

    CombatCharacter* getCharacter() const;

    int getCharacterIndex() const;

    void setCharacter(CombatCharacter* pCharacter, int index);

    void reset();
};

struct MiniGameStateManagerData
{
    EMiniGameState      mLastFrameStateEnum = EMiniGameState_INVALID;
    EMiniGameState	    mCurStateEnum = EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT;
    MiniGameStateData   mStateData = MiniGameStateData();
    std::stack<MiniGameStateData> mPreviousStateDatas;
    std::stack<std::vector<CombatCharacterSnapShot>> mPreTickCharacterSnapShots;
};




std::vector <AttackTile> returnAttackTileCoordsWithPlayersOnThem(MiniGameWorldData& worldData, Tile* pReferenceTile, CombatCharacter* pCharacter);

bool tileInAttackRange(const Attack& attack, EDirection attackDirection, Grid& grid, Tile* pGivenTile, CombatCharacter* pGivenCharacter);

bool attackMultipleTiles(const Attack& attack, EDirection attackDirection, MiniGameWorldData& worldData, std::vector <Tile*>& pTilesToAttack, CombatCharacter* pGivenCharacter);


bool characterOnTile(const Tile& tile, const std::vector <CombatCharacter*>& pCharacters);

std::vector <Tile*> returnListWithoutTilesWithCharacters(const CombatManager& pCombatManager, const std::vector <Tile*>& pListOfTiles);

std::vector <TileDistance> returnListOfTileDistances(std::vector <CombatCharacter*>& pCurCombatCharacters, std::vector <Tile*>& pMoveTiles, CombatCharacter* pCurEnemy);

std::vector<CombatCharacterSnapShot> createCombatCharacterSnapShots(const CombatManager& combatManager);

std::string getCharacterChangesString(const CombatManager& combatManager, const std::vector<CombatCharacterSnapShot>& preTickCharacters);

EMiniGameState getPostBufferState(const CombatCharacter& character);
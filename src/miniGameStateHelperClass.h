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
   
    int mBaseDamage = -1;
    int mCurDamage = -1;
    std::vector<std::pair<float, int>> mAttackDamageModifiers;  // multiplier amount, num turns

    int mCurHealth = -1;
    int mBaseHealthCapacity = -1;
    int mCurHealthCapacity = -1;
    std::vector<std::pair<float, int>> mHealthCapacityModifiers; // multiplier amount, num turns

    int mCurDefense = 0;
    int mBaseDefenseCapacity = -1;
    int mCurDefenseCapacity = -1;
    std::vector<std::pair<float, int>> mDefenseCapacityModifiers; // multiplier amount, num turns

    int mTurnsToPass = 0;

    Tile* mpTile;

    CombatCharacterSnapShot(CombatCharacter* pCharacter);

    ~CombatCharacterSnapShot();
};

struct TileDistance
{
    Tile&  mTile1;
    Tile&  mTile2;
    float  mDistance;

    TileDistance(Tile& tile1, Tile& tile2);
};

class MiniGameStateData
{
private:
    CombatCharacter* mpCharacter = nullptr;
    int              mCharacterIndex = -1;
public:
    Tile* mpTileToMoveTo    = nullptr;
    Tile* mpTileLastMovedTo = nullptr;

    bool                                mGoingToAttack      = false;
    Tile*                               mpTileToAttack      = nullptr;
    std::vector <Tile*>                 mpTilesToAttack;
    Attack*                             mpCurAttack         = nullptr;
    EMiniGameCombatAttackCategoryType   mAttackCategory     = EMiniGameCombatAttackCategoryType_INVALID;
    EDirection                          mCurAttackDirection = EDirection_INVALID;

    EMiniGameCombatCharacterType mTargetCharacterType = EMiniGameCombatCharacterType_INVALID;
    CombatCharacter* mpTargetCharacter = nullptr;

    bool                mAttacked   = false;
    bool                mDefended   = false;
    bool                mHealed     = false;
    std::string         mDebugLine  = " ";

    EMiniGameState      mNextMiniGameState      = EMiniGameState_INVALID;
    EMiniGameState      mPostBufferGameState    = EMiniGameState_INVALID;

    int                 mTicks = 0;
    int                 mTicksBeforeAction = 50;
    bool                mTickYet = false;

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


std::vector <Tile*> returnTilesFromAttackWithPlayersOnThem(const MiniGameWorldData& worldData, const Tile* const pReferenceTile, const Attack& curAttack, const EDirection direction);

std::vector <Tile*> returnTilesFromAttacksWithPlayersOnThem(const MiniGameWorldData& worldData, const Tile* const pReferenceTile, const std::vector<Attack>& attacks, const EDirection direction);


bool tileInAttackRange(const Attack& attack, const EDirection attackDirection, const Grid& grid, const Tile* const pGivenTile, const Tile* const pTileToAttackFrom);

std::vector <Tile*> returnTilesWithoutCharacters(const CombatManager& pCombatManager, const std::vector <Tile*>& pListOfTiles);

std::vector <TileDistance> returnListOfTileDistancesFromPlayers(const std::vector <CombatCharacter*>& pCurCombatCharacters, const std::vector <Tile*>& pMoveTiles, const CombatCharacter* const pCurEnemy);


std::vector<CombatCharacterSnapShot> createCombatCharacterSnapShots(const CombatManager& combatManager);

std::string getCharacterChangesString(const CombatManager& combatManager, const std::vector<CombatCharacterSnapShot>& preTickCharacters);


void setUpForBufferState(const MiniGameWorldData& worldData, MiniGameStateData& data);

EMiniGameState getPostBufferState(const CombatCharacter& character);
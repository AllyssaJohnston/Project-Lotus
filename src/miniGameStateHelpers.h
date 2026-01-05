#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <stack>

#include "tile.h"
#include "tileCoords.h"
#include "combatCharacter.h"
#include "miniGameWorldData.h"

class MiniGameStateData
{
private:
    CombatCharacter* mpCharacter = nullptr;
    int              mCharacterIndex = -1;
public:
    Tile* mpTileToMoveTo    = nullptr;
    Tile* mpTileLastMovedTo = nullptr;

    bool                                mGoingToAttack      = false;
    std::vector <Tile*>                 mpTilesToAttack;
    Attack*                             mpCurAttack         = nullptr;
    ECombatAttackCategoryType           mAttackCategory     = ECombatAttackCategoryType_INVALID;
    EDirection                          mCurAttackDirection = EDirection_INVALID;

    ECombatCharacterType mTargetCharacterType = ECombatCharacterType_INVALID;
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
    bool                mNeedToReset = false;

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
    std::stack<std::pair<EMiniGameState, MiniGameStateData>> mPreviousStateDatas;
    std::stack<std::vector<CombatCharacter>> mPreTickCharacters;
    std::stack<int>                          mRoundNum;
    int mTicksSinceUndo = 0;
};


std::vector <Tile*> returnTilesFromAttackWithPlayersOnThem(const MiniGameWorldData& worldData, const Tile* const pReferenceTile, const Attack& curAttack, const EDirection direction);

std::vector <Tile*> returnTilesFromAttacksWithPlayersOnThem(const MiniGameWorldData& worldData, const Tile* const pReferenceTile, const std::vector<Attack>& attacks, const EDirection direction);

Tile* returnTileFromAttackWithLowestHealthPlayer(const MiniGameWorldData& worldData, const Tile* const pReferenceTile, const Attack& curAttack, const EDirection direction);


bool tileInAttackRange(const Attack& attack, const EDirection attackDirection, const Grid& grid, const Tile* const pGivenTile, const Tile* const pTileToAttackFrom);

std::vector <Tile*> returnTilesWithoutCharacters(const CombatManager& pCombatManager, const std::vector <Tile*>& pListOfTiles);



std::string getCharacterChangesString(const CombatManager& combatManager, const std::vector<CombatCharacter>& preTickCharacters);


void setUpForBufferState(const MiniGameWorldData& worldData, MiniGameStateData& data);

EMiniGameState getPostBufferState(const CombatCharacter& character);
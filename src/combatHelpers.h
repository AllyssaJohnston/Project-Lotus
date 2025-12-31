#pragma once
#include <string>
#include <SDL3/SDL.h>

std::string createTileName(const int row, const int col);


enum ECombatActionGridPattern
{
    ECombatActionGridPattern_INVALID = -1,
    ECombatActionGridPattern_NONE,
    ECombatActionGridPattern_SQUARE,
    ECombatActionGridPattern_CROSS,
    ECombatActionGridPattern_CHECKERBOARD,
    ECombatActionGridPattern_WHOLE_GRID,
    ECombatActionGridPattern_MAX
};

enum ECombatNumTilesToAttack
{
    ECombatNumTilesToAttack_INVALID = -1,
    ECombatNumTilesToAttack_ONE,
    ECombatNumTilesToAttack_DIRECTION,
    ECombatNumTilesToAttack_ALL,
    ECombatNumTilesToAttack_MAX
};

enum ECombatSpecialEffectTypes
{
    ECombatSpecialEffectTypes_INVALID = -1,
    ECombatSpecialEffectTypes_NONE,
    ECombatSpecialEffectTypes_STUN,
    ECombatSpecialEffectTypes_LOSE_TURN,
    ECombatSpecialEffectTypes_ATTACK_MULTIPLIER,
    ECombatSpecialEffectTypes_DEFENSE_CAPACITY_MULTIPLIER,
    ECombatSpecialEffectTypes_HEALTH_CAPACITY_MULTIPLIER,
    ECombatSpecialEffectTypes_HEAL,
    ECombatSpecialEffectTypes_FULL_HEAL,
    ECombatSpecialEffectTypes_POISON,
    ECombatSpecialEffectTypes_REVIVE,
    ECombatSpecialEffectTypes_MAX
};

enum ECombatActionType
{
    ECombatActionType_INVALID = -1,
    ECombatActionType_NONE,
    ECombatActionType_MOVE,
    ECombatActionType_ATTACK,
    ECombatActionType_DEFEND,
    ECombatActionType_PASS,
    ECombatActionType_MAX
};

// under the hood, attack and support, are implemented as attacks,
// however I separate these in the menu into categories that will make more sense
// to the player
enum ECombatAttackCategoryType
{
    ECombatAttackCategoryType_INVALID = -1,
    ECombatAttackCategoryType_ATTACK,
    ECombatAttackCategoryType_SUPPORT,
    ECombatAttackCategoryType_MAX
};

enum ECombatCharacterType
{
    ECombatCharacterType_INVALID = -1,
    ECombatCharacterType_CHARACTER,
    ECombatCharacterType_PLAYER,
    ECombatCharacterType_ENEMY,
    ECombatCharacterType_MAX
};

enum ECombatTileType
{
    ECombatTileType_INVALID = -1,
    ECombatTileType_NORMAL,
    ECombatTileType_IMPASSABLE,
    ECombatTileType_TELEPORTER,
    ECombatTileType_MAX
};
   
enum ECombatTileMode
{
    ECombatTileMode_INVALID = -1,
    ECombatTileMode_NOT_SELECTED,
    ECombatTileMode_SELECTED,
    ECombatTileMode_HIGHLIGHTED,
    ECombatTileMode_MAX
};

enum ECombatAttackTargetAlive
{
    ECombatAttackTargetAlive_INVALID = -1,
    ECombatAttackTargetAlive_ALIVE,
    ECombatAttackTargetAlive_DEAD,
    ECombatAttackTargetAlive_MAX
};

struct GameOverStats
{
    bool mGameOver = false;
    bool mWonGame = false;
};


bool characterTypeFit(ECombatCharacterType targetType, ECombatAttackTargetAlive targetAlive, ECombatCharacterType givenCharacterType, bool givenCharacterAlive);

std::string returnDescriptionOfGridPattern(const ECombatActionGridPattern moveAttackType, const int num, const int out);

std::string returnDescriptionCharacterAttack(const ECombatNumTilesToAttack numTiles, const ECombatCharacterType targetType);

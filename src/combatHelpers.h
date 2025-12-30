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

enum ECombatAttackTargetType
{
    ECombatAttackTargetType_INVALID = -1,
    ECombatAttackTargetType_SELF,

    ECombatAttackTargetType_ALIVE_PLAYERS,
    ECombatAttackTargetType_ONE_ALIVE_PLAYER,
    ECombatAttackTargetType_ONE_PLAYER,
    ECombatAttackTargetType_ALL_ALIVE_PLAYERS,

    ECombatAttackTargetType_ALIVE_ENEMIES,
    ECombatAttackTargetType_ONE_ALIVE_ENEMY,
    ECombatAttackTargetType_ONE_ENEMY,
    ECombatAttackTargetType_ALL_ALIVE_ENEMIES,

    ECombatAttackTargetType_ALIVE_CHARACTERS,
    ECombatAttackTargetType_ONE_ALIVE_CHARACTER,
    ECombatAttackTargetType_ONE_CHARACTER,
    ECombatAttackTargetType_ALL_ALIVE_CHARACTERS,
    ECombatAttackTargetType_MAX
};

struct GameOverStats
{
    bool mGameOver = false;
    bool mWonGame = false;
};


bool characterTypeFit(ECombatAttackTargetType targetType, ECombatCharacterType characterType, bool targetAlive);

ECombatCharacterType getCharacterTypeFromAttackTargetType(ECombatAttackTargetType targetType);

std::string returnDescriptionOfGridPattern(const ECombatActionGridPattern moveAttackType, const int num, const int out);

std::string returnDescriptionOfAttackTargetType(const ECombatAttackTargetType targetType);

std::string returnDescirptionOfNumTilesToAttack(const ECombatNumTilesToAttack numTiles);
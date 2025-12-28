#pragma once
#include <string>
#include <SDL3/SDL.h>

std::string createTileName(const int row, const int col);


enum EMiniGameCombatMoveAttackTypes
{
    EMiniGameCombatMoveAttackTypes_INVALID = -1,
    EMiniGameCombatMoveAttackTypes_NONE,
    EMiniGameCombatMoveAttackTypes_SQUARE,
    EMiniGameCombatMoveAttackTypes_CROSS,
    EMiniGameCombatMoveAttackTypes_CHECKERBOARD,
    EMiniGameCombatMoveAttackTypes_WHOLE_GRID,
    EMiniGameCombatMoveAttackTypes_ONE_CHARACTER,
    EMiniGameCombatMoveAttackTypes_ONE_PLAYER,
    EMiniGameCombatMoveAttackTypes_ONE_ENEMY,
    EMiniGameCombatMoveAttackTypes_MAX
};

enum EMiniGameCombatSpecialEffectTypes
{
    EMiniGameCombatSpecialEffectTypes_INVALID = -1,
    EMiniGameCombatSpecialEffectTypes_NONE,
    EMiniGameCombatSpecialEffectTypes_STUN,
    EMiniGameCombatSpecialEffectTypes_LOSE_TURN,
    EMiniGameCombatSpecialEffectTypes_ATTACK_MULTIPLIER,
    EMiniGameCombatSpecialEffectTypes_DEFENSE_CAPACITY_MULTIPLIER,
    EMiniGameCombatSpecialEffectTypes_HEALTH_CAPACITY_MULTIPLIER,
    EMiniGameCombatSpecialEffectTypes_HEAL,
    EMiniGameCombatSpecialEffectTypes_FULL_HEAL,
    EMiniGameCombatSpecialEffectTypes_POISON,
    EMiniGameCombatSpecialEffectTypes_MAX
};

enum EMiniGameCombatActionType
{
    EMiniGameCombatActionType_INVALID = -1,
    EMiniGameCombatActionType_NONE,
    EMiniGameCombatActionType_MOVE,
    EMiniGameCombatActionType_ATTACK,
    EMiniGameCombatActionType_DEFEND,
    EMiniGameCombatActionType_PASS,
    EMiniGameCombatActionType_MAX
};

// under the hood, attack and support, are implemented as attacks,
// however I separate these in the menu into categories that will make more sense
// to the player
enum EMiniGameCombatAttackCategoryType
{
    EMiniGameCombatAttackCategoryType_INVALID = -1,
    EMiniGameCombatAttackCategoryType_ATTACK,
    EMiniGameCombatAttackCategoryType_SUPPORT,
    EMiniGameCombatAttackCategoryType_MAX
};

enum EMiniGameCombatCharacterType
{
    EMiniGameCombatCharacterType_INVALID = -1,
    EMiniGameCombatCharacterType_CHARACTER,
    EMiniGameCombatCharacterType_PLAYER,
    EMiniGameCombatCharacterType_ENEMY,
    EMiniGameCombatCharacterType_MAX
};

enum EMiniGameCombatTileType
{
    EMiniGameCombatTileType_INVALID = -1,
    EMiniGameCombatTileType_NORMAL,
    EMiniGameCombatTileType_IMPASSABLE,
    EMiniGameCombatTileType_TELEPORTER,
    EMiniGameCombatTileType_MAX
};
   
enum EMiniGameCombatTileMode
{
    EMiniGameCombatTileMode_INVALID = -1,
    EMiniGameCombatTileMode_NOT_SELECTED,
    EMiniGameCombatTileMode_SELECTED,
    EMiniGameCombatTileMode_HIGHLIGHTED,
    EMiniGameCombatTileMode_MAX
};

enum EAttackTargetType
{
    EAttackTargetType_INVALID = -1,
    EAttackTargetType_ALL_PLAYERS,
    EAttackTargetType_ALL_ENEMIES,
    EAttackTargetType_SELF,
    EAttackTargetType_ONE_PLAYER,
    EAttackTargetType_ONE_ENEMY,
    EAttackTargetType_ONE_CHARACTER,
    EAttackTargetType_ALL_CHARACTERS,
    EAttackTargetType_MAX
};

struct GameOverStats
{
    bool mGameOver = false;
    bool mWonGame = false;
};


bool characterTypeFit(EAttackTargetType targetType, EMiniGameCombatCharacterType characterType);

std::string returnDescriptionOfMoveAttackType(const EMiniGameCombatMoveAttackTypes moveAttackType, const int num, const int out);

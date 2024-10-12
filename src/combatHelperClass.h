#pragma once
#include <string>
#include <vector>
#include <cmath>

std::string returnDescriptionOfMoveAttackType(int moveAttackType);

enum EMiniGameCombatMoveAttackTypes
{
    EMiniGameCombatMoveAttackTypes_INVALID = -1,
    EMiniGameCombatMoveAttackTypes_NONE,
    EMiniGameCombatMoveAttackTypes_SQUARE1,
    EMiniGameCombatMoveAttackTypes_SQUARE2,
    EMiniGameCombatMoveAttackTypes_SQUARE2_1UNIT_OUT,
    EMiniGameCombatMoveAttackTypes_SQUARE3_2UNITS_OUT,
    EMiniGameCombatMoveAttackTypes_CROSS1,
    EMiniGameCombatMoveAttackTypes_CROSS1_1UNIT_OUT,
    EMiniGameCombatMoveAttackTypes_CROSS2,
    EMiniGameCombatMoveAttackTypes_CROSS2_1UNIT_OUT,
    EMiniGameCombatMoveAttackTypes_CROSS4,
    EMiniGameCombatMoveAttackTypes_CHECKERBOARD2UNITS,
    EMiniGameCombatMoveAttackTypes_MAX
};

enum EMiniGameCombatSpecialEffectTypes
{
    EMiniGameCombatSpecialEffectTypes_INVALID = -1,
    EMiniGameCombatSpecialEffectTypes_NONE,
    EMiniGameCombatSpecialEffectTypes_KNOCKBACK,
    EMiniGameCombatSpecialEffectTypes_STUN,
    EMiniGameCombatSpecialEffectTypes_LOSE_TURN,
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

    /*
class AttackedCharacterChanges():
    def __init__(self, character, healthChange, defenseChange, turnsToPassChange, statusChange):
    self.character         = character
    self.healthChange      = healthChange
    self.defenseChange     = defenseChange
    self.turnsToPassChange = turnsToPassChange
    self.statusChange      = statusChange
    self.movementChange    = None*/
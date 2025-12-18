#pragma once
#include "combatAttackHelper.h"
#include "combatAttackPreset.h"
#include "combatHelperClass.h"
#include <vector>

struct CombatCharacterPreset
{
    EMiniGameCombatCharacterType mType          = EMiniGameCombatCharacterType_INVALID;
    std::vector <Attack> mAttacks;

    EMiniGameCombatMoveAttackTypes mMoveType    = EMiniGameCombatMoveAttackTypes_INVALID;

    int mAttackDamage                           = -1;
    int mHealthCapacity                         = -1;
    int mDefenseCapacity                        = -1;

    std::string mModelFileName;
    std::string mIconFileName;

    int mMaxWidth  = 30;
    int mMaxHeight = 75;
};

struct PlayerCombatPreset : public CombatCharacterPreset { PlayerCombatPreset(); };

struct RatCombatPreset : public CombatCharacterPreset { RatCombatPreset(); };

struct FrogCombatPreset : public CombatCharacterPreset { FrogCombatPreset(); };

#pragma once
#include "combatAttackHelper.h"
#include "combatAttackPresets.h"
#include "combatHelperClass.h"
#include <vector>


struct CombatCharacterPreset
{
    EMiniGameCombatCharacterType mType          = EMiniGameCombatCharacterType_INVALID;
    std::vector <Attack> mAttacks;

    EMiniGameCombatMoveAttackTypes mMoveType    = EMiniGameCombatMoveAttackTypes_INVALID;
    int                            mMoveNum     = 1;
    int                            mMoveOut     = 0;

    int mAttackDamage                           = -1;
    int mHealAmount                             = 0;
    int mHealthCapacity                         = -1;
    int mDefenseCapacity                        = -1;

    std::string mModelFileName;
    std::string mIconFileName;

    int mMaxWidth  = 30;
    int mMaxHeight = 75;

    void setMove(EMiniGameCombatMoveAttackTypes type, int num, int out);
};


struct LotusCombatPreset : public CombatCharacterPreset { LotusCombatPreset(); };

struct EzraCombatPreset : public CombatCharacterPreset { EzraCombatPreset(); };


struct RatCombatPreset : public CombatCharacterPreset { RatCombatPreset(); };

struct BouncyRatCombatPreset : public CombatCharacterPreset { BouncyRatCombatPreset(); };

struct FrogCombatPreset : public CombatCharacterPreset { FrogCombatPreset(); };

struct BouncyFrogCombatPreset : public CombatCharacterPreset { BouncyFrogCombatPreset(); };

struct HidingRatCombatPreset : public CombatCharacterPreset { HidingRatCombatPreset(); };

struct FastRatCombatPreset : public CombatCharacterPreset { FastRatCombatPreset(); };
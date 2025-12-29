#pragma once
#include "combatAttack.h"
#include "combatAttackPresets.h"
#include "combatHelpers.h"
#include <vector>

enum EMiniGameCombatCharacterSpecies
{
    EMiniGameCombatCharacterSpecies_INVALID = -1,
    EMiniGameCombatCharacterSpecies_LOTUS,
    EMiniGameCombatCharacterSpecies_EZRA,
    EMiniGameCombatCharacterSpecies_RAT,
    EMiniGameCombatCharacterSpecies_BOUNCY_RAT,
    EMiniGameCombatCharacterSpecies_FROG,
    EMiniGameCombatCharacterSpecies_BOUNCY_FROG,
    EMiniGameCombatCharacterSpecies_HIDING_RAT,
    EMiniGameCombatCharacterSpecies_FAST_RAT,
    EMiniGameCombatCharacterSpecies_MAX
};


struct CombatCharacterPreset
{
    EMiniGameCombatCharacterType mType          = EMiniGameCombatCharacterType_INVALID;
    EMiniGameCombatCharacterSpecies mSpecies = EMiniGameCombatCharacterSpecies_INVALID;
    std::string mName;
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

    void setMove(const EMiniGameCombatMoveAttackTypes type, const int num, const int out);
};


struct LotusCombatPreset : public CombatCharacterPreset { LotusCombatPreset(); };

struct EzraCombatPreset : public CombatCharacterPreset { EzraCombatPreset(); };


struct RatCombatPreset : public CombatCharacterPreset { RatCombatPreset(); };

struct BouncyRatCombatPreset : public CombatCharacterPreset { BouncyRatCombatPreset(); };

struct FrogCombatPreset : public CombatCharacterPreset { FrogCombatPreset(); };

struct BouncyFrogCombatPreset : public CombatCharacterPreset { BouncyFrogCombatPreset(); };

struct HidingRatCombatPreset : public CombatCharacterPreset { HidingRatCombatPreset(); };

struct FastRatCombatPreset : public CombatCharacterPreset { FastRatCombatPreset(); };
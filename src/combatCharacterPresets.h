#pragma once
#include "combatAttack.h"
#include "combatAttackPresets.h"
#include "combatHelpers.h"
#include <vector>

enum ECombatCharacterSpecies
{
    ECombatCharacterSpecies_INVALID = -1,
    ECombatCharacterSpecies_LOTUS,
    ECombatCharacterSpecies_EZRA,
    ECombatCharacterSpecies_RAT,
    ECombatCharacterSpecies_BOUNCY_RAT,
    ECombatCharacterSpecies_FROG,
    ECombatCharacterSpecies_BOUNCY_FROG,
    ECombatCharacterSpecies_HIDING_RAT,
    ECombatCharacterSpecies_FAST_RAT,
    ECombatCharacterSpecies_MAX
};


struct CombatCharacterPreset
{
    ECombatCharacterType mType          = ECombatCharacterType_INVALID;
    ECombatCharacterSpecies mSpecies    = ECombatCharacterSpecies_INVALID;
    std::string mName;
    std::vector <Attack> mAttacks;

    ECombatActionGridPattern mMoveType  = ECombatActionGridPattern_INVALID;
    int                      mMoveNum   = 1;
    int                      mMoveOut   = 0;

    int mAttackDamage                   = -1;
    int mHealAmount                     = 0;
    int mHealthCapacity                 = -1;
    int mDefenseCapacity                = -1;

    std::string mModelFileName;
    std::string mIconFileName;

    int mMaxWidth  = 30;
    int mMaxHeight = 75;

    void setMove(const ECombatActionGridPattern type, const int num, const int out);
};


struct LotusCombatPreset : public CombatCharacterPreset { LotusCombatPreset(); };

struct EzraCombatPreset : public CombatCharacterPreset { EzraCombatPreset(); };


struct RatCombatPreset : public CombatCharacterPreset { RatCombatPreset(); };

struct BouncyRatCombatPreset : public CombatCharacterPreset { BouncyRatCombatPreset(); };

struct FrogCombatPreset : public CombatCharacterPreset { FrogCombatPreset(); };

struct BouncyFrogCombatPreset : public CombatCharacterPreset { BouncyFrogCombatPreset(); };

struct HidingRatCombatPreset : public CombatCharacterPreset { HidingRatCombatPreset(); };

struct FastRatCombatPreset : public CombatCharacterPreset { FastRatCombatPreset(); };
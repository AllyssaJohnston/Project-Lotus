#pragma once
#include "combatHelperClass.h"
#include "helperClass.h"

struct SpecialEffectPreset
{
    EMiniGameCombatSpecialEffectTypes   mType = EMiniGameCombatSpecialEffectTypes_INVALID;
    EAttackTargetType                   mAttackTargetType = EAttackTargetType_INVALID;
    std::string                         mName = "";
    float                               mAmount = 0.0;
    bool                                mSpecial = false;
    int                                 mTurns = 1;

    SpecialEffectPreset() {}
};

struct SpecialEffectStun : public SpecialEffectPreset { SpecialEffectStun(int numTurns = 1); };

struct SpecialEffectSelfStun : public SpecialEffectPreset { SpecialEffectSelfStun(int numTurns = 1); };

struct SpecialEffectAttackMultiplier : public SpecialEffectPreset { SpecialEffectAttackMultiplier(float multiplier, int turns, EAttackTargetType attackTargetType); };

struct SpecialEffectDefenseCapacityMultiplier : public SpecialEffectPreset { SpecialEffectDefenseCapacityMultiplier(float multiplier, int turns, EAttackTargetType attackTargetType); };

struct SpecialEffectHeal : public SpecialEffectPreset { SpecialEffectHeal(int amount, EAttackTargetType attackTargetType); };

struct SpecialEffectFullHeal : public SpecialEffectPreset { SpecialEffectFullHeal(EAttackTargetType attackTargetType); };
#pragma once
#include "combatHelpers.h"
#include "helpers.h"

struct SpecialEffectPreset
{
    EMiniGameCombatSpecialEffectTypes   mType = EMiniGameCombatSpecialEffectTypes_INVALID;
    EAttackTargetType                   mAttackTargetType = EAttackTargetType_INVALID;
    std::string                         mName = "";
    float                               mAmount = 0.0;
    int                                 mTurns = 1;

    SpecialEffectPreset() {}
};

struct SpecialEffectStun : public SpecialEffectPreset { SpecialEffectStun(const int numTurns = 1); };

struct SpecialEffectSelfStun : public SpecialEffectPreset { SpecialEffectSelfStun(const int numTurns = 1); };

struct SpecialEffectAttackMultiplier : public SpecialEffectPreset { SpecialEffectAttackMultiplier(const float multiplier, const int turns, const EAttackTargetType attackTargetType); };

struct SpecialEffectDefenseCapacityMultiplier : public SpecialEffectPreset { SpecialEffectDefenseCapacityMultiplier(const float multiplier, const int turns, const EAttackTargetType attackTargetType); };

struct SpecialEffectHeal : public SpecialEffectPreset { SpecialEffectHeal(const int amount, EAttackTargetType attackTargetType); };

struct SpecialEffectFullHeal : public SpecialEffectPreset { SpecialEffectFullHeal(const EAttackTargetType attackTargetType); };

struct SpecialEffectRevive : public SpecialEffectPreset { SpecialEffectRevive(const EAttackTargetType attackTargetType); };

struct SpecialEffectPoison : public SpecialEffectPreset { SpecialEffectPoison(const int amount, const int turns); };
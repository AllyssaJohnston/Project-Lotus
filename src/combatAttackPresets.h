#pragma once
#include "combatHelpers.h"
#include "helpers.h"

struct SpecialEffectPreset
{
    ECombatSpecialEffectTypes   mType               = ECombatSpecialEffectTypes_INVALID;
    ECombatCharacterType        mAttackTargetType   = ECombatCharacterType_INVALID;
    std::string                 mName = "";
    float                       mAmount = 0.0;
    int                         mTurns = 1;

    SpecialEffectPreset() {}
};

struct SpecialEffectStun : public SpecialEffectPreset { SpecialEffectStun(const int numTurns = 1); };

struct SpecialEffectSelfStun : public SpecialEffectPreset { SpecialEffectSelfStun(const int numTurns = 1); };

struct SpecialEffectAttackMultiplier : public SpecialEffectPreset { SpecialEffectAttackMultiplier(const float multiplier, const int turns, const ECombatCharacterType attackTargetType); };

struct SpecialEffectDefenseCapacityMultiplier : public SpecialEffectPreset { SpecialEffectDefenseCapacityMultiplier(const float multiplier, const int turns, const ECombatCharacterType attackTargetType); };

struct SpecialEffectHeal : public SpecialEffectPreset { SpecialEffectHeal(const int amount, ECombatCharacterType attackTargetType); };

struct SpecialEffectFullHeal : public SpecialEffectPreset { SpecialEffectFullHeal(const ECombatCharacterType attackTargetType); };

struct SpecialEffectRevive : public SpecialEffectPreset { SpecialEffectRevive(const ECombatCharacterType attackTargetType); };

struct SpecialEffectPoison : public SpecialEffectPreset { SpecialEffectPoison(const int amount, const int turns); };
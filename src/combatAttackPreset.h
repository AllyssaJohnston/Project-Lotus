#pragma once
#include <SDL3/SDL.h>
#include "combatHelperClass.h"

struct SpecialEffectPreset
{
    EMiniGameCombatSpecialEffectTypes   mType               = EMiniGameCombatSpecialEffectTypes_INVALID;
    EAttackTargetType                   mAttackTargetType   = EAttackTargetType_INVALID;
    std::string                         mName               = "";
    float                               mAmount             = 0.0;
    int                                 mTurns              = 1;

    SpecialEffectPreset() {}
};

struct SpecialEffectStun : public SpecialEffectPreset { SpecialEffectStun(int numTurns = 1); };

struct SpecialEffectSelfStun : public SpecialEffectPreset { SpecialEffectSelfStun(int numTurns = 1); };

struct SpecialEffectAttackMultiplier : public SpecialEffectPreset { SpecialEffectAttackMultiplier(float multiplier, int turns, EAttackTargetType attackTargetType);  };

struct SpecialEffectHeal : public SpecialEffectPreset { SpecialEffectHeal(int amount, EAttackTargetType attackTargetType); };
#pragma once
#include <vector>
#include <string>

#include "combatHelperClass.h"
#include "combatAttackPreset.h"

struct SpecialEffect
{
    EMiniGameCombatSpecialEffectTypes mType;
    std::string mName;
    int mAmount;

    SpecialEffect(const SpecialEffectPreset& preset) : mType(preset.mType), mName(preset.mName), mAmount(preset.mAmount) {}
};


struct Attack
{
    EMiniGameCombatMoveAttackTypes   mType                      = EMiniGameCombatMoveAttackTypes_INVALID;
    float                            mDamagePercent             = -1.0f;
    bool                             mDamageDistanceDependent   = false;
    std::vector <SpecialEffect>      mSpecialEffects;
    bool                             mRequiresDirectionInput    = false;
    std::string                      mName;
    std::string                      mDescription;
   
    Attack() { ; }
};

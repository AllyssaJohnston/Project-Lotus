#pragma once
#include <vector>
#include <string>

#include "combatHelperClass.h"
#include "combatAttackPresets.h"

struct SpecialEffect
{
    EMiniGameCombatSpecialEffectTypes mType;
    EAttackTargetType mAttackTargetType;
    std::string mName;
    float mAmount;
    int mTurns;
    bool mSpecial;

    SpecialEffect(const SpecialEffectPreset& preset) : mType(preset.mType), mName(preset.mName), mAmount(preset.mAmount), mTurns(preset.mTurns), mAttackTargetType(preset.mAttackTargetType), mSpecial(preset.mSpecial) { ; }
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

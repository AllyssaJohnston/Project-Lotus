#pragma once
#include <vector>
#include <string>

#include "combatHelperClass.h"
#include "combatAttackPreset.h"

class SpecialEffect
{
public:
    EMiniGameCombatSpecialEffectTypes mType;
    std::string mName;

    SpecialEffect(SpecialEffectPreset* pPreset) : mType(pPreset->mType), mName(pPreset->mName) {}
};


class Attack
{
public:
    EMiniGameCombatMoveAttackTypes   mType                      = EMiniGameCombatMoveAttackTypes_INVALID;
    float                            mDamagePercent             = -1.0f;
    bool                             mDamageDistanceDependent   = false;
    std::vector <SpecialEffect>      mSpecialEffects;
    bool                             mRequiresDirectionInput    = false;
    std::string                      mName;
    std::string                      mDescription;

public:
   
    Attack(AttackPreset* pPreset);

    Attack() {}

};

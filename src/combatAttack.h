#pragma once
#include <vector>
#include <string>

#include "combatHelpers.h"
#include "combatAttackPresets.h"

struct SpecialEffect
{
    EMiniGameCombatSpecialEffectTypes   mType;
    EAttackTargetType                   mAttackTargetType;
    std::string                         mName;
    float                               mAmount;
    int                                 mTurns;
    bool                                mSpecial;

    SpecialEffect(const SpecialEffectPreset& preset);
};


struct Attack
{
    EMiniGameCombatMoveAttackTypes      mType;
    int                                 mNum = -1;
    int                                 mOut = -1;
    EMiniGameCombatAttackCategoryType   mCategory;
    float                               mDamagePercent;
    bool                                mDamageDistanceDependent   = false;
    float                               mDamageDropOff             = .25f;
    std::vector <SpecialEffect>         mSpecialEffects;
    bool                                mRequiresDirectionInput    = false;
    std::string                         mName;
    std::string                         mDescription;
   
    Attack(const EMiniGameCombatMoveAttackTypes type, const int num, const int out, const EMiniGameCombatAttackCategoryType category, const float damagePercent, const std::string name, const std::string description);

    Attack(const EMiniGameCombatMoveAttackTypes type, const int num, const int out, const EMiniGameCombatAttackCategoryType category, const float damagePercent, const std::string name);

    Attack(const EMiniGameCombatMoveAttackTypes type, const EMiniGameCombatAttackCategoryType category, const float damagePercent, const std::string name, const std::string description);

    Attack(const EMiniGameCombatMoveAttackTypes type, const EMiniGameCombatAttackCategoryType category, const float damagePercent, const std::string name);
};

#pragma once
#include <vector>
#include <string>

#include "combatHelperClass.h"
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
    std::vector <SpecialEffect>         mSpecialEffects;
    bool                                mRequiresDirectionInput    = false;
    std::string                         mName;
    std::string                         mDescription;
   
    Attack(EMiniGameCombatMoveAttackTypes type, int num, int out, EMiniGameCombatAttackCategoryType category, float damagePercent, std::string name, std::string description);

    Attack(EMiniGameCombatMoveAttackTypes type, int num, int out, EMiniGameCombatAttackCategoryType category, float damagePercent, std::string name);

    Attack(EMiniGameCombatMoveAttackTypes type, EMiniGameCombatAttackCategoryType category, float damagePercent, std::string name, std::string description);

    Attack(EMiniGameCombatMoveAttackTypes type, EMiniGameCombatAttackCategoryType category, float damagePercent, std::string name);
};

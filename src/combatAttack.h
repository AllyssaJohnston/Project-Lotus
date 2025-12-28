#pragma once
#include <vector>
#include <string>
#include <iostream>

#include "combatHelpers.h"
#include "combatAttackPresets.h"

struct SpecialEffect
{
    EMiniGameCombatSpecialEffectTypes   mType;
    EAttackTargetType                   mAttackTargetType;
    std::string                         mName;
    float                               mAmount;
    int                                 mTurns;

    SpecialEffect(const SpecialEffectPreset& preset);
};


class Attack
{
public:
    EMiniGameCombatMoveAttackTypes      mType;
    int                                 mNum = -1;
    int                                 mOut = -1;
    EMiniGameCombatAttackCategoryType   mCategory;
    float                               mDamagePercent;
    bool                                mDamageDistanceDependent   = false;
    float                               mDamageDropOff             = .25f;
    std::vector <SpecialEffect>         mCharacterTileSpecialEffects;   // special effects that target a character/ tile (s)
    std::vector <SpecialEffect>         mGenericSpecialEffects;         // special effects that happen regardless of the character/ tile
    bool                                mRequiresDirectionInput    = false;
    std::string                         mName;
    std::string                         mDescription;
    int                                 mCurCooldown = 0;
    int                                 mCooldownAmount;
    int                                 mUses = 0;
    int                                 mLimit = -1;

private:
    bool                                mUsed = false;

public:
   
    Attack(const EMiniGameCombatMoveAttackTypes type, const int num, const int out, const EMiniGameCombatAttackCategoryType category, const float damagePercent, const int cooldownAmount, const std::string name, const std::string description);

    Attack(const EMiniGameCombatMoveAttackTypes type, const int num, const int out, const EMiniGameCombatAttackCategoryType category, const float damagePercent, const int cooldownAmount, const std::string name);

    Attack(const EMiniGameCombatMoveAttackTypes type, const EMiniGameCombatAttackCategoryType category, const float damagePercent, const int cooldownAmount, const std::string name, const std::string description);

    Attack(const EMiniGameCombatMoveAttackTypes type, const EMiniGameCombatAttackCategoryType category, const float damagePercent, const int cooldownAmount, const std::string name);

    bool canUse() const;

    void use();

    void postTick();
};

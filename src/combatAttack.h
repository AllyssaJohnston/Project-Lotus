#pragma once
#include <vector>
#include <string>
#include <iostream>

#include "combatHelpers.h"
#include "combatAttackPresets.h"

struct SpecialEffect
{
    ECombatSpecialEffectTypes   mType;
    ECombatCharacterType        mAttackTargetType;
    std::string                 mName;
    float                       mAmount;
    int                         mTurns;

    SpecialEffect(const SpecialEffectPreset& preset);
};


class Attack
{
public:
    ECombatActionGridPattern        mType                       = ECombatActionGridPattern_INVALID;
    int                             mNum                        = -1;
    int                             mOut                        = -1;
    ECombatAttackCategoryType       mCategory                   = ECombatAttackCategoryType_INVALID;
    ECombatCharacterType            mAttackTargetType           = ECombatCharacterType_INVALID;
    ECombatAttackTargetAlive        mAttackTargetAlive          = ECombatAttackTargetAlive_ALIVE;
    ECombatNumTilesToAttack         mNumTilesToAttack           = ECombatNumTilesToAttack_ONE;
    float                           mDamagePercent              = 0;
    std::string                     mName;
    std::string                     mDescription;
    int                             mCooldownAmount             = 0;

    bool                            mDamageDistanceDependent    = false;
    float                           mDamageDropOff              = .25f;
    std::vector <SpecialEffect>     mCharacterTileSpecialEffects;   // special effects that target a character/ tile (s)
    std::vector <SpecialEffect>     mGenericSpecialEffects;         // special effects that happen regardless of the character/ tile
    int                             mLimit                      = -1; // per battle

    int                             mCurCooldown = 0;

private:
    bool                            mUsed = false;
    int                             mUses = 0;

public:
   
    Attack(const ECombatActionGridPattern type, ECombatCharacterType targetType, const int num, const int out, const ECombatAttackCategoryType category, const float damagePercent, const int cooldownAmount, const std::string name, const std::string description);

    Attack(const ECombatActionGridPattern type, ECombatCharacterType targetType, const int num, const int out, const ECombatAttackCategoryType category, const float damagePercent, const int cooldownAmount, const std::string name);

    Attack(const ECombatActionGridPattern type, ECombatCharacterType targetType, const ECombatAttackCategoryType category, const float damagePercent, const int cooldownAmount, const std::string name, const std::string description);

    Attack(const ECombatActionGridPattern type, ECombatCharacterType targetType, const ECombatAttackCategoryType category, const float damagePercent, const int cooldownAmount, const std::string name);

    bool canUse() const;

    void use();

    void postTick();
};

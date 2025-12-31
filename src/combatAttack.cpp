#include "combatAttack.h"

SpecialEffect::SpecialEffect(const SpecialEffectPreset& preset) : mType(preset.mType), mName(preset.mName), mAmount(preset.mAmount), mTurns(preset.mTurns), mAttackTargetType(preset.mAttackTargetType) { ; }

Attack::Attack(const ECombatActionGridPattern type, ECombatCharacterType targetType, const int num, const int out, const ECombatAttackCategoryType category, const float damagePercent, const int cooldownAmount, const std::string name, const std::string description)
    : mType(type), mAttackTargetType(targetType), mNum(num), mOut(out), mCategory(category), mDamagePercent(damagePercent), mCooldownAmount(cooldownAmount), mName(name)
{
    mDescription = name + ": " + description;
}

Attack::Attack(const ECombatActionGridPattern type, ECombatCharacterType targetType, const int num, const int out, const ECombatAttackCategoryType category, const float damagePercent, const int cooldownAmount, const std::string name)
    : mType(type), mAttackTargetType(targetType), mNum(num), mOut(out), mCategory(category), mDamagePercent(damagePercent), mCooldownAmount(cooldownAmount), mName(name)
{
    mDescription = name + ": " + returnDescriptionOfGridPattern(type, num, out);
}

Attack::Attack(const ECombatActionGridPattern type, ECombatCharacterType targetType, const ECombatAttackCategoryType category, const float damagePercent, const int cooldownAmount, const std::string name, const std::string description)
    : mType(type), mAttackTargetType(targetType), mCategory(category), mDamagePercent(damagePercent), mCooldownAmount(cooldownAmount), mName(name)
{
    mDescription = name + ": " + description;
}

Attack::Attack(const ECombatActionGridPattern type, ECombatCharacterType targetType, const ECombatAttackCategoryType category, const float damagePercent, const int cooldownAmount, const std::string name)
    : mType(type), mAttackTargetType(targetType), mCategory(category), mDamagePercent(damagePercent), mCooldownAmount(cooldownAmount), mName(name)
{
    mDescription = name + ": " + returnDescriptionOfGridPattern(type, -1, -1);
}

bool Attack::canUse() const { return mCurCooldown == 0 && (mLimit == -1 || mUses < mLimit); }

void Attack::use() 
{
    mUsed = true;
    mCurCooldown = mCooldownAmount;  
    mUses++;
}

void Attack::postTick()  
{
    if (mUsed)
    {
        mUsed = false;
    }
    else
    {
        mCurCooldown = std::max(0, mCurCooldown - 1);
    }
}
#include "combatAttack.h"

SpecialEffect::SpecialEffect(const SpecialEffectPreset& preset) : mType(preset.mType), mName(preset.mName), mAmount(preset.mAmount), mTurns(preset.mTurns), mAttackTargetType(preset.mAttackTargetType), mSpecial(preset.mSpecial) { ; }

Attack::Attack(const EMiniGameCombatMoveAttackTypes type, const int num, const int out, const EMiniGameCombatAttackCategoryType category, const float damagePercent, const std::string name, const std::string description)
    : mType(type), mNum(num), mOut(out), mCategory(category), mDamagePercent(damagePercent), mName(name)
{
    mDescription = name + ": " + description;
}

Attack::Attack(const EMiniGameCombatMoveAttackTypes type, const int num, const int out, const EMiniGameCombatAttackCategoryType category, const float damagePercent, const std::string name)
    : mType(type), mNum(num), mOut(out), mCategory(category), mDamagePercent(damagePercent), mName(name)
{
    mDescription = name + ": " + returnDescriptionOfMoveAttackType(type, num, out);
}

Attack::Attack(const EMiniGameCombatMoveAttackTypes type, const EMiniGameCombatAttackCategoryType category, const float damagePercent, const std::string name, const std::string description)
    : mType(type), mCategory(category), mDamagePercent(damagePercent), mName(name)
{
    mDescription = name + ": " + description;
}

Attack::Attack(const EMiniGameCombatMoveAttackTypes type, const EMiniGameCombatAttackCategoryType category, const float damagePercent, const std::string name)
    : mType(type), mCategory(category), mDamagePercent(damagePercent), mName(name)
{
    mDescription = name + ": " + returnDescriptionOfMoveAttackType(type, -1, -1);
}
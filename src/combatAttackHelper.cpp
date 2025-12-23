#include "combatAttackHelper.h"

SpecialEffect::SpecialEffect(const SpecialEffectPreset& preset) : mType(preset.mType), mName(preset.mName), mAmount(preset.mAmount), mTurns(preset.mTurns), mAttackTargetType(preset.mAttackTargetType), mSpecial(preset.mSpecial) { ; }

Attack::Attack(EMiniGameCombatMoveAttackTypes type, int num, int out, EMiniGameCombatAttackCategoryType category, float damagePercent, std::string name, std::string description) 
    : mType(type), mNum(num), mOut(out), mCategory(category), mDamagePercent(damagePercent), mName(name)
{
    mDescription = name + ": " + description;
}

Attack::Attack(EMiniGameCombatMoveAttackTypes type, int num, int out, EMiniGameCombatAttackCategoryType category, float damagePercent, std::string name)
    : mType(type), mNum(num), mOut(out), mCategory(category), mDamagePercent(damagePercent), mName(name)
{
    mDescription = name + ": " + returnDescriptionOfMoveAttackType(type, num, out);
}

Attack::Attack(EMiniGameCombatMoveAttackTypes type, EMiniGameCombatAttackCategoryType category, float damagePercent, std::string name, std::string description)
    : mType(type), mCategory(category), mDamagePercent(damagePercent), mName(name)
{
    mDescription = name + ": " + description;
}

Attack::Attack(EMiniGameCombatMoveAttackTypes type, EMiniGameCombatAttackCategoryType category, float damagePercent, std::string name)
    : mType(type), mCategory(category), mDamagePercent(damagePercent), mName(name)
{
    mDescription = name + ": " + returnDescriptionOfMoveAttackType(type, -1, -1);
}
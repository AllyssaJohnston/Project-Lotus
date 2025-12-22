#include "combatAttackHelper.h"


Attack::Attack(EMiniGameCombatMoveAttackTypes type, EMiniGameCombatAttackCategoryType category, float damagePercent, std::string name, std::string description) 
    : mType(type), mCategory(category), mDamagePercent(damagePercent), mName(name), mDescription(description) { ; }
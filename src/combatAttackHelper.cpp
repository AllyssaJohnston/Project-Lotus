#include "combatAttackHelper.h"

Attack::Attack(AttackPreset* pPreset) : mType(pPreset->mType), mDescription(pPreset->mDescription), mName(pPreset->mName), 
mDamagePercent(pPreset->mDamagePercent), mDamageDistanceDependent(pPreset->mDamageDistanceDependent), 
mRequiresDirectionInput(pPreset->mRequiresDirectionInput)
{
    for (SpecialEffectPreset effectPreset : pPreset->mSpecialEffectPresets)
    {
        mSpecialEffects.push_back(SpecialEffect(&effectPreset));
    }
}
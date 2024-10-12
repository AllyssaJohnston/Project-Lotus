#include "combatCharacterPresets.h"

SpecialEffectStun::SpecialEffectStun() : SpecialEffectPreset()
{
    mType = EMiniGameCombatSpecialEffectTypes_STUN;
    mName = "STUN";
}



AttackSwordOverheadStrike::AttackSwordOverheadStrike() : AttackPreset()
{
    mType                    = EMiniGameCombatMoveAttackTypes_CROSS1;
    mName                    = "STRIKE";
    mDescription             = "Sword Overhead Strike: CROSS 1";
    mDamagePercent           = 1.0;
    mDamageDistanceDependent = false;
    mRequiresDirectionInput  = false;
    mSpecialEffectPresets.push_back(SpecialEffectStun());
}

AttackSwordSwipe::AttackSwordSwipe() : AttackPreset()
{
    mType                    = EMiniGameCombatMoveAttackTypes_SQUARE1;
    mName                    = "Swipe";
    mDescription             = "Sword Swipe: Square 1";
    mDamagePercent           = .5;
    mDamageDistanceDependent = false;
    mRequiresDirectionInput  = true;
}
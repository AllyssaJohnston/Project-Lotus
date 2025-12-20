#include "combatAttackPreset.h"

SpecialEffectStun::SpecialEffectStun(int numTurns) : SpecialEffectPreset()
{
    mType = EMiniGameCombatSpecialEffectTypes_STUN;
    mName = "STUN " + std::to_string(numTurns) + ((numTurns == 1) ? " TURN" : " TURNS");
    mTurns = numTurns;
}

SpecialEffectSelfStun::SpecialEffectSelfStun(int numTurns) : SpecialEffectPreset()
{
    mType = EMiniGameCombatSpecialEffectTypes_LOSE_TURN;
    mTurns = numTurns;
    mName = "LOSE " + std::to_string(numTurns) + ((numTurns == 1) ? " TURN" : " TURNS");
}

SpecialEffectAttackMultiplier::SpecialEffectAttackMultiplier(float multiplier, int turns, EAttackTargetType attackTargetType)
{
    mType = EMiniGameCombatSpecialEffectTypes_ATTACK_MULTIPLIER;
    mAttackTargetType = attackTargetType;
    mAmount = multiplier;
    mTurns = turns;
    mName = "INCREASE ATTACK BY " + std::to_string(multiplier) + " FOR " + std::to_string(turns);
}

SpecialEffectHeal::SpecialEffectHeal(int amount, EAttackTargetType attackTargetType) : SpecialEffectPreset()
{
    mType = EMiniGameCombatSpecialEffectTypes_HEAL;
    mName = "HEAL " + std::to_string(amount);
    mTurns = 0;
}
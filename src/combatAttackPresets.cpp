#include "combatAttackPresets.h"

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

SpecialEffectAttackMultiplier::SpecialEffectAttackMultiplier(float multiplier, int turns, EAttackTargetType attackTargetType) : SpecialEffectPreset()
{
    mType = EMiniGameCombatSpecialEffectTypes_ATTACK_MULTIPLIER;
    mAttackTargetType = attackTargetType;
    mAmount = multiplier;
    mTurns = turns;
    mName = "INCREASE ATTACK BY " + floatDecimalToString(multiplier) + ((turns == INT_MAX) ? "" : (" FOR " + std::to_string(turns)) + ((turns == 1) ? " TURN" : " TURNS"));
}

SpecialEffectDefenseCapacityMultiplier::SpecialEffectDefenseCapacityMultiplier(float multiplier, int turns, EAttackTargetType attackTargetType) : SpecialEffectPreset()
{
    mType = EMiniGameCombatSpecialEffectTypes_DEFENSE_CAPACITY_MULTIPLIER;
    mAttackTargetType = attackTargetType;
    mAmount = multiplier;
    mTurns = turns;
    mName = "INCREASE DEFENSE CAPACITY BY " + floatDecimalToString(multiplier) + ((turns == INT_MAX) ? "" : (" FOR " + std::to_string(turns)) + ((turns == 1) ? " TURN" : " TURNS"));
}

SpecialEffectHeal::SpecialEffectHeal(int amount, EAttackTargetType attackTargetType) : SpecialEffectPreset()
{
    mType = EMiniGameCombatSpecialEffectTypes_HEAL;
    mName = "HEAL " + std::to_string(amount);
    mAmount = (float)amount;
    mTurns = 0;
    mAttackTargetType = attackTargetType;
}

SpecialEffectFullHeal::SpecialEffectFullHeal(EAttackTargetType attackTargetType) : SpecialEffectPreset()
{
    mType = EMiniGameCombatSpecialEffectTypes_HEAL;
    mName = "FULL HEAL";
    mSpecial = true;
    mTurns = 0;
    mAttackTargetType = attackTargetType;
}
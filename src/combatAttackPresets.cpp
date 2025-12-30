#include "combatAttackPresets.h"

SpecialEffectStun::SpecialEffectStun(const int numTurns) : SpecialEffectPreset()
{
    mType = ECombatSpecialEffectTypes_STUN;
    mAttackTargetType = ECombatAttackTargetType_ONE_ALIVE_CHARACTER;
    mName = "STUN " + std::to_string(numTurns) + ((numTurns == 1) ? " TURN" : " TURNS");
    mTurns = numTurns;
}

SpecialEffectSelfStun::SpecialEffectSelfStun(const int numTurns) : SpecialEffectPreset()
{
    mType = ECombatSpecialEffectTypes_LOSE_TURN;
    mAttackTargetType = ECombatAttackTargetType_SELF;
    mTurns = numTurns;
    mName = "LOSE " + std::to_string(numTurns) + ((numTurns == 1) ? " TURN" : " TURNS");
}

SpecialEffectAttackMultiplier::SpecialEffectAttackMultiplier(const float multiplier, const int turns, const ECombatAttackTargetType attackTargetType) : SpecialEffectPreset()
{
    mType = ECombatSpecialEffectTypes_ATTACK_MULTIPLIER;
    mAttackTargetType = attackTargetType;
    mAmount = multiplier;
    mTurns = turns;
    mName = "INCREASE ATTACK BY " + floatDecimalToString(multiplier) + ((turns == INT_MAX) ? "" : (" FOR " + std::to_string(turns)) + ((turns == 1) ? " TURN" : " TURNS"));
}

SpecialEffectDefenseCapacityMultiplier::SpecialEffectDefenseCapacityMultiplier(const float multiplier, const int turns, const ECombatAttackTargetType attackTargetType) : SpecialEffectPreset()
{
    mType = ECombatSpecialEffectTypes_DEFENSE_CAPACITY_MULTIPLIER;
    mAttackTargetType = attackTargetType;
    mAmount = multiplier;
    mTurns = turns;
    mName = "INCREASE DEFENSE CAPACITY BY " + floatDecimalToString(multiplier) + ((turns == INT_MAX) ? "" : (" FOR " + std::to_string(turns)) + ((turns == 1) ? " TURN" : " TURNS"));
}

SpecialEffectHeal::SpecialEffectHeal(const int amount, const ECombatAttackTargetType attackTargetType) : SpecialEffectPreset()
{
    mType = ECombatSpecialEffectTypes_HEAL;
    mName = "HEAL " + std::to_string(amount);
    mAmount = (float)amount;
    mTurns = 0;
    mAttackTargetType = attackTargetType;
}

SpecialEffectFullHeal::SpecialEffectFullHeal(const ECombatAttackTargetType attackTargetType) : SpecialEffectPreset()
{
    mType = ECombatSpecialEffectTypes_FULL_HEAL;
    mName = "FULL HEAL";
    mTurns = 0;
    mAttackTargetType = attackTargetType;
}

SpecialEffectRevive::SpecialEffectRevive(const ECombatAttackTargetType attackTargetType) : SpecialEffectPreset()
{
    mType = ECombatSpecialEffectTypes_REVIVE;
    mName = "REVIVE";
    mTurns = 0;
    mAttackTargetType = attackTargetType;
}

SpecialEffectPoison::SpecialEffectPoison(const int amount, const int turns)
{
    mType = ECombatSpecialEffectTypes_POISON;
    mName = "POISON";
    mAmount = (float)amount;
    mTurns = turns;
    mAttackTargetType = ECombatAttackTargetType_ONE_ALIVE_CHARACTER;
}
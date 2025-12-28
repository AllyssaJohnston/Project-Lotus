#include "combatAttackPresets.h"

SpecialEffectStun::SpecialEffectStun(const int numTurns) : SpecialEffectPreset()
{
    mType = EMiniGameCombatSpecialEffectTypes_STUN;
    mAttackTargetType = EAttackTargetType_ONE_CHARACTER;
    mName = "STUN " + std::to_string(numTurns) + ((numTurns == 1) ? " TURN" : " TURNS");
    mTurns = numTurns;
}

SpecialEffectSelfStun::SpecialEffectSelfStun(const int numTurns) : SpecialEffectPreset()
{
    mType = EMiniGameCombatSpecialEffectTypes_LOSE_TURN;
    mAttackTargetType = EAttackTargetType_SELF;
    mTurns = numTurns;
    mName = "LOSE " + std::to_string(numTurns) + ((numTurns == 1) ? " TURN" : " TURNS");
}

SpecialEffectAttackMultiplier::SpecialEffectAttackMultiplier(const float multiplier, const int turns, const EAttackTargetType attackTargetType) : SpecialEffectPreset()
{
    mType = EMiniGameCombatSpecialEffectTypes_ATTACK_MULTIPLIER;
    mAttackTargetType = attackTargetType;
    mAmount = multiplier;
    mTurns = turns;
    mName = "INCREASE ATTACK BY " + floatDecimalToString(multiplier) + ((turns == INT_MAX) ? "" : (" FOR " + std::to_string(turns)) + ((turns == 1) ? " TURN" : " TURNS"));
}

SpecialEffectDefenseCapacityMultiplier::SpecialEffectDefenseCapacityMultiplier(const float multiplier, const int turns, const EAttackTargetType attackTargetType) : SpecialEffectPreset()
{
    mType = EMiniGameCombatSpecialEffectTypes_DEFENSE_CAPACITY_MULTIPLIER;
    mAttackTargetType = attackTargetType;
    mAmount = multiplier;
    mTurns = turns;
    mName = "INCREASE DEFENSE CAPACITY BY " + floatDecimalToString(multiplier) + ((turns == INT_MAX) ? "" : (" FOR " + std::to_string(turns)) + ((turns == 1) ? " TURN" : " TURNS"));
}

SpecialEffectHeal::SpecialEffectHeal(const int amount, const EAttackTargetType attackTargetType) : SpecialEffectPreset()
{
    mType = EMiniGameCombatSpecialEffectTypes_HEAL;
    mName = "HEAL " + std::to_string(amount);
    mAmount = (float)amount;
    mTurns = 0;
    mAttackTargetType = attackTargetType;
}

SpecialEffectFullHeal::SpecialEffectFullHeal(const EAttackTargetType attackTargetType) : SpecialEffectPreset()
{
    mType = EMiniGameCombatSpecialEffectTypes_FULL_HEAL;
    mName = "FULL HEAL";
    mTurns = 0;
    mAttackTargetType = attackTargetType;
}

SpecialEffectPoison::SpecialEffectPoison(const int amount, const int turns)
{
    mType = EMiniGameCombatSpecialEffectTypes_POISON;
    mName = "POISON";
    mAmount = (float)amount;
    mTurns = turns;
    mAttackTargetType = EAttackTargetType_ONE_CHARACTER;
}
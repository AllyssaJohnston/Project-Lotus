#include "combatAttackPreset.h"

SpecialEffectStun::SpecialEffectStun(int numTurns) : SpecialEffectPreset()
{
    mType = EMiniGameCombatSpecialEffectTypes_STUN;
    mName = "STUN " + std::to_string(numTurns) + ((numTurns == 1) ? " TURN" : " TURNS");
    mAmount = numTurns;
}

SpecialEffectSelfStun::SpecialEffectSelfStun(int numTurns) : SpecialEffectPreset()
{
    mType = EMiniGameCombatSpecialEffectTypes_LOSE_TURN;
    mAmount = numTurns;
    mName = "LOSE " + std::to_string(numTurns) + ((numTurns == 1) ? " TURN" : " TURNS");
}

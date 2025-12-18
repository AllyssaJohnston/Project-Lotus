#pragma once
#include <SDL3/SDL.h>
#include "combatHelperClass.h"

struct SpecialEffectPreset
{
    EMiniGameCombatSpecialEffectTypes mType = EMiniGameCombatSpecialEffectTypes_INVALID;
    std::string mName = "";
    int mAmount = -1;

    SpecialEffectPreset() {}
};

struct SpecialEffectStun : public SpecialEffectPreset 
{ 
    SpecialEffectStun(int numTurns = 1);
};

struct SpecialEffectSelfStun : public SpecialEffectPreset 
{ 
    SpecialEffectSelfStun(int numTurns = 1);
};

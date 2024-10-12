#pragma once
#include <string>
#include "tileHelper.h"
#include "combatCharacterPresets.h"
#include "combatMovementHelper.h"

class CombatCharacter
{
public:
    std::string                  mName = "invalid";
    EMiniGameCombatCharacterType mType = EMiniGameCombatCharacterType_INVALID;
    CombatMovementManager        mCombatMovementManager;

    bool mAmAlive           = true;

    int mCurAttackDamage    = -1;
    int mCurHealthCapacity  = -1;
    int mCurHealth          = -1;

    int mCurDefenseCapacity = -1;
    int mCurDefense         = 0;

    int mTurnsToPass        = 0;

    CombatCharacter(std::string name, Tile* pCurTile, CombatCharacterPreset* preset);

    ~CombatCharacter() {};

    void preTick();

    void resetStats();

    void takeDamage(int damageToTake);

    void updateAmAlive();

    int defend();

    int stun();

    void move(Tile* pTileInput);

    void postRound();

    bool returnIsLowLife() const;
};

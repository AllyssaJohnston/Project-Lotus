#pragma once
#include <string>
#include "tileHelper.h"
#include "combatCharacterPresets.h"
#include "combatMovementHelper.h"
#include "imageHelper.h"

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

    std::string mModelFileName;
    std::string mIconFileName;

    ImageObject mModel = ImageObject();

    CombatCharacter(const std::string name, Tile* pCurTile, const CombatCharacterPreset& preset);

    ~CombatCharacter() { ; }

    void preTick();

    void resetStats();

    void takeDamage(int damageToTake);

    void updateAmAlive();

    void defend();

    void stun(int numTurnsStunned);

    void move(Tile* pTileInput);

    bool returnIsLowLife() const;

    float getHealthRatio() const;

};

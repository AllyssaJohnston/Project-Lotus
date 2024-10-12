#include "combatCharacterHelper.h"

CombatCharacter::CombatCharacter(std::string name, Tile* pCurTile, CombatCharacterPreset* preset)
{
    mName = name;
    mType = preset->mType;
    mCombatMovementManager = CombatMovementManager(pCurTile, preset);

    mCurAttackDamage    = preset->mAttackDamage;
    mCurHealthCapacity  = preset->mHealthCapacity;
    mCurHealth          = preset->mHealthCapacity;

    mCurDefenseCapacity = preset->mDefenseCapacity;
}

void CombatCharacter::preTick()
{
    if (mAmAlive)
    {
        mCombatMovementManager.preTick();
    }
}

void CombatCharacter::resetStats()
{
    mAmAlive        = true;
    mCurHealth      = mCurHealthCapacity;
    mCurDefense     = 0;
    mTurnsToPass    = 0;
    mCombatMovementManager.resetStats();
}

void CombatCharacter::takeDamage(int damageToTake)
{
    int defenseChange = 0;

    if (mCurDefense - damageToTake >= 0)
    {
        //Will have leftover defense
        defenseChange = damageToTake;
    }
    else
    {
        defenseChange = mCurDefense;
    }
    mCurDefense -= defenseChange;

    int healthChange  = damageToTake - defenseChange;
    mCurHealth -= healthChange;

    mTurnsToPass = 0;

    bool statusChange = false;
    updateAmAlive();
    if (mAmAlive == false)
    {
        statusChange = true;
    }

    //create AttackCharacterChanges
}

void CombatCharacter::updateAmAlive()
{
    if (mCurHealth <= 0)
    {
        mAmAlive = false;
        return;
    }
    mAmAlive = true;
}

int CombatCharacter::defend()
{
    int defenseChange = mCurDefenseCapacity - mCurDefense;
    mCurDefense += defenseChange;
    return defenseChange;
}

int CombatCharacter::stun()
{
    int turnsToPassChange = 1 - mTurnsToPass;
    mTurnsToPass          = 1;
    return turnsToPassChange;
}

void CombatCharacter::move(Tile* pTileInput)
{
    mCombatMovementManager.setCurTile(pTileInput);
}

void CombatCharacter::postRound()
{
    mTurnsToPass -= 1;
    if (mTurnsToPass < 0)
    {
        mTurnsToPass = 0;
    }
}

bool CombatCharacter::returnIsLowLife() const
{
    if ((mCurHealth / mCurHealthCapacity) * 100 <= 10)
    {
        return true;
    }
    return false;
}
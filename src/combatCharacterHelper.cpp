#include "combatCharacterHelper.h"

CombatCharacter::CombatCharacter(const std::string name, Tile* pCurTile, const CombatCharacterPreset& preset) : mCombatMovementManager(CombatMovementManager(pCurTile, preset))
{
    mName = name;
    mType = preset.mType;


    mCurAttackDamage    = preset.mAttackDamage;
    mCurHealthCapacity  = preset.mHealthCapacity;
    mCurHealth          = preset.mHealthCapacity;

    mCurDefenseCapacity = preset.mDefenseCapacity;

    mModelFileName = preset.mModelFileName;
    mIconFileName = preset.mIconFileName;

    mModel.setupImageObject(mModelFileName, preset.mMaxWidth, preset.mMaxHeight, EHowToDetermineWidthHeight_GET_BEST_IMAGE_RATIO);
}

void CombatCharacter::preTick()
{
    if (mAmAlive)
    {
        mCombatMovementManager.preTick();
    }
    mTurnsToPass = std::max(0, mTurnsToPass -1);
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
    // take away defense, then health if needed.
    // don't let defense go below 0
    int defenseChange = (mCurDefense - damageToTake >= 0) ? damageToTake : mCurDefense;
    mCurDefense -= defenseChange;

    mCurHealth -= damageToTake - defenseChange;
    
    updateAmAlive();
}

void CombatCharacter::updateAmAlive() { mAmAlive = mCurHealth > 0; }

// defend up to max defense
void CombatCharacter::defend() { mCurDefense = mCurDefenseCapacity; }

void CombatCharacter::stun(int numTurnsStunned) { mTurnsToPass += numTurnsStunned; }

void CombatCharacter::move(Tile* pTileInput) { mCombatMovementManager.setCurTile(pTileInput); }

bool CombatCharacter::returnIsLowLife() const { return ((mCurHealth / mCurHealthCapacity) * 100 <= 10); }

float CombatCharacter::getHealthRatio() const { return (float)mCurHealth / (float)mCurHealthCapacity; }
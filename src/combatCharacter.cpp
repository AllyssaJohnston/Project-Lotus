#include "combatCharacter.h"

CombatCharacter::CombatCharacter(Tile& curTile, const CombatCharacterPreset& preset) : mCombatMovementManager(CombatMovementManager(curTile, preset))
{
    mName = preset.mName;
    mType = preset.mType;
    mSpecies = preset.mSpecies;

    mStandardAttackDamage       = preset.mAttackDamage;
    mStandardHealthCapacity     = preset.mHealthCapacity;
    mCurHealth                  = preset.mHealthCapacity;
    mHealAmount                 = preset.mHealAmount;
    mStandardDefenseCapacity    = preset.mDefenseCapacity;

    mModelFileName = preset.mModelFileName;
    mIconFileName = preset.mIconFileName;

    mModel.setupImageObject(mModelFileName, preset.mMaxWidth, preset.mMaxHeight, EHowToDetermineWidthHeight_GET_BEST_IMAGE_RATIO);
}

void CombatCharacter::start() { mStarted = true; }

void CombatCharacter::preTick()
{
    if (mAmAlive)
    {
        mCombatMovementManager.preTick();
    }
}

void CombatCharacter::postTick()
{
    if (mAmAlive)
    {
        mCombatMovementManager.preTick();
        mTurnsToPass = std::max(0, mTurnsToPass - 1);

        updateModifiers(mAttackDamageModifiers);
        updateModifiers(mHealthCapacityModifiers);
        updateModifiers(mDefenseCapacityModifiers);
    }
}

void CombatCharacter::resetStats()
{
    mStarted        = false;
    mAmAlive        = true;
    mCurHealth      = mStandardHealthCapacity;
    mCurDefense     = 0;
    mTurnsToPass    = 0;
    mAttackDamageModifiers.clear();
    mHealthCapacityModifiers.clear();
    mDefenseCapacityModifiers.clear();
    mCombatMovementManager.resetStats();
}


bool CombatCharacter::isStarted() const { return mStarted;  }

bool CombatCharacter::isAlive() const { return mAmAlive; }

void CombatCharacter::updateAmAlive() 
{ 
    mAmAlive = mCurHealth > 0; 
    if (!mAmAlive)
    {
        mTurnsToPass = 0;
        mCurHealth = 0;
    }
}


int CombatCharacter::getCurDamage() const
{ 
    if (mAttackDamageModifiers.size() == 0)
    {
        return mStandardAttackDamage;
    }
    float damage = 0.0;
    for (const std::pair<float, int>& modifier : mAttackDamageModifiers)
    {
        damage += mStandardAttackDamage * modifier.first;
    }
    return (int)damage;
}

int CombatCharacter::getBaseDamage() const { return mStandardAttackDamage; }

void CombatCharacter::addDamageModifier(const float multiplier, const int numTurns) 
{ 
    if (numTurns == INT_MAX)
    {
        mStandardAttackDamage = (int)(mStandardAttackDamage * multiplier);
    }
    else
    {
        mAttackDamageModifiers.push_back(std::pair<float, int>(multiplier, numTurns));
    }
}

void CombatCharacter::takeDamage(const int damageToTake)
{
    // take away defense, then health if needed.
    // don't let defense go below 0
    int defenseChange = (mCurDefense - damageToTake >= 0) ? damageToTake : mCurDefense;
    mCurDefense -= defenseChange;

    mCurHealth -= damageToTake - defenseChange;
    
    updateAmAlive();
}

std::vector<std::pair<float, int>> CombatCharacter::getDamageModifiers() const { return mAttackDamageModifiers; }


int CombatCharacter::getCurHealth() const { return mCurHealth; }

int CombatCharacter::getCurHealthCapacity() const 
{
    if (mHealthCapacityModifiers.size() == 0)
    {
        return mStandardHealthCapacity;
    }
    float healthCapacity = 0.0;
    for (const std::pair<float, int>& modifier : mHealthCapacityModifiers)
    {
        healthCapacity += mStandardHealthCapacity * modifier.first;
    }
    return (int)healthCapacity;
}

int CombatCharacter::getBaseHealthCapacity() const { return mStandardHealthCapacity; }

bool CombatCharacter::returnIsLowHealth() const { return ((mCurHealth / getCurHealthCapacity()) * 100 <= 10); }

float CombatCharacter::getHealthRatio() const { return (float)mCurHealth / (float)getCurHealthCapacity(); }

void CombatCharacter::addHealthCapacityModifier(const float multiplier, const int numTurns) 
{ 
    if (numTurns == INT_MAX)
    {
        mStandardHealthCapacity = (int)(mStandardHealthCapacity * multiplier);
    }
    else
    {
        mHealthCapacityModifiers.push_back(std::pair<float, int>(multiplier, numTurns));
    }
}

std::vector<std::pair<float, int>> CombatCharacter::getHealthCapacityModifiers() const { return mHealthCapacityModifiers; }

void CombatCharacter::heal(const int amount)
{
    mCurHealth += amount;
    mCurHealth = std::min(mCurHealth, getCurHealthCapacity());
}

void CombatCharacter::fullHeal() { mCurHealth = getCurHealthCapacity(); }

int CombatCharacter::getHealAmount() const { return mHealAmount; }


int CombatCharacter::getCurDefense() const { return mCurDefense; }

int CombatCharacter::getCurDefenseCapacity() const
{
    if (mDefenseCapacityModifiers.size() == 0)
    {
        return mStandardDefenseCapacity;
    }
    float defenseCapacity = 0.0;
    for (const std::pair<float, int>& modifier : mDefenseCapacityModifiers)
    {
        defenseCapacity += mStandardDefenseCapacity * modifier.first;
    }
    return (int)defenseCapacity;
}

int CombatCharacter::getBaseDefenseCapacity() const { return mStandardDefenseCapacity; }

float CombatCharacter::getDefenseRatio() const { return (float)mCurDefense / (float)getCurDefenseCapacity(); }

// defend up to max defense
void CombatCharacter::defend() { mCurDefense = getCurDefenseCapacity(); }

void CombatCharacter::addDefenseCapacityModifier(const float multiplier, const int numTurns) 
{
    if (numTurns == INT_MAX)
    {
        mStandardDefenseCapacity = (int)(mStandardDefenseCapacity * multiplier);
    }
    else
    {
        mDefenseCapacityModifiers.push_back(std::pair<float, int>(multiplier, numTurns));
    }
}

std::vector<std::pair<float, int>> CombatCharacter::getDefenseCapacityModifiers() const { return mDefenseCapacityModifiers; }


int CombatCharacter::getStuns() const { return mTurnsToPass; }

void CombatCharacter::stun(const int numTurnsStunned) { mTurnsToPass += numTurnsStunned; }


void CombatCharacter::move(Tile* pTileInput) { mCombatMovementManager.setCurTile(pTileInput); }

void CombatCharacter::revertToState(const CombatCharacter& prevState)
{
    mAmAlive                    = prevState.mAmAlive;

    mStandardAttackDamage       = prevState.mStandardAttackDamage;
    mAttackDamageModifiers      = prevState.mAttackDamageModifiers;

    mHealAmount                 = prevState.mHealAmount;
    mCurHealth                  = prevState.mCurHealth;
    mStandardHealthCapacity     = prevState.mStandardHealthCapacity;
    mHealthCapacityModifiers    = prevState.mHealthCapacityModifiers;

    mCurDefense                 = prevState.mCurDefense;
    mStandardDefenseCapacity    = prevState.mStandardDefenseCapacity;
    mDefenseCapacityModifiers   = prevState.mDefenseCapacityModifiers;

    mTurnsToPass                = prevState.mTurnsToPass;

    mCombatMovementManager.setCurTile(prevState.mCombatMovementManager.getCurTile());
    mCombatMovementManager.setMoveTiles();
}


void CombatCharacter::updateModifiers(std::vector<std::pair<float, int>>& modifierlist)
{
    for (int i = (int)modifierlist.size() - 1; i > -1; i--)
    {
        std::pair<float, int>& modifier = modifierlist[i];
        modifier.second -= 1;
        if (modifier.second < 0)
        {
            modifierlist.erase(modifierlist.begin() + i); // remove the modifier
        }
    }
}


bool SortCharacterByTileRow::operator()(const CombatCharacter* const pChar1, const CombatCharacter* const pChar2) const { return pChar2->mCombatMovementManager.getCurTile()->mRow > pChar1->mCombatMovementManager.getCurTile()->mRow; }
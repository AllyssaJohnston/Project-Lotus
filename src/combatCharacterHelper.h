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

    std::string mModelFileName;
    std::string mIconFileName;

    ImageObject mModel = ImageObject();

    CombatCharacter(const std::string name, Tile* pCurTile, const CombatCharacterPreset& preset);

    ~CombatCharacter() { ; }

    void preTick();

    void resetStats();


    bool isAlive() const;


    int getCurDamage() const;
    
    int getBaseDamage()const ;

    void addDamageModifier(float multiplier, int numTurns);

    std::vector<std::pair<float, int>> getDamageModifiers() const;

    void takeDamage(int damageToTake);


    int getCurHealth() const;

    int getCurHealthCapacity() const;

    int getBaseHealthCapacity() const;

    bool returnIsLowHealth() const;

    float getHealthRatio() const;

    void addHealthCapacityModifier(float multiplier, int numTurns);

    std::vector<std::pair<float, int>> getHealthCapacityModifiers() const;


    int getCurDefense() const;

    int getCurDefenseCapacity() const;

    int getBaseDefenseCapacity() const;

    void defend();

    void addDefenseCapacityModifier(float multiplier, int numTurns);

    std::vector<std::pair<float, int>> getDefenseCapacityModifiers() const;


    int getStuns() const;

    void stun(int numTurnsStunned);


    void move(Tile* pTile);

private:

    bool mAmAlive = true;

    int mStandardAttackDamage   = -1;
    std::vector<std::pair<float, int>> mAttackDamageModifiers;  // multiplier amount, num turns

    int mCurHealth = -1;
    int mStandardHealthCapacity = -1;
    std::vector<std::pair<float, int>> mHealthCapacityModifiers; // multiplier amount, num turns
    
    int mCurDefense = 0;
    int mStandardDefenseCapacity = -1;
    std::vector<std::pair<float, int>> mDefenseCapacityModifiers; // multiplier amount, num turns
    
    int mTurnsToPass = 0;

    void updateAmAlive();

    
};

void updateModifiers(std::vector<std::pair<float, int>>& modifierlist);

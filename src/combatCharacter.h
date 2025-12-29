#pragma once
#include <string>
#include "tile.h"
#include "combatHelpers.h"
#include "combatCharacterPresets.h"
#include "combatMovementManager.h"
#include "image.h"

class CombatCharacter
{
public:
    std::string                     mName = "invalid";
    EMiniGameCombatCharacterType    mType = EMiniGameCombatCharacterType_INVALID;
    EMiniGameCombatCharacterSpecies mSpecies = EMiniGameCombatCharacterSpecies_INVALID;
    CombatMovementManager           mCombatMovementManager;

    std::string mModelFileName;
    std::string mIconFileName;

    ImageObject mModel = ImageObject();

private:
    bool mStarted = false;
    bool mAmAlive = true;

    int mStandardAttackDamage = -1;
    std::vector<std::pair<float, int>> mAttackDamageModifiers;  // multiplier amount, num turns

    int mHealAmount = -1;
    int mCurHealth = -1;
    std::vector<std::pair<int, int>> mHealthModifiers;         // flat amount, num turns
    
    int mStandardHealthCapacity = -1;
    std::vector<std::pair<float, int>> mHealthCapacityModifiers; // multiplier amount, num turns

    int mCurDefense = 0;
    int mStandardDefenseCapacity = -1;
    std::vector<std::pair<float, int>> mDefenseCapacityModifiers; // multiplier amount, num turns

    int mTurnsToPass = 0;
public:

    CombatCharacter(Tile& curTile, const CombatCharacterPreset& preset);

    ~CombatCharacter() { ; }

    void start();

    void preTick();

    void postTick();

    void resetStats();


    bool isStarted() const;

    bool isAlive() const;


    int getCurDamage() const;
    
    int getBaseDamage()const ;

    void addDamageModifier(const float multiplier, const int numTurns);

    std::vector<std::pair<float, int>> getDamageModifiers() const;

    void takeDamage(const int damageToTake);


    int getCurHealth() const;

    int getCurHealthModifier() const;

    void addHealthModifier(const int amount, const int numTurns);

    std::vector<std::pair<int, int>> getHealthModifiers() const;


    int getCurHealthCapacity() const;

    int getBaseHealthCapacity() const;

    bool returnIsLowHealth() const;

    float getHealthRatio() const;

    void addHealthCapacityModifier(const float multiplier, const int numTurns);

    std::vector<std::pair<float, int>> getHealthCapacityModifiers() const;


    void heal(const int amount);

    void fullHeal();

    int getHealAmount() const; // flat rate character heals at


    int getCurDefense() const;

    int getCurDefenseCapacity() const;

    int getBaseDefenseCapacity() const;

    float getDefenseRatio() const;

    void defend();

    void addDefenseCapacityModifier(const float multiplier, const int numTurns);

    std::vector<std::pair<float, int>> getDefenseCapacityModifiers() const;


    int getStuns() const;

    void stun(int numTurnsStunned);


    void move(Tile* pTile);

    void revertToState(CombatCharacter& prevState);

private:
    void updateAmAlive();

    void updateModifiers(std::vector<std::pair<float, int>>& modifierlist);

    void updateModifiers(std::vector<std::pair<int, int>>& modifierlist);
};

struct SortCharacterByTileRow
{
    bool operator()(const CombatCharacter* const pChar1, const CombatCharacter* const pChar2) const;
};



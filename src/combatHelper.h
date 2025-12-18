#pragma once
#include <vector>
#include <SDL3/SDL.h>
#include "combatCharacterHelper.h"
#include "combatAttackHelper.h"
#include "combatHelperClass.h"
#include "tileHelperClass.h"

class CombatManager
{
public:
    std::vector <CombatCharacter*> mpCurCombatCharacters;
    std::vector <CombatCharacter*> mpAllCombatCharacters;

    CombatManager(){}

    ~CombatManager();

    void postTick();

    void createCurCharacterList();

    std::vector <CombatCharacter*> getCurCharactersThatCanPlay() const;

    std::vector <CombatCharacter*> getCurAliveCharacters() const;

    CombatCharacter* returnNextCharacter(CombatCharacter& curCharacter, bool preTick = true);

    CombatCharacter* returnNextCharacter(CombatCharacter& curCharacter, int& outIndex, bool preTick = true);

    int returnCharacterIndex(const CombatCharacter& givenCharacter) const;

    void tickAllAlive();


    void attack(CombatCharacter& attackingCharacter, Tile& givenTile, const Attack& attack);

    void specialEffect(CombatCharacter& attackingCharacter, CombatCharacter& attackedCharacter, Tile& givenTile, const Attack& attack);
    
    GameOverStats getGameOverStats();

    void resetStats();

private:
    CombatCharacter* getNextCharacter(const CombatCharacter& curCharacter);

    void preTickRange(int startIndex, int endIndex, bool tickLast);
};

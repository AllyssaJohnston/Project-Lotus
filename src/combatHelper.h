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
    std::vector <CombatCharacter*> mpCurAliveCombatCharacters;
    std::vector <CombatCharacter*> mpAllCombatCharacters;

    CombatManager(){}

    ~CombatManager();

    void postTick();

    void createCurAliveCharacterList();

    std::vector <CombatCharacter*> getCurAliveCharacters() const;

    CombatCharacter* returnNextAliveCharacter(CombatCharacter& curCharacter);

    CombatCharacter* returnNextAliveCharacter(CombatCharacter& curCharacter, int& outIndex);

    int getCharacterIndex(const CombatCharacter& givenCharacter) const;

    void tickAll();

    void attack(CombatCharacter& attackingCharacter, Tile& givenTile, const Attack& attack);

    void attackMultipleTiles(CombatCharacter& attackingCharacter, std::vector <Tile*>& pTilesToAttack, const Attack& attack);

    void specialEffect(CombatCharacter& attackingCharacter, CombatCharacter& attackedCharacter, Tile& givenTile, const Attack& attack);
    
    GameOverStats getGameOverStats();

    void resetStats();

private:
    CombatCharacter* getNextCharacter(const CombatCharacter& curCharacter);

    void preTickRange(int startIndex, int endIndex);
};

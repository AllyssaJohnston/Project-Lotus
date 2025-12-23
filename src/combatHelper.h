#pragma once
#include <vector>
#include <SDL3/SDL.h>
#include "combatCharacterHelper.h"
#include "combatAttackHelper.h"
#include "combatHelperClass.h"
#include "tileHelperClass.h"

class CombatManager
{
private:
    std::vector <CombatCharacter*> mpCurAliveCombatCharacters;
    std::vector <CombatCharacter*> mpAllCombatCharacters;
    int mRounds = 0;

    // int round number to spawn the list of characters
    std::map<int, std::vector<CombatCharacter*>> mpCharactersToSpawnInRound;

public:
    CombatManager() { ; }

    ~CombatManager();

    void postTick();

    void addCharacter(int roundNum, CombatCharacter* pCharacter);

    void createCurAliveCharacterList();

    std::vector <CombatCharacter*> getAllCharacters() const;

    CombatCharacter* getFromAllCharacters(int index) const;

    std::vector <CombatCharacter*> getCurAliveCharacters() const;

    std::vector <CombatCharacter*> getCurAlivePlayers() const;

    std::vector <CombatCharacter*> getCurAliveEnemies() const;

    CombatCharacter* returnNextAliveCharacter(CombatCharacter& curCharacter);

    CombatCharacter* returnNextAliveCharacter(CombatCharacter& curCharacter, int& outIndex);

    int getCharacterIndex(const CombatCharacter& givenCharacter) const;

    void tickAll();

    void attack(CombatCharacter& attackingCharacter, Tile& givenTile, const Attack& attack);

    void attack(CombatCharacter& attackingCharacter, CombatCharacter& attackedCharacter, const Attack& attack);

    void attackMultipleTiles(CombatCharacter& attackingCharacter, std::vector <Tile*>& pTilesToAttack, const Attack& attack);

    void specialEffect(CombatCharacter& attackingCharacter, CombatCharacter& attackedCharacter, const Attack& attack);
    
    bool characterOnTile(const Tile& tile);

    GameOverStats getGameOverStats();

    void resetStats();

private:
    CombatCharacter* getNextCharacter(const CombatCharacter& curCharacter) const;

    void preTickRange(int startIndex, int endIndex);
};

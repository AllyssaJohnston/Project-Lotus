#pragma once
#include <vector>
#include "combatCharacter.h"
#include "combatAttack.h"
#include "combatHelpers.h"

class CombatManager
{
private:
    std::vector <CombatCharacter*> mpCurAliveCombatCharacters;
    std::vector <CombatCharacter*> mpAllCombatCharacters;
    int mRounds = 0;

    const int mMaxCharactersInPlay = 5;

    // int round number to spawn the list of characters
    std::map<int, std::vector<CombatCharacter*>> mpCharactersToSpawnInRound;

public:
    CombatManager() { ; }

    ~CombatManager();

    // gets called when returning to the first alive player
    void endRound();

    // gets called each character switch
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
};

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
    std::vector <CombatCharacter*> mpCharctersSpawningNextRound;
    int mRounds = 0;

    const int mMaxCharactersInPlay = 5;

    // int round number to spawn the list of characters
    std::map<int, std::vector<CombatCharacter*>> mpCharactersToSpawnInRound;

public:
    CombatManager() { ; }

    ~CombatManager();

    // gets called when returning to the first alive player
    void endRound();

    // for undo
    void setRound(int round);

    // gets called each character switch
    void postTick();

    void addCharacter(int roundNum, CombatCharacter* pCharacter);

    void createCurAliveCharacterList();

    std::vector <CombatCharacter*> getAllCharacters() const;

    CombatCharacter* getFromAllCharacters(int index) const;

    std::vector <CombatCharacter*> getCurAliveCharacters() const;

    std::vector <CombatCharacter*> getAllPlayers() const;

    std::vector <CombatCharacter*> getCurAlivePlayers() const;

    std::vector <CombatCharacter*> getAllEnemies() const;

    std::vector <CombatCharacter*> getCurAliveEnemies() const;

    std::vector <CombatCharacter*> getGhostEnemies() const;

    CombatCharacter* returnNextAliveCharacter(CombatCharacter& curCharacter);

    CombatCharacter* returnNextAliveCharacter(CombatCharacter& curCharacter, int& outIndex);

    int getCharacterIndex(const CombatCharacter& givenCharacter) const;

    void tickAll();

    void attack(CombatCharacter& attackingCharacter, Tile& givenTile, Attack& attack);

    void attack(CombatCharacter& attackingCharacter, CombatCharacter& attackedCharacter, Attack& attack);

    void attackMultipleTiles(CombatCharacter& attackingCharacter, std::vector <Tile*>& pTilesToAttack, Attack& attack);


    std::vector<CombatCharacter> createCombatCharacterSnapShots();
    
    
    bool characterOnTile(const Tile& tile);

    int getRoundNum();

    GameOverStats getGameOverStats();

    void resetStats();

private:
    void attackInternal(CombatCharacter& attackingCharacter, CombatCharacter& attackedCharacter, const Attack& attack);

    // special effects that happen per attacked character/ tile
    void characterTileSpecialEffect(CombatCharacter& attackingCharacter, CombatCharacter& attackedCharacter, const Attack& attack);

    // special effects that happen once, per the attack
    void genericSpecialEffect(CombatCharacter& attackingCharacter, const Attack& attack);
};

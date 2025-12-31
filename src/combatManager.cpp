#include "combatManager.h"

CombatManager::~CombatManager()
{
    for (CombatCharacter* pCharacter : mpAllCombatCharacters)
    {
        delete pCharacter;
    }
    mpAllCombatCharacters.clear();
    mpCurAliveCombatCharacters.clear();
    mpCharctersSpawningNextRound.clear();
    mpCharactersToSpawnInRound.clear();
}

void CombatManager::endRound()
{
    mRounds++;
    if (mpCharactersToSpawnInRound[mRounds].size() > 0 || mpCharactersToSpawnInRound[mRounds + 1].size() > 0)
    {
        createCurAliveCharacterList();
    }
}

void CombatManager::setRound(int round) 
{
    mRounds = round;
    createCurAliveCharacterList();
}

void CombatManager::postTick()
{    
    // check if the cur alive character list needs to be updated
    for (CombatCharacter* pCharacter : mpCurAliveCombatCharacters)
    {
        if (pCharacter->getCurHealth() <= 0)
        {
            createCurAliveCharacterList();
            break;
        }
    }
}

void CombatManager::addCharacter(int roundNum, CombatCharacter* pCharacter)
{
    mpAllCombatCharacters.push_back(pCharacter);
    mpCharactersToSpawnInRound[roundNum].push_back(pCharacter);
}

void CombatManager::createCurAliveCharacterList()
{
    bool done = false;
    mpCurAliveCombatCharacters.clear();
    mpCharctersSpawningNextRound.clear();
    for (int i = 0; i <= mRounds; i++)
    {
        for (CombatCharacter* pCharacter : mpCharactersToSpawnInRound[i])
        {
            if (pCharacter->isAlive())
            {
                if (characterOnTile(*pCharacter->mCombatMovementManager.getCurTile()) || done)
                {
                    mpCharctersSpawningNextRound.push_back(pCharacter);
                    done = true;
                }
                else
                {
                    pCharacter->start();
                    mpCurAliveCombatCharacters.push_back(pCharacter);
                }

                if (mpCurAliveCombatCharacters.size() == mMaxCharactersInPlay)
                {
                    done = true;
                }
            }
        }
    }

    for (CombatCharacter* pCharacter : mpCharactersToSpawnInRound[mRounds + 1])
    {
        mpCharctersSpawningNextRound.push_back(pCharacter);
    }
}

std::vector <CombatCharacter*> CombatManager::getAllCharacters() const { return mpAllCombatCharacters; }

CombatCharacter* CombatManager::getFromAllCharacters(int index) const { return mpAllCombatCharacters[index]; }

std::vector <CombatCharacter*> CombatManager::getCurAliveCharacters() const { return mpCurAliveCombatCharacters; }

std::vector <CombatCharacter*> CombatManager::getCurAlivePlayers() const
{
    std::vector <CombatCharacter*> pCurAliveCombatPlayers;
    for (CombatCharacter* pCharacter : mpCurAliveCombatCharacters)
    {
        if (pCharacter->isAlive() && pCharacter->mType == ECombatCharacterType_PLAYER)
        {
            pCurAliveCombatPlayers.push_back(pCharacter);
        }
    }
    return pCurAliveCombatPlayers;
}

std::vector <CombatCharacter*> CombatManager::getAllPlayers() const
{
    std::vector <CombatCharacter*> pAllCombatPlayers;
    for (CombatCharacter* pCharacter : mpAllCombatCharacters)
    {
        if (pCharacter->mType == ECombatCharacterType_PLAYER)
        {
            pAllCombatPlayers.push_back(pCharacter);
        }
    }
    return pAllCombatPlayers;
}

std::vector <CombatCharacter*> CombatManager::getCurAliveEnemies() const
{
    std::vector <CombatCharacter*> pCurAliveCombatEnemies;
    for (CombatCharacter* pCharacter : mpCurAliveCombatCharacters)
    {
        if (pCharacter->isAlive() && pCharacter->mType == ECombatCharacterType_ENEMY)
        {
            pCurAliveCombatEnemies.push_back(pCharacter);
        }
    }
    return pCurAliveCombatEnemies;
}

std::vector <CombatCharacter*> CombatManager::getAllEnemies() const
{
    std::vector <CombatCharacter*> pAllCombatEnemies;
    for (CombatCharacter* pCharacter : mpAllCombatCharacters)
    {
        if (pCharacter->mType == ECombatCharacterType_ENEMY)
        {
            pAllCombatEnemies.push_back(pCharacter);
        }
    }
    return pAllCombatEnemies;
}

std::vector <CombatCharacter*> CombatManager::getGhostEnemies() const { return mpCharctersSpawningNextRound; }

CombatCharacter* CombatManager::returnNextAliveCharacter(CombatCharacter& curCharacter)
{
    int i = -1;
    return returnNextAliveCharacter(curCharacter, i);
}

// out all index is the index of the new character im the all character list
// find the next alive character, preticking and posticking characters as neccessary
CombatCharacter* CombatManager::returnNextAliveCharacter(CombatCharacter& curCharacter, int& outAllIndex)
{
    if (getCurAliveCharacters().size() == 0)
    {
        return nullptr;
    }
        
    // figure out the range between the old character and the new
    int indexOfOldCharacter = getCharacterIndex(curCharacter);
    mpAllCombatCharacters[indexOfOldCharacter]->postTick();

    int indexOfNewCharacter = indexOfOldCharacter + 1;
    if (indexOfNewCharacter >= mpAllCombatCharacters.size())
    {
        indexOfNewCharacter = 0;
    }

    while (!mpAllCombatCharacters[indexOfNewCharacter]->isAlive() || !mpAllCombatCharacters[indexOfNewCharacter]->isStarted())
    {
        mpAllCombatCharacters[indexOfNewCharacter]->preTick();
        mpAllCombatCharacters[indexOfNewCharacter]->postTick();
        indexOfNewCharacter++;
        // don't choose a character that is spawned yet
        if (indexOfNewCharacter >= mpAllCombatCharacters.size())
        {
            indexOfNewCharacter = 0;
        }
    }
    mpAllCombatCharacters[indexOfNewCharacter]->preTick();

    outAllIndex = indexOfNewCharacter;
    return mpAllCombatCharacters[indexOfNewCharacter];
}


int CombatManager::getCharacterIndex(const CombatCharacter& givenCharacter) const
{
    for (int count = 0; count < mpAllCombatCharacters.size(); count++)
    {
        if (mpAllCombatCharacters[count] == &givenCharacter)
        {
            return count;
        }
    }
    return -1;
}

void CombatManager::tickAll()
{
    for (CombatCharacter* pCharacter : mpAllCombatCharacters)
    {
        pCharacter->preTick();
    }
    createCurAliveCharacterList();
}

void CombatManager::attackMultipleTiles(CombatCharacter& attackingCharacter, std::vector <Tile*>& pTilesToAttack, Attack& characterAttack)
{
    for (Tile* pTile : pTilesToAttack)
    {
        for (CombatCharacter* pCurCharacter : mpCurAliveCombatCharacters)
        {
            if (pCurCharacter->mCombatMovementManager.getCurTile() == pTile)
            {
                attackInternal(attackingCharacter, *pCurCharacter, characterAttack);
                characterTileSpecialEffect(attackingCharacter, *pCurCharacter, characterAttack);
            }
        }
    }
    genericSpecialEffect(attackingCharacter, characterAttack);
    characterAttack.use();
}

void CombatManager::attack(CombatCharacter& attackingCharacter, Tile& givenTile, Attack& characterAttack)
{
    for (CombatCharacter* pCurCharacter : mpCurAliveCombatCharacters)
    {
        if (pCurCharacter->mCombatMovementManager.getCurTile() == &givenTile)
        {
            attackInternal(attackingCharacter, *pCurCharacter, characterAttack);
            characterTileSpecialEffect(attackingCharacter, *pCurCharacter, characterAttack);
        }
    }
    genericSpecialEffect(attackingCharacter, characterAttack);
    characterAttack.use();
}

void CombatManager::attack(CombatCharacter& attackingCharacter, CombatCharacter& attackedCharacter, Attack& characterAttack)
{
    attackInternal(attackingCharacter, attackedCharacter, characterAttack);
    characterTileSpecialEffect(attackingCharacter, attackedCharacter, characterAttack);
    genericSpecialEffect(attackingCharacter, characterAttack);
    characterAttack.use();
}

void CombatManager::attackInternal(CombatCharacter& attackingCharacter, CombatCharacter& attackedCharacter, const Attack& attack)
{
    if (!characterTypeFit(attack.mAttackTargetType, attack.mAttackTargetAlive, attackedCharacter.mType, attackedCharacter.isAlive()))
    {
        return;
    }
    int damageToTake = int(attackingCharacter.getCurDamage() * attack.mDamagePercent);
    if (damageToTake > 0 && attack.mDamageDistanceDependent)
    {
        Tile* pAttackingCharacterTile = attackingCharacter.mCombatMovementManager.getCurTile();
        float distance = getDistanceBetweenTiles(*attackedCharacter.mCombatMovementManager.getCurTile(), *pAttackingCharacterTile);

        damageToTake -= int(attack.mDamageDropOff * distance);
        damageToTake = std::max(damageToTake, 0);
    }
    attackedCharacter.takeDamage(damageToTake);
}

void CombatManager::characterTileSpecialEffect(CombatCharacter& attackingCharacter, CombatCharacter& attackedCharacter, const Attack& attack)
{
    std::vector<CombatCharacter*> pCharacters;
    for (const SpecialEffect& specialEffect : attack.mCharacterTileSpecialEffects)
    {
        if (!characterTypeFit(specialEffect.mAttackTargetType, attack.mAttackTargetAlive, attackedCharacter.mType, attackedCharacter.isAlive()))
        {
            continue;
        }

        switch (specialEffect.mType)
        {
        case ECombatSpecialEffectTypes_STUN:
            attackedCharacter.stun(specialEffect.mTurns);
            break;

        case ECombatSpecialEffectTypes_POISON:
            attackedCharacter.addHealthModifier(((int)specialEffect.mAmount), specialEffect.mTurns);
            break;

        case ECombatSpecialEffectTypes_HEAL:
            attackedCharacter.heal((int)specialEffect.mAmount);
            break;

        case ECombatSpecialEffectTypes_FULL_HEAL:
            attackedCharacter.fullHeal();
            break;
          
        default:
            SDL_assert(false);
            break;
        }
    }
}

void CombatManager::genericSpecialEffect(CombatCharacter& attackingCharacter, const Attack& attack)
{
    std::vector<CombatCharacter*> pCharacters;
    for (const SpecialEffect& specialEffect : attack.mGenericSpecialEffects)
    {
        switch (specialEffect.mType)
        {
        case ECombatSpecialEffectTypes_LOSE_TURN: // self stun
            attackingCharacter.stun(specialEffect.mTurns + 1);
            break;

        default:
            switch (specialEffect.mAttackTargetType)
            {
            case ECombatCharacterType_CHARACTER:
                pCharacters = getCurAliveCharacters();
                break;
            case ECombatCharacterType_PLAYER:
                pCharacters = getCurAlivePlayers();
                break;
            case ECombatCharacterType_ENEMY:
                pCharacters = getCurAliveEnemies();
                break;
            default:
                SDL_assert(false);
                break;
            }
            for (CombatCharacter* pCharacter : pCharacters)
            {
                switch (specialEffect.mType)
                {
                case ECombatSpecialEffectTypes_ATTACK_MULTIPLIER:
                    pCharacter->addDamageModifier(specialEffect.mAmount, specialEffect.mTurns);
                    break;
                case ECombatSpecialEffectTypes_DEFENSE_CAPACITY_MULTIPLIER:
                    pCharacter->addDefenseCapacityModifier(specialEffect.mAmount, specialEffect.mTurns);
                    break;
                default:
                    SDL_assert(false);
                    break;
                }
            }
            break;
        }
    }
}

// return copy of all characters
std::vector<CombatCharacter> CombatManager::createCombatCharacterSnapShots()
{
    std::vector<CombatCharacter> snapShots;
    for (CombatCharacter* pCharacter : mpAllCombatCharacters)
    {
        snapShots.push_back(*pCharacter);
    }
    return snapShots;
}


bool CombatManager::characterOnTile(const Tile& tile)
{
    for (CombatCharacter* pCurCharacterToTest : mpCurAliveCombatCharacters)
    {
        if (pCurCharacterToTest->mCombatMovementManager.getCurTile() == &tile)
        {
            // someone on this tile
            return true;
        }
    }
    return false;
}


int CombatManager::getRoundNum() { return mRounds; }

GameOverStats CombatManager::getGameOverStats()
{
    GameOverStats stats = GameOverStats();
    int numPlayers = 0;
    int numEnemies = 0;
    for (CombatCharacter* pCurCharacter : mpCurAliveCombatCharacters)
    {
        switch (pCurCharacter->mType)
        {
        case ECombatCharacterType_PLAYER:
            numPlayers += 1;
            break;
        case ECombatCharacterType_ENEMY:
            numEnemies += 1;
            break;
        default:
            SDL_assert(false);
            break;
        }
    }

    if (numPlayers == 0)
    {
        resetStats();
        stats.mGameOver = true;
        stats.mWonGame = false;
    }
    else if (numEnemies == 0)
    {
        resetStats();
        stats.mGameOver = true;
        stats.mWonGame = true;
    }
    return stats;
}

void CombatManager::resetStats()
{
    mRounds = 0;
    for (CombatCharacter* pCharacter : mpAllCombatCharacters)
    {
        pCharacter->resetStats();
    }
    createCurAliveCharacterList();
}
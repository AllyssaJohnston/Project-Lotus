#include "combatManager.h"

CombatManager::~CombatManager()
{
    for (CombatCharacter* pCharacter : mpAllCombatCharacters)
    {
        delete pCharacter;
    }
    mpAllCombatCharacters.clear();
    mpCurAliveCombatCharacters.clear();
}

void CombatManager::endRound()
{
    mRounds++;
    if (mpCharactersToSpawnInRound[mRounds].size() > 0)
    {
        createCurAliveCharacterList();
    }
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
    for (int i = 0; i <= mRounds; i++)
    {
        for (CombatCharacter* pCharacter : mpCharactersToSpawnInRound[i])
        {
            if (pCharacter->isAlive())
            {
                if (characterOnTile(*pCharacter->mCombatMovementManager.getCurTile()))
                {
                    done = true;
                    break;
                }
                else
                {
                    mpCurAliveCombatCharacters.push_back(pCharacter);
                }
            }
            if (mpCurAliveCombatCharacters.size() == mMaxCharactersInPlay)
            {
                done = true;
                break;
            }
        }
        if (done) 
        {
            break;
        }
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
        if (pCharacter->isAlive() && pCharacter->mType == EMiniGameCombatCharacterType_PLAYER)
        {
            pCurAliveCombatPlayers.push_back(pCharacter);
        }
    }
    return pCurAliveCombatPlayers;
}

std::vector <CombatCharacter*> CombatManager::getCurAliveEnemies() const
{
    std::vector <CombatCharacter*> pCurAliveCombatEnemies;
    for (CombatCharacter* pCharacter : mpCurAliveCombatCharacters)
    {
        if (pCharacter->isAlive() && pCharacter->mType == EMiniGameCombatCharacterType_ENEMY)
        {
            pCurAliveCombatEnemies.push_back(pCharacter);
        }
    }
    return pCurAliveCombatEnemies;
}

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

    int indexOfLastCharacterInPlay = getCharacterIndex(*mpCurAliveCombatCharacters[mpCurAliveCombatCharacters.size() - 1]);

    int indexOfNewCharacter = indexOfOldCharacter + 1;
    if (indexOfOldCharacter + 1 > indexOfLastCharacterInPlay)
    {
        indexOfNewCharacter = 0;
    }

    while (!mpAllCombatCharacters[indexOfNewCharacter]->isAlive()) 
    {
        mpAllCombatCharacters[indexOfNewCharacter]->preTick();
        mpAllCombatCharacters[indexOfNewCharacter]->postTick();
        indexOfNewCharacter++;
        // don't choose a character that is spawned yet
        if (indexOfOldCharacter + 1 > indexOfLastCharacterInPlay)
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

void CombatManager::attackMultipleTiles(CombatCharacter& attackingCharacter, std::vector <Tile*>& pTilesToAttack, const Attack& characterAttack)
{
    for (Tile* pTile : pTilesToAttack)
    {
        attack(attackingCharacter, *pTile, characterAttack);
    }
}

void CombatManager::attack(CombatCharacter& attackingCharacter, Tile& givenTile, const Attack& attack)
{
    for (CombatCharacter* pCurCharacter : mpCurAliveCombatCharacters)
    {
        if (pCurCharacter->mCombatMovementManager.getCurTile() == &givenTile)
        {
            int damageToTake = int(attackingCharacter.getCurDamage() * attack.mDamagePercent);
            if (damageToTake > 0 && attack.mDamageDistanceDependent)
            {
                Tile* pAttackingCharacterTile = attackingCharacter.mCombatMovementManager.getCurTile();
                float distance = getDistanceBetweenTiles(givenTile, *pAttackingCharacterTile);

                // TODO move magic number
                damageToTake -= int(.15f * distance);
                damageToTake = std::max(damageToTake, 0);
            }
            pCurCharacter->takeDamage(damageToTake);
            specialEffect(attackingCharacter, *pCurCharacter, attack);
            return;
        }
    }
}

void CombatManager::attack(CombatCharacter& attackingCharacter, CombatCharacter& attackedCharacter, const Attack& attack)
{
    int damageToTake = int(attackingCharacter.getCurDamage() * attack.mDamagePercent);
    if (damageToTake > 0 && attack.mDamageDistanceDependent)
    {
        Tile* pAttackingCharacterTile = attackingCharacter.mCombatMovementManager.getCurTile();
        float distance = getDistanceBetweenTiles(*attackedCharacter.mCombatMovementManager.getCurTile(), *pAttackingCharacterTile);

        // TODO move magic number
        damageToTake -= int(.15f * distance);
        damageToTake = std::max(damageToTake, 0);
    }
    attackedCharacter.takeDamage(damageToTake);
    specialEffect(attackingCharacter, attackedCharacter, attack);
}

void CombatManager::specialEffect(CombatCharacter& attackingCharacter, CombatCharacter& attackedCharacter, const Attack& attack)
{
    for (const SpecialEffect& specialEffect : attack.mSpecialEffects)
    {
        std::vector<CombatCharacter*> pCharacters;
        switch (specialEffect.mType)
        {
        case EMiniGameCombatSpecialEffectTypes_STUN:
            attackedCharacter.stun(specialEffect.mTurns);
            break;

        case EMiniGameCombatSpecialEffectTypes_LOSE_TURN: // self stun
            attackingCharacter.stun(specialEffect.mTurns + 1);
            break;

        case EMiniGameCombatSpecialEffectTypes_HEAL:
            switch (specialEffect.mAttackTargetType)
            {
            case EAttackTargetType_ALL_CHARACTERS:
                pCharacters = getCurAliveCharacters();
                break;
            case EAttackTargetType_ALL_PLAYERS:
                pCharacters = getCurAlivePlayers();
                break;
            case EAttackTargetType_ALL_ENEMIES:
                pCharacters = getCurAliveEnemies();
                break;
            case EAttackTargetType_SELF:
                pCharacters = { &attackingCharacter };
                break;
            case EAttackTargetType_ONE_PLAYER:
            case EAttackTargetType_ONE_ENEMY:
            case EAttackTargetType_ONE_CHARACTER:
                pCharacters = { &attackedCharacter };
                break;
            default:
                SDL_assert(false);
                break;
            }
            for (CombatCharacter* pCharacter : pCharacters)
            {
                if (specialEffect.mSpecial)
                {
                    pCharacter->fullHeal();
                }
                else
                {
                    pCharacter->heal((int)specialEffect.mAmount);
                }
                
            }
            break;

        case EMiniGameCombatSpecialEffectTypes_ATTACK_MULTIPLIER:
            switch (specialEffect.mAttackTargetType)
            {
            case EAttackTargetType_ALL_CHARACTERS:
                pCharacters = getCurAliveCharacters();
                break;
            case EAttackTargetType_ALL_PLAYERS:
                pCharacters = getCurAlivePlayers();
                break;
            case EAttackTargetType_ALL_ENEMIES:
                pCharacters = getCurAliveEnemies();
                break;
            case EAttackTargetType_SELF:
                pCharacters = { &attackingCharacter };
                break;
            default:
                SDL_assert(false);
                break;
            }
            for (CombatCharacter* pCharacter : pCharacters)
            {
                pCharacter->addDamageModifier(specialEffect.mAmount, specialEffect.mTurns);
            }
            break;
        case EMiniGameCombatSpecialEffectTypes_DEFENSE_CAPACITY_MULTIPLIER:
            switch (specialEffect.mAttackTargetType)
            {
            case EAttackTargetType_ALL_CHARACTERS:
                pCharacters = getCurAliveCharacters();
                break;
            case EAttackTargetType_ALL_PLAYERS:
                pCharacters = getCurAlivePlayers();
                break;
            case EAttackTargetType_ALL_ENEMIES:
                pCharacters = getCurAliveEnemies();
                break;
            case EAttackTargetType_SELF:
                pCharacters = { &attackingCharacter };
                break;
            default:
                SDL_assert(false);
                break;
            }
            for (CombatCharacter* pCharacter : pCharacters)
            {
                pCharacter->addDefenseCapacityModifier(specialEffect.mAmount, specialEffect.mTurns);
            }
            break;
        default:
            SDL_assert(false);
            break;
        }
    }
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

GameOverStats CombatManager::getGameOverStats()
{
    GameOverStats stats = GameOverStats();
    int numPlayers = 0;
    int numEnemies = 0;
    for (CombatCharacter* pCurCharacter : mpCurAliveCombatCharacters)
    {
        switch (pCurCharacter->mType)
        {
        case EMiniGameCombatCharacterType_PLAYER:
            numPlayers += 1;
            break;
        case EMiniGameCombatCharacterType_ENEMY:
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
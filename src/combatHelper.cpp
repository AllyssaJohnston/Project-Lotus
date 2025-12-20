#include "combatHelper.h"

CombatManager::~CombatManager()
{
    for (CombatCharacter* pCharacter : mpAllCombatCharacters)
    {
        delete pCharacter;
    }
    mpAllCombatCharacters.clear();
    mpCurCombatCharacters.clear();
}

void CombatManager::postTick()
{
    // check if the cur alive character list needs to be updated
    for (CombatCharacter* pCharacter : mpAllCombatCharacters)
    {
        if (pCharacter->getCurHealth() <= 0)
        {
            createCurCharacterList();
            break;
        }
    }
}


void CombatManager::createCurCharacterList()
{
    mpCurCombatCharacters.clear();
    for (CombatCharacter* pCharacter : mpAllCombatCharacters)
    {
        if (pCharacter->isAlive())
        {
            mpCurCombatCharacters.push_back(pCharacter);
        }
    }
}

std::vector <CombatCharacter*> CombatManager::getCurCharactersThatCanPlay() const
{
    std::vector <CombatCharacter*> pCurCombatCharactersThatCanPlay;
    for (CombatCharacter* pCharacter : mpAllCombatCharacters)
    {
        if (pCharacter->isAlive() && pCharacter->getStuns() == 0)
        {
            pCurCombatCharactersThatCanPlay.push_back(pCharacter);
        }
    }
    return pCurCombatCharactersThatCanPlay;
}

std::vector <CombatCharacter*> CombatManager::getCurAliveCharacters() const
{
    std::vector <CombatCharacter*> pCurAliveCombatCharacters;
    for (CombatCharacter* pCharacter : mpAllCombatCharacters)
    {
        if (pCharacter->isAlive())
        {
            pCurAliveCombatCharacters.push_back(pCharacter);
        }
    }
    return pCurAliveCombatCharacters;
}

CombatCharacter* CombatManager::returnNextCharacter(CombatCharacter& curCharacter, bool preTick)
{
    int i = -1;
    return returnNextCharacter(curCharacter, i, preTick);
}

// out all index is the index of the new character im the all character list
CombatCharacter* CombatManager::returnNextCharacter(CombatCharacter& curCharacter, int& outAllIndex, bool preTick)
{
    CombatCharacter* pNextCharacter = getNextCharacter(curCharacter);
    if (pNextCharacter == nullptr)
    {
        return nullptr;
    }
        
    // figure out the range between the old character and the new
    int indexOfOldCharInAllChars = 0;
    for (int count = 0; count < mpAllCombatCharacters.size(); count++)
    {
        if (mpAllCombatCharacters[count] == &curCharacter)
        {
            indexOfOldCharInAllChars = count;
        }
        if (mpAllCombatCharacters[count] == pNextCharacter)
        {
            outAllIndex = count;
        }
    }

    // pretick characters after the old character up to and including the new one
    preTickRange(indexOfOldCharInAllChars, outAllIndex, preTick);

    return pNextCharacter;
}


CombatCharacter* CombatManager::getNextCharacter(const CombatCharacter& curCharacter)
{
    std::vector <CombatCharacter*> curCharactersThatCanPlay = getCurCharactersThatCanPlay();
    if (curCharactersThatCanPlay.size() == 0)
    {
        return nullptr;
    }

    int indexOfCurCharacter = 0;
    CombatCharacter* pNextCharacter = nullptr;

    for (int count = 0; count < mpAllCombatCharacters.size(); count++)
    {
        if (mpAllCombatCharacters[count] == &curCharacter)
        {
            indexOfCurCharacter = count;
            break;
        }
    }

    if (indexOfCurCharacter + 1 >= curCharactersThatCanPlay.size())
    {
        return curCharactersThatCanPlay[0];
    }
    else
    {
        return curCharactersThatCanPlay[indexOfCurCharacter + 1];
    }
}

void CombatManager::preTickRange(int startIndex, int endIndex, bool tickLast)
{
    if (!tickLast) 
    { 
        endIndex--;  
    }

    // pretick characters after the old character up to and including the new one
    if (startIndex < endIndex)
    {
        for (int count = startIndex + 1; count <= endIndex; count++)
        {
            mpAllCombatCharacters[count]->preTick();
        }
    }
    else
    {
        for (int count = endIndex; count < mpAllCombatCharacters.size(); count++)
        {
            mpAllCombatCharacters[count]->preTick();
        }
        for (int count = 0; count < startIndex; count++)
        {
            mpAllCombatCharacters[count]->preTick();
        }
    }
}

int CombatManager::returnCharacterIndex(const CombatCharacter& givenCharacter) const
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
    createCurCharacterList();
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
    for (CombatCharacter* pCurCharacter : mpCurCombatCharacters)
    {
        if (pCurCharacter != &attackingCharacter and (pCurCharacter->mCombatMovementManager.getCurTile()->mRow == givenTile.mRow) 
                    and  (pCurCharacter->mCombatMovementManager.getCurTile()->mCol == givenTile.mCol))
        {
            int damageToTake = int(attackingCharacter.getCurDamage() * attack.mDamagePercent);
            if (attack.mDamageDistanceDependent)
            {
                Tile* pCurCharacterTile = pCurCharacter->mCombatMovementManager.getCurTile();
                Tile* pAttackingCharacterTile = attackingCharacter.mCombatMovementManager.getCurTile();
                int distance = getDistanceBetweenTiles(*pCurCharacterTile, *pAttackingCharacterTile);

                damageToTake -= int(.15f * distance);
                if (damageToTake < 0)
                {
                    damageToTake = 0;
                }
            }
            pCurCharacter->takeDamage(damageToTake);
            specialEffect(attackingCharacter, *pCurCharacter, givenTile, attack);
            return;
        }
    }
}

void CombatManager::specialEffect(CombatCharacter& attackingCharacter, CombatCharacter& attackedCharacter, Tile& givenTile, const Attack& attack)
{
    for (const SpecialEffect& specialEffect : attack.mSpecialEffects)
    {
        switch (specialEffect.mType)
        {
        case EMiniGameCombatSpecialEffectTypes_STUN:
            attackedCharacter.stun(specialEffect.mTurns);
            break;

        case EMiniGameCombatSpecialEffectTypes_LOSE_TURN: // self stun
            attackingCharacter.stun(specialEffect.mTurns);
            break;

        case EMiniGameCombatSpecialEffectTypes_ATTACK_MULTIPLIER:
            switch (specialEffect.mAttackTargetType)
            {
            case EAttackTargetType_ALL_CHARACTERS:
                for (CombatCharacter* pCharacter : mpCurCombatCharacters)
                {
                    pCharacter->addDamageModifier(specialEffect.mAmount, specialEffect.mTurns);
                }
                break;
            case EAttackTargetType_ALL_PLAYERS:
                for (CombatCharacter* pCharacter : mpCurCombatCharacters)
                {
                    if (pCharacter->mType == EMiniGameCombatCharacterType_PLAYER)
                    {
                        pCharacter->addDamageModifier(specialEffect.mAmount, specialEffect.mTurns);
                    }
                }
                break;
            case EAttackTargetType_ALL_ENEMIES:
                for (CombatCharacter* pCharacter : mpCurCombatCharacters)
                {
                    if (pCharacter->mType == EMiniGameCombatCharacterType_ENEMY)
                    {
                        pCharacter->addDamageModifier(specialEffect.mAmount, specialEffect.mTurns);
                    }
                }
                break;
            default:
                SDL_assert(false);
                break;
            }
            break;
        default:
            SDL_assert(false);
            break;
        }
    }
}

GameOverStats CombatManager::getGameOverStats()
{
    GameOverStats stats = GameOverStats();
    int numPlayers = 0;
    int numEnemies = 0;
    for (CombatCharacter* pCurCharacter : mpCurCombatCharacters)
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
    for (CombatCharacter* pCharacter : mpAllCombatCharacters)
    {
        pCharacter->resetStats();
    }
    createCurCharacterList();
}
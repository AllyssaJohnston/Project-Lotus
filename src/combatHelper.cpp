#include "combatHelper.h"

CombatManager::~CombatManager()
{
    for (CombatCharacter* pCharacter : mpAllCombatCharacters)
    {
        pCharacter = nullptr;
    }
}

void CombatManager::preTick()
{
    for (CombatCharacter* pCharacter : mpAllCombatCharacters)
    {
        pCharacter->preTick();
    }
}

void CombatManager::postTick()
{
    for (CombatCharacter* pCharacter : mpAllCombatCharacters)
    {
        if (pCharacter->mCurHealth <= 0)
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
        if (pCharacter->mAmAlive)
        {
            mpCurCombatCharacters.push_back(pCharacter);
        }
    }
}

std::vector <CombatCharacter*> CombatManager::getCurCharactersThatCanPlay() const
{
    std::vector <CombatCharacter*> curCombatCharactersThatCanPlay;
    for (CombatCharacter* pCharacter : mpAllCombatCharacters)
    {
        if (pCharacter->mTurnsToPass == 0)
        {
            curCombatCharactersThatCanPlay.push_back(pCharacter);
        }
    }
    return curCombatCharactersThatCanPlay;
}

CombatCharacter* CombatManager::returnNextCharacter(CombatCharacter* pCurCharacter)
{
    int i = -1;
    return returnNextCharacter(pCurCharacter, i);
}

CombatCharacter* CombatManager::returnNextCharacter(CombatCharacter* pCurCharacter, int& outIndex)
{
    std::vector <CombatCharacter*> curCharactersThatCanPlay = getCurCharactersThatCanPlay();
    if (curCharactersThatCanPlay.size() == 0)
    {
        SDL_assert(false);
        return nullptr;
    }
    else
    {
        int indexOfCurCharacter = 0;
        CombatCharacter* pNextCharacter = nullptr;

        for (int count = 0; count < curCharactersThatCanPlay.size(); count++)
        {
            if (curCharactersThatCanPlay[count] == pCurCharacter)
            {
                indexOfCurCharacter = count;
            }
        }
        if (indexOfCurCharacter + 1 < curCharactersThatCanPlay.size())
        {
            pNextCharacter = curCharactersThatCanPlay[indexOfCurCharacter + 1];
            outIndex = indexOfCurCharacter + 1;
        }
        else
        {
            pNextCharacter = curCharactersThatCanPlay[0];
            outIndex = 0;
        }
        updateCharacterPostTurn(pCurCharacter, pNextCharacter);
        return pNextCharacter;
    }
}


int CombatManager::returnCharacterIndex(CombatCharacter* pGivenCharacter) const
{
    for (int count = 0; count < mpAllCombatCharacters.size(); count++)
    {
        if (mpAllCombatCharacters[count] == pGivenCharacter)
        {
            return count;
        }
    }
    return -1;
}

void CombatManager::updateAllCharactersPostTurn()
{
    for(CombatCharacter* pCharacter : mpCurCombatCharacters)
    {
        pCharacter->postRound();
    }
}

void CombatManager::updateCharacterPostTurn(CombatCharacter* pOldCharacter, CombatCharacter* pNextCharacter)
{
    int oldCharacterIndex = std::numeric_limits<int>::max();
    int nextCharacterIndex = std::numeric_limits<int>::max();

    for (int count = 0; count < mpCurCombatCharacters.size(); count++)
    {
        CombatCharacter * pCurCharacter = mpCurCombatCharacters[count];
        if (pCurCharacter == pOldCharacter)
        {
            oldCharacterIndex = count;
        }
        if (pCurCharacter == pNextCharacter)
        {
            nextCharacterIndex = count;
        }
    }

    for (int count = oldCharacterIndex + 1; count < mpCurCombatCharacters.size(); count++)
    {
        CombatCharacter * pCurCharacter = mpCurCombatCharacters[count];
        pCurCharacter->postRound();
    }

    if (nextCharacterIndex < oldCharacterIndex)
    {
        for (int count = 0; count < nextCharacterIndex + 1; count++)
        {
            CombatCharacter * pCurCharacter = mpCurCombatCharacters[count];
            pCurCharacter->postRound();
        }
    }
}

//return attackedCharacterChanges
void CombatManager::attack(CombatCharacter* pAttackingCharacter, Tile* pGivenTile, const Attack& attack)
{
    for (CombatCharacter* pCurCharacter : mpCurCombatCharacters)
    {
        if (pCurCharacter != pAttackingCharacter)
        {
            if          ((pCurCharacter->mCombatMovementManager.getCurTile()->mRow == pGivenTile->mRow) 
                    and  (pCurCharacter->mCombatMovementManager.getCurTile()->mCol == pGivenTile->mCol))
            {
                int damageToTake = int(pAttackingCharacter->mCurAttackDamage * attack.mDamagePercent);
                if (attack.mDamageDistanceDependent)
                {
                    Tile* pCurCharacterTile = pCurCharacter->mCombatMovementManager.getCurTile();
                    Tile* pAttackingCharacterTile = pAttackingCharacter->mCombatMovementManager.getCurTile();
                    int distance = getDistanceBetweenTiles(pCurCharacterTile, pAttackingCharacterTile);

                    damageToTake -= int(.15f * distance);
                    if (damageToTake < 0)
                    {
                        damageToTake = 0;
                    }
                }
                pCurCharacter->takeDamage(damageToTake);
                //specialEffect(gameInstance, pAttackingCharacter, pCurCharacter, attack.mSpecialEffects);
                /*if (pCurCharacter->mCurHealth <= 0)
                {
                characterDied();
                }*/
            }
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
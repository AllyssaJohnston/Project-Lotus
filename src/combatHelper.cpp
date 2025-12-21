#include "combatHelper.h"

CombatManager::~CombatManager()
{
    for (CombatCharacter* pCharacter : mpAllCombatCharacters)
    {
        delete pCharacter;
    }
    mpAllCombatCharacters.clear();
    mpCurAliveCombatCharacters.clear();
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


void CombatManager::createCurAliveCharacterList()
{
    mpCurAliveCombatCharacters.clear();
    for (CombatCharacter* pCharacter : mpAllCombatCharacters)
    {
        if (pCharacter->isAlive())
        {
            mpCurAliveCombatCharacters.push_back(pCharacter);
        }
    }
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

std::vector <CombatCharacter*> CombatManager::getCurAlivePlayers() const
{
    std::vector <CombatCharacter*> pCurAliveCombatCharacters;
    for (CombatCharacter* pCharacter : mpAllCombatCharacters)
    {
        if (pCharacter->isAlive() && pCharacter->mType == EMiniGameCombatCharacterType_PLAYER)
        {
            pCurAliveCombatCharacters.push_back(pCharacter);
        }
    }
    return pCurAliveCombatCharacters;
}

std::vector <CombatCharacter*> CombatManager::getCurAliveEnemies() const
{
    std::vector <CombatCharacter*> pCurAliveCombatCharacters;
    for (CombatCharacter* pCharacter : mpAllCombatCharacters)
    {
        if (pCharacter->isAlive() && pCharacter->mType == EMiniGameCombatCharacterType_ENEMY)
        {
            pCurAliveCombatCharacters.push_back(pCharacter);
        }
    }
    return pCurAliveCombatCharacters;
}

CombatCharacter* CombatManager::returnNextAliveCharacter(CombatCharacter& curCharacter)
{
    int i = -1;
    return returnNextAliveCharacter(curCharacter, i);
}

// out all index is the index of the new character im the all character list
CombatCharacter* CombatManager::returnNextAliveCharacter(CombatCharacter& curCharacter, int& outAllIndex)
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
    preTickRange(indexOfOldCharInAllChars, outAllIndex);

    return pNextCharacter;
}


CombatCharacter* CombatManager::getNextCharacter(const CombatCharacter& curCharacter)
{
    std::vector <CombatCharacter*> curCharactersThatCanPlay = getCurAliveCharacters();
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

void CombatManager::preTickRange(int startIndex, int endIndex)
{
    // posttick cur character up to the start of the new oen
    // pretick characters after the old character up to and including the new one
    if (startIndex < endIndex)
    {
        mpAllCombatCharacters[startIndex]->postTick();
        for (int count = startIndex + 1; count < endIndex; count++)
        {
            mpAllCombatCharacters[count]->preTick();
            mpAllCombatCharacters[count]->postTick();
        }
        mpAllCombatCharacters[endIndex]->preTick();
    }
    else
    {
        mpAllCombatCharacters[startIndex]->postTick();
        for (int count = startIndex + 1; count < mpAllCombatCharacters.size(); count++)
        {
            mpAllCombatCharacters[count]->preTick();
            mpAllCombatCharacters[count]->postTick();
        }
        for (int count = 0; count < endIndex; count++)
        {
            mpAllCombatCharacters[count]->preTick();
            mpAllCombatCharacters[count]->postTick();
        }
        mpAllCombatCharacters[endIndex]->preTick();
    }
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
    for (CombatCharacter* pCharacter : mpAllCombatCharacters)
    {
        pCharacter->resetStats();
    }
    createCurAliveCharacterList();
}
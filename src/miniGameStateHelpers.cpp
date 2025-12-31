#include "miniGameStateHelpers.h"


MiniGameStateData::~MiniGameStateData()
{
	mpCharacter = nullptr;
	mpTargetCharacter = nullptr;
	mpTileToMoveTo = nullptr;
	mpTileLastMovedTo = nullptr;
	for (Tile* pTile : mpTilesToAttack)
	{
		pTile = nullptr;
	}
	mpTilesToAttack.clear();
}

CombatCharacter* MiniGameStateData::getCharacter() const { return mpCharacter; }

int MiniGameStateData::getCharacterIndex() const { return mCharacterIndex; }

void MiniGameStateData::setCharacter(CombatCharacter* pCharacter, int index)
{
	mpCharacter = pCharacter;
	mCharacterIndex = index;
}

void MiniGameStateData::reset()
{
	mpCharacter = nullptr;
	mCharacterIndex = -1;
	mpTileToMoveTo = nullptr;
	mpTilesToAttack.clear();
	mpCurAttack = nullptr;
	mCurAttackDirection = EDirection_INVALID;
	mGoingToAttack = false;
	mNextMiniGameState = EMiniGameState_INVALID;
	mPostBufferGameState = EMiniGameState_INVALID;

	mTicks = 0;

	mTickYet = false;
}




std::vector <Tile*> returnTilesFromAttackWithPlayersOnThem(const MiniGameWorldData& worldData, const Tile* const pReferenceTile, const Attack& curAttack, EDirection direction)
{
	std::vector <Tile*> pTilesWithPlayers;
	MiniGameStage* pStage = worldData.getStage();
	Grid& grid = pStage->mGrid;
	CombatManager& combatManager = pStage->mCombatManager;

	if (!curAttack.canUse())
	{
		return pTilesWithPlayers;
	}

	if (curAttack.mType == ECombatActionGridPattern_WHOLE_GRID)
	{
		// just return the tiles with players
		for (CombatCharacter* pCurCharacterToTest : combatManager.getCurAliveCharacters())
		{
			if (pCurCharacterToTest->mType == ECombatCharacterType_PLAYER)
			{
				pTilesWithPlayers.push_back(pCurCharacterToTest->mCombatMovementManager.getCurTile());
			}
		}
	}
	else
	{
		for (TileCoords& curTileCoord : returnTileCoords(*pReferenceTile, curAttack.mType, curAttack.mNum, curAttack.mOut, direction))
		{
			Tile* pCurAttackTile = grid.findTile(curTileCoord);
			if (pCurAttackTile == nullptr)
			{
				continue;
			}
			for (CombatCharacter* pCurCharacterToTest : combatManager.getCurAliveCharacters())
			{
				if (pCurCharacterToTest->mType == ECombatCharacterType_PLAYER && pCurCharacterToTest->mCombatMovementManager.getCurTile() == pCurAttackTile)
				{
					pTilesWithPlayers.push_back(pCurAttackTile);
				}
			}
		}
	}
	return pTilesWithPlayers;
}

std::vector <Tile*> returnTilesFromAttacksWithPlayersOnThem(const MiniGameWorldData& worldData, const Tile* const pReferenceTile, const std::vector<Attack>& attacks, const EDirection direction)
{
	std::vector <Tile*> pTilesWithPlayers;
	MiniGameStage* pStage = worldData.getStage();
	Grid& grid = pStage->mGrid;
	CombatManager& combatManager = pStage->mCombatManager;

	for (const Attack& curAttack : attacks)
	{
		if (!curAttack.canUse())
		{
			continue;
		}
		for (Tile* pCurTile : returnTilesFromAttackWithPlayersOnThem(worldData, pReferenceTile, curAttack, direction))
		{
			if (std::find(pTilesWithPlayers.begin(), pTilesWithPlayers.end(), pCurTile) == pTilesWithPlayers.end())
			{
				pTilesWithPlayers.push_back(pCurTile);
			}
		}
	}
	return pTilesWithPlayers;
}





bool tileInAttackRange(const Attack& attack, const EDirection attackDirection, const Grid& grid, const Tile* const pGivenTile, const Tile* const pTileToAttackFrom)
{
	if (attack.mType == ECombatActionGridPattern_WHOLE_GRID)
	{
		return true;
	}
	for (TileCoords& coords : returnTileCoords(*pTileToAttackFrom, attack.mType, attack.mNum, attack.mOut, (attack.mNumTilesToAttack == ECombatNumTilesToAttack_DIRECTION ? attackDirection : EDirection_ALL)))
	{
		Tile* pTile = grid.findTile(coords);
		if (pTile != nullptr && pTile == pGivenTile)
		{
			return true;
		}
	}
	return false;
}


std::vector <Tile*> returnTilesWithoutCharacters(const CombatManager& combatManager, const std::vector <Tile*>& listOfTiles)
{
	std::vector <Tile*> pTilesWithoutCharacters = listOfTiles;
	for (int countTile = (int)pTilesWithoutCharacters.size() - 1; countTile > -1; countTile--)
	{
		Tile* pCurTile = pTilesWithoutCharacters[countTile];
		for (CombatCharacter* pCurCharacter : combatManager.getCurAliveCharacters())
		{
			if (pCurCharacter->mCombatMovementManager.getCurTile() == pCurTile)
			{
				pTilesWithoutCharacters.erase(listOfTiles.begin() + countTile);
			}
		}
	}
	return pTilesWithoutCharacters;
}


std::string getCharacterChangesString(const CombatManager& combatManager, const std::vector<CombatCharacter>& preTickCharacters)
{
	std::string line = "";
	const std::vector<CombatCharacter*> pCurAliveCharacters = combatManager.getAllCharacters();
	for (int i = 0; i < (int)pCurAliveCharacters.size(); i++)
	{
		std::string curLine = "";
		const CombatCharacter* pCharacter = pCurAliveCharacters[i];
		const CombatCharacter& preTickCharacter = preTickCharacters[i];

		int curHealth = pCharacter->getCurHealth();
		int prevHealth = preTickCharacter.getCurHealth();
		if (pCharacter->isAlive() && preTickCharacter.isAlive())
		{
			
			int curDefense = pCharacter->getCurDefense();
			int prevDefense = preTickCharacter.getCurDefense();
			if (curHealth > prevHealth) // healed
			{
				curLine += pCharacter->mName + " healed " + std::to_string(curHealth - prevHealth) + " ";
			}
			else if (curHealth < prevHealth)
			{
				if (curDefense < prevDefense) // took damage and lost defense
				{
					curLine += pCharacter->mName + " lost " + std::to_string(prevDefense - curDefense) + " defense and took " + std::to_string(prevHealth - curHealth) + " damage";
				}
				else // just took damage
				{
					curLine += pCharacter->mName + " took " + std::to_string(prevHealth - curHealth) + " damage ";
				}
			}
			else if (curDefense < prevDefense) // lost defense but not health
			{
				curLine += pCharacter->mName + " lost " + std::to_string(prevDefense - curDefense) + " defense ";
			}
			else if (curDefense > prevDefense) // defended
			{
				curLine += pCharacter->mName + " gained " + std::to_string(curDefense - prevDefense) + " defense ";
			}

			int prevHealthModifier = preTickCharacter.getCurHealthModifier();
			int curHealthModifier = pCharacter->getCurHealthModifier();
			if (curHealthModifier < 0 && curHealthModifier != prevHealthModifier)
			{
				curLine += pCharacter->mName + "'s is posioned " + std::to_string(curHealthModifier) + " ";
			}
			else if (curHealthModifier > 0 && curHealthModifier != prevHealthModifier)
			{
				curLine += pCharacter->mName + "'s is healing " + std::to_string(curHealthModifier) + " each turn";
			}

			int baseHealthCapacity = pCharacter->getBaseHealthCapacity();
			int prevBaseHealthCapacity = preTickCharacter.getBaseHealthCapacity();
			int curHealthCapacity = pCharacter->getCurHealthCapacity();
			int prevCurHealthCapacity = preTickCharacter.getBaseHealthCapacity();
			if (baseHealthCapacity < prevBaseHealthCapacity)
			{
				curLine += pCharacter->mName + "'s total health capacity permanently dropped to " + std::to_string(baseHealthCapacity) + " ";
			}
			else if (baseHealthCapacity > prevBaseHealthCapacity)
			{
				curLine += pCharacter->mName + "'s total health capacity permanently increased to " + std::to_string(baseHealthCapacity) + " ";
			}
			else if (curHealthCapacity < prevCurHealthCapacity)
			{
				curLine += pCharacter->mName + "'s total health capacity dropped to " + std::to_string(curHealthCapacity) + " ";
			}
			else if (curHealthCapacity > prevCurHealthCapacity)
			{
				curLine += pCharacter->mName + "'s total health capacity increased to " + std::to_string(curHealthCapacity) + " ";
			}

			int baseDefenseCapacity = pCharacter->getBaseDefenseCapacity();
			int prevBaseDefenseCapacity = preTickCharacter.getBaseDefenseCapacity();
			int curDefenseCapacity = pCharacter->getCurDefenseCapacity();
			int prevDefenseCapacity = preTickCharacter.getCurDefenseCapacity();
			if (baseDefenseCapacity < prevBaseDefenseCapacity)
			{
				curLine += pCharacter->mName + "'s total defense capacity permanently dropped to " + std::to_string(baseDefenseCapacity) + " ";
			}
			else if (baseDefenseCapacity > prevBaseDefenseCapacity)
			{
				curLine += pCharacter->mName + "'s total defense capacity permanently increased to " + std::to_string(baseDefenseCapacity) + " ";
			}
			else if (curDefenseCapacity < prevDefenseCapacity)
			{
				curLine += pCharacter->mName + "'s total defense capacity dropped to " + std::to_string(curDefenseCapacity) + " ";
			}
			else if (curDefenseCapacity > prevDefenseCapacity)
			{
				curLine += pCharacter->mName + "'s total defense capacity increased to " + std::to_string(curDefenseCapacity) + " ";
			}

			int baseDamage = pCharacter->getBaseDamage();
			int prevBaseDamage = preTickCharacter.getBaseDamage();
			int curDamage = pCharacter->getCurDamage();
			int prevCurDamage = preTickCharacter.getCurDamage();
			if (baseDamage < prevBaseDamage)
			{
				curLine += pCharacter->mName + "'s total attack damage permanently dropped to " + std::to_string(baseDamage) + " ";
			}
			else if (baseDamage > prevBaseDamage)
			{
				curLine += pCharacter->mName + "'s total attack damage permanently increased to " + std::to_string(baseDamage) + " ";
			}
			else if (curDamage < prevCurDamage)
			{
				curLine += pCharacter->mName + "'s total attack damage dropped to " + std::to_string(curDamage) + " ";
			}
			else if (curDamage > prevCurDamage)
			{
				curLine += pCharacter->mName + "'s total attack damage increased to " + std::to_string(curDamage) + " ";
			}

			int stuns = pCharacter->getStuns();
			int prevStuns = preTickCharacter.getStuns();
			if (stuns > 0 && prevStuns == 0)
			{
				curLine += pCharacter->mName + " lost " + std::to_string(stuns) + (stuns == 1 ? " turn " : " turns ");
			}

		}
		else if (pCharacter->isAlive() && !preTickCharacter.isAlive())
		{
			curLine += pCharacter->mName + " spawned with " + std::to_string(curHealth);
		}
		else if (!pCharacter->isAlive() && preTickCharacter.isAlive())
		{
			curLine += pCharacter->mName + " took " + std::to_string(prevHealth - curHealth) + " damage and died";
		}

		if (curLine != "")
		{
			if (line != "") 
			{
				line += ". ";
			}
			line += curLine;
		}
	}
	return line;
}



void setUpForBufferState(const MiniGameWorldData& worldData, MiniGameStateData& data)
{
	CombatManager& combatManager = worldData.getStage()->mCombatManager;
	combatManager.postTick();
	int oldIndex = data.getCharacterIndex();
	int index = -1;
	CombatCharacter* pNextCharacter = combatManager.returnNextAliveCharacter(*data.getCharacter(), index);
	data.setCharacter(pNextCharacter, index);
	if (index < oldIndex)
	{
		// wrapped back around to the start, ended a round
		combatManager.endRound();
	}

	data.mPostBufferGameState = getPostBufferState(*pNextCharacter);
	data.mNextMiniGameState = EMiniGameState_BUFFER;
	data.mGoingToAttack = false;
	data.mpTilesToAttack.clear();
	data.mTicks = 0;
}

EMiniGameState getPostBufferState(const CombatCharacter& character)
{
	if (character.getStuns() > 0)
	{
		return EMiniGameState_CHARACTER_STUNNED;
	}
	switch (character.mType)
	{
	case ECombatCharacterType_PLAYER:
		return EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT;
	case ECombatCharacterType_ENEMY:
		return EMiniGameState_ENEMY_MOVE_CHARACTER;
	default:
		SDL_assert(false);
		return EMiniGameState_INVALID;
	}
}
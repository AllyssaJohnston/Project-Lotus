#include "miniGameStateHelperClass.h"




CombatCharacterSnapShot::CombatCharacterSnapShot(CombatCharacter* pCharacter) : mpCharacter(pCharacter)
{
	mAmAlive					= mpCharacter->isAlive();

	mCurDamage					= mpCharacter->getCurDamage();
	mAttackDamageModifiers		= mpCharacter->getDamageModifiers();

	mCurHealth					= mpCharacter->getCurHealth();
	mCurHealthCapacity			= mpCharacter->getCurHealthCapacity();
	mHealthCapacityModifiers	= mpCharacter->getHealthCapacityModifiers();
	

	mCurDefense					= mpCharacter->getCurDefense();
	mCurDefenseCapacity			= mpCharacter->getCurDefenseCapacity();
	mDefenseCapacityModifiers	= mpCharacter->getDefenseCapacityModifiers();
	
	mTurnsToPass				= mpCharacter->getStuns();

	mpTile						= mpCharacter->mCombatMovementManager.getCurTile();
}

CombatCharacterSnapShot::~CombatCharacterSnapShot()
{
	mpCharacter = nullptr;
	mpTile = nullptr;
}



MiniGameStateData::~MiniGameStateData()
{
	mpCharacter = nullptr;
	mpTileToMoveTo = nullptr;
	mpTileToAttack = nullptr;
	for (Tile* tile : mpTilesToAttack)
	{
		tile = nullptr;
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
	mpTileToAttack = nullptr;
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

	if (curAttack.mType == EMiniGameCombatMoveAttackTypes_WHOLE_GRID)
	{
		// just return the tiles with players
		for (CombatCharacter* pCurCharacterToTest : combatManager.mpCurCombatCharacters)
		{
			if (pCurCharacterToTest->mType == EMiniGameCombatCharacterType_PLAYER)
			{
				pTilesWithPlayers.push_back(pCurCharacterToTest->mCombatMovementManager.getCurTile());
			}
		}
	}
	else
	{
		for (TileCoords& curTileCoord : returnTileCoords(*pReferenceTile, curAttack.mType, direction))
		{
			Tile* pCurAttackTile = findTile(grid, curTileCoord);
			if (pCurAttackTile == nullptr)
			{
				continue;
			}
			for (CombatCharacter* pCurCharacterToTest : combatManager.mpCurCombatCharacters)
			{
				if (pCurCharacterToTest->mType == EMiniGameCombatCharacterType_PLAYER && pCurCharacterToTest->mCombatMovementManager.getCurTile() == pCurAttackTile)
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




bool tileInAttackRange(const Attack& attack, EDirection attackDirection, Grid& grid, Tile* pGivenTile, Tile* pTileToAttackFrom)
{
	for (TileCoords& coords : returnTileCoords(*pTileToAttackFrom, attack.mType, (attack.mRequiresDirectionInput ? attackDirection : EDirection_ALL)))
	{
		Tile* pTile = findTile(grid, coords);
		if (pTile != nullptr && pTile == pGivenTile)
		{
			return true;
		}
	}
	return false;
}

bool characterOnTile(const Tile& tile, const std::vector <CombatCharacter*>& pCharacters)
{
	for (CombatCharacter* pCurCharacterToTest : pCharacters)
	{
		if (pCurCharacterToTest->mCombatMovementManager.getCurTile()->mRow == tile.mRow and pCurCharacterToTest->mCombatMovementManager.getCurTile()->mCol == tile.mCol)
		{
			// someone on this tile
			return true;
		}
	}
	return false;
}

std::vector <Tile*> returnTilesWithoutCharacters(const CombatManager& combatManager, const std::vector <Tile*>& listOfTiles)
{
	std::vector <Tile*> tilesWithoutCharacters = listOfTiles;
	for (int countTile = (int)tilesWithoutCharacters.size() - 1; countTile > -1; countTile--)
	{
		Tile* pCurTile = tilesWithoutCharacters[countTile];
		for (CombatCharacter* pCurCharacter : combatManager.mpCurCombatCharacters)
		{
			if (pCurCharacter->mCombatMovementManager.getCurTile()->mRow == pCurTile->mRow and pCurCharacter->mCombatMovementManager.getCurTile()->mCol == pCurTile->mCol)
			{
				tilesWithoutCharacters.erase(listOfTiles.begin() + countTile);
			}
		}
	}
	return tilesWithoutCharacters;
}

std::vector <TileDistance> returnListOfTileDistances(std::vector <CombatCharacter*>& pCurCombatCharacters, std::vector <Tile*>& pTiles, CombatCharacter* pCurEnemy)
{
	std::vector <TileDistance> tileDistances;
	for (CombatCharacter* pCurCharacter : pCurCombatCharacters)
	{
		if (pCurCharacter->mType == EMiniGameCombatCharacterType_PLAYER)
		{
			for (Tile* pCurTile : pTiles)
			{
				int distanceRow = abs(pCurTile->mRow - pCurCharacter->mCombatMovementManager.getCurTile()->mRow);
				int distanceCol = abs(pCurTile->mCol - pCurCharacter->mCombatMovementManager.getCurTile()->mCol);
				float distance = (float)sqrt(pow(distanceRow, 2) + pow(distanceCol, 2));
				tileDistances.push_back(TileDistance(pCurTile, pCurEnemy, pCurCharacter, distance));
			}
		}
	}
	return tileDistances;
}



std::vector<CombatCharacterSnapShot> createCombatCharacterSnapShots(const CombatManager& combatManager) 
{
	std::vector<CombatCharacterSnapShot> snapShots;
	for (CombatCharacter* pCharacter : combatManager.mpAllCombatCharacters)
	{
		snapShots.push_back(CombatCharacterSnapShot(pCharacter));
	}
	return snapShots;
}


std::string getCharacterChangesString(const CombatManager& combatManager, const std::vector<CombatCharacterSnapShot>& preTickCharacters)
{
	std::string line = "";
	for (int i = 0; i < (int)combatManager.mpAllCombatCharacters.size(); i++)
	{
		std::string curLine = "";
		CombatCharacter* pCharacter = combatManager.mpAllCombatCharacters[i];
		const CombatCharacterSnapShot& preTickCharacter = preTickCharacters[i];

		int curHealth = pCharacter->getCurHealth();
		if (pCharacter->isAlive() && preTickCharacter.mAmAlive)
		{
			
			int curDefense = pCharacter->getCurDefense();
			if (curHealth < preTickCharacter.mCurHealth)
			{
				if (curDefense < preTickCharacter.mCurDefense) // took damage and lost defense
				{
					curLine += pCharacter->mName + " lost " + std::to_string(preTickCharacter.mCurDefense - curDefense) + " defense and took "
						+ std::to_string(preTickCharacter.mCurHealth - curHealth) + " damage";
				}
				else // just took damage
				{
					curLine += pCharacter->mName + " took " + std::to_string(preTickCharacter.mCurHealth - curHealth) + " damage";
				}
			}
			else if (curDefense < preTickCharacter.mCurDefense) // lost defense but not health
			{
				curLine += pCharacter->mName + " lost " + std::to_string(preTickCharacter.mCurDefense - curDefense) + " defense";
			}
			else if (curDefense > preTickCharacter.mCurDefense) // defended
			{
				curLine += pCharacter->mName + " gained " + std::to_string(curDefense - preTickCharacter.mCurDefense) + " defense";
			}


			int curHealthCapacity = pCharacter->getCurHealthCapacity();
			if (curHealthCapacity < preTickCharacter.mCurHealthCapacity)
			{
				curLine += pCharacter->mName + "'s total health capacity dropped to " + std::to_string(curHealthCapacity);
			}
			else if (curHealthCapacity > preTickCharacter.mCurHealthCapacity)
			{
				curLine += pCharacter->mName + "'s total health capacity increased to " + std::to_string(curHealthCapacity);
			}

			int curDefenseCapacity = pCharacter->getCurDefenseCapacity();
			if (curDefenseCapacity < preTickCharacter.mCurDefenseCapacity)
			{
				curLine += pCharacter->mName + "'s total defense capacity dropped to " + std::to_string(curDefenseCapacity);
			}
			else if (curDefenseCapacity > preTickCharacter.mCurDefenseCapacity)
			{
				curLine += pCharacter->mName + "'s total defense capacity increased to " + std::to_string(curDefenseCapacity);
			}

			int curDamage = pCharacter->getCurDamage();
			if (curDamage < preTickCharacter.mCurDamage)
			{
				curLine += pCharacter->mName + "'s total attack damage dropped to " + std::to_string(curDamage);
			}
			else if (curDamage > preTickCharacter.mCurDamage)
			{
				curLine += pCharacter->mName + "'s total attack damage increased to " + std::to_string(curDamage);
			}

		}
		else if (pCharacter->isAlive() && !preTickCharacter.mAmAlive)
		{
			curLine += pCharacter->mName + " revived with " + std::to_string(curHealth);
		}
		else if (!pCharacter->isAlive() && preTickCharacter.mAmAlive)
		{
			curLine += pCharacter->mName + " took " + std::to_string(preTickCharacter.mCurHealth - curHealth) + " damage and died";
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


EMiniGameState getPostBufferState(const CombatCharacter& character)
{
	switch (character.mType)
	{
	case EMiniGameCombatCharacterType_PLAYER:
		return EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT;
	case EMiniGameCombatCharacterType_ENEMY:
		return EMiniGameState_ENEMY_MOVE_CHARACTER;
	default:
		SDL_assert(false);
		return EMiniGameState_INVALID;
	}
}
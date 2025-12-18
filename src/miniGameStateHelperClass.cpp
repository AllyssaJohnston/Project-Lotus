#include "miniGameStateHelperClass.h"




CombatCharacterSnapShot::CombatCharacterSnapShot(CombatCharacter* pCharacter) : mpCharacter(pCharacter)
{
	mAmAlive			= mpCharacter->mAmAlive;

	mCurAttackDamage	= mpCharacter->mCurAttackDamage;

	mCurHealthCapacity	= mpCharacter->mCurHealthCapacity;
	mCurHealth			= mpCharacter->mCurHealth;

	mCurDefenseCapacity = mpCharacter->mCurDefenseCapacity;
	mCurDefense			= mpCharacter->mCurDefense;

	mTurnsToPass		= mpCharacter->mTurnsToPass;

	mpTile				= mpCharacter->mCombatMovementManager.getCurTile();
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






std::vector <AttackTile> returnAttackTileCoordsWithPlayersOnThem(MiniGameWorldData& worldData, Tile* pReferenceTile, CombatCharacter* pCharacter)
{
	std::vector <AttackTile> attackTileCoordsWithPlayers;
	MiniGameStage* pStage = worldData.getStage();
	Grid& grid = pStage->mGrid;
	CombatManager& combatManager = pStage->mCombatManager;

	for (Attack& curAttack : pCharacter->mCombatMovementManager.getAttacks())
	{
		AttackAndCorrespondingTilesCoords curListOfAttackTileCoords = returnAttackTileCoordsBasedOnAttack(*pReferenceTile, curAttack);

		for (TileCoords& curTileCoord : curListOfAttackTileCoords.mTileCoords)
		{
			if (grid.isLegalCoords(curTileCoord.mRow, curTileCoord.mCol))
			{
				Tile* pCurAttackTile = grid.mpTiles[grid.getIndex(curTileCoord.mRow, curTileCoord.mCol)];
				for (CombatCharacter* pCurCharacterToTest : combatManager.mpCurCombatCharacters)
				{
					if (pCurCharacterToTest->mType == EMiniGameCombatCharacterType_PLAYER && pCurCharacterToTest->mCombatMovementManager.getCurTile() == pCurAttackTile)
					{
						attackTileCoordsWithPlayers.push_back(AttackTile(*pCurAttackTile, curListOfAttackTileCoords.mAttack));
					}
				}
			}
		}
	}
	return attackTileCoordsWithPlayers;
}




bool tileInAttackRange(const Attack& attack, EDirection attackDirection, Grid& grid, Tile* pGivenTile, CombatCharacter* pGivenCharacter)
{
	std::vector <TileCoords> possibleAttackTileCoords;
	if (attack.mRequiresDirectionInput)
	{
		possibleAttackTileCoords = returnAttackTileCoordsBasedOnAttackAndDirection(*pGivenCharacter->mCombatMovementManager.getCurTile(), attack,
			attackDirection).mTileCoords;
	}
	else
	{
		possibleAttackTileCoords = returnAttackTileCoordsBasedOnAttack(*pGivenCharacter->mCombatMovementManager.getCurTile(), attack).mTileCoords;
	}

	for (int count = 0; count < possibleAttackTileCoords.size(); count++)
	{
		Tile* pTile = findTile(grid, possibleAttackTileCoords[count]);
		if (pTile == pGivenTile)
		{
			return true;
		}
	}
	return false;
}

bool attackMultipleTiles(const Attack& attack, EDirection attackDirection, MiniGameWorldData& worldData, std::vector <Tile*>& pTilesToAttack, CombatCharacter* pGivenCharacter)
{
	MiniGameStage* pStage = worldData.getStage();
	Grid& grid = pStage->mGrid;
	CombatManager& combatManager = pStage->mCombatManager;
	bool attacked = false;
	for (Tile* pTile : pTilesToAttack)
	{
		if (tileInAttackRange(attack, attackDirection, grid, pTile, pGivenCharacter))
		{
			combatManager.attack(*pGivenCharacter, *pTile, attack);
			attacked = true;
		}
	}
	return attacked;
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

std::vector <Tile*> returnListWithoutTilesWithCharacters(const CombatManager& combatManager, const std::vector <Tile*>& listOfTiles)
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

std::vector <TileDistance> returnListOfTileDistances(std::vector <CombatCharacter*>& pCurCombatCharacters, std::vector <Tile*>& pMoveTiles, CombatCharacter* pCurEnemy)
{
	std::vector <TileDistance> tileDistances;

	for (CombatCharacter* pCurCharacter : pCurCombatCharacters)
	{
		if (pCurCharacter->mType == EMiniGameCombatCharacterType_PLAYER)
		{
			for (Tile* pCurTile : pMoveTiles)
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

		if (pCharacter->mAmAlive && preTickCharacter.mAmAlive)
		{
			if (pCharacter->mCurHealth < preTickCharacter.mCurHealth) 
			{
				if (pCharacter->mCurDefense < preTickCharacter.mCurDefense) // took damage and lost defense
				{
					curLine += pCharacter->mName + " lost " + std::to_string(preTickCharacter.mCurDefense - pCharacter->mCurDefense) + " defense and took "
						+ std::to_string(preTickCharacter.mCurHealth - pCharacter->mCurHealth) + " damage";
				}
				else // just took damage
				{
					curLine += pCharacter->mName + " took " + std::to_string(preTickCharacter.mCurHealth - pCharacter->mCurHealth) + " damage";
				}
			}
			else if (pCharacter->mCurDefense < preTickCharacter.mCurDefense) // lost defense but not health
			{
				curLine += pCharacter->mName + " lost " + std::to_string(preTickCharacter.mCurDefense - pCharacter->mCurDefense) + " defense";
			}
			else if (pCharacter->mCurDefense > preTickCharacter.mCurDefense) // defended
			{
				curLine += pCharacter->mName + " gained " + std::to_string(pCharacter->mCurDefense - preTickCharacter.mCurDefense) + " defense";
			}


			if (pCharacter->mCurHealthCapacity < preTickCharacter.mCurHealthCapacity)
			{
				curLine += pCharacter->mName + "'s total health capacity dropped to " + std::to_string(pCharacter->mCurHealthCapacity);
			}
			else if (pCharacter->mCurHealthCapacity > preTickCharacter.mCurHealthCapacity)
			{
				curLine += pCharacter->mName + "'s total health capacity increased to " + std::to_string(pCharacter->mCurHealthCapacity);
			}

			if (pCharacter->mCurDefenseCapacity < preTickCharacter.mCurDefenseCapacity)
			{
				curLine += pCharacter->mName + "'s total defense capacity dropped to " + std::to_string(pCharacter->mCurDefenseCapacity);
			}
			else if (pCharacter->mCurDefenseCapacity > preTickCharacter.mCurDefenseCapacity)
			{
				curLine += pCharacter->mName + "'s total defense capacity increased to " + std::to_string(pCharacter->mCurDefenseCapacity);
			}

			if (pCharacter->mCurAttackDamage < preTickCharacter.mCurAttackDamage)
			{
				curLine += pCharacter->mName + "'s total attack damage dropped to " + std::to_string(pCharacter->mCurAttackDamage);
			}
			else if (pCharacter->mCurAttackDamage > preTickCharacter.mCurAttackDamage)
			{
				curLine += pCharacter->mName + "'s total attack damage increased to " + std::to_string(pCharacter->mCurAttackDamage);
			}

		}
		else if (pCharacter->mAmAlive && !preTickCharacter.mAmAlive)
		{
			curLine += pCharacter->mName + " revived";
		}
		else if (!pCharacter->mAmAlive && preTickCharacter.mAmAlive)
		{
			curLine += pCharacter->mName + " took " + std::to_string(preTickCharacter.mCurHealth - pCharacter->mCurHealth) + " damage and died";
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
#include "miniGameStateHelperClass.h"




CombatCharacterSnapShot::CombatCharacterSnapShot(CombatCharacter* pCharacter) : mpCharacter(pCharacter)
{
	mAmAlive					= mpCharacter->isAlive();

	mBaseDamage					= mpCharacter->getBaseDamage();
	mCurDamage					= mpCharacter->getCurDamage();
	mAttackDamageModifiers		= mpCharacter->getDamageModifiers();

	mCurHealth					= mpCharacter->getCurHealth();
	mBaseHealthCapacity			= mpCharacter->getBaseHealthCapacity();
	mCurHealthCapacity			= mpCharacter->getCurHealthCapacity();
	mHealthCapacityModifiers	= mpCharacter->getHealthCapacityModifiers();
	

	mCurDefense					= mpCharacter->getCurDefense();
	mBaseDefenseCapacity		= mpCharacter->getBaseDefenseCapacity();
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


TileDistance::TileDistance(Tile& tile1, Tile& tile2) : mTile1(tile1), mTile2(tile2) { mDistance = getDistanceBetweenTiles(tile1, tile2); }



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
		for (CombatCharacter* pCurCharacterToTest : combatManager.mpCurAliveCombatCharacters)
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
			for (CombatCharacter* pCurCharacterToTest : combatManager.mpCurAliveCombatCharacters)
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
		for (CombatCharacter* pCurCharacter : combatManager.mpCurAliveCombatCharacters)
		{
			if (pCurCharacter->mCombatMovementManager.getCurTile() == pCurTile)
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
				tileDistances.push_back(TileDistance(*pCurEnemy->mCombatMovementManager.getCurTile(), *pCurCharacter->mCombatMovementManager.getCurTile()));
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
			if (curHealth > preTickCharacter.mCurHealth) // healed
			{
				curLine += pCharacter->mName + " healed " + std::to_string(curHealth - preTickCharacter.mCurHealth) + " ";
			}
			else if (curHealth < preTickCharacter.mCurHealth)
			{
				if (curDefense < preTickCharacter.mCurDefense) // took damage and lost defense
				{
					curLine += pCharacter->mName + " lost " + std::to_string(preTickCharacter.mCurDefense - curDefense) + " defense and took "
						+ std::to_string(preTickCharacter.mCurHealth - curHealth) + " damage";
				}
				else // just took damage
				{
					curLine += pCharacter->mName + " took " + std::to_string(preTickCharacter.mCurHealth - curHealth) + " damage ";
				}
			}
			else if (curDefense < preTickCharacter.mCurDefense) // lost defense but not health
			{
				curLine += pCharacter->mName + " lost " + std::to_string(preTickCharacter.mCurDefense - curDefense) + " defense ";
			}
			else if (curDefense > preTickCharacter.mCurDefense) // defended
			{
				curLine += pCharacter->mName + " gained " + std::to_string(curDefense - preTickCharacter.mCurDefense) + " defense ";
			}


			int baseHealthCapacity = pCharacter->getBaseHealthCapacity();
			int curHealthCapacity = pCharacter->getCurHealthCapacity();
			if (baseHealthCapacity < preTickCharacter.mBaseHealthCapacity)
			{
				curLine += pCharacter->mName + "'s total health capacity permanently dropped to " + std::to_string(baseHealthCapacity) + " ";
			}
			else if (baseHealthCapacity > preTickCharacter.mBaseHealthCapacity)
			{
				curLine += pCharacter->mName + "'s total health capacity permanently increased to " + std::to_string(baseHealthCapacity) + " ";
			}
			else if (curHealthCapacity < preTickCharacter.mCurHealthCapacity)
			{
				curLine += pCharacter->mName + "'s total health capacity dropped to " + std::to_string(curHealthCapacity) + " ";
			}
			else if (curHealthCapacity > preTickCharacter.mCurHealthCapacity)
			{
				curLine += pCharacter->mName + "'s total health capacity increased to " + std::to_string(curHealthCapacity) + " ";
			}

			int baseDefenseCapacity = pCharacter->getBaseDefenseCapacity();
			int curDefenseCapacity = pCharacter->getCurDefenseCapacity();
			if (baseDefenseCapacity < preTickCharacter.mBaseDefenseCapacity)
			{
				curLine += pCharacter->mName + "'s total defense capacity permanently dropped to " + std::to_string(baseDefenseCapacity) + " ";
			}
			else if (baseDefenseCapacity > preTickCharacter.mBaseDefenseCapacity)
			{
				curLine += pCharacter->mName + "'s total defense capacity permanently increased to " + std::to_string(baseDefenseCapacity) + " ";
			}
			else if (curDefenseCapacity < preTickCharacter.mCurDefenseCapacity)
			{
				curLine += pCharacter->mName + "'s total defense capacity dropped to " + std::to_string(curDefenseCapacity) + " ";
			}
			else if (curDefenseCapacity > preTickCharacter.mCurDefenseCapacity)
			{
				curLine += pCharacter->mName + "'s total defense capacity increased to " + std::to_string(curDefenseCapacity) + " ";
			}

			int baseDamage = pCharacter->getCurDamage();
			int curDamage = pCharacter->getCurDamage();
			if (baseDamage < preTickCharacter.mBaseDamage)
			{
				curLine += pCharacter->mName + "'s total attack damage permanently dropped to " + std::to_string(baseDamage) + " ";
			}
			else if (baseDamage > preTickCharacter.mBaseDamage)
			{
				curLine += pCharacter->mName + "'s total attack damage permanently increased to " + std::to_string(baseDamage) + " ";
			}
			else if (curDamage < preTickCharacter.mCurDamage)
			{
				curLine += pCharacter->mName + "'s total attack damage dropped to " + std::to_string(curDamage) + " ";
			}
			else if (curDamage > preTickCharacter.mCurDamage)
			{
				curLine += pCharacter->mName + "'s total attack damage increased to " + std::to_string(curDamage) + " ";
			}

			int stuns = pCharacter->getStuns();
			if (stuns > 0 && preTickCharacter.mTurnsToPass == 0)
			{
				curLine += pCharacter->mName + " lost " + std::to_string(stuns) + (stuns == 1 ? " turn " : " turns ");
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



void setUpForBufferState(const MiniGameWorldData& worldData, MiniGameStateData& data)
{
	CombatManager& combatManager = worldData.getStage()->mCombatManager;
	combatManager.postTick();
	int index = -1;
	CombatCharacter* pNextCharacter = combatManager.returnNextAliveCharacter(*data.getCharacter(), index);
	data.setCharacter(pNextCharacter, index);

	data.mPostBufferGameState = getPostBufferState(*pNextCharacter);
	data.mNextMiniGameState = EMiniGameState_BUFFER;
	data.mGoingToAttack = false;
	data.mpTileToAttack = nullptr;
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
	case EMiniGameCombatCharacterType_PLAYER:
		return EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT;
	case EMiniGameCombatCharacterType_ENEMY:
		return EMiniGameState_ENEMY_MOVE_CHARACTER;
	default:
		SDL_assert(false);
		return EMiniGameState_INVALID;
	}
}
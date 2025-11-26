#include "gameStateHelperClass.h"

MiniGameStateData::~MiniGameStateData()
{
	mpCharacter = nullptr;
	mpTileToMoveTo = nullptr;
	mpTileToAttack = nullptr;
	for (Tile* tile : mpTilesToAttack)
	{
		delete tile;
	}
}

CombatCharacter* MiniGameStateData::getCharacter() { return mpCharacter; }

int MiniGameStateData::getCharacterIndex() { return mCharacterIndex;  }

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
	mTicksBeforeAction = 40;

	mTickYet = false;
}


std::vector <TileCoords> removeDuplicateTiles(std::vector <TileCoords> givenList)
{
	std::vector <TileCoords> uniqueList;
	for (int count = 0; count < givenList.size(); count++)
	{
		if (!inTileCoordList(uniqueList, &(givenList[count])))
		{
            uniqueList.push_back(givenList[count]);
		}
	}
	return uniqueList;
}

bool inTileCoordList(std::vector <TileCoords> givenList, TileCoords* curTileCoords)
{
	for (int count = 0; count < givenList.size(); count++)
	{
		if ((givenList[count].mRow == curTileCoords->mRow) and (givenList[count].mCol == curTileCoords->mCol))
		{
			return true;
		}
	}
	return false;
}

SDL_Color blendColors(SDL_Color* color1, SDL_Color* color2, float blendPercent)
{
	SDL_Color newColor;
	newColor.r = Uint8(color1->r * (1- blendPercent)) + Uint8(color2->r * blendPercent);
	newColor.g = Uint8(color1->g * (1- blendPercent)) + Uint8(color2->g * blendPercent);
	newColor.b = Uint8(color1->b * (1- blendPercent)) + Uint8(color2->b * blendPercent);
	return newColor;
}

bool checkIfTileInCharacterMoveRange(Tile* pGivenTile, CombatCharacter* pGivenCharacter)
{
    std::vector <TileCoords> tileCoordsList = pGivenCharacter->mCombatMovementManager.getMoveTiles();
    for (int count = 0; count < tileCoordsList.size(); count++)
    {
        TileCoords pCurTile = tileCoordsList[count];
        if (pGivenTile->mRow == pCurTile.mRow and pGivenTile->mCol == pCurTile.mCol)
        {
            return true;
        }
    }
    return false;
}

std::vector <AttackTile> returnAttackTileCoordsWithPlayersOnThem(MiniGameWorldData& worldData, Tile* pReferenceTile, CombatCharacter* pCharacter)
{
	std::vector <AttackTile> attackTileCoordsWithPlayers;
	Grid& grid = worldData.mpMiniGameLevels[worldData.mCurMiniGameLevelNumber]->mGrid;
	CombatManager& combatManager = worldData.mpMiniGameLevels[worldData.mCurMiniGameLevelNumber]->mCombatManager;

	for (Attack curAttack : pCharacter->mCombatMovementManager.getAttacks())
	{
		AttackAndListOfTileCoordsToCorrespondingTilesCoords curListOfAttackTileCoords = returnAttackTileCoordsBasedOnAttack(pReferenceTile, curAttack);

		for (TileCoords& curTileCoord : curListOfAttackTileCoords.mTileCoords)
		{
			if (grid.isLegalCoords(curTileCoord.mRow, curTileCoord.mCol))
			{
				Tile* pCurTile = grid.mpTiles[grid.getIndex(curTileCoord.mRow, curTileCoord.mCol)];
				for (int countCharacter = 0; countCharacter < combatManager.mpCurCombatCharacters.size(); countCharacter++)
				{
					CombatCharacter* pCurCharacterToTest = combatManager.mpCurCombatCharacters[countCharacter];
					if (pCurCharacterToTest->mType == EMiniGameCombatCharacterType_PLAYER)
					{
						if (pCurCharacterToTest->mCombatMovementManager.getCurTile()->mRow == pCurTile->mRow and pCurCharacterToTest->mCombatMovementManager.getCurTile()->mCol == pCurTile->mCol)
						{
							//Attack attack(curListOfAttackTileCoords.mAttack.mType, curListOfAttackTileCoords.mAttack.mName, curListOfAttackTileCoords.mAttack.mDescription, curListOfAttackTileCoords.mAttack.mDamagePercent, 
							//	curListOfAttackTileCoords.mAttack.mDamageDistanceDependent, curListOfAttackTileCoords.mAttack.mRequiresDirectionInput, curListOfAttackTileCoords.mAttack.mSpecialEffects);
							attackTileCoordsWithPlayers.push_back(AttackTile(pCurTile, curListOfAttackTileCoords.mAttack));
						}
					}
				}
			}
		}
	}
	return attackTileCoordsWithPlayers;
}

Tile* findTile(Grid& grid, TileCoords& tileCoords)
{
	if (grid.isLegalCoords(tileCoords.mRow, tileCoords.mCol))
	{
		return grid.mpTiles[grid.getIndex(tileCoords.mRow, tileCoords.mCol)];
	}
	return nullptr;
}

bool isPlayableTile(Tile* pGivenTile)
{
	switch (pGivenTile->mType)
	{
	case EMiniGameCombatTileType_IMPASSABLE:
		break;
	case EMiniGameCombatTileType_TELEPORTER:
		//TODO
		break;
	default:
		return true;
		break;
	}
	return false;
}

bool validAttackTile(MiniGameStateData& stateData, MiniGameWorldData& worldData, Tile* pGivenTile, CombatCharacter* pGivenCharacter)
{
	std::vector <TileCoords> possibleAttackTileCoords;
	if (stateData.mpCurAttack->mRequiresDirectionInput)
	{
		possibleAttackTileCoords = returnAttackTileCoordsBasedOnAttackAndDirection(pGivenCharacter->mCombatMovementManager.getCurTile(), *stateData.mpCurAttack,
			stateData.mCurAttackDirection).mTileCoords;
	}
	else
	{
		possibleAttackTileCoords = returnAttackTileCoordsBasedOnAttack(pGivenCharacter->mCombatMovementManager.getCurTile(), *stateData.mpCurAttack).mTileCoords;
	}

	for (int count = 0; count < possibleAttackTileCoords.size(); count++)
	{
		Tile* pTile = findTile(worldData.mpMiniGameLevels[worldData.mCurMiniGameLevelNumber]->mGrid,
			possibleAttackTileCoords[count]);
		if (pTile == pGivenTile)
		{
			return true;
		}
	}
	return false;
}

void attemptAttackMultipleTiles(MiniGameStateData& stateData, MiniGameWorldData& worldData, std::vector <Tile*> tilesToAttack, CombatCharacter* pGivenCharacter)
{
	for (int count = 0; count < (int)tilesToAttack.size(); count++)
	{
		if (validAttackTile(stateData, worldData, tilesToAttack[count], pGivenCharacter))
		{
			Attack* pAttack = stateData.mpCurAttack;
			CombatManager& combatManager = worldData.mpMiniGameLevels[worldData.mCurMiniGameLevelNumber]->mCombatManager;
			combatManager.attack(pGivenCharacter, tilesToAttack[count], *pAttack);
			continue;
			//attackCharacterChanges
		}
	}
}

bool characterOnTile(Tile* pTile, std::vector <CombatCharacter*> characters)
{
	for (int countCharacter = 0; countCharacter < characters.size(); countCharacter++)
	{
		CombatCharacter* pCurCharacterToTest = characters[countCharacter];
		if (pCurCharacterToTest->mCombatMovementManager.getCurTile()->mRow == pTile->mRow and pCurCharacterToTest->mCombatMovementManager.getCurTile()->mCol == pTile->mCol)
		{
			// someone is on this tile, ignore it
			return true;
		}

	}
	return false;
}

std::vector <Tile*> returnListWithoutTilesWithCharacters(CombatManager& combatManager, CombatCharacter* pGivenCharacter, std::vector <Tile*> listOfTiles)
{
	for (int countTile = (int)listOfTiles.size() - 1; countTile > -1; countTile--)
	{
		Tile* pCurTile = listOfTiles[countTile];
		for (CombatCharacter* pCurCharacter : combatManager.mpCurCombatCharacters)
		{
			if (pCurCharacter->mCombatMovementManager.getCurTile()->mRow == pCurTile->mRow
				and pCurCharacter->mCombatMovementManager.getCurTile()->mCol == pCurTile->mCol)
			{
				//std::vector<Tile*>::iterator  it = listOfTiles.begin();
				listOfTiles.erase(listOfTiles.begin() + countTile);
			}
		}
	}
	return listOfTiles;
}

std::vector <TileDistance> returnListOfTileDistances(std::vector <CombatCharacter*> pCurCombatCharacters, std::vector <Tile*> pMoveTiles, CombatCharacter* pCurEnemy)
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


/*

class TileMoveChange():
    def __init__(self, rowChange, colChange):
        self.rowChange = rowChange
        self.colChange = colChange

class EnemyTurn():
    def __init__(self, moveIndexes, attackIndexes):
        self.moveIndexes   = moveIndexes
        self.attackIndexes = attackIndexes

def calculateMoveChange(formerTile, newTile):
    rowChange = newTile.row - formerTile.row
    colChange = newTile.col - formerTile.col
    return TileMoveChange(rowChange, colChange)

#attack multiple tiles and then return the attacked character changes
def attemptAttackMultipleTilesReturnAttackedCharacterChangesList(gameInstance, tilesToAttack, curCharacter):
    attackedCharactersChangesList = list()
    for count in range(0, len(tilesToAttack)):
        if validAttackTile(gameInstance, tilesToAttack[count], curCharacter):
            curAttackType = gameInstance.miniGameStateManager.curState.curAttack
            combatManager = gameInstance.miniGameLevels[gameInstance.miniGameLevelNumber].combatManager
            attackedCharactersChangesList.append(combatManager.attack(gameInstance, curCharacter, tilesToAttack[count], curAttackType))
    return attackedCharactersChangesList
*/
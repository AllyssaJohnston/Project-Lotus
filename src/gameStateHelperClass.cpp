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


MiniGameStateManagerData::~MiniGameStateManagerData()
{
	mpData              = nullptr;
	mpMiniGameWorldData = nullptr;
}

void MiniGameStateManagerData::printBoard(ScreenObject& screenObject, StyleManager& styleManager)
{
	Grid * pGrid = mpMiniGameWorldData->mpMiniGameLevels[mpMiniGameWorldData->mCurMiniGameLevelNumber]->mpGrid;
	pGrid->preTick();
	updateTileColors(styleManager);
	pGrid->printGrid(screenObject.mpRenderer, screenObject.mGameScreenToGameLevelChunkRatio);
	printCharacters(screenObject, styleManager);
	//printObjects
}

void MiniGameStateManagerData::printCharacters(ScreenObject& screenObject, StyleManager& styleManager)
{
	Grid * pGrid = mpMiniGameWorldData->mpMiniGameLevels[mpMiniGameWorldData->mCurMiniGameLevelNumber]->mpGrid;
	CombatManager * pCombatManager = mpMiniGameWorldData->mpMiniGameLevels[mpMiniGameWorldData->mCurMiniGameLevelNumber]->mpCombatManager;
	for (int count = 0; count < pCombatManager->mpCurCombatCharacters.size(); count++)
	{
		CombatCharacter* curCombatCharacter = pCombatManager->mpCurCombatCharacters[count];
		Tile * curTile = curCombatCharacter->mCombatMovementManager.getCurTile();
		SDL_Color curColor = styleManager.white;
		SDL_Renderer * pRenderer = screenObject.mpRenderer;
		if (curCombatCharacter->mType == EMiniGameCombatCharacterType_ENEMY)
		{
			curColor = styleManager.sunYellow;
		}
		SDL_SetRenderDrawColor(pRenderer, curColor.r, curColor.g, curColor.b, curColor.a);

		float gameScreenToGameLevelChunkRatio = screenObject.mGameScreenToGameLevelChunkRatio;
		float width  = float(curTile->mCoords.mWidth)  * .75f;
		float height = float(curTile->mCoords.mHeight) * .75f;
		float x      = float(curTile->mCoords.mX1) + (float(curTile->mCoords.mWidth  - width)  / 2.0f);
		float y      = float(curTile->mCoords.mY1) + (float(curTile->mCoords.mHeight - height) / 2.0f);
		x *= gameScreenToGameLevelChunkRatio;
		y *= gameScreenToGameLevelChunkRatio;
		width  *= gameScreenToGameLevelChunkRatio;
		height *= gameScreenToGameLevelChunkRatio;

		SDL_FRect curTileRect {x, y, width, height};
		SDL_RenderFillRect(pRenderer, &curTileRect);
	}

}

void MiniGameStateManagerData::updateTileColors(StyleManager& styleManager)
{
	Grid * pGrid = mpMiniGameWorldData->mpMiniGameLevels[mpMiniGameWorldData->mCurMiniGameLevelNumber]->mpGrid;
	CombatManager * pCombatManager = mpMiniGameWorldData->mpMiniGameLevels[mpMiniGameWorldData->mCurMiniGameLevelNumber]->mpCombatManager;

	//SELECTED OR HIGHLIGHTED
	for (int count = 0; count < pGrid->mpTiles.size(); count++)
	{
		Tile* curTile = pGrid->mpTiles[count];
		SDL_Color curColor = curTile->mCurColor;
		SDL_Color colorToDraw = curColor;
		float alpha = 0.0f;
		if (curTile->getMode() == EMiniGameCombatTileMode_SELECTED)
		{
			alpha = .1f;
		}
		else if (curTile->getMode() == EMiniGameCombatTileMode_HIGHLIGHTED)
		{
			alpha = .35f;
		}
		SDL_Color curWhite {255, 255, 255, (Uint8)alpha};
		colorToDraw = blendColors(&curColor, &curWhite, alpha);

		curTile->setCurColor(&colorToDraw);
	}

	//SHOW MOVE TILES
	for (int count = 0; count < pCombatManager->mpCurCombatCharacters.size(); count++)
	{
		CombatCharacter * curCombatCharacter = pCombatManager->mpCurCombatCharacters[count];
		if (curCombatCharacter == mpData->mpCharacter)
		{
			EMiniGameCombatActionType tileType = EMiniGameCombatActionType_MOVE;
			std::vector <TileCoords> tileCoordsList;
			if (mCurStateEnum == EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT or mCurStateEnum == EMiniGameState_ENEMY_MOVE_CHARACTER)
			{
				tileCoordsList = curCombatCharacter->mCombatMovementManager.getMoveTiles();
				tileType = EMiniGameCombatActionType_MOVE;
			}
			else if (mCurStateEnum == EMiniGameState_PLAYER_TAKE_ACTION_ATTACK or mCurStateEnum == EMiniGameState_PLAYER_COMPLETE_DIRECTIONAL_ATTACK
				or mCurStateEnum == EMiniGameState_ENEMY_TAKE_ACTION)
			{
				Attack& curAttack = mpData->mCurAttack;
				EDirection curAttackDirection = mpData->mCurAttackDirection;
				if (curAttack.mRequiresDirectionInput and curAttackDirection != EDirection_NONE)
				{
					tileCoordsList = returnAttackTileCoordsBasedOnAttackAndDirection(curCombatCharacter->mCombatMovementManager.getCurTile(), curAttack, curAttackDirection).mTileCoords;
				}
				else
				{
					tileCoordsList = returnAttackTileCoordsBasedOnAttack(curCombatCharacter->mCombatMovementManager.getCurTile(), curAttack).mTileCoords;
				}
				tileType = EMiniGameCombatActionType_ATTACK;

			}
			tileCoordsList = removeDuplicateTiles(tileCoordsList);

			for (int countTile = 0; countTile < tileCoordsList.size(); countTile++)
			{
				int row = tileCoordsList[countTile].mRow;
				int col = tileCoordsList[countTile].mCol;

				if (pGrid->isLegalCoords(row, col))
				{
					Tile* curTile = pGrid->mpTiles[pGrid->getIndex(row, col)];
					SDL_Color curColor = curTile->mCurColor;
					SDL_Color otherColor;
					if (tileType == EMiniGameCombatActionType_MOVE)
					{
						otherColor = styleManager.sunYellow;
					}
					else if (tileType == EMiniGameCombatActionType_ATTACK)
					{
						otherColor = styleManager.red;
					}
					else
					{
						SDL_assert(false);
					}
					float alpha = .25;
					SDL_Color updatedColor = blendColors(&curColor, &otherColor, alpha);
					curTile->setCurColor(&updatedColor);
				}
			}
		}
	}
}




std::vector <TileCoords> removeDuplicateTiles(std::vector <TileCoords> givenList)
{
	std::vector <TileCoords> uniqueList;
	for (int count = 0; count < givenList.size(); count++)
	{
		if (inTileCoordList(uniqueList, &(givenList[count])) == false)
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
	Grid* pGrid = worldData.mpMiniGameLevels[worldData.mCurMiniGameLevelNumber]->mpGrid;
	CombatManager* pCombatManager = worldData.mpMiniGameLevels[worldData.mCurMiniGameLevelNumber]->mpCombatManager;

	for (int countList = 0; countList < pCharacter->mCombatMovementManager.getAttacks().size(); countList++)
	{
		Attack curAttack = pCharacter->mCombatMovementManager.getAttacks()[countList];
		AttackAndListOfTileCoordsToCorrespondingTilesCoords curListOfAttackTileCoords = returnAttackTileCoordsBasedOnAttack(pReferenceTile, curAttack);


		for (int countTile = 0; countTile < curListOfAttackTileCoords.mTileCoords.size(); countTile++)
		{
			TileCoords* pCurTileCoord = &(curListOfAttackTileCoords.mTileCoords[countTile]);
			if (pGrid->isLegalCoords(pCurTileCoord->mRow, pCurTileCoord->mCol))
			{
				Tile* pCurTile = pGrid->mpTiles[pGrid->getIndex(pCurTileCoord->mRow, pCurTileCoord->mCol)];
				for (int countCharacter = 0; countCharacter < pCombatManager->mpCurCombatCharacters.size(); countCharacter++)
				{
					CombatCharacter* pCurCharacterToTest = pCombatManager->mpCurCombatCharacters[countCharacter];
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
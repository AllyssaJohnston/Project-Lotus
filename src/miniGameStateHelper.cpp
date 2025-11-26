#include "miniGameStateHelper.h"


MiniGameState::MiniGameState(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData) : mKeyboardData(keyboardData), mData(data), mWorldData(worldData) {}

void MiniGameState::useMouseInput(EMiniGameState curStateEnum, ScreenObject& screenObject) 
{
	if (curStateEnum == EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT or curStateEnum == EMiniGameState_PLAYER_TAKE_ACTION_ATTACK)
	{
		float x;
		float y;
		SDL_GetMouseState(&x, &y);
		
		Vect2 pos((int)(x / screenObject.mGameScreenToGameLevelChunkRatio), (int)(y / screenObject.mGameScreenToGameLevelChunkRatio));
		
		highlightTile(pos);
		if (mKeyboardData.mKeyState[int(EKeyboardInput_MOUSE_LEFT)] == true)
		{
			selectTile(pos);
		}
	}
};

void MiniGameState::highlightTile(Vect2 pos)
{
	Grid& grid = mWorldData.mpMiniGameLevels[mWorldData.mCurMiniGameLevelNumber]->mGrid;
	for (Tile* pTile : grid.mpTiles)
	{
		if (pTile->getMode() == EMiniGameCombatTileMode_HIGHLIGHTED)
		{
			pTile->setMode(EMiniGameCombatTileMode_NOT_SELECTED);
		}

		Tile* pTile = grid.getTileFromCoords(pos.getX(), pos.getY());

		if (pTile != nullptr)
		{
			if (pTile->getMode() != EMiniGameCombatTileMode_SELECTED)
			{
				pTile->setMode(EMiniGameCombatTileMode_HIGHLIGHTED);
			}
		}
	}
}


MiniGamePlayerWaitForMoveInput::MiniGamePlayerWaitForMoveInput(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData) :
	MiniGameState(keyboardData, data, worldData){;}

void MiniGamePlayerWaitForMoveInput::selectTile(Vect2 pos)
{
	Tile * pCurTile = mWorldData.mpMiniGameLevels[mWorldData.mCurMiniGameLevelNumber]->mGrid.getTileFromCoords(pos.getX(), pos.getY());
	if (pCurTile != nullptr)
	{
		pCurTile->setMode(EMiniGameCombatTileMode_SELECTED);
		moveToTile(pCurTile);
	}
}

void MiniGamePlayerWaitForMoveInput::moveToTile(Tile* pGivenTile)
{

	if (checkIfTileInCharacterMoveRange(pGivenTile, mData.getCharacter()) && !characterOnTile(pGivenTile, mWorldData.mpMiniGameLevels[mWorldData.mCurMiniGameLevelNumber]->mCombatManager.mpCurCombatCharacters))
	{
		if (isPlayableTile(pGivenTile))
		{
			postTick(pGivenTile); //+moveChange
		}
	}
}

void MiniGamePlayerWaitForMoveInput::postTick(Tile* pNextTile)
{
	mData.mNextMiniGameState = EMiniGameState_PLAYER_MOVE_CHARACTER;
	mData.mpTileToMoveTo = pNextTile;
	mData.getCharacter()->mCombatMovementManager.setCurTile(pNextTile);
}



MiniGamePlayerMoveCharacter::MiniGamePlayerMoveCharacter(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData)
	: MiniGameState(keyboardData, data, worldData){;}

void MiniGamePlayerMoveCharacter::tick()
{
	mData.getCharacter()->move(mData.mpTileToMoveTo);
	postTick();
}

void MiniGamePlayerMoveCharacter::postTick()
{
	mData.mNextMiniGameState = EMiniGameState_PLAYER_WAIT_FOR_ACTION_INPUT;
	mData.mpTileToMoveTo = nullptr;
}



MiniGamePlayerWaitForActionInput::MiniGamePlayerWaitForActionInput(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData)
	: MiniGameState(keyboardData, data, worldData){;}

void MiniGamePlayerWaitForActionInput::postTick(EMiniGameState nextStateEnum)
{
	if (nextStateEnum == EMiniGameState_BUFFER)
	{
		int index = -1;
		CombatCharacter* nextCharacter = mWorldData.mpMiniGameLevels[mWorldData.mCurMiniGameLevelNumber]->mCombatManager.returnNextCharacter(mData.getCharacter(), index);
		mData.setCharacter(nextCharacter, index);
		switch (nextCharacter->mType) {
		case EMiniGameCombatCharacterType_PLAYER:
			mData.mPostBufferGameState = EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT;
			break;
		case EMiniGameCombatCharacterType_ENEMY:
			mData.mPostBufferGameState = EMiniGameState_ENEMY_MOVE_CHARACTER;
			break;
		default:
			SDL_assert(false);
			break;
		}
	}
	mData.mNextMiniGameState = nextStateEnum;
}



MiniGamePlayerWaitForAttackInput::MiniGamePlayerWaitForAttackInput(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData)
	: MiniGameState(keyboardData, data, worldData){;}

void MiniGamePlayerWaitForAttackInput::postTick(Attack attack)
{
	mData.mpCurAttack = new Attack(attack);
	mData.mNextMiniGameState = attack.mRequiresDirectionInput ? EMiniGameState_PLAYER_WAIT_FOR_ATTACK_SUB_INPUT : EMiniGameState_PLAYER_TAKE_ACTION_ATTACK;
}



MiniGamePlayerWaitForAttackSubInput::MiniGamePlayerWaitForAttackSubInput(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData)
	: MiniGameState(keyboardData, data, worldData){;}

void MiniGamePlayerWaitForAttackSubInput::postTick(EDirection curAttackDirection)
{
	mData.mCurAttackDirection = curAttackDirection;
	mData.mNextMiniGameState = EMiniGameState_PLAYER_COMPLETE_DIRECTIONAL_ATTACK;
}



MiniGamePlayerCompleteDirectionalAttack::MiniGamePlayerCompleteDirectionalAttack(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData)
	: MiniGameState(keyboardData, data, worldData){;}

void MiniGamePlayerCompleteDirectionalAttack::tick()
{
	if (mData.mTicks < mData.mTicksBeforeAction)
	{
		mData.mTicks += 1;
	}
	else
	{
		attackTiles();
	}
}

void MiniGamePlayerCompleteDirectionalAttack::attackTiles()
{
	Grid& grid = mWorldData.mpMiniGameLevels[mWorldData.mCurMiniGameLevelNumber]->mGrid;
	AttackAndListOfTileCoordsToCorrespondingTilesCoords attackTileCoords = 
			returnAttackTileCoordsBasedOnAttackAndDirection(mData.getCharacter()->mCombatMovementManager.getCurTile(), *mData.mpCurAttack, mData.mCurAttackDirection);
	std::vector <Tile* > tilesToAttack;
	for (int count = 0; count < attackTileCoords.mTileCoords.size(); count++)
	{
		Tile* curTile = grid.mpTiles[grid.getIndex(attackTileCoords.mTileCoords[count].mRow, attackTileCoords.mTileCoords[count].mCol)];
		tilesToAttack.push_back(curTile);
	}
	attemptAttackMultipleTiles(mData, mWorldData, tilesToAttack, mData.getCharacter());
	postTick();
}

//attackCharacterChanges
void MiniGamePlayerCompleteDirectionalAttack::postTick()
{
	CombatManager& combatManager = mWorldData.mpMiniGameLevels[mWorldData.mCurMiniGameLevelNumber]->mCombatManager;

	combatManager.postTick();
	int index = -1;
	CombatCharacter* nextCharacter = combatManager.returnNextCharacter(mData.getCharacter(), index);
	mData.setCharacter(nextCharacter, index);
	switch (nextCharacter->mType) {
	case EMiniGameCombatCharacterType_PLAYER:
		mData.mPostBufferGameState = EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT;
		break;
	case EMiniGameCombatCharacterType_ENEMY:
		mData.mPostBufferGameState = EMiniGameState_ENEMY_MOVE_CHARACTER;
		break;
	default:
		SDL_assert(false);
		break;
	}
	mData.mTicks = 0;
	mData.mpTileToAttack = nullptr;
	mData.mpTilesToAttack.clear();
	mData.mpCurAttack = nullptr;
	mData.mCurAttackDirection = EDirection_NONE;
	mData.mNextMiniGameState = EMiniGameState_BUFFER;
}



MiniGamePlayerTakeActionAttack::MiniGamePlayerTakeActionAttack(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData) :
	MiniGameState(keyboardData, data, worldData){;}

void MiniGamePlayerTakeActionAttack::selectTile(Vect2 pos)
{
	Tile * pTile = mWorldData.mpMiniGameLevels[mWorldData.mCurMiniGameLevelNumber]->mGrid.getTileFromCoords(pos.getX(), pos.getY());

	if (pTile != nullptr)
	{
		pTile->setMode(EMiniGameCombatTileMode_SELECTED);
		std::vector <Tile *> tilesToAttack;
		tilesToAttack.push_back(pTile);
		attemptAttackMultipleTiles(mData, mWorldData, tilesToAttack, mData.getCharacter());
		postTick();
	}
}

void MiniGamePlayerTakeActionAttack::postTick()
{
	CombatManager& combatManager = mWorldData.mpMiniGameLevels[mWorldData.mCurMiniGameLevelNumber]->mCombatManager;

	combatManager.postTick();
	int index = -1;
	CombatCharacter* nextCharacter = combatManager.returnNextCharacter(mData.getCharacter(), index);
	mData.setCharacter(nextCharacter, index);
	switch (nextCharacter->mType) {
	case EMiniGameCombatCharacterType_PLAYER:
		mData.mPostBufferGameState = EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT;
		break;
	case EMiniGameCombatCharacterType_ENEMY:
		mData.mPostBufferGameState = EMiniGameState_ENEMY_MOVE_CHARACTER;
		break;
	default:
		SDL_assert(false);
		break;
	}
	mData.mpTileToAttack = nullptr;
	mData.mpTilesToAttack.clear();
	mData.mpCurAttack = nullptr;
	mData.mCurAttackDirection = EDirection_NONE;
	mData.mNextMiniGameState = EMiniGameState_BUFFER;
}



MiniGamePlayerTakeActionDefend::MiniGamePlayerTakeActionDefend(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData)
		: MiniGameState(keyboardData, data, worldData){;}

void MiniGamePlayerTakeActionDefend::tick()
{
	mData.getCharacter()->defend();
	postTick();
}

//defense change
void MiniGamePlayerTakeActionDefend::postTick()
{
	CombatManager& combatManager = mWorldData.mpMiniGameLevels[mWorldData.mCurMiniGameLevelNumber]->mCombatManager;

	combatManager.postTick();
	int index = -1;
	CombatCharacter* nextCharacter = combatManager.returnNextCharacter(mData.getCharacter(), index);
	mData.setCharacter(nextCharacter, index);
	switch (nextCharacter->mType) {
	case EMiniGameCombatCharacterType_PLAYER:
		mData.mPostBufferGameState = EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT;
		break;
	case EMiniGameCombatCharacterType_ENEMY:
		mData.mPostBufferGameState = EMiniGameState_ENEMY_MOVE_CHARACTER;
		break;
	default:
		SDL_assert(false);
		break;
	}
	mData.mNextMiniGameState = EMiniGameState_BUFFER;
}



MiniGameEnemyMoveCharacter::MiniGameEnemyMoveCharacter(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData)
		: MiniGameState(keyboardData, data, worldData){;}

void MiniGameEnemyMoveCharacter::tick()
{
	if (mData.mTicks <= 0)
	{
		decideTileToMoveTo();
	}

	if (mData.mTicks < mData.mTicksBeforeAction)
	{
		mData.mTicks += 1;
	}
	else
	{
		mData.getCharacter()->move(mData.mpTileToMoveTo);
		mData.mTicks = -1;
		postTick();
	}
}

void MiniGameEnemyMoveCharacter::decideTileToMoveTo()
{
	MiniGameLevel*	pCurLevel		= mWorldData.mpMiniGameLevels[mWorldData.mCurMiniGameLevelNumber];
	Grid&			grid			= pCurLevel->mGrid;
	CombatManager&  combatManager	= pCurLevel->mCombatManager;
	mData.mTicks = 0;

	std::vector <Tile*> allPossibleMoveTiles;

	for (TileCoords curTileCoords : mData.getCharacter()->mCombatMovementManager.getMoveTiles())
	{
		if (grid.isLegalCoords(curTileCoords.mRow, curTileCoords.mCol))
		{
			Tile *pCurTile = grid.mpTiles[grid.getIndex(curTileCoords.mRow, curTileCoords.mCol)];
			if (isPlayableTile(pCurTile))
			{
				allPossibleMoveTiles.push_back(pCurTile);
			}
		}
	}

	int maxNumberOfCharactersCanAttack = 0;
	Tile* pBestTileToMoveTo = nullptr;

	for (Tile* pCurTile : allPossibleMoveTiles)
	{
		std::vector <AttackTile> allPossibleAttacksFromCurTile = returnAttackTileCoordsWithPlayersOnThem(mWorldData, pCurTile, 
				mData.getCharacter());
		if (allPossibleAttacksFromCurTile.size() > maxNumberOfCharactersCanAttack)
		{
			pBestTileToMoveTo = pCurTile;
		}
	}
	if (pBestTileToMoveTo != nullptr)
	{
		mData.mpTileToMoveTo = pBestTileToMoveTo;
		return;
	}

	//No tiles I can attack from
	allPossibleMoveTiles = returnListWithoutTilesWithCharacters(combatManager, mData.getCharacter(), allPossibleMoveTiles);
	std::vector <TileDistance> tileDistances = returnListOfTileDistances(combatManager.mpCurCombatCharacters, allPossibleMoveTiles, mData.getCharacter());
	int minDistanceFromPlayer = std::numeric_limits<int>::max();
	Tile* pCurTile = mData.getCharacter()->mCombatMovementManager.getCurTile();

	for (TileDistance curTileDistance : tileDistances)
	{
		if (minDistanceFromPlayer > curTileDistance.mDistance and curTileDistance.mpTile != mData.getCharacter()->mCombatMovementManager.getCurTile())
		{
			minDistanceFromPlayer = (int)curTileDistance.mDistance;
			pCurTile = curTileDistance.mpTile;
		}
	}
	mData.mpTileToMoveTo = pCurTile;
}

void MiniGameEnemyMoveCharacter::postTick()
{
	mData.mNextMiniGameState	= EMiniGameState_ENEMY_TAKE_ACTION;
	mData.mpTileToMoveTo		= nullptr;
}



MiniGameEnemyTakeAction:: MiniGameEnemyTakeAction(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData)
		: MiniGameState(keyboardData, data, worldData){;}

void MiniGameEnemyTakeAction::tick()
{
	if (mData.mTicks == int(mData.mTicksBeforeAction / 2))
	{
		shouldAttack();
	}

	if (mData.mTicks < mData.mTicksBeforeAction)
	{
		mData.mTicks += 1;
	}
	else
	{
		if (mData.mGoingToAttack)
		{
			performAttack();
		}
		else if (shouldDefend())
		{
			postTick();
		}
		else
		{
			postTick();
		}
	}
}

bool MiniGameEnemyTakeAction::shouldAttack()
{
	Tile * refTile = mData.getCharacter()->mCombatMovementManager.getCurTile();
	std::vector <AttackTile> attackTilesWithCharacters = returnAttackTileCoordsWithPlayersOnThem(mWorldData, refTile, mData.getCharacter());
	Grid& grid = mWorldData.mpMiniGameLevels[mWorldData.mCurMiniGameLevelNumber]->mGrid;

	AttackTile *pCurBestAttackTile = nullptr;
	AttackAndListOfTileCoordsToCorrespondingTilesCoords curBestAttackTiles;
	float maxDamageOutput = 0;


	for (AttackTile& curAttackTile : attackTilesWithCharacters)
	{
		if (curAttackTile.mAttack.mRequiresDirectionInput)
		{
			EDirection attackDirection = getDirectionBetweenTiles(mData.getCharacter()->mCombatMovementManager.getCurTile(), curAttackTile.mpTile);
			AttackAndListOfTileCoordsToCorrespondingTilesCoords allAttackTiles = returnAttackTileCoordsBasedOnAttackAndDirection(mData.getCharacter()->mCombatMovementManager.getCurTile(), curAttackTile.mAttack, attackDirection);
			float damageOutput = allAttackTiles.mTileCoords.size() * allAttackTiles.mAttack.mDamagePercent * mData.getCharacter()->mCurAttackDamage;
			if (damageOutput > maxDamageOutput)
			{
				maxDamageOutput = damageOutput;
				curBestAttackTiles.mTileCoords.clear();
				curBestAttackTiles = allAttackTiles;
				pCurBestAttackTile = nullptr;
			}

		}
		else
		{
			float damageOutput = curAttackTile.mAttack.mDamagePercent * mData.getCharacter()->mCurAttackDamage;
			if (damageOutput > maxDamageOutput)
			{
				maxDamageOutput = damageOutput;
				pCurBestAttackTile = &curAttackTile;
				curBestAttackTiles.mTileCoords.clear();
			}
		}

		if (curBestAttackTiles.mTileCoords.size() == 0 and pCurBestAttackTile == nullptr)
		{
			return false;
		}
		else if (pCurBestAttackTile != nullptr)
		{
			if (curBestAttackTiles.mTileCoords.size() == 0)
			{
				mData.mpTilesToAttack.push_back(pCurBestAttackTile->mpTile);
				mData.mpCurAttack = new Attack(pCurBestAttackTile->mAttack);
			}
			else
			{
				for (TileCoords coords: curBestAttackTiles.mTileCoords)
				{
					Tile* pCurTile = grid.mpTiles[grid.getIndex(coords.mRow, coords.mCol)];
					mData.mpTilesToAttack.push_back(pCurTile);
					mData.mpCurAttack = new Attack(curBestAttackTiles.mAttack);
				}
			}

			if (mData.mpCurAttack->mRequiresDirectionInput)
			{
				mData.mCurAttackDirection = getDirectionBetweenTiles(mData.getCharacter()->mCombatMovementManager.getCurTile(), mData.mpTilesToAttack[0]);
			}
		}
	}
	mData.mGoingToAttack = true;
	return true;
}

bool MiniGameEnemyTakeAction::shouldDefend()
{
	Grid&			grid			= mWorldData.mpMiniGameLevels[mWorldData.mCurMiniGameLevelNumber]->mGrid;
	CombatManager&	combatManager	= mWorldData.mpMiniGameLevels[mWorldData.mCurMiniGameLevelNumber]->mCombatManager;
	Tile*			pCurTile		= mData.getCharacter()->mCombatMovementManager.getCurTile();

	for (CombatCharacter* curCombatCharacter : combatManager.mpCurCombatCharacters)
	{
		std::vector <AttackTile> attackTileListWithCharacters = returnAttackTileCoordsWithPlayersOnThem(mWorldData, pCurTile, mData.getCharacter());

		for (AttackTile attackTile : attackTileListWithCharacters)
		{
			Tile* pCurTileWithCharacter = attackTile.mpTile;
			if (grid.isLegalCoords(pCurTileWithCharacter->mRow, pCurTileWithCharacter->mCol))
			{
				if (pCurTileWithCharacter->mRow == pCurTile->mRow and pCurTileWithCharacter->mCol and pCurTile->mCol)
				{
					return true;
				}
			}
		}
	}
	return false;
}

void MiniGameEnemyTakeAction::performAttack()
{
	attemptAttackMultipleTiles(mData, mWorldData, mData.mpTilesToAttack, mData.getCharacter());
	postTick();
	//add attackedCharacterChangesList
}

void MiniGameEnemyTakeAction::postTick()
{
	CombatManager& combatManager = mWorldData.mpMiniGameLevels[mWorldData.mCurMiniGameLevelNumber]->mCombatManager;
	mWorldData.mpMiniGameLevels[mWorldData.mCurMiniGameLevelNumber]->mCombatManager.postTick();
	int index = -1;
	CombatCharacter* nextCharacter = combatManager.returnNextCharacter(mData.getCharacter(), index);
	mData.setCharacter(nextCharacter, index);
	mData.mNextMiniGameState = EMiniGameState_BUFFER;

	switch (nextCharacter->mType)
	{
	case EMiniGameCombatCharacterType_PLAYER:
		mData.mPostBufferGameState = EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT;
		break;
	case EMiniGameCombatCharacterType_ENEMY:
		mData.mPostBufferGameState = EMiniGameState_ENEMY_MOVE_CHARACTER;
		break;
	default:
		SDL_assert(false);
		break;
	}
	mData.mGoingToAttack = false;
	mData.mpTilesToAttack.clear();
	//mpData->mCurAttack = NULL;
	mData.mCurAttackDirection = EDirection_NONE;
	mData.mTicks = 0;
}



MiniGameBuffer:: MiniGameBuffer(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData)
		: MiniGameState(keyboardData, data, worldData){;}

void MiniGameBuffer::tick()
{
	if (!mData.mTickYet)
	{
		mData.mTickYet = true;
	}
	else
	{
		postTick();
	}
}

void MiniGameBuffer::postTick()
{
	//check if game is over
	GameOverStats stats = mWorldData.mpMiniGameLevels[mWorldData.mCurMiniGameLevelNumber]->mCombatManager.getGameOverStats();
	if (stats.mGameOver)
	{
		if (mWorldData.mpMiniGameLevels[mWorldData.mCurMiniGameLevelNumber]->mMainGameNextLevelData.mType == ELevelType_MINI_GAME)
		{
			mData.mNextMiniGameState = EMiniGameState_BUILD_NEXT_LEVEL;
		}
		else
		{
			mData.mNextMiniGameState = EMiniGameState_EXIT;
		}
	}
	else 
	{
		mData.mNextMiniGameState = mData.mPostBufferGameState;
	}

	//continue game / set up for next time
	mData.mPostBufferGameState = EMiniGameState_INVALID;
	mData.mTickYet = false;
}

MiniGameBuildNextLevel::MiniGameBuildNextLevel(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData)
	: MiniGameState(keyboardData, data, worldData) {
	;
}

MiniGameExit::MiniGameExit(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData)
	: MiniGameState(keyboardData, data, worldData) {
	;
}




//MINI GAME STATE MANAGER
MiniGameStateManager::MiniGameStateManager(KeyboardData& keyboardData, MiniGameWorldData& miniGameWorldData) : mWorldData(miniGameWorldData)
{
	mpStates = {new MiniGamePlayerWaitForMoveInput(keyboardData, mData.mStateData, mWorldData),
				new MiniGamePlayerMoveCharacter(keyboardData, mData.mStateData, mWorldData),
				new MiniGamePlayerWaitForActionInput(keyboardData, mData.mStateData, mWorldData),
				new MiniGamePlayerWaitForAttackInput(keyboardData, mData.mStateData, mWorldData),
				new MiniGamePlayerWaitForAttackSubInput(keyboardData, mData.mStateData, mWorldData),
				new MiniGamePlayerCompleteDirectionalAttack(keyboardData, mData.mStateData, mWorldData),
				new MiniGamePlayerTakeActionAttack(keyboardData, mData.mStateData, mWorldData),
				new MiniGamePlayerTakeActionDefend(keyboardData, mData.mStateData, mWorldData),
				new MiniGameEnemyMoveCharacter(keyboardData, mData.mStateData, mWorldData),
				new MiniGameEnemyTakeAction(keyboardData, mData.mStateData, mWorldData),
				new MiniGameBuffer(keyboardData, mData.mStateData, mWorldData),
				new MiniGameExit(keyboardData, mData.mStateData, mWorldData),
				new MiniGameBuildNextLevel(keyboardData, mData.mStateData, mWorldData) 
	};
	mData.mCurStateEnum = EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT;

	mpCurState = mpStates[mData.mCurStateEnum];
}

MiniGameStateManager::~MiniGameStateManager()
{
	for (int i = (int) mpStates.size() - 1; i > -1; i--)
	{
		mpStates[i]->~MiniGameState();
	}
	mpStates.clear();
	mpCurState = nullptr;
}

void MiniGameStateManager::start() 
{
	CombatManager& combatManager = mWorldData.mpMiniGameLevels[mWorldData.mCurMiniGameLevelNumber]->mCombatManager;
	combatManager.createCurCharacterList();
	mWorldData.resetLevel();
	mData.mStateData.reset();
	mData.mStateData.setCharacter(combatManager.mpCurCombatCharacters[0], 0);
	mData.mLastFrameStateEnum = EMiniGameState_INVALID;
	mData.mCurStateEnum = EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT;
	mpCurState = mpStates[mData.mCurStateEnum];
	setUp = true;
}

void MiniGameStateManager::preTick() { mData.mLastFrameStateEnum = mData.mCurStateEnum; }

void MiniGameStateManager::tick()
{
	if (!setUp) { start(); }
	mpCurState->tick();
}

void MiniGameStateManager::postTick()
{
	if (mData.mStateData.mNextMiniGameState != EMiniGameState_INVALID)
	{
		mData.mCurStateEnum = mData.mStateData.mNextMiniGameState;
		mpCurState = mpStates[mData.mStateData.mNextMiniGameState];
	}
}

void MiniGameStateManager::printBoard(ScreenObject& screenObject, StyleManager& styleManager)
{
	Grid& grid = mWorldData.mpMiniGameLevels[mWorldData.mCurMiniGameLevelNumber]->mGrid;
	grid.preTick();
	updateTileColors(styleManager);
	grid.printGrid(screenObject.mpRenderer, screenObject.mGameScreenToGameLevelChunkRatio);
	printCharacters(screenObject, styleManager);
	//printObjects
}

void MiniGameStateManager::printCharacters(ScreenObject& screenObject, StyleManager& styleManager)
{
	Grid& grid = mWorldData.mpMiniGameLevels[mWorldData.mCurMiniGameLevelNumber]->mGrid;
	CombatManager& combatManager = mWorldData.mpMiniGameLevels[mWorldData.mCurMiniGameLevelNumber]->mCombatManager;
	for (int count = 0; count < combatManager.mpCurCombatCharacters.size(); count++)
	{
		CombatCharacter* curCombatCharacter = combatManager.mpCurCombatCharacters[count];
		Tile* curTile = curCombatCharacter->mCombatMovementManager.getCurTile();
		SDL_Color curColor = styleManager.white;
		SDL_Renderer* pRenderer = screenObject.mpRenderer;
		if (curCombatCharacter->mType == EMiniGameCombatCharacterType_ENEMY)
		{
			curColor = styleManager.sunYellow;
		}
		SDL_SetRenderDrawColor(pRenderer, curColor.r, curColor.g, curColor.b, curColor.a);

		float gameScreenToGameLevelChunkRatio = screenObject.mGameScreenToGameLevelChunkRatio;
		float width = float(curTile->mCoords.mWidth) * .75f;
		float height = float(curTile->mCoords.mHeight) * .75f;
		float x = float(curTile->mCoords.mX1) + (float(curTile->mCoords.mWidth - width) / 2.0f);
		float y = float(curTile->mCoords.mY1) + (float(curTile->mCoords.mHeight - height) / 2.0f);
		x *= gameScreenToGameLevelChunkRatio;
		y *= gameScreenToGameLevelChunkRatio;
		width *= gameScreenToGameLevelChunkRatio;
		height *= gameScreenToGameLevelChunkRatio;

		SDL_FRect curTileRect{ x, y, width, height };
		SDL_RenderFillRect(pRenderer, &curTileRect);
	}
}

void MiniGameStateManager::updateTileColors(StyleManager& styleManager)
{
	Grid& grid = mWorldData.mpMiniGameLevels[mWorldData.mCurMiniGameLevelNumber]->mGrid;
	CombatManager& combatManager = mWorldData.mpMiniGameLevels[mWorldData.mCurMiniGameLevelNumber]->mCombatManager;

	//SELECTED OR HIGHLIGHTED
	for (int count = 0; count < grid.mpTiles.size(); count++)
	{
		Tile* curTile = grid.mpTiles[count];
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
		SDL_Color curWhite{ 255, 255, 255, (Uint8)alpha };
		colorToDraw = blendColors(&curColor, &curWhite, alpha);

		curTile->setCurColor(&colorToDraw);
	}

	//SHOW MOVE TILES
	for (int count = 0; count < combatManager.mpCurCombatCharacters.size(); count++)
	{
		CombatCharacter* curCombatCharacter = combatManager.mpCurCombatCharacters[count];
		if (curCombatCharacter == mData.mStateData.getCharacter())
		{
			EMiniGameCombatActionType tileType = EMiniGameCombatActionType_MOVE;
			std::vector <TileCoords> tileCoordsList;
			switch (mData.mCurStateEnum)
			{
			case EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT:
			case EMiniGameState_ENEMY_MOVE_CHARACTER:
				tileCoordsList = curCombatCharacter->mCombatMovementManager.getMoveTiles();
				tileType = EMiniGameCombatActionType_MOVE;
				break;
			case EMiniGameState_PLAYER_TAKE_ACTION_ATTACK:
			case EMiniGameState_PLAYER_COMPLETE_DIRECTIONAL_ATTACK:
			case EMiniGameState_ENEMY_TAKE_ACTION:
				if (mData.mStateData.mpCurAttack != nullptr)
				{
					if (mData.mStateData.mpCurAttack->mRequiresDirectionInput and (mData.mStateData.mCurAttackDirection != EDirection_NONE)
						and (mData.mStateData.mCurAttackDirection != EDirection_INVALID))
					{
						tileCoordsList = returnAttackTileCoordsBasedOnAttackAndDirection(curCombatCharacter->mCombatMovementManager.getCurTile(),
							*mData.mStateData.mpCurAttack, mData.mStateData.mCurAttackDirection).mTileCoords;
					}
					else
					{
						tileCoordsList = returnAttackTileCoordsBasedOnAttack(curCombatCharacter->mCombatMovementManager.getCurTile(), *mData.mStateData.mpCurAttack).mTileCoords;
					}
					tileType = EMiniGameCombatActionType_ATTACK;
				}
				break;
			default:
				break;
			}

			tileCoordsList = removeDuplicateTiles(tileCoordsList);
			for (int countTile = 0; countTile < tileCoordsList.size(); countTile++)
			{
				int row = tileCoordsList[countTile].mRow;
				int col = tileCoordsList[countTile].mCol;

				if (grid.isLegalCoords(row, col))
				{
					Tile* curTile = grid.mpTiles[grid.getIndex(row, col)];
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

void MiniGameStateManager::updateCurState(EMiniGameState state) 
{
	mData.mCurStateEnum = state;
	mpCurState = mpStates[state];
	mpCurState->mData.mNextMiniGameState = EMiniGameState_INVALID;
}
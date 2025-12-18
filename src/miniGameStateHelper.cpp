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
	Grid& grid = mWorldData.getStage()->mGrid;
	for (Tile* pTile : grid.mpTiles)
	{
		if (pTile->getMode() == EMiniGameCombatTileMode_HIGHLIGHTED)
		{
			pTile->setMode(EMiniGameCombatTileMode_NOT_SELECTED);
		}
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


MiniGamePlayerWaitForMoveInput::MiniGamePlayerWaitForMoveInput(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData) : MiniGameState(keyboardData, data, worldData){;}

void MiniGamePlayerWaitForMoveInput::selectTile(const Vect2 pos)
{
	Tile* pCurTile = mWorldData.getStage()->mGrid.getTileFromCoords(pos.getX(), pos.getY());
	if (pCurTile != nullptr && isPlayableTile(*pCurTile))
	{
		pCurTile->setMode(EMiniGameCombatTileMode_SELECTED);
		moveToTile(*pCurTile);
	}
}

void MiniGamePlayerWaitForMoveInput::moveToTile(Tile& givenTile)
{
	mData.getCharacter()->mCombatMovementManager.setMoveTiles();
	if (mData.getCharacter()->mCombatMovementManager.isTileInMoveRange(givenTile) && !characterOnTile(givenTile, mWorldData.getStage()->mCombatManager.mpCurCombatCharacters))
	{
		if (isPlayableTile(givenTile))
		{
			postTick(givenTile);
		}
	}
}

void MiniGamePlayerWaitForMoveInput::postTick(Tile& tileToMoveTo)
{
	mData.mNextMiniGameState = EMiniGameState_PLAYER_MOVE_CHARACTER;
	mData.mpTileToMoveTo = &tileToMoveTo;
	mData.getCharacter()->mCombatMovementManager.setCurTile(&tileToMoveTo);
}


MiniGamePlayerMoveCharacter::MiniGamePlayerMoveCharacter(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData) : MiniGameState(keyboardData, data, worldData){;}

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



MiniGamePlayerWaitForActionInput::MiniGamePlayerWaitForActionInput(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData) : MiniGameState(keyboardData, data, worldData){;}

void MiniGamePlayerWaitForActionInput::postTick(EMiniGameState nextStateEnum)
{
	if (nextStateEnum == EMiniGameState_BUFFER)
	{
		int index = -1;
		CombatCharacter* pNextCharacter = mWorldData.getStage()->mCombatManager.returnNextCharacter(*mData.getCharacter(), index);
		if (pNextCharacter == nullptr)
		{
			// all players stunned, must stall for rounds
			mData.mPostBufferGameState = EMiniGameState_BUFFER;
		}
		else
		{
			mData.setCharacter(pNextCharacter, index);
			mData.mPostBufferGameState = getPostBufferState(*pNextCharacter);
		}
	}
	mData.mNextMiniGameState = nextStateEnum;
}



MiniGamePlayerWaitForAttackInput::MiniGamePlayerWaitForAttackInput(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData) : MiniGameState(keyboardData, data, worldData){;}

void MiniGamePlayerWaitForAttackInput::postTick(Attack attack)
{
	mData.mpCurAttack = new Attack(attack);
	mData.mNextMiniGameState = attack.mRequiresDirectionInput ? EMiniGameState_PLAYER_WAIT_FOR_ATTACK_SUB_INPUT : EMiniGameState_PLAYER_TAKE_ACTION_ATTACK;
}



MiniGamePlayerWaitForAttackSubInput::MiniGamePlayerWaitForAttackSubInput(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData) : MiniGameState(keyboardData, data, worldData){;}

void MiniGamePlayerWaitForAttackSubInput::postTick(EDirection curAttackDirection)
{
	mData.mCurAttackDirection = curAttackDirection;
	mData.mNextMiniGameState = EMiniGameState_PLAYER_COMPLETE_DIRECTIONAL_ATTACK;
}



MiniGamePlayerCompleteDirectionalAttack::MiniGamePlayerCompleteDirectionalAttack(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData) : MiniGameState(keyboardData, data, worldData){;}

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
	Grid& grid = mWorldData.getStage()->mGrid;
	AttackAndCorrespondingTilesCoords attackTileCoords = returnAttackTileCoordsBasedOnAttackAndDirection(*mData.getCharacter()->mCombatMovementManager.getCurTile(), *mData.mpCurAttack, mData.mCurAttackDirection);
	std::vector <Tile* > tilesToAttack;
	for (const TileCoords& curTileCoord : attackTileCoords.mTileCoords)
	{
		Tile* pCurTile = grid.mpTiles[grid.getIndex(curTileCoord.mRow, curTileCoord.mCol)];
		tilesToAttack.push_back(pCurTile);
	}
	attackMultipleTiles(*mData.mpCurAttack, mData.mCurAttackDirection, mWorldData, tilesToAttack, mData.getCharacter());
	postTick();
}

void MiniGamePlayerCompleteDirectionalAttack::postTick()
{
	CombatManager& combatManager = mWorldData.getStage()->mCombatManager;

	combatManager.postTick();
	int index = -1;
	CombatCharacter* pNextCharacter = combatManager.returnNextCharacter(*mData.getCharacter(), index);
	if (pNextCharacter == nullptr)
	{
		// all players stunned, must stall for rounds
		mData.mPostBufferGameState = EMiniGameState_BUFFER;
	}
	else
	{
		mData.setCharacter(pNextCharacter, index);
		mData.mPostBufferGameState = getPostBufferState(*pNextCharacter);
	}
	mData.mTicks = 0;
	mData.mpTileToAttack = nullptr;
	mData.mpTilesToAttack.clear();
	mData.mNextMiniGameState = EMiniGameState_BUFFER;
	mData.mAttacked = true;
}



MiniGamePlayerTakeActionAttack::MiniGamePlayerTakeActionAttack(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData) : MiniGameState(keyboardData, data, worldData){;}

void MiniGamePlayerTakeActionAttack::selectTile(const Vect2 pos)
{
	Tile* pTile = mWorldData.getStage()->mGrid.getTileFromCoords(pos.getX(), pos.getY());

	if (pTile != nullptr)
	{
		pTile->setMode(EMiniGameCombatTileMode_SELECTED);
		std::vector <Tile*> tilesToAttack = { pTile };
		if (attackMultipleTiles(*mData.mpCurAttack, mData.mCurAttackDirection, mWorldData, tilesToAttack, mData.getCharacter()))
		{
			postTick();
		}
	}
}

void MiniGamePlayerTakeActionAttack::postTick()
{
	CombatManager& combatManager = mWorldData.getStage()->mCombatManager;
	combatManager.postTick();
	int index = -1;
	CombatCharacter* pNextCharacter = combatManager.returnNextCharacter(*mData.getCharacter(), index);
	if (pNextCharacter == nullptr)
	{
		// all players stunned, must stall for rounds
		mData.mPostBufferGameState = EMiniGameState_BUFFER;
	}
	else
	{
		mData.setCharacter(pNextCharacter, index);
		mData.mPostBufferGameState = getPostBufferState(*pNextCharacter);
	}
	mData.mTicks = 0;
	mData.mpTileToAttack = nullptr;
	mData.mpTilesToAttack.clear();
	mData.mNextMiniGameState = EMiniGameState_BUFFER;
	mData.mAttacked = true;
}



MiniGamePlayerTakeActionDefend::MiniGamePlayerTakeActionDefend(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData) : MiniGameState(keyboardData, data, worldData){;}

void MiniGamePlayerTakeActionDefend::tick()
{
	mData.getCharacter()->defend();
	postTick();
}

void MiniGamePlayerTakeActionDefend::postTick()
{
	CombatManager& combatManager = mWorldData.getStage()->mCombatManager;

	combatManager.postTick();
	int index = -1;
	CombatCharacter* pNextCharacter = combatManager.returnNextCharacter(*mData.getCharacter(), index);
	if (pNextCharacter == nullptr)
	{
		// all players stunned, must stall for rounds
		mData.mPostBufferGameState = EMiniGameState_BUFFER;
	}
	else
	{
		mData.setCharacter(pNextCharacter, index);
		mData.mPostBufferGameState = getPostBufferState(*pNextCharacter);
	}
	mData.mNextMiniGameState = EMiniGameState_BUFFER;
	mData.mDefended = true;
	mData.mTicks = 0;
}



MiniGameEnemyMoveCharacter::MiniGameEnemyMoveCharacter(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData) : MiniGameState(keyboardData, data, worldData){;}

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
	MiniGameStage*	pCurStage		= mWorldData.getStage();
	Grid&			grid			= pCurStage->mGrid;
	CombatManager&  combatManager	= pCurStage->mCombatManager;
	mData.mTicks = 0;

	// get all tiles that enemy can actually move to
	std::vector <Tile*> pAllPossibleMoveTiles;
	for (TileCoords& curTileCoords : mData.getCharacter()->mCombatMovementManager.getMoveTiles())
	{
		if (grid.isLegalCoords(curTileCoords.mRow, curTileCoords.mCol))
		{
			Tile *pCurTile = grid.mpTiles[grid.getIndex(curTileCoords.mRow, curTileCoords.mCol)];
			if (isPlayableTile(*pCurTile))
			{
				pAllPossibleMoveTiles.push_back(pCurTile);
			}
		}
	}

	// try to find a tile they can attack a player from
	int maxNumberOfCharactersCanAttack = 0;
	Tile* pBestTileToMoveTo = nullptr;

	for (Tile* pCurTile : pAllPossibleMoveTiles)
	{
		std::vector <AttackTile> allPossibleAttacksFromCurTile = returnAttackTileCoordsWithPlayersOnThem(mWorldData, pCurTile,  mData.getCharacter());
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

	// No tiles enemy can attack from
	// move closer to a player
	pAllPossibleMoveTiles = returnListWithoutTilesWithCharacters(combatManager, pAllPossibleMoveTiles);
	std::vector <TileDistance> tileDistances = returnListOfTileDistances(combatManager.mpCurCombatCharacters, pAllPossibleMoveTiles, mData.getCharacter());
	int minDistanceFromPlayer = std::numeric_limits<int>::max();
	Tile* pCurTile = mData.getCharacter()->mCombatMovementManager.getCurTile();

	for (TileDistance& curTileDistance : tileDistances)
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
	mData.mpTileLastMovedTo = mData.mpTileToMoveTo;
	mData.mpTileToMoveTo = nullptr;
}



MiniGameEnemyTakeAction:: MiniGameEnemyTakeAction(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData) : MiniGameState(keyboardData, data, worldData){;}

void MiniGameEnemyTakeAction::tick()
{
	
	if (mData.mTicks == int(mData.mTicksBeforeAction / 2))
	{
		mData.mGoingToAttack = shouldAttack();
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
			mData.mAttacked = true;
		}
		else if (shouldDefend())
		{
			mData.mDefended = true;
		}
		postTick();
	}
}

bool MiniGameEnemyTakeAction::shouldAttack()
{
	Tile* pRefTile = mData.getCharacter()->mCombatMovementManager.getCurTile();
	std::vector <AttackTile> attackTilesWithCharacters = returnAttackTileCoordsWithPlayersOnThem(mWorldData, pRefTile, mData.getCharacter());
	pRefTile = nullptr;

	Grid& grid = mWorldData.getStage()->mGrid;

	AttackTile *pCurBestAttackTile = nullptr;
	AttackAndCorrespondingTilesCoords curBestAttackTiles;
	float maxDamageOutput = 0;
	EDirection attackDir = EDirection_NONE;

	for (AttackTile& curAttackTile : attackTilesWithCharacters)
	{
		if (curAttackTile.mAttack.mRequiresDirectionInput)
		{
			EDirection attackDirection = getDirectionBetweenTiles(*mData.getCharacter()->mCombatMovementManager.getCurTile(), curAttackTile.mTile);
			AttackAndCorrespondingTilesCoords allAttackTiles = returnAttackTileCoordsBasedOnAttackAndDirection(*mData.getCharacter()->mCombatMovementManager.getCurTile(), curAttackTile.mAttack, attackDirection);
			for (int i = (int)allAttackTiles.mTileCoords.size() - 1;  i > -1; i --)
			{
				if (!grid.isLegalCoords(allAttackTiles.mTileCoords[i].mRow, allAttackTiles.mTileCoords[i].mCol))
				{
					allAttackTiles.mTileCoords.erase(allAttackTiles.mTileCoords.begin() + i);
				}
			}
			float damageOutput = allAttackTiles.mTileCoords.size() * allAttackTiles.mAttack.mDamagePercent * mData.getCharacter()->mCurAttackDamage;
			if (damageOutput > maxDamageOutput)
			{
				maxDamageOutput = damageOutput;
				curBestAttackTiles.mTileCoords.clear();
				curBestAttackTiles = allAttackTiles;
				pCurBestAttackTile = nullptr;
				attackDir = attackDirection;
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
				attackDir = EDirection_INVALID;
			}
		}
	}
	if (curBestAttackTiles.mTileCoords.size() == 0 and pCurBestAttackTile == nullptr)
	{
		return false;
	}

	if (curBestAttackTiles.mTileCoords.size() == 0 and pCurBestAttackTile != nullptr) // do single tile attack
	{
		mData.mpTilesToAttack.push_back(&pCurBestAttackTile->mTile);
		mData.mpCurAttack = new Attack(pCurBestAttackTile->mAttack);
	}
	else // do multi tile attack
	{
		mData.mpCurAttack = new Attack(curBestAttackTiles.mAttack);
		for (TileCoords& coords : curBestAttackTiles.mTileCoords)
		{
			mData.mpTilesToAttack.push_back(grid.mpTiles[grid.getIndex(coords.mRow, coords.mCol)]);
		}
	}

	mData.mCurAttackDirection = attackDir;
	return true;
}

bool MiniGameEnemyTakeAction::shouldDefend()
{
	MiniGameStage*	pStage			= mWorldData.getStage();
	Grid&			grid			= pStage->mGrid;
	CombatManager&	combatManager	= pStage->mCombatManager;
	Tile*			pCurTile		= mData.getCharacter()->mCombatMovementManager.getCurTile();

	std::vector <AttackTile> attackTileListWithCharacters = returnAttackTileCoordsWithPlayersOnThem(mWorldData, pCurTile, mData.getCharacter());
	for (AttackTile& attackTile : attackTileListWithCharacters)
	{
		Tile& curTileWithCharacter = attackTile.mTile;
		if (grid.isLegalCoords(curTileWithCharacter.mRow, curTileWithCharacter.mCol) and curTileWithCharacter.mRow == pCurTile->mRow and curTileWithCharacter.mCol and pCurTile->mCol)
		{
			return true;
		}
	}
	return false;
}

void MiniGameEnemyTakeAction::performAttack() { attackMultipleTiles(*mData.mpCurAttack, mData.mCurAttackDirection, mWorldData, mData.mpTilesToAttack, mData.getCharacter()); }

void MiniGameEnemyTakeAction::postTick()
{
	mData.mpTileLastMovedTo = nullptr;
	CombatManager& combatManager = mWorldData.getStage()->mCombatManager;
	combatManager.postTick();
	int index = -1;
	CombatCharacter* pNextCharacter = combatManager.returnNextCharacter(*mData.getCharacter(), index);
	if (pNextCharacter == nullptr)
	{
		// all players stunned, must stall for rounds
		mData.mPostBufferGameState = EMiniGameState_BUFFER;
	}
	else
	{
		mData.setCharacter(pNextCharacter, index);
		mData.mPostBufferGameState = getPostBufferState(*pNextCharacter);
	}

	mData.mNextMiniGameState = EMiniGameState_BUFFER;
	mData.mGoingToAttack = false;
	mData.mpTileToAttack = nullptr;
	mData.mpTilesToAttack.clear();
	mData.mTicks = 0;
}



MiniGameBuffer:: MiniGameBuffer(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData) : MiniGameState(keyboardData, data, worldData){;}

void MiniGameBuffer::tick()
{
	if (!mData.mTickYet)
	{
		mData.mTickYet = true;
	}
	else if (mData.mPostBufferGameState == EMiniGameState_BUFFER && mData.mTicks < mData.mTicksBeforeAction)
	{
		mData.mTicks++;
		mData.mForcePrintLog = true;
	}
	else
	{
		postTick();
	}
}

void MiniGameBuffer::postTick()
{
	mData.mTicks = 0;
	mData.mTickYet = false;

	mData.mAttacked = false;
	mData.mDefended = false;

	mData.mpCurAttack = nullptr;
	mData.mCurAttackDirection = EDirection_NONE;

	// check if game is over
	MiniGameLevel* pLevel = mWorldData.getLevel();
	MiniGameStage* pStage = mWorldData.getStage();
	GameOverStats stats = pStage->mCombatManager.getGameOverStats();
	if (stats.mGameOver)
	{
		if (!mWorldData.onLastStage() or mWorldData.onLastStage() and (pLevel->mNextLevelData.mType == ELevelType_MINI_GAME))
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
		if (mData.mPostBufferGameState == EMiniGameState_BUFFER)
		{
			
			// force tick a round, to drain turns to pass
			CombatManager& combatManager = pStage->mCombatManager;
			combatManager.tickAllAlive();
			int index = -1;
			CombatCharacter* pNextCharacter = combatManager.returnNextCharacter(*mData.getCharacter(), index, false);
			mData.setCharacter(pNextCharacter, index);
			if (pNextCharacter == nullptr)
			{
				mData.mForcePrintLog = true;
				// all players stunned, must continue to stall for rounds
				mData.mPostBufferGameState = EMiniGameState_BUFFER;
				mData.mTicks = 0;
				return;
			}
			else
			{
				mData.mNextMiniGameState = getPostBufferState(*pNextCharacter);;
			}
			pNextCharacter = nullptr;
		}
		else
		{
			mData.mNextMiniGameState = mData.mPostBufferGameState;
		}
	}

	// continue game / set up for next time
	mData.mPostBufferGameState = EMiniGameState_INVALID;
	pStage = nullptr;
	pLevel = nullptr;
}


MiniGameBuildNextLevel::MiniGameBuildNextLevel(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData) : MiniGameState(keyboardData, data, worldData) { ; }

MiniGameExit::MiniGameExit(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData) : MiniGameState(keyboardData, data, worldData) { ; }




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
	CombatManager& combatManager = mWorldData.getStage()->mCombatManager;
	combatManager.createCurCharacterList();
	mWorldData.resetStage();
	mData.mStateData.reset();
	mData.mStateData.setCharacter(combatManager.mpAllCombatCharacters[0], 0);
	combatManager.mpAllCombatCharacters[0]->preTick();
	mData.mLastFrameStateEnum = EMiniGameState_INVALID;
	mData.mCurStateEnum = EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT;
	mpCurState = mpStates[mData.mCurStateEnum];
	while (!mData.mPreviousStateDatas.empty())
	{
		mData.mPreviousStateDatas.top().~MiniGameStateData();
		mData.mPreviousStateDatas.pop();
		for (int i = (int)mData.mPreTickCharacterSnapShots.top().size() - 1; i > -1; i--)
		{
			mData.mPreTickCharacterSnapShots.top()[i].~CombatCharacterSnapShot();
			mData.mPreTickCharacterSnapShots.top().erase(mData.mPreTickCharacterSnapShots.top().begin() + i);
			
		}
		mData.mPreTickCharacterSnapShots.pop();
	}
	mData.mStateData.mDebugLine = " ";
	setUp = true;
}

void MiniGameStateManager::preTick() 
{
	MiniGameStage* pStage = mWorldData.getStage();
	if (mData.mLastFrameStateEnum != mData.mCurStateEnum)
	{
		// changed state
		mData.mPreTickCharacterSnapShots.push(createCombatCharacterSnapShots(pStage->mCombatManager));
		mData.mPreviousStateDatas.push(mData.mStateData);
	}
	mData.mLastFrameStateEnum = mData.mCurStateEnum; 
	
	pStage->preTick();
	pStage = nullptr;
}

void MiniGameStateManager::tick()
{
	if (!setUp) { start(); }
	mpCurState->tick();
}

void MiniGameStateManager::postTick()
{
	if (mData.mStateData.mNextMiniGameState != EMiniGameState_INVALID)
	{
		if (mData.mCurStateEnum != mData.mStateData.mNextMiniGameState)
		{
			createDebugLog();
		}
		else if (mData.mStateData.mForcePrintLog)
		{
			mData.mStateData.mDebugLine = "EVERYONE is stunned";
			mData.mStateData.mForcePrintLog = false;
		}
		
		mData.mCurStateEnum = mData.mStateData.mNextMiniGameState;
		mpCurState = mpStates[mData.mStateData.mNextMiniGameState];
	}
}

void MiniGameStateManager::printBoard(ScreenObject& screenObject, const StyleManager& styleManager)
{
	Grid& grid = mWorldData.getStage()->mGrid;
	updateTileColors(styleManager);
	grid.printGrid(screenObject.mpRenderer, screenObject.mGameScreenToGameLevelChunkRatio);
	printCharacters(screenObject, styleManager);
}

void MiniGameStateManager::printCharacters(ScreenObject& screenObject, const StyleManager& styleManager)
{
	Grid& grid = mWorldData.getStage()->mGrid;
	CombatManager& combatManager = mWorldData.getStage()->mCombatManager;
	for (CombatCharacter* pCurCombatCharacter : combatManager.mpCurCombatCharacters)
	{
		Tile* pCurTile = pCurCombatCharacter->mCombatMovementManager.getCurTile();

		// center x and align with bottom
		float printX		= (float(pCurTile->mCoords.mX1) + (float)(pCurTile->mCoords.mWidth - pCurCombatCharacter->mModel.mIdealImageWidth) / 2.0f) * screenObject.mGameScreenToGameLevelChunkRatio;
		float printY		= (float(pCurTile->mCoords.mY1) + (float)(pCurTile->mCoords.mHeight - pCurCombatCharacter->mModel.mIdealImageHeight))      * screenObject.mGameScreenToGameLevelChunkRatio;
		float printWidth	= float(pCurCombatCharacter->mModel.mIdealImageWidth) * screenObject.mGameScreenToGameLevelChunkRatio;
		float printHeight	= float(pCurCombatCharacter->mModel.mIdealImageHeight) * screenObject.mGameScreenToGameLevelChunkRatio;

		SDL_FRect curEntityPositionToPrintTo = { printX, printY, printWidth, printHeight };
		SDL_RenderTextureRotated(screenObject.mpRenderer, pCurCombatCharacter->mModel.getTexture(), NULL, &curEntityPositionToPrintTo, NULL, NULL, SDL_FLIP_NONE);
	}
}

void MiniGameStateManager::updateTileColors(const StyleManager& styleManager)
{
	Grid& grid = mWorldData.getStage()->mGrid;
	CombatManager& combatManager = mWorldData.getStage()->mCombatManager;

	// figure out if SELECTED OR HIGHLIGHTED
	for (Tile* pCurTile : grid.mpTiles)
	{
		SDL_Color curColor = pCurTile->mCurColor;
		SDL_Color colorToDraw = curColor;
		float alpha = 0.0f;
		if (pCurTile->getMode() == EMiniGameCombatTileMode_SELECTED)
		{
			alpha = .1f;
		}
		else if (pCurTile->getMode() == EMiniGameCombatTileMode_HIGHLIGHTED)
		{
			alpha = .35f;
		}
		SDL_Color curWhite{ 255, 255, 255, (Uint8)alpha };
		colorToDraw = blendColors(&curColor, &curWhite, alpha);

		pCurTile->setCurColor(colorToDraw);
	}

	// SHOW MOVE and ATTACK TILES
	for (CombatCharacter* pCurCombatCharacter : combatManager.mpCurCombatCharacters)
	{
		if (pCurCombatCharacter == mData.mStateData.getCharacter())
		{
			EMiniGameCombatActionType tileType = EMiniGameCombatActionType_MOVE;
			std::vector <TileCoords> tileCoordsList;
			switch (mData.mCurStateEnum)
			{
			case EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT:
			case EMiniGameState_ENEMY_MOVE_CHARACTER:
				tileCoordsList = pCurCombatCharacter->mCombatMovementManager.getMoveTiles();
				tileType = EMiniGameCombatActionType_MOVE;
				break;
			case EMiniGameState_PLAYER_TAKE_ACTION_ATTACK:
			case EMiniGameState_PLAYER_COMPLETE_DIRECTIONAL_ATTACK:
				if (mData.mStateData.mpCurAttack != nullptr)
				{
					if (mData.mStateData.mpCurAttack->mRequiresDirectionInput and (mData.mStateData.mCurAttackDirection != EDirection_NONE)
						and (mData.mStateData.mCurAttackDirection != EDirection_INVALID))
					{
						tileCoordsList = returnAttackTileCoordsBasedOnAttackAndDirection(*pCurCombatCharacter->mCombatMovementManager.getCurTile(),
							*mData.mStateData.mpCurAttack, mData.mStateData.mCurAttackDirection).mTileCoords;
					}
					else
					{
						tileCoordsList = returnAttackTileCoordsBasedOnAttack(*pCurCombatCharacter->mCombatMovementManager.getCurTile(), *mData.mStateData.mpCurAttack).mTileCoords;
					}
					tileType = EMiniGameCombatActionType_ATTACK;
				}
				break;
			case EMiniGameState_ENEMY_TAKE_ACTION:
				if (mData.mStateData.mGoingToAttack)
				{
					if (mData.mStateData.mpCurAttack->mRequiresDirectionInput)
					{
						tileCoordsList = returnAttackTileCoordsBasedOnAttackAndDirection(*pCurCombatCharacter->mCombatMovementManager.getCurTile(),
							*mData.mStateData.mpCurAttack, mData.mStateData.mCurAttackDirection).mTileCoords;
					}
					else
					{
						tileCoordsList = returnAttackTileCoordsBasedOnAttack(*pCurCombatCharacter->mCombatMovementManager.getCurTile(), *mData.mStateData.mpCurAttack).mTileCoords;
					}
					tileType = EMiniGameCombatActionType_ATTACK;
				}
				break;
			default:
				break;
			}

			tileCoordsList = removeDuplicateTiles(tileCoordsList);
			for (TileCoords& tileCoord : tileCoordsList)
			{
				int row = tileCoord.mRow;
				int col = tileCoord.mCol;

				if (grid.isLegalCoords(row, col))
				{
					Tile* curTile = grid.mpTiles[grid.getIndex(row, col)];
					SDL_Color& curColor = curTile->mCurColor;
					SDL_Color otherColor;
					switch (tileType)
					{
					case EMiniGameCombatActionType_MOVE:
						otherColor = styleManager.sunYellow;
						break;
					case EMiniGameCombatActionType_ATTACK:
						otherColor = styleManager.red;
						break;
					default:
						SDL_assert(false);
						break;
					}
					float alpha = .25;
					SDL_Color updatedColor = blendColors(&curColor, &otherColor, alpha);
					curTile->setCurColor(updatedColor);
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

void MiniGameStateManager::createDebugLog()
{
	// state data at the start of the tick
	MiniGameStateData& preTickStateData = mData.mPreviousStateDatas.top();
	// current state data is post tick
	std::string line;
	switch (mData.mCurStateEnum)
	{
	case EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT:
		line = mData.mStateData.getCharacter()->mName + " moved to " + std::to_string(mData.mStateData.mpTileToMoveTo->mRow) + ", " + std::to_string(mData.mStateData.mpTileToMoveTo->mCol);
		break;

	case EMiniGameState_PLAYER_COMPLETE_DIRECTIONAL_ATTACK:
	case EMiniGameState_PLAYER_TAKE_ACTION_ATTACK:
	case EMiniGameState_ENEMY_TAKE_ACTION:
		if (mData.mStateData.mAttacked)
		{
			line = preTickStateData.getCharacter()->mName + " choose to " + mData.mStateData.mpCurAttack->mName
				+ (mData.mStateData.mpCurAttack->mRequiresDirectionInput ? (" " + directionToString(mData.mStateData.mCurAttackDirection)) : "");

		}
		else if (mData.mStateData.mDefended)
		{
			// skip. Will be covered in character stat change section of the log
		}
		else
		{
			line = preTickStateData.getCharacter()->mName + " choose to pass.";
		}
		break;

	case EMiniGameState_PLAYER_WAIT_FOR_ACTION_INPUT:
		if (mData.mStateData.mNextMiniGameState == EMiniGameState_BUFFER)
		{
			line = preTickStateData.getCharacter()->mName + " choose to pass.";
		}
		break;

	case EMiniGameState_ENEMY_MOVE_CHARACTER:
		line = mData.mStateData.getCharacter()->mName + " moved to " + std::to_string(mData.mStateData.mpTileLastMovedTo->mRow) + ", " + std::to_string(mData.mStateData.mpTileLastMovedTo->mCol);
		break;	
	}

	std::string characterChanges =  getCharacterChangesString(mWorldData.getStage()->mCombatManager, mData.mPreTickCharacterSnapShots.top());
	if (line != "" && characterChanges != "")
	{
		line += ". " + characterChanges;
	}
	else if (characterChanges != "")
	{
		line = characterChanges;
	}

	
	if (line != "")
	{
		mData.mStateData.mDebugLine = line;
	}
}
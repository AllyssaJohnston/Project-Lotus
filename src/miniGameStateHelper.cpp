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
	if (mData.getCharacter()->mCombatMovementManager.isTileInMoveRange(givenTile) && !characterOnTile(givenTile, mWorldData.getStage()->mCombatManager.mpCurAliveCombatCharacters) && isPlayableTile(givenTile))
	{
		postTick(givenTile);
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
		setUpForBufferState(mWorldData, mData);
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
	std::vector <TileCoords> tileCoords = returnTileCoords(*mData.getCharacter()->mCombatMovementManager.getCurTile(), mData.mpCurAttack->mType, mData.mCurAttackDirection);
	std::vector <Tile* > pTilesToAttack;
	for (const TileCoords& curCoords : tileCoords)
	{
		Tile* pTile = findTile(mWorldData.getStage()->mGrid, curCoords);
		if (pTile!= nullptr)
		{
			pTilesToAttack.push_back(pTile);
		}
	}
	mWorldData.getStage()->mCombatManager.attackMultipleTiles(*mData.getCharacter(), pTilesToAttack, *mData.mpCurAttack);
	postTick();
}

void MiniGamePlayerCompleteDirectionalAttack::postTick()
{
	setUpForBufferState(mWorldData, mData);
	mData.mAttacked = true;
}



MiniGamePlayerTakeActionAttack::MiniGamePlayerTakeActionAttack(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData) : MiniGameState(keyboardData, data, worldData){;}

void MiniGamePlayerTakeActionAttack::selectTile(const Vect2 pos)
{
	Tile* pTile = mWorldData.getStage()->mGrid.getTileFromCoords(pos.getX(), pos.getY());

	if (pTile != nullptr && tileInAttackRange(*mData.mpCurAttack, mData.mCurAttackDirection, mWorldData.getStage()->mGrid, pTile, mData.getCharacter()->mCombatMovementManager.getCurTile()))
	{
		pTile->setMode(EMiniGameCombatTileMode_SELECTED);
		std::vector <Tile*> pTilesToAttack = { pTile };
		mWorldData.getStage()->mCombatManager.attackMultipleTiles(*mData.getCharacter(), pTilesToAttack, *mData.mpCurAttack);
		postTick();
	}
}

void MiniGamePlayerTakeActionAttack::postTick()
{
	setUpForBufferState(mWorldData, mData);
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
	setUpForBufferState(mWorldData, mData);
	mData.mDefended = true;
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
	MiniGameStage*	 pCurStage		= mWorldData.getStage();
	Grid&			 grid			= pCurStage->mGrid;
	CombatManager&   combatManager	= pCurStage->mCombatManager;
	CombatCharacter& curEnemy		= *mData.getCharacter();
	Tile&			 curEnemyTile	= *curEnemy.mCombatMovementManager.getCurTile();
	mData.mTicks = 0;

	// get all tiles that enemy can actually move to
	std::vector <Tile*> pAllPossibleMoveTiles;
	for (TileCoords& curTileCoords : curEnemy.mCombatMovementManager.getMoveTiles())
	{
		Tile* pCurTile = findTile(grid, curTileCoords);
		if (pCurTile != nullptr && isPlayableTile(*pCurTile) && !characterOnTile(*pCurTile, combatManager.mpCurAliveCombatCharacters))
		{
			pAllPossibleMoveTiles.push_back(pCurTile);
		}
	}

	// now try to find the best tile they can attack from
	int maxNumberOfCharactersCanAttack = 0;
	int minDistanceFromPlayer = std::numeric_limits<int>::max();
	Tile* pBestTileToMoveTo = nullptr;

	for (Tile* pMoveTile : pAllPossibleMoveTiles)
	{
		std::vector<Tile*> playerTiles = returnTilesFromAttacksWithPlayersOnThem(mWorldData, pMoveTile, curEnemy.mCombatMovementManager.getAttacks(), EDirection_ALL);
		int curNumCharactersCanAttack = (int)playerTiles.size();
		for (Tile* pPlayerTile : playerTiles)
		{
			int curDistance = getDistanceBetweenTiles(*pMoveTile, *pPlayerTile);
			if (curNumCharactersCanAttack > maxNumberOfCharactersCanAttack)
			{
				// can attack more things
				pBestTileToMoveTo = pMoveTile;
				maxNumberOfCharactersCanAttack = curNumCharactersCanAttack;
				minDistanceFromPlayer = curDistance;
			}
			else if (curDistance < minDistanceFromPlayer)
			{
				// shorter distance to move
				pBestTileToMoveTo = pMoveTile;
				minDistanceFromPlayer = curDistance;
			}
		}
		
	}
	if (pBestTileToMoveTo != nullptr)
	{
		mData.mpTileToMoveTo = pBestTileToMoveTo;
		return;
	}

	// No tiles enemy can attack from
	// move closer to a player
	std::vector <TileDistance> tileDistances = returnListOfTileDistances(combatManager.mpCurAliveCombatCharacters, pAllPossibleMoveTiles, &curEnemy);
	minDistanceFromPlayer = std::numeric_limits<int>::max();
	Tile* pCurTile = &curEnemyTile;
	for (TileDistance& curTileDistance : tileDistances)
	{
		if (minDistanceFromPlayer > curTileDistance.mDistance)
		{
			minDistanceFromPlayer = (int)curTileDistance.mDistance;
			pCurTile = &curTileDistance.mTile2;
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
	Grid& grid				= mWorldData.getStage()->mGrid;
	Attack* pBestAttack		= nullptr;
	std::vector<Tile*> pBestTilesToAttack;
	float maxDamageOutput	= -1.0f;
	EDirection attackDir	= EDirection_NONE;

	// figure out if enemy can attack any characters
	// choose the attack with the highest damage output
	for (Attack& attack : mData.getCharacter()->mCombatMovementManager.getAttacks())
	{
		if (attack.mRequiresDirectionInput)
		{
			// choose direction that can attack the most characters
			std::vector <Tile*> pTilesToAttackWithCharacters;
			EDirection curBestDirection = EDirection_INVALID;
			for (int i = 0; i < 4; i++)
			{
				std::vector <Tile*> pCurTilesToAttackWithCharacters = returnTilesFromAttackWithPlayersOnThem(mWorldData, mData.getCharacter()->mCombatMovementManager.getCurTile(), attack, (EDirection)i);
				if (pCurTilesToAttackWithCharacters.size() > pTilesToAttackWithCharacters.size())
				{
					pTilesToAttackWithCharacters = pCurTilesToAttackWithCharacters;
					curBestDirection = (EDirection)i;
				}

			}
			
			float damageOutput = pTilesToAttackWithCharacters.size() * attack.mDamagePercent * mData.getCharacter()->getCurDamage();
			if (damageOutput > maxDamageOutput && !(pTilesToAttackWithCharacters.size() == 0))
			{
				maxDamageOutput = damageOutput;
				pBestTilesToAttack = pTilesToAttackWithCharacters;
				pBestAttack = &attack;
				attackDir = curBestDirection;
			}
		}
		else
		{
			std::vector <Tile*> pTilesToAttackWithCharacters = returnTilesFromAttackWithPlayersOnThem(mWorldData, mData.getCharacter()->mCombatMovementManager.getCurTile(), attack, EDirection_ALL);
			float damageOutput = pTilesToAttackWithCharacters.size() * attack.mDamagePercent * mData.getCharacter()->getCurDamage();
			if (damageOutput > maxDamageOutput && !(pTilesToAttackWithCharacters.size() == 0))
			{
				maxDamageOutput		= damageOutput;
				pBestAttack			= &attack;
				pBestTilesToAttack	= pTilesToAttackWithCharacters;
				attackDir			= EDirection_ALL;
			}
		}
	}
	

	if (pBestTilesToAttack.size() == 0)
	{
		return false;
	}

	mData.mpCurAttack			= pBestAttack;
	mData.mpTilesToAttack		= pBestTilesToAttack;
	mData.mCurAttackDirection	= attackDir;
	return true;
}

bool MiniGameEnemyTakeAction::shouldDefend()
{
	MiniGameStage*	pStage			= mWorldData.getStage();
	Grid&			grid			= pStage->mGrid;
	CombatManager&	combatManager	= pStage->mCombatManager;
	Tile*			pCurTile		= mData.getCharacter()->mCombatMovementManager.getCurTile();

	for (CombatCharacter* pCharacter : combatManager.mpCurAliveCombatCharacters)
	{
		if (pCharacter->mType != EMiniGameCombatCharacterType_PLAYER || pCharacter->getStuns() > 0)
		{
			continue;
		}
		for (Attack& attack : pCharacter->mCombatMovementManager.getAttacks())
		{
			if (tileInAttackRange(attack, EDirection_ALL, grid, pCurTile, pCharacter->mCombatMovementManager.getCurTile()) && attack.mDamagePercent > 0)
			{
				// enemy is in this player's attack range
				return true;
			}
		}
	}
	return false;
}

void MiniGameEnemyTakeAction::performAttack() { mWorldData.getStage()->mCombatManager.attackMultipleTiles(*mData.getCharacter(), mData.mpTilesToAttack, *mData.mpCurAttack); }

void MiniGameEnemyTakeAction::postTick()
{
	mData.mpTileLastMovedTo = nullptr;
	setUpForBufferState(mWorldData, mData);
}



MiniGameCharacterStunned::MiniGameCharacterStunned(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData) : MiniGameState(keyboardData, data, worldData) { ; }

void MiniGameCharacterStunned::tick()
{

	if (mData.mTicks < mData.mTicksBeforeAction)
	{
		mData.mTicks += 1;
	}
	else
	{
		postTick();
	}
}

void MiniGameCharacterStunned::postTick() { setUpForBufferState(mWorldData, mData); }




MiniGameBuffer:: MiniGameBuffer(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData) : MiniGameState(keyboardData, data, worldData){;}

void MiniGameBuffer::tick()
{
	if (!mData.mTickYet)
	{
		mData.mTickYet = true;
	}
	else if (mData.mTicks < (mData.mTicksBeforeAction * 2))
	{
		mData.mTicks++;
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
		mData.mNextMiniGameState = mData.mPostBufferGameState;
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
	mpStates = {new MiniGamePlayerWaitForMoveInput(			keyboardData, mData.mStateData, mWorldData),
				new MiniGamePlayerMoveCharacter(			keyboardData, mData.mStateData, mWorldData),
				new MiniGamePlayerWaitForActionInput(		keyboardData, mData.mStateData, mWorldData),
				new MiniGamePlayerWaitForAttackInput(		keyboardData, mData.mStateData, mWorldData),
				new MiniGamePlayerWaitForAttackSubInput(	keyboardData, mData.mStateData, mWorldData),
				new MiniGamePlayerCompleteDirectionalAttack(keyboardData, mData.mStateData, mWorldData),
				new MiniGamePlayerTakeActionAttack(			keyboardData, mData.mStateData, mWorldData),
				new MiniGamePlayerTakeActionDefend(			keyboardData, mData.mStateData, mWorldData),
				new MiniGameEnemyMoveCharacter(				keyboardData, mData.mStateData, mWorldData),
				new MiniGameEnemyTakeAction(				keyboardData, mData.mStateData, mWorldData),
				new MiniGameCharacterStunned(				keyboardData, mData.mStateData, mWorldData),
				new MiniGameBuffer(							keyboardData, mData.mStateData, mWorldData),
				new MiniGameExit(							keyboardData, mData.mStateData, mWorldData),
				new MiniGameBuildNextLevel(					keyboardData, mData.mStateData, mWorldData) 
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
	combatManager.createCurAliveCharacterList();
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
	if (!setUp) 
	{ 
		start(); 
	}
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
	for (CombatCharacter* pCurCombatCharacter : combatManager.mpCurAliveCombatCharacters)
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
	Grid& grid						= mWorldData.getStage()->mGrid;
	CombatManager& combatManager	= mWorldData.getStage()->mCombatManager;

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
	CombatCharacter* pCurCombatCharacter = mData.mStateData.getCharacter();
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
	case EMiniGameState_ENEMY_TAKE_ACTION:
		tileType = EMiniGameCombatActionType_ATTACK;
		if (mData.mStateData.mpCurAttack != nullptr)
		{
			if (mData.mStateData.mpCurAttack->mRequiresDirectionInput and mData.mStateData.mCurAttackDirection != EDirection_NONE and mData.mStateData.mCurAttackDirection != EDirection_INVALID)
			{
				tileCoordsList = returnTileCoords(*pCurCombatCharacter->mCombatMovementManager.getCurTile(), mData.mStateData.mpCurAttack->mType, mData.mStateData.mCurAttackDirection);
			}
			else
			{
				tileCoordsList = returnTileCoords(*pCurCombatCharacter->mCombatMovementManager.getCurTile(), mData.mStateData.mpCurAttack->mType, EDirection_ALL);
			}
		}
		break;
	default:
		break;
	}

	for (TileCoords& tileCoord : tileCoordsList)
	{
		Tile* pTile = findTile(grid, tileCoord);
		if (pTile != nullptr && isPlayableTile(*pTile))
		{
			SDL_Color& curColor = pTile->mCurColor;
			SDL_Color otherColor;
			switch (tileType)
			{
			case EMiniGameCombatActionType_MOVE:
				if (characterOnTile(*pTile, combatManager.mpCurAliveCombatCharacters))
				{
					continue;
				}
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
			pTile->setCurColor(updatedColor);
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
	case EMiniGameState_BUFFER:
		if (mData.mStateData.mNextMiniGameState == EMiniGameState_CHARACTER_STUNNED)
		{
			line = preTickStateData.getCharacter()->mName + " is stunned";
		}
		
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
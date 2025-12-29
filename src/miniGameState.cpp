#include "miniGameState.h"

MiniGameState::MiniGameState(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData) : mKeyboardData(keyboardData), mData(data), mWorldData(worldData) {}

void MiniGameState::useMouseInput(EMiniGameState curStateEnum, ScreenObject& screenObject) 
{
	if (curStateEnum == EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT or curStateEnum == EMiniGameState_PLAYER_WAIT_FOR_ATTACK_TILE_INPUT)
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
	grid.setMouseTileMode(EMiniGameCombatTileMode_NOT_SELECTED);
	grid.setMouseTile(pos.getX(), pos.getY());
	Tile* pMouseTile = grid.getMouseTile();
	if (pMouseTile != nullptr)
	{
		if (pMouseTile->getMode() != EMiniGameCombatTileMode_SELECTED)
		{
			pMouseTile->setMode(EMiniGameCombatTileMode_HIGHLIGHTED);
		}
	}
}


MiniGamePlayerWaitForMoveInput::MiniGamePlayerWaitForMoveInput(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData) : MiniGameState(keyboardData, data, worldData){;}

void MiniGamePlayerWaitForMoveInput::selectTile(const Vect2 pos)
{
	Grid& grid = mWorldData.getStage()->mGrid;
	grid.setMouseTile(pos.getX(), pos.getY());
	Tile* pMouseTile = grid.getMouseTile();
	if (pMouseTile != nullptr && isPlayableTile(*pMouseTile))
	{
		pMouseTile->setMode(EMiniGameCombatTileMode_SELECTED);
		moveToTile(*pMouseTile);
	}
}

void MiniGamePlayerWaitForMoveInput::moveToTile(Tile& givenTile)
{
	mData.getCharacter()->mCombatMovementManager.setMoveTiles();
	if (mData.getCharacter()->mCombatMovementManager.isTileInMoveRange(givenTile) && !mWorldData.getStage()->mCombatManager.characterOnTile(givenTile) && isPlayableTile(givenTile))
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



MiniGamePlayerWaitForAttackOptionInput::MiniGamePlayerWaitForAttackOptionInput(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData) : MiniGameState(keyboardData, data, worldData){;}

void MiniGamePlayerWaitForAttackOptionInput::postTick(Attack& attack)
{
	mData.mpCurAttack = &attack;
	if (attack.mRequiresDirectionInput)
	{
		mData.mNextMiniGameState = EMiniGameState_PLAYER_WAIT_FOR_ATTACK_DIRECTION_INPUT;
	}
	else
	{
		switch (attack.mType)
		{
		case EMiniGameCombatMoveAttackTypes_WHOLE_GRID:
			mData.mCurAttackDirection = EDirection_ALL;
			mData.mNextMiniGameState = EMiniGameState_PLAYER_COMPLETE_ACTION_ATTACK;
			break;
		case EMiniGameCombatMoveAttackTypes_ANY_ONE_TILE:
			mData.mNextMiniGameState = EMiniGameState_PLAYER_WAIT_FOR_ATTACK_CHARACTER_INPUT;
			mData.mTargetCharacterType = getCharacterTypeFromAttackTargetType(mData.mpCurAttack->mAttackTargetType);
			break;
		default:
			mData.mCurAttackDirection = EDirection_ALL;
			mData.mNextMiniGameState = EMiniGameState_PLAYER_WAIT_FOR_ATTACK_TILE_INPUT;
			break;
		}
	}
	
}



MiniGamePlayerWaitForAttackDirectionInput::MiniGamePlayerWaitForAttackDirectionInput(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData) : MiniGameState(keyboardData, data, worldData){;}

void MiniGamePlayerWaitForAttackDirectionInput::postTick(EDirection curAttackDirection)
{
	mData.mCurAttackDirection = curAttackDirection;
	mData.mNextMiniGameState = EMiniGameState_PLAYER_COMPLETE_ACTION_ATTACK;
}


MiniGamePlayerWaitForAttackTileInput::MiniGamePlayerWaitForAttackTileInput(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData) : MiniGameState(keyboardData, data, worldData) { ; }

void MiniGamePlayerWaitForAttackTileInput::selectTile(const Vect2 pos)
{
	Grid& grid = mWorldData.getStage()->mGrid;
	grid.setMouseTile(pos.getX(), pos.getY());
	Tile* pMouseTile = grid.getMouseTile();
	if (pMouseTile != nullptr && tileInAttackRange(*mData.mpCurAttack, mData.mCurAttackDirection, mWorldData.getStage()->mGrid, pMouseTile, mData.getCharacter()->mCombatMovementManager.getCurTile()))
	{
		pMouseTile->setMode(EMiniGameCombatTileMode_SELECTED);
		mData.mpTilesToAttack = { pMouseTile };
		postTick();
	}
}

void MiniGamePlayerWaitForAttackTileInput::postTick() { mData.mNextMiniGameState = EMiniGameState_PLAYER_COMPLETE_ACTION_ATTACK; }


MiniGamePlayerWaitForAttackCharacterInput::MiniGamePlayerWaitForAttackCharacterInput(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData) : MiniGameState(keyboardData, data, worldData) { ; }

void MiniGamePlayerWaitForAttackCharacterInput::postTick(CombatCharacter* pTarget)
{
	if (pTarget->mType == mData.mTargetCharacterType)
	{
		mData.mpTargetCharacter = pTarget;
		mData.mNextMiniGameState = EMiniGameState_PLAYER_COMPLETE_ACTION_ATTACK;
	}
}


MiniGamePlayerCompleteActionAttack::MiniGamePlayerCompleteActionAttack(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData) : MiniGameState(keyboardData, data, worldData){;}

void MiniGamePlayerCompleteActionAttack::tick()
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

void MiniGamePlayerCompleteActionAttack::attackTiles()
{
	CombatManager& combatManager = mWorldData.getStage()->mCombatManager;
	if (mData.mpTargetCharacter != nullptr)
	{
		combatManager.attack(*mData.getCharacter(), *mData.mpTargetCharacter, *mData.mpCurAttack);
	}
	else
	{
		Grid& grid = mWorldData.getStage()->mGrid;
		std::vector <Tile* > pTilesToAttack;
		if (mData.mpCurAttack->mType == EMiniGameCombatMoveAttackTypes_WHOLE_GRID)
		{
			pTilesToAttack = grid.mpTiles;
		}
		else if (mData.mpTilesToAttack.size() > 0)
		{
			pTilesToAttack = mData.mpTilesToAttack;
		}
		else
		{
			std::vector <TileCoords> tileCoords = returnTileCoords(*mData.getCharacter()->mCombatMovementManager.getCurTile(), mData.mpCurAttack->mType, mData.mpCurAttack->mNum, mData.mpCurAttack->mOut, mData.mCurAttackDirection);
			for (const TileCoords& curCoords : tileCoords)
			{
				Tile* pTile = grid.findTile(curCoords);
				if (pTile != nullptr)
				{
					pTilesToAttack.push_back(pTile);
				}
			}
		}
		combatManager.attackMultipleTiles(*mData.getCharacter(), pTilesToAttack, *mData.mpCurAttack);
	}
	postTick();
}

void MiniGamePlayerCompleteActionAttack::postTick()
{
	setUpForBufferState(mWorldData, mData);
	mData.mAttacked = true;
}





MiniGamePlayerCompleteActionDefend::MiniGamePlayerCompleteActionDefend(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData) : MiniGameState(keyboardData, data, worldData){;}

void MiniGamePlayerCompleteActionDefend::tick()
{
	mData.getCharacter()->defend();
	postTick();
}

void MiniGamePlayerCompleteActionDefend::postTick()
{
	setUpForBufferState(mWorldData, mData);
	mData.mDefended = true;
}


MiniGamePlayerCompleteActionHeal::MiniGamePlayerCompleteActionHeal(KeyboardData& keyboardData, MiniGameStateData& data, MiniGameWorldData& worldData) : MiniGameState(keyboardData, data, worldData) { ; }

void MiniGamePlayerCompleteActionHeal::tick()
{
	// self heal
	mData.getCharacter()->heal(mData.getCharacter()->getHealAmount());
	postTick();
}

void MiniGamePlayerCompleteActionHeal::postTick()
{
	setUpForBufferState(mWorldData, mData);
	mData.mHealed = true;
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
	for (TileCoords& curTileCoords : curEnemy.mCombatMovementManager.getMoveTileCoords())
	{
		Tile* pCurTile = grid.findTile(curTileCoords);
		if (pCurTile != nullptr && isPlayableTile(*pCurTile) && !combatManager.characterOnTile(*pCurTile))
		{
			pAllPossibleMoveTiles.push_back(pCurTile);
		}
	}

	// now try to find the best tile they can attack from
	int maxNumberOfCharactersCanAttack = 0;
	float minDistanceFromPlayer = FLT_MAX;
	Tile* pBestTileToMoveTo = nullptr;
	for (Tile* pMoveTile : pAllPossibleMoveTiles)
	{
		std::vector<Tile*> playerTiles = returnTilesFromAttacksWithPlayersOnThem(mWorldData, pMoveTile, curEnemy.mCombatMovementManager.getAttacks(), EDirection_ALL);
		int curNumCharactersCanAttack = (int)playerTiles.size();
		for (Tile* pPlayerTile : playerTiles)
		{
			float curDistance = getDistanceBetweenTiles(*pMoveTile, *pPlayerTile);
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
	minDistanceFromPlayer = FLT_MAX;
	Tile* pCurTileToMoveTo = &curEnemyTile;
	for (CombatCharacter* pCurCharacter : combatManager.getCurAlivePlayers())
	{
		for (Tile* pCurTile : pAllPossibleMoveTiles)
		{
			float distance = getDistanceBetweenTiles(*pCurTile, *pCurCharacter->mCombatMovementManager.getCurTile());
			if (distance < minDistanceFromPlayer)
			{
				minDistanceFromPlayer = distance;
				pCurTileToMoveTo = pCurTile;
			}
		}
	}
	mData.mpTileToMoveTo = pCurTileToMoveTo;
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
	if (!shouldHeal() && mData.mTicks == int(mData.mTicksBeforeAction / 2))
	{
		mData.mGoingToAttack = shouldAttack();
	}

	if (mData.mTicks < mData.mTicksBeforeAction)
	{
		mData.mTicks += 1;
	}
	else
	{
		if (shouldHeal())
		{
			mData.getCharacter()->heal(mData.getCharacter()->getHealAmount());
			mData.mHealed = true;
		}
		else if (mData.mGoingToAttack)
		{
			performAttack();
			mData.mAttacked = true;
		}
		else if (shouldDefend())
		{
			mData.getCharacter()->defend();
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
		if (!attack.canUse())
		{
			continue;
		}
		else if (attack.mType == EMiniGameCombatMoveAttackTypes_ANY_ONE_TILE)
		{
			CombatCharacter* characterWithLowestHealth = nullptr;
			// choose tile with player with lowest health
			for (CombatCharacter* pCurCharacterToTest : mWorldData.getStage()->mCombatManager.getCurAliveCharacters())
			{
				if (pCurCharacterToTest->mType == EMiniGameCombatCharacterType_PLAYER && pCurCharacterToTest->getCurHealth() > characterWithLowestHealth->getCurHealth())
				{
					characterWithLowestHealth = pCurCharacterToTest;
				}
			}
			pBestTilesToAttack = { characterWithLowestHealth->mCombatMovementManager.getCurTile() };
		}
		else if (attack.mRequiresDirectionInput)
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
	MiniGameStage* pStage = mWorldData.getStage();
	Grid& grid = pStage->mGrid;
	CombatManager& combatManager = pStage->mCombatManager;
	Tile* pCurTile = mData.getCharacter()->mCombatMovementManager.getCurTile();

	for (CombatCharacter* pCharacter : combatManager.getCurAlivePlayers())
	{
		if (pCharacter->getStuns() > 0)
		{
			// potential attacker can't attack
			continue;
		}
		for (const Attack& attack : pCharacter->mCombatMovementManager.getAttacks())
		{
			if (attack.mCurCooldown != 0 || !characterTypeFit(attack.mAttackTargetType, EMiniGameCombatCharacterType_ENEMY, true))
			{
				// potential attacker can't use this attack
				continue;
			}
			if (tileInAttackRange(attack, EDirection_ALL, grid, pCurTile, pCharacter->mCombatMovementManager.getCurTile()) && attack.mDamagePercent > 0)
			{
				// enemy is in this player's attack range
				return true;
			}
		}
	}
	return false;
}

// can only heal if heal amount is greater than zero
bool MiniGameEnemyTakeAction::shouldHeal() { return mData.getCharacter()->getHealAmount() > 0 && mData.getCharacter()->returnIsLowHealth(); }

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
	mData.mpTilesToAttack.clear();
	mData.mpTargetCharacter = nullptr;
	mData.mCurAttackDirection = EDirection_NONE;

	// check if game is over
	MiniGameLevel* pLevel = mWorldData.getLevel();
	MiniGameStage* pStage = mWorldData.getStage();
	GameOverStats stats = pStage->mCombatManager.getGameOverStats();
	if (stats.mGameOver)
	{
		if (!stats.mWonGame)
		{
			mData.mNextMiniGameState = EMiniGameState_EXIT;
			mData.mNeedToReset = true;
		}
		else
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



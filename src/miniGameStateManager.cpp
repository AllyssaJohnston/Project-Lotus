#include "miniGameStateManager.h"

MiniGameStateManager::MiniGameStateManager(KeyboardData& keyboardData, MiniGameWorldData& miniGameWorldData) : mWorldData(miniGameWorldData)
{
	mpStates = { new MiniGamePlayerWaitForMoveInput(keyboardData, mData.mStateData, mWorldData),
				new MiniGamePlayerMoveCharacter(keyboardData, mData.mStateData, mWorldData),
				new MiniGamePlayerWaitForActionInput(keyboardData, mData.mStateData, mWorldData),
				new MiniGamePlayerWaitForAttackOptionInput(keyboardData, mData.mStateData, mWorldData),
				new MiniGamePlayerWaitForAttackDirectionInput(keyboardData, mData.mStateData, mWorldData),
				new MiniGamePlayerWaitForAttackTileInput(keyboardData, mData.mStateData, mWorldData),
				new MiniGamePlayerWaitForAttackCharacterInput(keyboardData, mData.mStateData, mWorldData),
				new MiniGamePlayerCompleteActionAttack(keyboardData, mData.mStateData, mWorldData),
				new MiniGamePlayerCompleteActionDefend(keyboardData, mData.mStateData, mWorldData),
				new MiniGamePlayerCompleteActionHeal(keyboardData, mData.mStateData, mWorldData),
				new MiniGameEnemyMoveCharacter(keyboardData, mData.mStateData, mWorldData),
				new MiniGameEnemyTakeAction(keyboardData, mData.mStateData, mWorldData),
				new MiniGameCharacterStunned(keyboardData, mData.mStateData, mWorldData),
				new MiniGameBuffer(keyboardData, mData.mStateData, mWorldData),
				new MiniGameExit(keyboardData, mData.mStateData, mWorldData),
				new MiniGameBuildNextLevel(keyboardData, mData.mStateData, mWorldData)
	};
	mData.mCurStateEnum = EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT;

	mpCurState = mpStates[mData.mCurStateEnum];
}

MiniGameStateManager::~MiniGameStateManager()
{
	for (int i = (int)mpStates.size() - 1; i > -1; i--)
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
	CombatCharacter* pCharacter = combatManager.getFromAllCharacters(0);
	mData.mStateData.setCharacter(pCharacter, 0);
	pCharacter->preTick();
	mData.mLastFrameStateEnum = EMiniGameState_INVALID;
	mData.mCurStateEnum = EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT;
	mpCurState = mpStates[mData.mCurStateEnum];
	while (!mData.mPreviousStateDatas.empty())
	{
		mData.mPreviousStateDatas.top().second.~MiniGameStateData();
		mData.mPreviousStateDatas.pop();
		mData.mPreTickCharacters.pop();
	}
	mData.mStateData.mDebugLine = " ";
	setUp = true;
	pCharacter = nullptr;
}

void MiniGameStateManager::preTick()
{
	MiniGameStage& stage = *mWorldData.getStage();
	CombatManager& combatManager = stage.mCombatManager;
	if (mData.mLastFrameStateEnum != mData.mCurStateEnum || mData.mPreviousStateDatas.empty())
	{
		// changed state
		mData.mPreTickCharacters.push(combatManager.createCombatCharacterSnapShots());
		mData.mPreviousStateDatas.push(std::pair<EMiniGameState, MiniGameStateData>(mData.mCurStateEnum, mData.mStateData));
		mData.mRoundNum.push(combatManager.getRoundNum());
	}
	mData.mLastFrameStateEnum = mData.mCurStateEnum;

	stage.preTick();
}

void MiniGameStateManager::tick()
{
	if (!setUp)
	{
		start();
	}
	mpCurState->tick();
	mData.mTicksSinceUndo++;
}

void MiniGameStateManager::postTick()
{
	if (mData.mStateData.mNextMiniGameState != EMiniGameState_INVALID)
	{
		if (mData.mStateData.mNextMiniGameState == EMiniGameState_EXIT && mData.mStateData.mNeedToReset)
		{
			start();
			return;
		}
		if (mData.mCurStateEnum != mData.mStateData.mNextMiniGameState && mData.mStateData.mNextMiniGameState != EMiniGameState_EXIT && mData.mStateData.mNextMiniGameState != EMiniGameState_BUILD_NEXT_LEVEL)
		{
			createDebugLog();
		}

		mData.mCurStateEnum = mData.mStateData.mNextMiniGameState;
		mpCurState = mpStates[mData.mStateData.mNextMiniGameState];
	}
}

void MiniGameStateManager::undo()
{
	bool doneOnce = false;
	if (mData.mPreviousStateDatas.empty())
	{
		return;
	}
	mData.mPreviousStateDatas.pop();
	mData.mPreTickCharacters.pop();

	while (!mData.mPreviousStateDatas.empty() && (!isWaitForInput(mData.mCurStateEnum) || !doneOnce))
	{
		doneOnce = true;
		mData.mCurStateEnum = mData.mPreviousStateDatas.top().first;
		mData.mStateData = mData.mPreviousStateDatas.top().second;
		mData.mPreviousStateDatas.pop();
		mData.mLastFrameStateEnum = mData.mPreviousStateDatas.empty() ? EMiniGameState_INVALID : mData.mPreviousStateDatas.top().first;

		std::vector<CombatCharacter>& preTickCharacters = mData.mPreTickCharacters.top();
		CombatManager& combatManager = mWorldData.getStage()->mCombatManager;
		for (int i = 0; i < (int)combatManager.getAllCharacters().size(); i++)
		{
			combatManager.getFromAllCharacters(i)->revertToState(preTickCharacters[i]);
		}
		mData.mPreTickCharacters.pop();
		mData.mTicksSinceUndo++;
		combatManager.setRound(mData.mRoundNum.top());
		mData.mRoundNum.pop();
	}
	updateCurState(mData.mCurStateEnum);
	mData.mTicksSinceUndo = 0;
}

void MiniGameStateManager::printBoard(ScreenObject& screenObject)
{
	updateTileColors();
	mWorldData.getStage()->mGrid.printGrid(screenObject.mpRenderer, screenObject.mGameScreenToGameLevelChunkRatio);
	printCharacters(screenObject);
}

void MiniGameStateManager::printCharacters(ScreenObject& screenObject)
{
	CombatManager& combatManager = mWorldData.getStage()->mCombatManager;
	// characters in play
	// sort characters by row, so that characters on rows further down print over characters on rows further up
	std::vector<CombatCharacter*> pCharactersInPrintOrder = combatManager.getCurAliveCharacters();
	std::sort(pCharactersInPrintOrder.begin(), pCharactersInPrintOrder.end(), SortCharacterByTileRow());
	for (CombatCharacter* pCurCombatCharacter : pCharactersInPrintOrder)
	{
		Tile* pCurTile = pCurCombatCharacter->mCombatMovementManager.getCurTile();

		// center x and align with bottom
		float printX = (float(pCurTile->mCoords.mX1) + (float)(pCurTile->mCoords.mWidth - pCurCombatCharacter->mModel.mIdealImageWidth) / 2.0f) * screenObject.mGameScreenToGameLevelChunkRatio;
		float printY = (float(pCurTile->mCoords.mY1) + (float)(pCurTile->mCoords.mHeight - pCurCombatCharacter->mModel.mIdealImageHeight)) * screenObject.mGameScreenToGameLevelChunkRatio;
		float printWidth = float(pCurCombatCharacter->mModel.mIdealImageWidth) * screenObject.mGameScreenToGameLevelChunkRatio;
		float printHeight = float(pCurCombatCharacter->mModel.mIdealImageHeight) * screenObject.mGameScreenToGameLevelChunkRatio;

		SDL_FRect curEntityPositionToPrintTo = { printX, printY, printWidth, printHeight };
		SDL_RenderTextureRotated(screenObject.mpRenderer, pCurCombatCharacter->mModel.getTexture(), NULL, &curEntityPositionToPrintTo, NULL, NULL, SDL_FLIP_NONE);
	}

	// ghost enemies
	pCharactersInPrintOrder = combatManager.getGhostEnemies();
	std::sort(pCharactersInPrintOrder.begin(), pCharactersInPrintOrder.end(), SortCharacterByTileRow());
	Uint8 alpha = 128; // 50% transparency
	for (CombatCharacter* pCurCombatCharacter : pCharactersInPrintOrder)
	{
		Tile* pCurTile = pCurCombatCharacter->mCombatMovementManager.getCurTile();

		// center x and align with bottom
		float printX = (float(pCurTile->mCoords.mX1) + (float)(pCurTile->mCoords.mWidth - pCurCombatCharacter->mModel.mIdealImageWidth) / 2.0f) * screenObject.mGameScreenToGameLevelChunkRatio;
		float printY = (float(pCurTile->mCoords.mY1) + (float)(pCurTile->mCoords.mHeight - pCurCombatCharacter->mModel.mIdealImageHeight)) * screenObject.mGameScreenToGameLevelChunkRatio;
		float printWidth = float(pCurCombatCharacter->mModel.mIdealImageWidth) * screenObject.mGameScreenToGameLevelChunkRatio;
		float printHeight = float(pCurCombatCharacter->mModel.mIdealImageHeight) * screenObject.mGameScreenToGameLevelChunkRatio;

		SDL_FRect curEntityPositionToPrintTo = { printX, printY, printWidth, printHeight };
		Uint8 setAlpha = SDL_SetTextureAlphaMod(pCurCombatCharacter->mModel.getTexture(), alpha);
		SDL_RenderTextureRotated(screenObject.mpRenderer, pCurCombatCharacter->mModel.getTexture(), NULL, &curEntityPositionToPrintTo, NULL, NULL, SDL_FLIP_NONE);
		SDL_SetTextureAlphaMod(pCurCombatCharacter->mModel.getTexture(), 255); // set it back so that it doesn't stay at 50% forever
	}
}

void MiniGameStateManager::updateTileColors()
{
	Grid& grid = mWorldData.getStage()->mGrid;
	bool colorWholeGrid = false;

	// figure out if SELECTED OR HIGHLIGHTED
	for (Tile* pCurTile : grid.mpTiles)
	{
		SDL_Color curColor = pCurTile->mCurColor;
		SDL_Color colorToDraw = curColor;
		float alpha = 0.0f;
		if (pCurTile->getMode() == ECombatTileMode_SELECTED)
		{
			alpha = .1f;
		}
		else if (pCurTile->getMode() == ECombatTileMode_HIGHLIGHTED)
		{
			alpha = .35f;
		}
		SDL_Color curWhite{ 255, 255, 255, (Uint8)alpha };
		colorToDraw = blendColors(&curColor, &curWhite, alpha);

		pCurTile->setCurColor(colorToDraw);
	}

	// SHOW MOVE and ATTACK TILES
	CombatCharacter* pCurCombatCharacter = mData.mStateData.getCharacter();
	ECombatActionType tileType = ECombatActionType_INVALID;
	std::vector <TileCoords> tileCoordsList;
	switch (mData.mCurStateEnum)
	{
	case EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT:
	case EMiniGameState_ENEMY_MOVE_CHARACTER:
		tileCoordsList = pCurCombatCharacter->mCombatMovementManager.getMoveTileCoords();
		tileType = ECombatActionType_MOVE;
		break;
	case EMiniGameState_PLAYER_WAIT_FOR_ATTACK_TILE_INPUT:
	case EMiniGameState_PLAYER_COMPLETE_ACTION_ATTACK:
	case EMiniGameState_ENEMY_TAKE_ACTION:
		tileType = ECombatActionType_ATTACK;
		if (mData.mStateData.mpCurAttack != nullptr)
		{
			if (mData.mStateData.mpCurAttack->mType == ECombatActionGridPattern_WHOLE_GRID)
			{
				colorWholeGrid = true;
			}
			else if (mData.mStateData.mpCurAttack->mNumTilesToAttack == ECombatNumTilesToAttack_DIRECTION and mData.mStateData.mCurAttackDirection != EDirection_NONE and mData.mStateData.mCurAttackDirection != EDirection_INVALID)
			{
				tileCoordsList = returnTileCoords(*pCurCombatCharacter->mCombatMovementManager.getCurTile(), mData.mStateData.mpCurAttack->mType, mData.mStateData.mpCurAttack->mNum, mData.mStateData.mpCurAttack->mOut, mData.mStateData.mCurAttackDirection);
			}
			else
			{
				tileCoordsList = returnTileCoords(*pCurCombatCharacter->mCombatMovementManager.getCurTile(), mData.mStateData.mpCurAttack->mType, mData.mStateData.mpCurAttack->mNum, mData.mStateData.mpCurAttack->mOut, EDirection_ALL);
			}
		}
		break;
	default:
		break;
	}

	if (colorWholeGrid)
	{
		for (Tile* pTile : grid.mpTiles)
		{
			colorTile(*pTile, tileType);
		}
	}
	else
	{
		for (const TileCoords& tileCoord : tileCoordsList)
		{
			Tile* pTile = grid.findTile(tileCoord);
			if (pTile == nullptr)
			{
				continue;
			}
			colorTile(*pTile, tileType);
		}
	}
}

void MiniGameStateManager::colorTile(Tile& tile, const ECombatActionType tileType)
{
	CombatManager& combatManager = mWorldData.getStage()->mCombatManager;
	if (isPlayableTile(tile))
	{
		SDL_Color& curColor = tile.mCurColor;
		SDL_Color otherColor;
		switch (tileType)
		{
		case ECombatActionType_MOVE:
			if (combatManager.characterOnTile(tile))
			{
				return;
			}
			otherColor = StyleManager::sunYellow;
			break;
		case ECombatActionType_ATTACK:
			otherColor = StyleManager::red;
			break;
		default:
			SDL_assert(false);
			break;
		}
		float alpha = .25;
		SDL_Color updatedColor = blendColors(&curColor, &otherColor, alpha);
		tile.setCurColor(updatedColor);
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
	if (mData.mPreviousStateDatas.empty())
	{
		mData.mStateData.mDebugLine = " ";
		return;
	}

	MiniGameStateData& preTickStateData = mData.mPreviousStateDatas.top().second;
	// current state data is post tick
	std::string line;

	switch (mData.mCurStateEnum)
	{
	case EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT:
		line = mData.mStateData.getCharacter()->mName + " moved to " + std::to_string(mData.mStateData.mpTileToMoveTo->mRow) + ", " + std::to_string(mData.mStateData.mpTileToMoveTo->mCol);
		break;

	case EMiniGameState_PLAYER_COMPLETE_ACTION_ATTACK:
	case EMiniGameState_ENEMY_TAKE_ACTION:
		if (mData.mStateData.mAttacked)
		{
			line = preTickStateData.getCharacter()->mName + " choose to " + mData.mStateData.mpCurAttack->mName
				+ (mData.mStateData.mpCurAttack->mNumTilesToAttack == ECombatNumTilesToAttack_DIRECTION ? (" " + directionToString(mData.mStateData.mCurAttackDirection)) : "") 
				+ (mData.mStateData.mpCurAttack->mCurCooldown != 0 ? (". " + mData.mStateData.mpCurAttack->mName + " is now on " + std::to_string(mData.mStateData.mpCurAttack->mCurCooldown) + " turn cooldown") : "");
		}
		else if (mData.mStateData.mDefended || mData.mStateData.mHealed)
		{
			// skip. Will be covered in character stat change section of the log
		}
		else
		{
			// passed
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

	std::string characterChanges = getCharacterChangesString(mWorldData.getStage()->mCombatManager, mData.mPreTickCharacters.top());
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
		if (DEBUG)
		{
			std::cout << line << "\n";
		}
		mData.mStateData.mDebugLine = line;
	}
}
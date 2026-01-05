#include "gameState.h"

// GAME STATE
GameState::GameState(GameStateData& gameStateData, KeyboardData& keyboardData, MenuManager& menuManager, MenuDataController& menuController, 
		SettingsManager& settingsManager, ScreenObject& screen)
		: mGameStateData(gameStateData), mKeyboardData(keyboardData), mMenuManager(menuManager), mMenuDataController(menuController), 
		mSettingsManager(settingsManager), mScreen(screen) { ; }

void GameState::preTick()
{
	mMenuManager.preTick();
}

void GameState::getInput()
{
	for (int count = 0; count < mKeyboardData.mNumKeys; count++)
	{
		mKeyboardData.mLastFrameKeyState[count] = mKeyboardData.mKeyState[count];
	}
	mKeyboardData.mCurKeysString = "Keys: ";

	int numEvents = SDL_PeepEvents(mKeyboardData.mEvents, numEventsToGrab, SDL_GETEVENT, SDL_EVENT_FIRST, SDL_EVENT_LAST);
	SDL_assert(numEvents <= numEventsToGrab);

	for (int count = 0; count < numEvents; count++)
	{
		EKeyboardInput curKey = EKeyboardInput_INVALID;
		bool down = false;
		std::map<const SDL_Keycode, const EKeyboardInput>::const_iterator it;
		switch (mKeyboardData.mEvents[count].type)
		{
		case SDL_EVENT_KEY_DOWN:
		case SDL_EVENT_KEY_UP:
			down = mKeyboardData.mEvents[count].type == SDL_EVENT_KEY_DOWN;
			it = SDLKToKeyboardMap.find(mKeyboardData.mEvents[count].key.keysym.sym);
			if (it != SDLKToKeyboardMap.end())
			{
				curKey = it->second;
			}

			break;
		case SDL_EVENT_MOUSE_BUTTON_DOWN:
		case SDL_EVENT_MOUSE_BUTTON_UP:
			down = mKeyboardData.mEvents[count].type == SDL_EVENT_MOUSE_BUTTON_DOWN;
			switch (mKeyboardData.mEvents[count].button.button)
			{
			case SDL_BUTTON_LEFT:
				curKey = EKeyboardInput_MOUSE_LEFT;
				break;
			case SDL_BUTTON_MIDDLE:
				curKey = EKeyboardInput_MOUSE_MIDDLE;
				break;
			case SDL_BUTTON_RIGHT:
				curKey = EKeyboardInput_MOUSE_RIGHT;
				break;

			default:
				break;
			}
			break;

		case SDL_EVENT_FINGER_DOWN:
			down = true;
			curKey = EKeyboardInput_MOUSE_LEFT;
			break;
		case SDL_EVENT_FINGER_UP:
			down = false;
			curKey = EKeyboardInput_MOUSE_LEFT;
			break;
		case SDL_EVENT_QUIT:
			down = true;
			curKey = EKeyboardInput_ESC;
			break;
		default:
			break;
		}

		if (curKey != EKeyboardInput_INVALID)
		{
			mKeyboardData.mKeyState[(int)curKey] = down;
		}
	}

	for (int count = 0; count < mKeyboardData.mNumKeys; count++)
	{
		if (mKeyboardData.mLastFrameKeyState[count] == mKeyboardData.mKeyState[count])
		{
			mKeyboardData.mKeyStateRepeat[count] += 1;
		}
		else
		{
			mKeyboardData.mKeyStateRepeat[count] = 0;
		}

		if (mKeyboardData.mKeyState[count])
		{
			mKeyboardData.mCurKeysString += keyboardToStringMap.at(EKeyboardInput(count)) + " ";
		}

	}
}

void GameState::useInput(GameStateManagerData& gameStateManagerData)
{
	eventVect.clear();
	for (int countEvent = 0; countEvent < mKeyboardData.mNumKeys; countEvent++)
	{
		if (mKeyboardData.mKeyState[countEvent] == true)
		{
			KeyData key = KeyData(countEvent, mKeyboardData.mKeyStateRepeat[countEvent]);
			eventVect.push_back(key);

			switch (key.mKey)
			{
			case EKeyboardInput_1:
				mGameStateData.mNextGameState = EGameState_PLAY;
				break;
			case EKeyboardInput_2:
				mGameStateData.mNextGameState = EGameState_MENU;
				break;
			case EKeyboardInput_UP:
			case EKeyboardInput_W:
				if (mTicksSinceInput > mTicksBeforeUseInput)
				{
					mMenuManager.mpCurMenuPage->setCurTextBoxIfValid(mMenuManager.mpCurMenuPage->getCurTextBoxIndex() - 1);
					mTicksSinceInput = 0;
				}
				break;

			case EKeyboardInput_DOWN:
			case EKeyboardInput_S:
				if (mTicksSinceInput > mTicksBeforeUseInput)
				{
					mMenuManager.mpCurMenuPage->setCurTextBoxIfValid(mMenuManager.mpCurMenuPage->getCurTextBoxIndex() + 1);
					mTicksSinceInput = 0;
				}
				break;

			case EKeyboardInput_ENTER:
				if (mTicksSinceInput > mTicksBeforeUseInput)
				{
					mMenuManager.mpCurMenuPage->setCurSelectedTextBox(mMenuManager.mpCurMenuPage->getCurTextBox());
					mTicksSinceInput = 0;
				}
				break;

			case EKeyboardInput_MOUSE_LEFT:
				if (mTicksSinceInput > mTicksBeforeUseInput)
				{
					TextBox* pTextBox = mMenuManager.returnMouseTextBox(mousePos);
					if (pTextBox != nullptr && pTextBox == mMenuManager.mpCurMenuPage->getCurTextBox())
					{
						mMenuManager.mpCurMenuPage->setCurSelectedTextBox(mMenuManager.mpCurMenuPage->getCurTextBox());
					}
					mTicksSinceInput = 0;
				}
				break;

			case EKeyboardInput_ESC:
				gameStateManagerData.mRunGame = false;
				break;
			}
		}
	}

	eventVect.clear();
}

void GameState::useMouseCursor()
{
	float x;
	float y;
	SDL_GetMouseState(&x, &y);

	mousePos = Vect2(x, y);

	TextBox* pTextBox = mMenuManager.returnMouseTextBox(mousePos);
	if (pTextBox != nullptr)
	{
		mMenuManager.mpCurMenuPage->setCurTextBox(pTextBox);
		mMenuManager.mpCurMenuPage->setCurSelectedTextBox(nullptr);
	}
	pTextBox = nullptr;
}

void GameState::takeMenuAction(MiniGameStateManager& miniStateManager)
{
	TextBox* pCurSelectedTextBox = mMenuManager.mpCurMenuPage->getCurSelectedTextBox();
	MiniGameState* pCurState = miniStateManager.mpCurState;
	Attack* pAttack = nullptr;
	if (pCurSelectedTextBox == nullptr)
	{
		return;
	}

	switch (pCurSelectedTextBox->mFunction)
	{
	case ETextBoxFunction_PLAY_GAME_BOX:
		mMenuManager.setCurMenuPage(mMenuManager.mpMenuPages[int(EMenuPageType_MAIN_GAME_MENU)]);
		mGameStateData.mNextGameState = EGameState_PLAY;
		break;
	case ETextBoxFunction_GO_TO_MAIN_MENU_BOX:
		mMenuManager.setCurMenuPage(mMenuManager.mpMenuPages[int(EMenuPageType_MAIN_MENU)]);
		mGameStateData.mNextGameState = EGameState_MENU;
		break;
	case ETextBoxFunction_PLAY_MINI_GAME_BOX:
		mMenuManager.setCurMenuPage(mMenuManager.mpMenuPages[int(EMenuPageType_MINI_GAME_MENU)]);
		mGameStateData.mNextGameState = EGameState_PLAY_MINI_GAME;
		miniStateManager.mWorldData.setNextLevel(miniStateManager.mWorldData.mCheatWorldNumber, miniStateManager.mWorldData.mCheatLevelNumber, miniStateManager.mWorldData.mCheatStageNumber);
		break;
	case ETextBoxFunction_ATTACK_CUR_COMBAT_CHARACTER_BOX:
		if (miniStateManager.mData.mCurStateEnum == EMiniGameState_PLAYER_WAIT_FOR_ACTION_INPUT)
		{
			miniStateManager.mData.mStateData.mAttackCategory = ECombatAttackCategoryType_ATTACK;
			MiniGamePlayerWaitForActionInput* pSpecificCurState = (MiniGamePlayerWaitForActionInput*)pCurState;
			pSpecificCurState->postTick(EMiniGameState_PLAYER_WAIT_FOR_ATTACK_OPTION_INPUT);
			pSpecificCurState = nullptr;
		}
		break;
	case ETextBoxFunction_SUPPORT_CUR_COMBAT_CHARACTER_BOX:
		if (miniStateManager.mData.mCurStateEnum == EMiniGameState_PLAYER_WAIT_FOR_ACTION_INPUT)
		{
			miniStateManager.mData.mStateData.mAttackCategory = ECombatAttackCategoryType_SUPPORT;
			MiniGamePlayerWaitForActionInput* pSpecificCurState = (MiniGamePlayerWaitForActionInput*)pCurState;
			pSpecificCurState->postTick(EMiniGameState_PLAYER_WAIT_FOR_ATTACK_OPTION_INPUT);
			pSpecificCurState = nullptr;
		}
		break;
	case ETextBoxFunction_DEFEND_CUR_COMBAT_CHARACTER_BOX:
		if (miniStateManager.mData.mCurStateEnum == EMiniGameState_PLAYER_WAIT_FOR_ACTION_INPUT)
		{
			MiniGamePlayerWaitForActionInput* pSpecificCurState = (MiniGamePlayerWaitForActionInput*)pCurState;
			pSpecificCurState->postTick(EMiniGameState_PLAYER_COMPLETE_ACTION_DEFEND);
			pSpecificCurState = nullptr;
		}
		break;
	case ETextBoxFunction_HEAL_CUR_COMBAT_CHARACTER_BOX:
		if (miniStateManager.mData.mCurStateEnum == EMiniGameState_PLAYER_WAIT_FOR_ACTION_INPUT)
		{
			MiniGamePlayerWaitForActionInput* pSpecificCurState = (MiniGamePlayerWaitForActionInput*)pCurState;
			pSpecificCurState->postTick(EMiniGameState_PLAYER_COMPLETE_ACTION_HEAL);
			pSpecificCurState = nullptr;
		}
		break;
	case ETextBoxFunction_PASS_CUR_COMBAT_CHARACTER_TURN_BOX:
		if (miniStateManager.mData.mCurStateEnum == EMiniGameState_PLAYER_WAIT_FOR_ACTION_INPUT)
		{
			MiniGamePlayerWaitForActionInput* pSpecificCurState = (MiniGamePlayerWaitForActionInput*)pCurState;
			pSpecificCurState->postTick(EMiniGameState_BUFFER);
			pSpecificCurState = nullptr;
		}
		break;
	case ETextBoxFunction_ATTACK_STYLE_BOX:
		if (miniStateManager.mData.mCurStateEnum == EMiniGameState_PLAYER_WAIT_FOR_ATTACK_OPTION_INPUT)
		{
			MiniGamePlayerWaitForAttackOptionInput* pSpecificCurState = (MiniGamePlayerWaitForAttackOptionInput*)pCurState;
			pAttack = &miniStateManager.mData.mStateData.getCharacter()->mCombatMovementManager.getAttacks()[pCurSelectedTextBox->mData.mAttackNum];
			if (pAttack->canUse())
			{
				pSpecificCurState->postTick(*pAttack);
			}
			pAttack = nullptr;
			pSpecificCurState = nullptr;
		}
		break;
	case ETextBoxFunction_ATTACK_DIRECTION_BOX:
		if (miniStateManager.mData.mCurStateEnum == EMiniGameState_PLAYER_WAIT_FOR_ATTACK_DIRECTION_INPUT)
		{
			MiniGamePlayerWaitForAttackDirectionInput* pSpecificCurState = (MiniGamePlayerWaitForAttackDirectionInput*)pCurState;
			pSpecificCurState->postTick(pCurSelectedTextBox->mData.mAttackDirection);
			pSpecificCurState = nullptr;
		}
		break;
	case ETextBoxFunction_ATTACK_CHARACTER_BOX:
		if (miniStateManager.mData.mCurStateEnum == EMiniGameState_PLAYER_WAIT_FOR_ATTACK_CHARACTER_INPUT)
		{
			MiniGamePlayerWaitForAttackCharacterInput* pSpecificCurState = (MiniGamePlayerWaitForAttackCharacterInput*)pCurState;
			pSpecificCurState->postTick(miniStateManager.mWorldData.getStage()->mCombatManager.getFromAllCharacters(pCurSelectedTextBox->mData.mCombatCharacterIndex));
			pSpecificCurState = nullptr;
		}
		break;

	case ETextBoxFunction_MINI_GAME_UNDO_BOX:
		miniStateManager.undo();
		break;
	}

	pCurState = nullptr;
	pCurSelectedTextBox = nullptr;

	mMenuManager.mpCurMenuPage->setCurSelectedTextBox(nullptr);
}





// TYPES GAME STATES
GameStatePlay::GameStatePlay(GameStateData& gameStateData, KeyboardData& keyboardData, WorldData& worldData, MenuManager& menuManager, MenuDataController& menuController, 
	SettingsManager& settingsManager, CollisionManager& collisionManager, SlashManager& slashManager) 
	: GameState(gameStateData, keyboardData, menuManager, menuController, settingsManager, worldData.mScreen),
	mWorldData(worldData), mCollisionManager(collisionManager), mSlashManager(slashManager) {
}

void GameStatePlay::tick(GameStateManagerData& gameStateManagerData, MiniGameStateManager& miniGameStateManager)
{
	mTicksSinceInput += 1;
	mWorldData.updateCurLevelChunk();

	SDL_PumpEvents();
	getInput();

	if (!mSettingsManager.mSingleSteppingMode or (mSettingsManager.mSingleSteppingMode and mSettingsManager.mFrameStepInputRequest))
	{	// don't tick if in single step mode and there's no input this frame.
		useMouseCursor();
		useInput(gameStateManagerData);
		mWorldData.entityPreTickCalcMovement();

		// COLLISIONS
		mWorldData.entityCollisions();

		if (mSettingsManager.mFrameStepInputRequest)
		{
			// Reset
			mSettingsManager.mFrameStepInputRequest = false;
		}

		// GO TO NEXT LEVEL
		if (mWorldData.mGoToNextLevel)
		{
			switch (mWorldData.mpNextLevelData->mType)
			{
			case ELevelType_PLATFORMING:
				mWorldData.setNextLevel(mWorldData.mpNextLevelData->mWorldNumber, mWorldData.mpNextLevelData->mLevelNumber);

				break;

			case ELevelType_MINI_GAME:
				mGameStateData.mNextGameState = EGameState_PLAY_MINI_GAME;
				mGameStateData.mCleanNextState = true;
			}

		}
	}

	// RENDERING
	render(gameStateManagerData.mCurStateEnum);

	mWorldData.entityPostTick();
	mCollisionManager.postTick();
	mWorldData.clearDeadProjectiles();
	mWorldData.updatePermanentCollectibles();

	mWorldData.updateBackgroundEffects();

}

void GameStatePlay::useInput(GameStateManagerData& gameStateManagerData)
{
	eventVect.clear();
	for (int countEvent = 0; countEvent < mKeyboardData.mNumKeys; countEvent++)
	{
		if (mKeyboardData.mKeyState[countEvent])
		{
			eventVect.push_back(KeyData(countEvent, mKeyboardData.mKeyStateRepeat[countEvent]));

			const EKeyboardInput resetLevelKey = mSettingsManager.resetLevel;
			const EKeyboardInput resetCheckpointKey = mSettingsManager.resetCheckpoint;
			const EKeyboardInput throwProjectileHorizontalKey = mSettingsManager.shootProjectileHorizontal;
			const EKeyboardInput throwProjectileVerticalKey = mSettingsManager.shootProjectileVertical;
			const EKeyboardInput slashKey = mSettingsManager.slash;

			EKeyboardInput curKey = static_cast<EKeyboardInput>(countEvent);

			if (countEvent == resetLevelKey)
			{
				mWorldData.resetStats();
			}
			else if (countEvent == resetCheckpointKey)
			{
				mWorldData.resetToCheckpoint();
			}
			else if (countEvent == throwProjectileHorizontalKey and !mKeyboardData.mLastFrameKeyState[countEvent])
			{
				mWorldData.playerShootProjectile(EEntityMovementPath_HORIZONTAL);
			}
			else if (countEvent == throwProjectileVerticalKey and !mKeyboardData.mLastFrameKeyState[countEvent])
			{
				mWorldData.playerShootProjectile(EEntityMovementPath_VERTICAL);
			}
			else if (countEvent == slashKey and !mKeyboardData.mLastFrameKeyState[countEvent])
			{
				mWorldData.playerSwordSlash();
			}
			else if (countEvent == EKeyboardInput_1)
			{
				mGameStateData.mNextGameState = EGameState_PLAY;
			}
			else if (countEvent == EKeyboardInput_2)
			{
				mGameStateData.mNextGameState = EGameState_MENU;
			}
			else if (countEvent == EKeyboardInput_P && !mKeyboardData.mLastFrameKeyState[countEvent])
			{
				mWorldData.mGoToNextLevel = true;
				mWorldData.mpNextLevelData = &mWorldData.mpWorlds[mWorldData.mCurWorldNumber]->mpLevels[mWorldData.mCurLevelNumber]->mNextLevelData;
			}
			else if (countEvent == EKeyboardInput_ESC)
			{
				gameStateManagerData.mRunGame = false;
			}
		}
	}

	mWorldData.mPlayer.useInput(eventVect);
}

void GameStatePlay::render(EGameState curState)
{
	Level* pCurLevel = mWorldData.mpWorlds[mWorldData.mCurWorldNumber]->mpLevels[mWorldData.mCurLevelNumber];
	SDL_SetRenderDrawColor(mWorldData.mScreen.mpRenderer, 0, 0, 0, 1);
	SDL_FRect screenRect{ 0.0, 0.0, (float)mWorldData.mScreen.mGameScreenWidth, (float)mWorldData.mScreen.mGameScreenHeight };
	SDL_RenderFillRect(mWorldData.mScreen.mpRenderer, &screenRect);
	mWorldData.renderBackgrounds();
	mWorldData.renderBackgroundEffects();


	// platforms
	// standard platforms
	for (Platform* pPlatform : pCurLevel->mpPlatforms)
	{
		if (pPlatform->mPrintViaChunk)
		{
			mWorldData.renderEntityViaChunk(*pPlatform);
		}
		else if (pPlatform->mSplice)
		{
			mWorldData.renderEntityViaSplice(*pPlatform);
		}
		else
		{
			mWorldData.renderEntity(*pPlatform);
		}
	}
	// non static
	for (Platform* pPlatform : pCurLevel->mpActiveNonStaticPlatforms)
	{
		if (pPlatform->mPrintViaChunk)
		{
			mWorldData.renderEntityViaChunk(*pPlatform);
		}
		else if (pPlatform->mSplice)
		{
			mWorldData.renderEntityViaSplice(*pPlatform);
		}
		else
		{
			mWorldData.renderEntity(*pPlatform);
		}
	}
	// area effect
	for (AreaEffectPlatform* pPlatform : pCurLevel->mpAreaEffectPlatforms)
	{
		if (pPlatform->mIsVisible)
		{
			AnimationManager& animationManager = pPlatform->mAreaEffectAnimationManager;
			ImageObject& curImageObject = *pPlatform->mAreaEffectAnimationManager.getCurImage();
			EDirection imageDirection = pPlatform->getMovementManager().getCurFacingDirection();
			EImageOffset imageOffsetType = EImageOffset_MIDDLE;
			Hitbox& hitbox = pPlatform->mAreaEffectHitbox;

			if (pPlatform->mPrintViaChunk)
			{
				mWorldData.renderEntityViaChunk(*pPlatform);
				mWorldData.renderEntityViaChunk(curImageObject, hitbox);
			}
			else if (pPlatform->mSplice)
			{
				mWorldData.renderEntityViaSplice(*pPlatform);
				mWorldData.renderEntityViaSplice(animationManager, hitbox);
			}
			else
			{
				mWorldData.renderEntity(*pPlatform);
				mWorldData.renderEntity(curImageObject, imageOffsetType, imageDirection, hitbox);
			}
		}
	}

	// collectibles
	for (Collectible* pCollectible : pCurLevel->mpActiveCollectibles)
	{
		mWorldData.renderEntity(*pCollectible);
	}

	// enemies
	for (Enemy* pEnemy : pCurLevel->mpActiveEnemies)
	{
		mWorldData.renderEntity(*pEnemy);
	}

	// projectiles
	for (Projectile* pProjectile : mWorldData.mpProjectiles)
	{
		mWorldData.renderEntity(*pProjectile);
	}

	// slash
	if (mSlashManager.mCurSlash)
	{
		Vect2 topLeft = mSlashManager.mCenterOfRotation - Vect2(mSlashManager.mHitbox.getWidth() / 2, mSlashManager.mHitbox.getHeight() / 2);
		Hitbox slashImageHitbox = Hitbox(topLeft, mSlashManager.mAnimationManager.getCurImage()->mIdealImageWidth, mSlashManager.mAnimationManager.getCurImage()->mIdealImageHeight);
		EImageOffset offsetType = EImageOffset_LEFT_X_MIDDLE_Y;
		bool rotating = true;
		// sword
		mWorldData.renderEntityWithHitbox(*mSlashManager.mAnimationManager.getCurImage(), mSlashManager.mpSlashImageHitboxTexture, offsetType,
			mSlashManager.mCurSlashDirection, slashImageHitbox, rotating, degreesToImageRotationDegrees(mSlashManager.mImageRotation,
				mSlashManager.mCurRotation));
		if (DEBUG)
		{
			// slash hitbox
			offsetType = EImageOffset_PRINT_TOP_LEFT;
			rotating = false;
			float rotation = 0;
			Vect2 hitboxOffset = Vect2(0, slashImageHitbox.getHeight() / 2);
			mWorldData.renderTexture(mSlashManager.mpHitboxTexture, mSlashManager.mHitbox, offsetType, hitboxOffset, mSlashManager.mCurSlashDirection, rotating, 0.0);
		}
	}


	// PLAYER
	mWorldData.renderEntity(mWorldData.mPlayer);
	mMenuDataController.updateUIElements();
	mMenuManager.setCurMenuPage(mMenuManager.mpMenuPages[int(EMenuPageType_MAIN_GAME_MENU)]);

	mMenuDataController.getUpdatedMenuBoxes(curState, false);
	mMenuManager.renderMenus();
}




GameStatePlayMiniGame::GameStatePlayMiniGame(GameStateData& gameStateData, KeyboardData& keyboardData, MiniGameStateManager& miniGameStateManager, 
	MenuManager& menuManager, MenuDataController& menuController, ScreenObject& screen, SettingsManager& settingsManager) 
	: GameState(gameStateData, keyboardData, menuManager, menuController, settingsManager, screen), mMiniGameStateManager(miniGameStateManager) { ; }

void GameStatePlayMiniGame::setUp() { mTicks = 0; }

void GameStatePlayMiniGame::tick(GameStateManagerData& gameStateManagerData, MiniGameStateManager& miniGameStateManager)
{
	mTicks += 1;
	mTicksSinceInput += 1;
	SDL_PumpEvents();
	CombatManager& combatManager = mMiniGameStateManager.mWorldData.getStage()->mCombatManager;

	if (combatManager.getAllCharacters().size() == 0)
	{
		combatManager.createCurAliveCharacterList();
		mMiniGameStateManager.mData.mStateData.setCharacter(combatManager.getFromAllCharacters(0), 0);
	}

	getInput();
	useMouseCursor();
	useInput(gameStateManagerData);
	mMiniGameStateManager.mpCurState->useInput(mMiniGameStateManager.mData.mCurStateEnum, mScreen);
	takeMenuAction(miniGameStateManager);

	mMiniGameStateManager.preTick();
	mMiniGameStateManager.tick();
	mMiniGameStateManager.postTick();
	if (mMiniGameStateManager.mData.mCurStateEnum == EMiniGameState_EXIT)
	{
		mGameStateData.mNextGameState = EGameState_PLAY; // go back to main game next
	}

	render(gameStateManagerData.mCurStateEnum);
}

void GameStatePlayMiniGame::render(EGameState curState)
{
	SDL_SetRenderDrawColor(mScreen.mpRenderer, 0, 0, 0, 1);
	SDL_FRect screenRect{ 0.0f, 0.0f, float(mScreen.mGameScreenWidth), float(mScreen.mGameScreenHeight) };
	SDL_RenderFillRect(mScreen.mpRenderer, &screenRect);
	mMiniGameStateManager.printBoard(mScreen);

	mMenuDataController.getUpdatedMenuBoxes(curState, mTicks <= 1);
	mMenuManager.renderMenus();

	SDL_SetRenderDrawColor(mScreen.mpRenderer, 50, 50, 50, 1);
	SDL_FRect rect{ (mousePos.getX() - 10.0f), (mousePos.getY() - 10.0f), 20.0f, 20.0f };
	SDL_RenderFillRect(mScreen.mpRenderer, &rect);

	SDL_SetRenderDrawColor(mScreen.mpRenderer, 10, 100, 100, 1);
	SDL_FRect rect2{ (10.0f / (mTicksSinceInput + 1)), 700.0f, 20.0f, 20.0f };
	SDL_RenderFillRect(mScreen.mpRenderer, &rect2);
}




GameStateMenu::GameStateMenu(GameStateData& gameStateData, KeyboardData& keyboardData, MenuManager& menuManager, MenuDataController& menuController, 
	SettingsManager& settingsManager, WorldData& worldData)
	: GameState(gameStateData, keyboardData, menuManager, menuController, settingsManager, worldData.mScreen) { ; }

void GameStateMenu::tick(GameStateManagerData& gameStateManagerData, MiniGameStateManager& miniGameStateManager)
{
	mTicksSinceInput += 1;

	SDL_PumpEvents();
	mMenuManager.preTick();
	// PRE TICK
	getInput();
	useMouseCursor();
	useInput(gameStateManagerData);
	takeMenuAction(miniGameStateManager);

	// RENDERING
	render(gameStateManagerData.mCurStateEnum);
}

void GameStateMenu::render(EGameState curState)
{
	SDL_Color curTextBoxColor = { 10, 10, 10, 255 };
	SDL_SetRenderDrawColor(mScreen.mpRenderer, curTextBoxColor.r, curTextBoxColor.g, curTextBoxColor.b, curTextBoxColor.a);
	SDL_FRect box = { 0.0f, 0.0f, (float)mScreen.mGameScreenWidth, (float)mScreen.mGameScreenHeight };
	SDL_RenderFillRect(mScreen.mpRenderer, &box);

	mMenuDataController.getUpdatedMenuBoxes(curState, false);
	mMenuManager.renderMenus();

	SDL_SetRenderDrawColor(mScreen.mpRenderer, 50, 50, 50, 1);
	SDL_FRect rect{ (mousePos.getX() - 10.0f), (mousePos.getY() - 10.0f), 20.0f, 20.0f };
	SDL_RenderFillRect(mScreen.mpRenderer, &rect);
}





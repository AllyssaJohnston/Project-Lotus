#include "menuHelper.h"

void MenuPage::setCurSelectableAndDisplayOnlyTextBoxes(std::vector <TextBox*> curSelectableTextBoxes, std::vector<TextBox*> curDisplayOnlyTextBoxes)
{
	mpCurSelectableTextBoxes  = curSelectableTextBoxes;
	mpCurDisplayOnlyTextBoxes = curDisplayOnlyTextBoxes;
	if (curSelectableTextBoxes.size() > 0)
	{
		setCurTextBox(curSelectableTextBoxes[0]);
	}
}

void MenuPage::useInput(const std::vector <int>& eventVect)
{
	for (int curEventEnum : eventVect)
	{
		switch (curEventEnum)
		{
		case EKeyboardInput_UP:

			setCurTextBoxIfValid(getCurTextBoxIndex() - 1);
			break;

		case EKeyboardInput_DOWN:

			setCurTextBoxIfValid(getCurTextBoxIndex() + 1);
			break;
		case EKeyboardInput_ENTER:

			mpCurSelectedTextBox = mpCurTextBox;
			break;
		}
	}
}

void MenuPage::setCurTextBoxIfValid(int count)
{
	if (count >= 0 and count < mpCurSelectableTextBoxes.size())
	{
		setCurTextBox(mpCurSelectableTextBoxes[count]);
	}
}

void MenuPage::setCurTextBox(TextBox * textBox)
{
	if (mpCurTextBox != nullptr)
	{
		mpCurTextBox->changeIsHighlighted(false);
	}
	mpCurTextBox = textBox;
	mpCurTextBox->changeIsHighlighted(true);
}

void MenuPage::setCurSelectedTextBox(TextBox* textBox)
{
	mpCurSelectedTextBox = textBox;
}

TextBox* MenuPage::getCurTextBox()
{
	return mpCurTextBox;
}

TextBox* MenuPage::getCurSelectedTextBox()
{
	return mpCurSelectedTextBox;
}

int MenuPage::getCurTextBoxIndex()
{
	for (int count = 0; count < mpCurSelectableTextBoxes.size(); count++)
	{
		if (mpCurSelectableTextBoxes[count] == mpCurTextBox)
		{
			return count;
		}
	}
	return -1;
}

std::vector <TextBox*> MenuPage::returnAllTextBoxes()
{
	std::vector <TextBox*> allTextBoxes;
	for (TextBox* pTextBox : mpAllSelectableTextBoxes)
	{
		allTextBoxes.push_back(pTextBox);
	}
	for (TextBox* pTextBox : mpAllDisplayOnlyTextBoxes)
	{
		allTextBoxes.push_back(pTextBox);
	}
	return allTextBoxes;
}

std::vector <TextBox*> MenuPage::returnAllCurTextBoxes()
{
	std::vector <TextBox*> allCurTextBoxes;
	for (TextBox* pTextBox : mpCurSelectableTextBoxes)
	{
		allCurTextBoxes.push_back(pTextBox);
	}
	for (TextBox* pTextBox : mpCurDisplayOnlyTextBoxes)
	{
		allCurTextBoxes.push_back(pTextBox);
	}
	return allCurTextBoxes;
}

void MenuPage::adjustBlocks(float textSizeFactor)
{
	for (UIBlock* pBlock : mpBlocks)
	{
		pBlock->updateBlocks((int)textSizeFactor);
	}
}





MenuManager::~MenuManager()
{
	for (MenuPage* page : mpMenuPages)
	{
		delete page;
	}
	mpCurMenuPage		= nullptr;
	mpLastFrameMenuPage = nullptr;
}

void MenuManager::preTick()
{
	mpLastFrameMenuPage = mpCurMenuPage;
	mpCurMenuPage->setCurSelectedTextBox(nullptr);
}

TextBox* MenuManager::returnMouseTextBox(Vect2 gameUnitsMousePos, ScreenObject& screenObject)
{
	int  x = int( (gameUnitsMousePos.getX() - 5) / screenObject.mGameScreenToGameLevelChunkRatio);
	int x2 = int( (gameUnitsMousePos.getX() + 5) / screenObject.mGameScreenToGameLevelChunkRatio);
	int  y = int( (gameUnitsMousePos.getY() - 5) / screenObject.mGameScreenToGameLevelChunkRatio);
	int y2 = int( (gameUnitsMousePos.getY() + 5) / screenObject.mGameScreenToGameLevelChunkRatio);

	if (x < 0)
	{
		x = 0;
	}
	if (y < 0)
	{
		y = 0;
	}

	Hitbox mouseHitbox(x, x2, y, y2);

	for (TextBox* pTextBox : mpCurMenuPage->mpCurSelectableTextBoxes)
	{
		if (pTextBox->mHitbox.overlap(mouseHitbox))
		{
			return pTextBox;
		}
	}
	return nullptr;
}

bool MenuManager::shouldShowTextBox(MiniGameStateManagerData& miniGameStateManagerData, TextBox* pTextBox)
{
	switch (pTextBox->mType)
	{
	case ETextBoxType_MINI_GAME_PLAYER_BOX:
	{
		if (miniGameStateManagerData.mpData->mpCharacter == nullptr)
		{
			return false;
		}
		if (miniGameStateManagerData.mpData->mpCharacter->mType != EMiniGameCombatCharacterType_PLAYER)
		{
			return false;
		}
		//NO BREAK!!!
	}
	case ETextBoxType_MINI_GAME_BOX: //or ETextBoxType_MINI_GAME_PLAYER_BOX:
	{
		std::vector <EMiniGameState> miniGameStateWhenToShowList = pTextBox->mDataStorage.mMiniGameStateWhenToShowList;
		bool isInWhenToShow = false;
		for (EMiniGameState eState : miniGameStateWhenToShowList)
		{
			if (eState == miniGameStateManagerData.mCurStateEnum)
			{
				isInWhenToShow = true;
			}
		}
		if (!isInWhenToShow)
		{
			return false;
		}
		break;
	}
	default:
		break;
	}
	
	return true;
}

void MenuManager::setAllTextBoxTextures(SDL_Renderer * pRenderer, int textIncrease)
{
	for (MenuPage* pPage : mpMenuPages)
	{
		for (TextBox* pTextBox : pPage->returnAllTextBoxes())
		{
			pTextBox->updateTexture(pRenderer, textIncrease);
		}
	}
}

void MenuManager::setUpBlocks()
{
	for (MenuPage* pPage : mpMenuPages)
	{
		for (UIBlock* pBlock : pPage->mpBlocks)
		{
			pBlock->setMaxForBoxes();
		}
	}
}

std::vector<SDL_Texture*> MenuManager::getCurTextBoxTextures()
{
	return mpCurMenuPage->getCurTextBox()->getTextBoxTexture();
}

int MenuManager::getCurTextBoxIndex() const
{
	for (int count = 0; count < mpCurMenuPage->mpCurSelectableTextBoxes.size(); count++)
	{
		if (mpCurMenuPage->mpCurSelectableTextBoxes[count] == mpCurMenuPage->getCurTextBox())
		{
			return count;
		}
	}
	return -1;
}

void MenuManager::setCurMenuPage(MenuPage* newMenuPage)
{
	mpCurMenuPage->setCurSelectedTextBox(nullptr);
	mpCurMenuPage = newMenuPage;
}

void MenuManager::renderMenus(ScreenObject& screenObject, GameStateManagerData& gameStateManagerData, MiniGameStateManagerData& miniGameStateManagerData,
	WorldData& worldData, SettingsManager& settingsManager)
{

	SDL_Renderer  * pRenderer = screenObject.mpRenderer;

	shouldSetMenuManagerCurSelectableAndDisplayOnlyTextBoxes(gameStateManagerData, miniGameStateManagerData);
	std::vector<TextBox*> allTextBoxes = mpCurMenuPage->returnAllCurTextBoxes();
	if (allTextBoxes.size() == 0)
	{
		setMenuManagerCurSelectableAndDisplayOnlyTextBoxes(miniGameStateManagerData);
	}

	bool changeText = false;

	for (TextBox* pCurTextBox : allTextBoxes)
	{
		std::string newMessage = pCurTextBox->mFullString;
		switch (pCurTextBox->mType)
		{
		case ETextBoxType_GAME_STAT_BOX:
			newMessage = updateGameStatBoxCurTextBoxMessage(worldData, settingsManager, pCurTextBox);
			break;
		case ETextBoxType_MINI_GAME_PLAYER_BOX:
			newMessage = updateCharacterStatBoxCurTextBoxMessage(miniGameStateManagerData, pCurTextBox);
			break;
		case ETextBoxType_MINI_GAME_CHARACTER_BOX:
			newMessage = updateCharacterStatBoxCurTextBoxMessage(miniGameStateManagerData, pCurTextBox);
			break;
		default:
			break;

		}

		if ((pCurTextBox->mFullString != newMessage) or (settingsManager.mTextIncrease != settingsManager.mLastFrameTextIncrease) or (pCurTextBox->mSetUp == false))
		{
			pCurTextBox->updateMessage(pRenderer, newMessage.c_str(), settingsManager.mTextIncrease);
			changeText = true;
		}
	}

	if (changeText)
	{
		mpCurMenuPage->adjustBlocks(settingsManager.mTextBoxChangeWidthFactor * settingsManager.mTextIncrease);
	}
	/*if (settingsManager.mTextIncrease != settingsManager.mLastFrameTextIncrease)
	{
	for (int count = 0; count < mpCurMenuPage->mpImageBoxes.size(); count++)
	{
	ImageBox* pCurImageBox = mpCurMenuPage->mpImageBoxes[count];
	pCurImageBox->updateHitbox(settingsManager.mTextIncrease);
	}
	}*/

	//Render
	for (TextBox* pCurTextBox : allTextBoxes)
	{
		SDL_Color curTextBoxColor = pCurTextBox->getTextBoxColor();
		SDL_SetRenderDrawColor(pRenderer, curTextBoxColor.r, curTextBoxColor.g, curTextBoxColor.b, curTextBoxColor.a);
		Hitbox box = pCurTextBox->mHitbox;

		//background box
		if (curTextBoxColor.a != 0)
		{
			//TODO TEXT ALIGN
			SDL_FRect destinationBox;
			destinationBox.x = (box.getTopLeft().getX() - 10) * screenObject.mGameScreenToGameLevelChunkRatio;
			destinationBox.y = (box.getTopLeft().getY() - 10) * screenObject.mGameScreenToGameLevelChunkRatio;
			destinationBox.w = (box.getWidth()  + 20) * screenObject.mGameScreenToGameLevelChunkRatio;
			destinationBox.h = (box.getHeight() + 20) * screenObject.mGameScreenToGameLevelChunkRatio;
			SDL_RenderFillRect(pRenderer, &destinationBox);
		}

		for (int i = 0; i < pCurTextBox->mpCurTextures.size(); i++)
		{
			Hitbox curLineBox = pCurTextBox->mCurLineHitboxes[i];
			SDL_FRect destinationText;
			destinationText.x = curLineBox.getTopLeft().getX() * screenObject.mGameScreenToGameLevelChunkRatio;
			destinationText.y = curLineBox.getTopLeft().getY() * screenObject.mGameScreenToGameLevelChunkRatio;
			destinationText.w = curLineBox.getWidth()  * screenObject.mGameScreenToGameLevelChunkRatio;
			destinationText.h = curLineBox.getHeight() * screenObject.mGameScreenToGameLevelChunkRatio;

			SDL_RenderTexture(pRenderer, pCurTextBox->mpCurTextures[i], NULL, &destinationText);
		}
	}

	//image boxes
	for (ImageBox* pCurImageBox : mpCurMenuPage->mpImageBoxes)
	{
		if (pCurImageBox->mShow == true)
		{
			float printX = float(pCurImageBox->mHitbox.getTopLeft().getX()) * screenObject.mGameScreenToGameLevelChunkRatio;
			float printY = float(pCurImageBox->mHitbox.getTopLeft().getY()) * screenObject.mGameScreenToGameLevelChunkRatio;
			float printWidth  = float(pCurImageBox->mHitbox.getWidth())  * screenObject.mGameScreenToGameLevelChunkRatio;
			float printHeight = float(pCurImageBox->mHitbox.getHeight()) * screenObject.mGameScreenToGameLevelChunkRatio;
			int rotation = pCurImageBox->mRotation;

			SDL_FRect curEntityPositionToPrintTo = { printX, printY, printWidth, printHeight };
			SDL_RenderTextureRotated(pRenderer, pCurImageBox->mImageObject.getTexture(), NULL, &curEntityPositionToPrintTo, rotation, NULL, SDL_FLIP_NONE);
		}

	}
}

void MenuManager::shouldSetMenuManagerCurSelectableAndDisplayOnlyTextBoxes(GameStateManagerData& gameStateManagerData, MiniGameStateManagerData& miniGameStateManagerData)
{	
	//Mini Game Option Boxes have a whenToShowList 
	//bool didCurMenuPageChange   = mpLastFrameMenuPage						 != mpCurMenuPage;	
	//bool didGameStateChange     = gameInstance.mGameStateManager.mLastFrameStateEnum     != gameInstance.mGameStateManager.mCurStateEnum;
	bool didMiniGameStateChange = miniGameStateManagerData.mLastFrameStateEnum != miniGameStateManagerData.mCurStateEnum;
	bool isMiniGame             = gameStateManagerData.mCurStateEnum == EGameState_PLAY_MINI_GAME;


	/*if (didCurMenuPageChange or didGameStateChange or (isMiniGame and didMiniGameStateChange))
	{
	setMenuManagerCurSelectableAndDisplayOnlyTextBoxes(gameInstance);
	}*/
	if ((isMiniGame and didMiniGameStateChange))
	{
		setMenuManagerCurSelectableAndDisplayOnlyTextBoxes(miniGameStateManagerData);
	}
}

void MenuManager::setMenuManagerCurSelectableAndDisplayOnlyTextBoxes(MiniGameStateManagerData& miniGameStateManagerData)
{
	//update menu lists
	std::vector <TextBox*> updatedCurSelectableTextBoxes;
	for (TextBox* pCurTextBox : mpCurMenuPage->mpAllSelectableTextBoxes)
	{
		if (shouldShowTextBox(miniGameStateManagerData, pCurTextBox))
		{
			updatedCurSelectableTextBoxes.push_back(pCurTextBox);
		}
	}

	std::vector <TextBox*> updatedCurDisplayOnlyTextBoxes;
	for (TextBox* pCurTextBox : mpCurMenuPage->mpAllDisplayOnlyTextBoxes)
	{
		if (shouldShowTextBox(miniGameStateManagerData, pCurTextBox))
		{
			updatedCurDisplayOnlyTextBoxes.push_back(pCurTextBox);
		}
	}
	mpCurMenuPage->setCurSelectableAndDisplayOnlyTextBoxes(updatedCurSelectableTextBoxes, updatedCurDisplayOnlyTextBoxes);
}

std::string MenuManager::updateGameStatBoxCurTextBoxMessage(WorldData& worldData, SettingsManager& settingsManager, TextBox* pTextBox)
{
	std::string message = " ";
	switch (pTextBox->mDataStorage.mGameStatToDisplay)
	{
	case EGameStatBoxValueToDisplay_CUR_LEVEL_NUMBER:
		message = "CUR LEVEL: " + std::to_string(worldData.mCurWorldNumber) + " - " + std::to_string(worldData.mCurLevelNumber);
		break;
	case EGameStatBoxValueToDisplay_CUR_ENEMIES_LEFT:
		message = std::to_string(worldData.mpWorlds[worldData.mCurWorldNumber]->mpLevels[worldData.mCurLevelNumber]->mpActiveEnemies.size());
		break;
	case EGameStatBoxValueToDisplay_CUR_KEYS:
		message = std::to_string(worldData.mPlayer.mKeys);
		break;
	case EGameStatBoxValueToDisplay_CUR_TARGETS:
		message = std::to_string(worldData.mPlayer.mTargets);
		break;
	case EGameStatBoxValueToDisplay_CUR_COLLECTIBLES:
		message = std::to_string(worldData.mNumLotusCollectibles);
		break;
	case EGameStatBoxValueToDisplay_MOVEMENT_LEFT_KEY:
		message = createStringFromKeyboardList(settingsManager.leftKeys);
		break;
	case EGameStatBoxValueToDisplay_MOVEMENT_RIGHT_KEY:
		message = createStringFromKeyboardList(settingsManager.rightKeys);
		break;
	case EGameStatBoxValueToDisplay_MOVEMENT_UP_KEY:
		message = createStringFromKeyboardList(settingsManager.upKeys);
		break;
	case EGameStatBoxValueToDisplay_LEVEL_RESET_KEY:
		message = convertKeyboardEnumToString(settingsManager.resetLevel);
		break;
	case EGameStatBoxValueToDisplay_CHECKPOINT_RESET_KEY:
		message = convertKeyboardEnumToString(settingsManager.resetCheckpoint);
		break;
	case EGameStatBoxValueToDisplay_PROJECTILE_HORIZONTAL_KEY:
		{
		if (worldData.mpWorlds[worldData.mCurWorldNumber]->mpLevels[worldData.mCurLevelNumber]->mThrowProjectileAllowed)
		{
			message = convertKeyboardEnumToString(settingsManager.shootProjectileHorizontal);
		}
		break;
	}
	case EGameStatBoxValueToDisplay_PROJECTILE_VERTICAL_KEY:
		{
		if (worldData.mpWorlds[worldData.mCurWorldNumber]->mpLevels[worldData.mCurLevelNumber]->mThrowDownwardProjectileAllowed)
		{
			message = convertKeyboardEnumToString(settingsManager.shootProjectileVertical);
		}
		break;
	}
	case EGameStatBoxValueToDisplay_DOUBLE_JUMP_KEY:
		{
		if (worldData.mpWorlds[worldData.mCurWorldNumber]->mpLevels[worldData.mCurLevelNumber]->mDoubleJumpAllowed)
		{
			message = createStringFromKeyboardList(settingsManager.upKeys);
		}
		break;
	}
	case EGameStatBoxValueToDisplay_SLASH_KEY:
		{
		if (worldData.mpWorlds[worldData.mCurWorldNumber]->mpLevels[worldData.mCurLevelNumber]->mSlashAllowed)
		{
			message = convertKeyboardEnumToString(settingsManager.slash);
		}
		break;
	}
	default:
		SDL_assert(false);
		break;
	}

	return message;
}

std::string MenuManager::updateCharacterStatBoxCurTextBoxMessage(MiniGameStateManagerData& miniGameStateManagerData, TextBox* pTextBox)
{
	CombatCharacter* pCharacter = miniGameStateManagerData.mpData->mpCharacter;
	std::string message;
	switch (pTextBox->mDataStorage.mCharacterStatToDisplay)
	{
	case ECharacterStatBoxValueToDisplay_CHARACTER_NAME:
		message = "NAME: " + pCharacter->mName;
		break;
	case ECharacterStatBoxValueToDisplay_CHARACTER_HEALTH:
		message = "HEALTH " + std::to_string(pCharacter->mCurHealth);
		break;
	case ECharacterStatBoxValueToDisplay_CHARACTER_DEFENSE:
		message = "DEFENSE " + std::to_string(pCharacter->mCurDefense);
		break;
	case ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK:
		message = "ATTACK " + std::to_string(pCharacter->mCurAttackDamage);
		break;
	case ECharacterStatBoxValueToDisplay_CHARACTER_STUN:
		message = "STUNS " + std::to_string(pCharacter->mTurnsToPass);
		break;
	case ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_1_NAME:
		message = getAttackName(pCharacter->mCombatMovementManager.getAttacks()[0]);
		break;
	case ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_1_TYPE:
		message = getAttackType(pCharacter->mCombatMovementManager.getAttacks()[0]);
		break;
	case ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_1_DAMAGE:
		message = getAttackDamage(pCharacter->mCombatMovementManager.getAttacks()[0], pCharacter->mCurAttackDamage);
		break;
	case ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_1_SPECIAL_EFFECTS_AND_NOTES:
		message = getSpecialEffect(pCharacter->mCombatMovementManager.getAttacks()[0]);
		break;
	case ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_2_NAME:
		message = getAttackName(pCharacter->mCombatMovementManager.getAttacks()[1]);
		break;
	case ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_2_TYPE:
		message = getAttackType(pCharacter->mCombatMovementManager.getAttacks()[1]);
		break;
	case ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_2_DAMAGE:
		message = getAttackDamage(pCharacter->mCombatMovementManager.getAttacks()[1], pCharacter->mCurAttackDamage);
		break;
	case ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_2_SPECIAL_EFFECTS_AND_NOTES:
		message = getSpecialEffect(pCharacter->mCombatMovementManager.getAttacks()[1]);
		break;
	case ECharacterStatBoxValueToDisplay_CHARACTER_CUR_ATTACK_NAME:
		message = miniGameStateManagerData.mpData->mCurAttack.mName;
		break;
	case ECharacterStatBoxValueToDisplay_CHARACTER_MOVE_TYPE:
		{
		int moveType = pCharacter->mCombatMovementManager.getMoveType();
		message = returnDescriptionOfMoveAttackType(moveType);
		break;
	}
	default:
		SDL_assert(false);
		break;
	}

	return message;
}

void MenuManager::updateUIElements(ScreenObject& screenObject, WorldData& worldData)
{
	Level* pCurLevel = worldData.mpWorlds[worldData.mCurWorldNumber]->mpLevels[worldData.mCurLevelNumber];

	int curProjectileBoxIndex = -1;
	for (int count = 0; count < mpCurMenuPage->mpImageBoxes.size(); count++)
	{
		ImageBox* pCurImageBox = mpCurMenuPage->mpImageBoxes[count];
		switch (pCurImageBox->mID)
		{
		case ETextBoxID_TAKE_DAMAGE_SCREEN:
		{
			if (worldData.mPlayer.takingDamage())
			{
				pCurImageBox->mShow = true;
			}
			else
			{
				pCurImageBox->mShow = false;
			}
			break;
		}
		case ETextBoxID_PROJECTILE_UI:
			if (curProjectileBoxIndex == -1)
			{
				curProjectileBoxIndex = count;
			}
			break;
		case ETextBoxID_DOUBLE_JUMP_UI:
			{
			if (pCurLevel->mDoubleJumpAllowed)
			{
				pCurImageBox->mShow = true;
			}
			else
			{
				pCurImageBox->mShow = false;
			}
			break;
		}
		case ETextBoxID_SLASH_UI:
			{
			if (pCurLevel->mSlashAllowed)
			{
				pCurImageBox->mShow = true;
			}
			else
			{
				pCurImageBox->mShow = false;
			}
			break;
		}
		default:
			break;
		}
	}

	//projectiles
	if (pCurLevel->mThrowProjectileAllowed)
	{
		for (int count = 0; count < worldData.mProjectileLimit - worldData.getNumPlayerProjectiles(); count++)
		{
			ImageBox* pCurImageBox = mpCurMenuPage->mpImageBoxes[curProjectileBoxIndex];
			pCurImageBox->mShow = true;
			curProjectileBoxIndex++;
		}
		for (int count = worldData.mProjectileLimit - worldData.getNumPlayerProjectiles(); count < worldData.mMaxProjectileLimit; count++)
		{
			ImageBox* pCurImageBox = mpCurMenuPage->mpImageBoxes[curProjectileBoxIndex];
			pCurImageBox->mShow = false;
			curProjectileBoxIndex++;
		}
	}
	else
	{
		for (int count = 0; count < worldData.mMaxProjectileLimit; count++)
		{
			ImageBox* pCurImageBox = mpCurMenuPage->mpImageBoxes[curProjectileBoxIndex];
			pCurImageBox->mShow = false;
			curProjectileBoxIndex++;
		}
	}

}

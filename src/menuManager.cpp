#include "menuManager.h"


MenuManager::MenuManager(ScreenObject& screen, WorldData& worldData, SettingsManager& settingsManager, FontSizeChart& fontSizeChart, 
		MiniGameStateManagerData& miniGameStateManagerData, MiniGameWorldData& miniWorldData) 
		: mScreen(screen), mWorldData(worldData), mSettingsManager(settingsManager), mFontSizeChart(fontSizeChart),
		mMiniGameStateManagerData(miniGameStateManagerData), mMiniGameWorldData(miniWorldData) { ; }

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
	mpCurMenuPage->preTick();
}

TextBox* MenuManager::returnMouseTextBox(Vect2& gameUnitsMousePos)
{
	int  x = int( (gameUnitsMousePos.getX() - 5) / mScreen.mGameScreenToGameLevelChunkRatio);
	int x2 = int( (gameUnitsMousePos.getX() + 5) / mScreen.mGameScreenToGameLevelChunkRatio);
	int  y = int( (gameUnitsMousePos.getY() - 5) / mScreen.mGameScreenToGameLevelChunkRatio);
	int y2 = int( (gameUnitsMousePos.getY() + 5) / mScreen.mGameScreenToGameLevelChunkRatio);

	if (x < 0)
	{
		x = 0;
	}
	if (y < 0)
	{
		y = 0;
	}

	Hitbox mouseHitbox(x, x2, y, y2);

	for (TextBox* pTextBox : mpCurMenuPage->mpAllSelectableTextBoxes)
	{
		if (pTextBox->mShow && pTextBox->mpCurHitbox->overlap(mouseHitbox))
		{
			return pTextBox;
		}
	}
	return nullptr;
}

void MenuManager::setAllTextBoxTextures()
{
	for (MenuPage* pPage : mpMenuPages)
	{
		for (TextBox* pTextBox : pPage->getAllTextBoxes())
		{
			pTextBox->updateTexture(mScreen.mpRenderer);
		}
	}
}

void MenuManager::setUpBlocks()
{
	for (MenuPage* pPage : mpMenuPages)
	{
		for (UIElement* pElem : pPage->mpElems)
		{
			if (pElem->mClassType == EUIClass_BLOCK)
			{
				((UIBlock*)pElem)->setMaxSize();
			}
		}
	}
}

void MenuManager::setCurMenuPage(MenuPage* pNewMenuPage)
{
	mpCurMenuPage->setCurSelectedTextBox(nullptr);
	mpCurMenuPage->setCurTextBox(nullptr);
	mpCurMenuPage = pNewMenuPage;
}

void MenuManager::renderMenus(EGameState curState, bool forceUpdate, std::string& curKeys)
{
	getUpdatedMenuBoxes(curState, forceUpdate, curKeys);
	printBoxes();
}

void MenuManager::getUpdatedMenuBoxes(EGameState curState, bool forceUpdate, std::string& curKeys)
{
	SDL_Renderer* pRenderer = mScreen.mpRenderer;

	bool updated = false;
	if (shouldUpdateTextBoxShowState(curState, forceUpdate))
	{
		updated = true;
		mpCurMenuPage->updateAllUIBoxesShowState(mMiniGameStateManagerData, mMiniGameWorldData.getStage()->mCombatManager);
	}
	if (mpCurMenuPage->getCurTextBox() == nullptr) 
	{
		mpCurMenuPage->setDefaultSelectedBox();
	}
	if (mpCurMenuPage->curTextBoxChange()) 
	{
		updated = true;
	}

	for (TextBox* pCurTextBox : mpCurMenuPage->getCurTextBoxes())
	{
		std::string updatedMessage = pCurTextBox->mMessage;
		switch (pCurTextBox->mData.mType)
		{
		case EUIBoxType_GAME_STAT_BOX:
			updatedMessage = updateGameStatBoxCurTextBoxMessage(*pCurTextBox, curKeys, mWorldData, mSettingsManager);
			break;
		case EUIBoxType_MINI_GAME_STAT_BOX:
			updatedMessage = updateMiniGameStatBoxCurTextBoxMessage(*pCurTextBox, mMiniGameStateManagerData, mMiniGameWorldData);
			break;
		case EUIBoxType_MINI_GAME_PLAYER_BOX:
		case EUIBoxType_MINI_GAME_CHARACTER_BOX:
		case EUIBoxType_MINI_GAME_PLAYER_ATTACK_BOX:
			updatedMessage = updateCharacterStatBoxCurTextBoxMessage(*pCurTextBox, mMiniGameStateManagerData, mMiniGameWorldData);
			break;
		default:
			break;
		}

		if ((pCurTextBox->mMessage != updatedMessage) or !pCurTextBox->mSetUp)
		{
			pCurTextBox->updateMessage(pRenderer, mFontSizeChart, updatedMessage);
			updated = true;
		}
	}

	for (HealthBox* pHealthBox : mpCurMenuPage->mpHealthBoxes)
	{
		std::string updatedMessage = updateHealthStatBoxCurTextBoxMessage(*pHealthBox, mMiniGameWorldData);

		if ((pHealthBox->mHealthText.mMessage != updatedMessage) or !pHealthBox->mHealthText.mSetUp)
		{
			float ratio = updateHealthStatBoxCurTextBoxRatio(*pHealthBox, mMiniGameWorldData);
			pHealthBox->updateMessage(pRenderer, mFontSizeChart, updatedMessage, ratio);
			updated = true;
		}
	}

	if (updated)
	{
		mpCurMenuPage->adjustBlocks();
	}
}

void MenuManager::printBoxes()
{
	UIBox* pBox;

	for (UIElement* pElem : mpCurMenuPage->getAllElems()) 
	{
		if (!pElem->isActive())
		{
			continue;
		}
		switch (pElem->mClassType)
		{
		case EUIClass_BLOCK:
			printBlock(mScreen, *(UIBlock*)pElem);
			break;
		case EUIClass_BOX:
			pBox = (UIBox*)pElem;
			switch (pBox->mBoxType)
			{
			case EUIBoxClass_TEXTBOX:
				printTextBox(mScreen, *(TextBox*)pBox);
				break;

			case EUIBoxClass_IMAGEBOX:
				printImageBox(mScreen, *(ImageBox*)pBox);
				break;

			case EUIBoxClass_SHAPEBOX:
				printShapeBox(mScreen, *(ShapeBox*)pBox);
				break;

			case EUIBoxClass_HEALTHBOX:

				printHealthBox(mScreen, *(HealthBox*)pBox);
				break;
			}
			break;
		}
		
	}
}

bool MenuManager::shouldUpdateTextBoxShowState(EGameState curState, bool forceUpdate)
{	
	// Mini Game Option Boxes have a whenToShowList 
	bool didMiniGameStateChange = (mMiniGameStateManagerData.mLastFrameStateEnum != mMiniGameStateManagerData.mCurStateEnum || mMiniGameStateManagerData.mTicksSinceUndo <= 2);
	bool isMiniGame             = (curState == EGameState_PLAY_MINI_GAME);

	return (isMiniGame and didMiniGameStateChange) or forceUpdate;
}

void MenuManager::updateUIElements()
{
	Level* pCurLevel = mWorldData.mpWorlds[mWorldData.mCurWorldNumber]->mpLevels[mWorldData.mCurLevelNumber];

	int curProjectileBoxIndex = -1;
	for (int count = 0; count < mpCurMenuPage->mpImageBoxes.size(); count++)
	{
		ImageBox* pCurImageBox = mpCurMenuPage->mpImageBoxes[count];
		switch (pCurImageBox->mData.mType)
		{
		case EUIBoxType_TAKE_DAMAGE_SCREEN:
			pCurImageBox->mShow = mWorldData.mPlayer.takingDamage();
			break;
		case EUIBoxType_PROJECTILE_UI:
			if (curProjectileBoxIndex == -1)
			{
				curProjectileBoxIndex = count;
			}
			break;
		case EUIBoxType_DOUBLE_JUMP_UI:
			pCurImageBox->mShow = pCurLevel->mDoubleJumpAllowed;
			break;
		case EUIBoxType_SLASH_UI:
			pCurImageBox->mShow = pCurLevel->mSlashAllowed;
			break;
		default:
			break;
		}
	}

	// draw available player projectiles
	if (pCurLevel->mThrowProjectileAllowed)
	{
		for (int count = 0; count < mWorldData.mProjectileLimit - mWorldData.getNumPlayerProjectiles(); count++)
		{
			ImageBox* pCurImageBox = mpCurMenuPage->mpImageBoxes[curProjectileBoxIndex];
			pCurImageBox->mShow = true;
			curProjectileBoxIndex++;
		}
		for (int count = mWorldData.mProjectileLimit - mWorldData.getNumPlayerProjectiles(); count < mWorldData.mMaxProjectileLimit; count++)
		{
			ImageBox* pCurImageBox = mpCurMenuPage->mpImageBoxes[curProjectileBoxIndex];
			pCurImageBox->mShow = false;
			curProjectileBoxIndex++;
		}
	}
	else
	{
		for (int count = 0; count < mWorldData.mMaxProjectileLimit; count++)
		{
			ImageBox* pCurImageBox = mpCurMenuPage->mpImageBoxes[curProjectileBoxIndex];
			pCurImageBox->mShow = false;
			curProjectileBoxIndex++;
		}
	}

}

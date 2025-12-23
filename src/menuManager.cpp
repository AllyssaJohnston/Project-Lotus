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
		for (UIBlock* pBlock : pPage->mpBlocks)
		{
			pBlock->setMaxForBoxes();
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
	SDL_Renderer* pRenderer = mScreen.mpRenderer;

	for (UIBlock* pBlock : mpCurMenuPage->getAllBlocks()) 
	{
		SDL_Color curTextBoxColor = pBlock->mBackgroundColor;
		if (curTextBoxColor.a != 0)
		{
			SDL_SetRenderDrawColor(pRenderer, curTextBoxColor.r, curTextBoxColor.g, curTextBoxColor.b, curTextBoxColor.a);

			const Hitbox& box = pBlock->mHitbox;
			SDL_FRect rect = {	box.getTopLeft().getX() * mScreen.mGameScreenToGameLevelChunkRatio,
								box.getTopLeft().getY() * mScreen.mGameScreenToGameLevelChunkRatio,
								box.getWidth() * mScreen.mGameScreenToGameLevelChunkRatio,
								box.getHeight() * mScreen.mGameScreenToGameLevelChunkRatio };
			SDL_RenderFillRect(pRenderer, &rect);
		}
	}

	std::vector<TextBox*> allTextBoxes = mpCurMenuPage->getCurTextBoxes();
	for (TextBox* pCurTextBox : allTextBoxes)
	{
		SDL_Color curTextBoxColor = pCurTextBox->getTextBoxColor();
		SDL_Color curOutlineColor = pCurTextBox->getIsHighlighted() ? pCurTextBox->mHighlightedOutlineColor : pCurTextBox->mOutlineColor;
		
		const Hitbox& box = *pCurTextBox->mpCurHitbox;
		// outline box
		if (pCurTextBox->mOutlineWidth != 0 && curOutlineColor.a != 0)
		{
			SDL_SetRenderDrawColor(pRenderer, curOutlineColor.r, curOutlineColor.g, curOutlineColor.b, curOutlineColor.a);
			SDL_FRect rect = {	box.getTopLeft().getX() * mScreen.mGameScreenToGameLevelChunkRatio ,
								box.getTopLeft().getY() * mScreen.mGameScreenToGameLevelChunkRatio,
								box.getWidth() * mScreen.mGameScreenToGameLevelChunkRatio,
								box.getHeight() * mScreen.mGameScreenToGameLevelChunkRatio };
			SDL_RenderFillRect(pRenderer, &rect);
		}

		// background box
		if (curTextBoxColor.a != 0)
		{
			SDL_SetRenderDrawColor(pRenderer, curTextBoxColor.r, curTextBoxColor.g, curTextBoxColor.b, curTextBoxColor.a);
			SDL_FRect rect = {	(box.getTopLeft().getX() + pCurTextBox->mOutlineWidth) * mScreen.mGameScreenToGameLevelChunkRatio ,
								(box.getTopLeft().getY() + pCurTextBox->mOutlineWidth) * mScreen.mGameScreenToGameLevelChunkRatio,
								(box.getWidth()  - 2 * pCurTextBox->mOutlineWidth) * mScreen.mGameScreenToGameLevelChunkRatio,
								(box.getHeight() - 2 * pCurTextBox->mOutlineWidth) * mScreen.mGameScreenToGameLevelChunkRatio };
			SDL_RenderFillRect(pRenderer, &rect);
		}

		// text
		for (int i = 0; i < (*(pCurTextBox->mpCurTextures)).size(); i++)
		{
			const Hitbox& curLineBox = (*(pCurTextBox->mpCurLineHitboxes))[i];
			SDL_FRect destinationText = {	curLineBox.getTopLeft().getX() * mScreen.mGameScreenToGameLevelChunkRatio ,
											curLineBox.getTopLeft().getY() * mScreen.mGameScreenToGameLevelChunkRatio,
											curLineBox.getWidth() * mScreen.mGameScreenToGameLevelChunkRatio,
											curLineBox.getHeight() * mScreen.mGameScreenToGameLevelChunkRatio };
			SDL_RenderTexture(pRenderer, (*(pCurTextBox->mpCurTextures))[i], NULL, &destinationText);
		}
	}

	// image boxes
	for (ImageBox* pCurImageBox : mpCurMenuPage->mpImageBoxes)
	{
		if (pCurImageBox->mShow)
		{
			SDL_FRect rect = {  pCurImageBox->mpCurHitbox->getTopLeft().getX() * mScreen.mGameScreenToGameLevelChunkRatio ,
								pCurImageBox->mpCurHitbox->getTopLeft().getY() * mScreen.mGameScreenToGameLevelChunkRatio,
								pCurImageBox->mpCurHitbox->getWidth() * mScreen.mGameScreenToGameLevelChunkRatio,
								pCurImageBox->mpCurHitbox->getHeight() * mScreen.mGameScreenToGameLevelChunkRatio };
			int rotation = pCurImageBox->mRotation;

			SDL_RenderTextureRotated(pRenderer, pCurImageBox->mImageObject.getTexture(), NULL, &rect, rotation, NULL, SDL_FLIP_NONE);
		}

	}

	// shape boxes
	for (ShapeBox* pShapeBox : mpCurMenuPage->mpShapeBoxes)
	{
		if (pShapeBox->mShow && pShapeBox->mColor.a != 0)
		{
			SDL_FRect rect;

			switch (pShapeBox->mShapeType) 
			{
			case EShapeBoxClass_CIRCLE:
				drawCircle(pShapeBox->mColor, pShapeBox->mpCurHitbox->getCenter(), pShapeBox->mpCurHitbox->getWidth() / 2, mScreen);
				break;
			case EShapeBoxClass_RECT:
				rect = {	pShapeBox->mpCurHitbox->getTopLeft().getX() * mScreen.mGameScreenToGameLevelChunkRatio ,
							pShapeBox->mpCurHitbox->getTopLeft().getY() * mScreen.mGameScreenToGameLevelChunkRatio,
							pShapeBox->mpCurHitbox->getWidth() * mScreen.mGameScreenToGameLevelChunkRatio,
							pShapeBox->mpCurHitbox->getHeight() * mScreen.mGameScreenToGameLevelChunkRatio };
				SDL_RenderFillRect(pRenderer, &rect);
				break;
			default:
				SDL_assert(false);
			}
		}
	}

	// health boxes
	for (HealthBox* pHealthBox : mpCurMenuPage->mpHealthBoxes)
	{
		if (pHealthBox->mShow)
		{
			SDL_SetRenderDrawColor(pRenderer, pHealthBox->mBoundingBox.mColor.r, pHealthBox->mBoundingBox.mColor.g, pHealthBox->mBoundingBox.mColor.b, pHealthBox->mBoundingBox.mColor.a);
			SDL_FRect boundingRect = { pHealthBox->mBoundingBox.mpCurHitbox->getTopLeft().getX() * mScreen.mGameScreenToGameLevelChunkRatio ,
										pHealthBox->mBoundingBox.mpCurHitbox->getTopLeft().getY() * mScreen.mGameScreenToGameLevelChunkRatio,
										pHealthBox->mBoundingBox.mpCurHitbox->getWidth() * mScreen.mGameScreenToGameLevelChunkRatio,
										pHealthBox->mBoundingBox.mpCurHitbox->getHeight() * mScreen.mGameScreenToGameLevelChunkRatio };
			SDL_RenderFillRect(pRenderer, &boundingRect);

			SDL_SetRenderDrawColor(pRenderer, pHealthBox->mHealthLeftBox.mColor.r, pHealthBox->mHealthLeftBox.mColor.g, pHealthBox->mHealthLeftBox.mColor.b, pHealthBox->mHealthLeftBox.mColor.a);
			SDL_FRect healthRect = { pHealthBox->mHealthLeftBox.mpCurHitbox->getTopLeft().getX() * mScreen.mGameScreenToGameLevelChunkRatio ,
										pHealthBox->mHealthLeftBox.mpCurHitbox->getTopLeft().getY() * mScreen.mGameScreenToGameLevelChunkRatio,
										pHealthBox->mHealthLeftBox.mpCurHitbox->getWidth() * mScreen.mGameScreenToGameLevelChunkRatio,
										pHealthBox->mHealthLeftBox.mpCurHitbox->getHeight() * mScreen.mGameScreenToGameLevelChunkRatio };
			SDL_RenderFillRect(pRenderer, &healthRect);

			// text
			for (int i = 0; i < (*(pHealthBox->mHealthText.mpCurTextures)).size(); i++)
			{
				const Hitbox& curLineBox = (*(pHealthBox->mHealthText.mpCurLineHitboxes))[i];
				SDL_FRect destinationText = { curLineBox.getTopLeft().getX() * mScreen.mGameScreenToGameLevelChunkRatio ,
												curLineBox.getTopLeft().getY() * mScreen.mGameScreenToGameLevelChunkRatio,
												curLineBox.getWidth() * mScreen.mGameScreenToGameLevelChunkRatio,
												curLineBox.getHeight() * mScreen.mGameScreenToGameLevelChunkRatio };
				SDL_RenderTexture(pRenderer, (*(pHealthBox->mHealthText.mpCurTextures))[i], NULL, &destinationText);
			}
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

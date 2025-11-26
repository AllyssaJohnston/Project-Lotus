#include "menuHelper.h"

MenuPage::~MenuPage()
{
	mpCurSelectedTextBox = nullptr;
	mpLastFrameCurTextBox = nullptr;
	mpCurTextBox = nullptr;
	for (TextBox* box : mpAllSelectableTextBoxes)
	{
		delete box;
	}
	for (TextBox* box : mpAllDisplayOnlyTextBoxes)
	{
		delete box;
	}
	for (ImageBox* box : mpImageBoxes)
	{
		delete box;
	}
	for (UIBlock* box : mpBlocks)
	{
		delete box;
	}
}

void MenuPage::preTick() 
{
	mpLastFrameCurTextBox = mpCurTextBox;
}

void MenuPage::addBox(TextBox* pTextBox, bool selectable, UIBlock* pBlock) 
{
	pBlock->mpAllBoxes.push_back(pTextBox);
	if (selectable) 
	{
		mpAllSelectableTextBoxes.push_back(pTextBox);
	}
	else 
	{
		mpAllDisplayOnlyTextBoxes.push_back(pTextBox);
	}
}

void MenuPage::addBox(ImageBox* pBox, UIBlock* pBlock)
{
	pBlock->mpAllBoxes.push_back(pBox);
	mpImageBoxes.push_back(pBox);
}

void MenuPage::addBox(ShapeBox* pBox, UIBlock* pBlock)
{
	pBlock->mpAllBoxes.push_back(pBox);
	mpShapeBoxes.push_back(pBox);
}

void MenuPage::addBox(HealthBox* pBox, UIBlock* pBlock)
{
	pBlock->mpAllBoxes.push_back(pBox);
	mpHealthBoxes.push_back(pBox);
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
		}
	}
}

void MenuPage::setCurTextBoxIfValid(int count)
{
	if (count >= 0 and count < mpAllSelectableTextBoxes.size() and mpAllSelectableTextBoxes[count]->mShow)
	{
		setCurTextBox(mpAllSelectableTextBoxes[count]);
	}
}

void MenuPage::setCurTextBox(TextBox * textBox)
{
	if (mpCurTextBox != nullptr)
	{
		mpCurTextBox->changeIsHighlighted(false);
	}
	mpCurTextBox = textBox;
	if (mpCurTextBox != nullptr) 
	{
		mpCurTextBox->changeIsHighlighted(true);
	}
}

void MenuPage::setCurSelectedTextBox(TextBox* textBox)  { mpCurSelectedTextBox = textBox; }

bool MenuPage::curTextBoxChange() { return mpLastFrameCurTextBox != mpCurTextBox; }

TextBox* MenuPage::getCurTextBox() { return mpCurTextBox; }

TextBox* MenuPage::getCurSelectedTextBox() { return mpCurSelectedTextBox; }

int MenuPage::getCurTextBoxIndex()
{
	for (int count = 0; count < mpAllSelectableTextBoxes.size(); count++)
	{
		if (mpAllSelectableTextBoxes[count] == mpCurTextBox)
		{
			return count;
		}
	}
	return -1;
}

std::vector<SDL_Texture*> MenuPage::getCurTextBoxTextures() const { return mpCurTextBox->getTextBoxTexture(); }

std::vector <TextBox*> MenuPage::getAllTextBoxes()
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

std::vector <TextBox*> MenuPage::getCurTextBoxes()
{
	std::vector <TextBox*> allCurTextBoxes;
	for (TextBox* pCurTextBox : mpAllSelectableTextBoxes)
	{
		if (pCurTextBox->mShow) 
		{
			allCurTextBoxes.push_back(pCurTextBox);
		}
	}
	
	for (TextBox* pCurTextBox : mpAllDisplayOnlyTextBoxes)
	{
		if (pCurTextBox->mShow)
		{
			allCurTextBoxes.push_back(pCurTextBox);
		}
	}

	return allCurTextBoxes;
}

std::vector <UIBlock*> MenuPage::getAllBlocks() 
{
	std::vector <UIBlock*> list;
	for (UIBlock* pBlock : mpBlocks) 
	{
		pBlock->getAllBlocksInternal(list);
	}
	return list;
}



void MenuPage::updateAllTextBoxShowState(MiniGameStateManagerData& data)
{
	//update show state for all textboxes
	for (TextBox* pCurTextBox : mpAllSelectableTextBoxes)
	{
		pCurTextBox->mShow = shouldShowTextBox(pCurTextBox, data);
	}

	for (TextBox* pCurTextBox : mpAllDisplayOnlyTextBoxes)
	{
		pCurTextBox->mShow = shouldShowTextBox(pCurTextBox, data);
	}

	for (ShapeBox* pShapeBox : mpShapeBoxes)
	{
		pShapeBox->mShow = shouldShowTextBox(pShapeBox, data);
	}
}

void MenuPage::setDefaultSelectedBox() 
{
	for (TextBox* pCurTextBox : mpAllSelectableTextBoxes)
	{
		if (pCurTextBox->mShow)
		{
			setCurTextBox(pCurTextBox);
			return;
		}
	}
}


void MenuPage::adjustBlocks()
{
	for (UIBlock* pBlock : mpBlocks)
	{
		pBlock->updateBlocks();
	}
}

void MenuPage::deleteBlock(UIBlock* pBlock)
{
	std::vector<UIBox*> allBoxes = pBlock->getAllBoxes();
	for (int i = (int)allBoxes.size() - 1; i > -1; i--) 
	{
		UIBox* pBox = allBoxes[i];
		switch (pBox->mClassType) 
		{
		case EUIBoxClass_TEXTBOX:
			mpAllDisplayOnlyTextBoxes.erase(std::remove(mpAllDisplayOnlyTextBoxes.begin(), mpAllDisplayOnlyTextBoxes.end(), pBox), mpAllDisplayOnlyTextBoxes.end());
			mpAllSelectableTextBoxes.erase(std::remove(mpAllSelectableTextBoxes.begin(), mpAllSelectableTextBoxes.end(), pBox), mpAllSelectableTextBoxes.end());
			break;
		case EUIBoxClass_IMAGEBOX:
			mpImageBoxes.erase(std::remove(mpImageBoxes.begin(), mpImageBoxes.end(), pBox), mpImageBoxes.end());
			break;
		case EUIBoxClass_SHAPEBOX:
			mpShapeBoxes.erase(std::remove(mpShapeBoxes.begin(), mpShapeBoxes.end(), pBox), mpShapeBoxes.end());
			break;
		case EUIBoxClass_HEALTHBOX:
			mpHealthBoxes.erase(std::remove(mpHealthBoxes.begin(), mpHealthBoxes.end(), pBox), mpHealthBoxes.end());
			break;
		}
		
		delete pBox;
	}
	delete pBlock;
}



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

TextBox* MenuManager::returnMouseTextBox(Vect2 gameUnitsMousePos)
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

void MenuManager::setCurMenuPage(MenuPage* newMenuPage)
{
	mpCurMenuPage->setCurSelectedTextBox(nullptr);
	mpCurMenuPage->setCurTextBox(nullptr);
	mpCurMenuPage = newMenuPage;
}

void MenuManager::renderMenus(EGameState curState, bool forceUpdate, std::string curKeys)
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
		mpCurMenuPage->updateAllTextBoxShowState(mMiniGameStateManagerData);
	}
	if (mpCurMenuPage->getCurTextBox() == nullptr) 
	{
		mpCurMenuPage->setDefaultSelectedBox();
	}
	if (mpCurMenuPage->curTextBoxChange()) 
	{
		updated = true;
	}
	std::vector<TextBox*> allTextBoxes = mpCurMenuPage->getCurTextBoxes();

	
	for (TextBox* pCurTextBox : allTextBoxes)
	{
		std::string updatedMessage = pCurTextBox->mDataStorage.mMessage;
		switch (pCurTextBox->mType)
		{
		case ETextBoxType_GAME_STAT_BOX:
			updatedMessage = updateGameStatBoxCurTextBoxMessage(pCurTextBox, curKeys, mWorldData, mSettingsManager);
			break;
		case ETextBoxType_MINI_GAME_STAT_BOX:
			updatedMessage = updateMiniGameStatBoxCurTextBoxMessage(pCurTextBox, mMiniGameStateManagerData, mMiniGameWorldData);
			break;
		case ETextBoxType_MINI_GAME_PLAYER_BOX:
		case ETextBoxType_MINI_GAME_CHARACTER_BOX:
			updatedMessage = updateCharacterStatBoxCurTextBoxMessage(pCurTextBox, mMiniGameStateManagerData, mMiniGameWorldData);
			break;
		default:
			break;
		}

		if ((pCurTextBox->mDataStorage.mMessage != updatedMessage) or (mSettingsManager.mTextIncrease != mSettingsManager.mLastFrameTextIncrease) or !pCurTextBox->mSetUp)
		{
			pCurTextBox->updateMessage(pRenderer, mFontSizeChart, updatedMessage);
			updated = true;
		}
	}
	for (HealthBox* pHealthBox : mpCurMenuPage->mpHealthBoxes)
	{
		std::string updatedMessage = updateHealthStatBoxCurTextBoxMessage(pHealthBox, mMiniGameWorldData);

		if ((pHealthBox->mHealthText.mDataStorage.mMessage != updatedMessage) or (mSettingsManager.mTextIncrease != mSettingsManager.mLastFrameTextIncrease) or !pHealthBox->mHealthText.mSetUp)
		{
			float healthRatio = mMiniGameWorldData.mpMiniGameLevels[mMiniGameWorldData.mCurMiniGameLevelNumber]->mCombatManager.mpAllCombatCharacters[pHealthBox->mCombatCharacterIndex]->getHealthRatio();
			pHealthBox->updateMessage(pRenderer, mFontSizeChart, updatedMessage, healthRatio);
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
		//outline box
		if (pCurTextBox->mOutlineWidth != 0 && curOutlineColor.a != 0)
		{
			SDL_SetRenderDrawColor(pRenderer, curOutlineColor.r, curOutlineColor.g, curOutlineColor.b, curOutlineColor.a);
			SDL_FRect rect = {	box.getTopLeft().getX() * mScreen.mGameScreenToGameLevelChunkRatio ,
								box.getTopLeft().getY() * mScreen.mGameScreenToGameLevelChunkRatio,
								box.getWidth() * mScreen.mGameScreenToGameLevelChunkRatio,
								box.getHeight() * mScreen.mGameScreenToGameLevelChunkRatio };
			SDL_RenderFillRect(pRenderer, &rect);
		}

		//background box
		if (curTextBoxColor.a != 0)
		{
			SDL_SetRenderDrawColor(pRenderer, curTextBoxColor.r, curTextBoxColor.g, curTextBoxColor.b, curTextBoxColor.a);
			SDL_FRect rect = {	(box.getTopLeft().getX() + pCurTextBox->mOutlineWidth) * mScreen.mGameScreenToGameLevelChunkRatio ,
								(box.getTopLeft().getY() + pCurTextBox->mOutlineWidth) * mScreen.mGameScreenToGameLevelChunkRatio,
								(box.getWidth()  - 2 * pCurTextBox->mOutlineWidth) * mScreen.mGameScreenToGameLevelChunkRatio,
								(box.getHeight() - 2 * pCurTextBox->mOutlineWidth) * mScreen.mGameScreenToGameLevelChunkRatio };
			SDL_RenderFillRect(pRenderer, &rect);
		}

		//text
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

	//image boxes
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

	//shape boxes
	for (ShapeBox* pShapeBox : mpCurMenuPage->mpShapeBoxes)
	{
		if (pShapeBox->mShow)
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

	//health boxes
	for (HealthBox* pHealthBox : mpCurMenuPage->mpHealthBoxes)
	{
		SDL_SetRenderDrawColor(pRenderer, pHealthBox->mBoundingBox.mColor.r, pHealthBox->mBoundingBox.mColor.g, pHealthBox->mBoundingBox.mColor.b, pHealthBox->mBoundingBox.mColor.a);
		SDL_FRect boundingRect = {	pHealthBox->mBoundingBox.mpCurHitbox->getTopLeft().getX() * mScreen.mGameScreenToGameLevelChunkRatio ,
									pHealthBox->mBoundingBox.mpCurHitbox->getTopLeft().getY() * mScreen.mGameScreenToGameLevelChunkRatio,
									pHealthBox->mBoundingBox.mpCurHitbox->getWidth()  * mScreen.mGameScreenToGameLevelChunkRatio,
									pHealthBox->mBoundingBox.mpCurHitbox->getHeight() * mScreen.mGameScreenToGameLevelChunkRatio };
		SDL_RenderFillRect(pRenderer, &boundingRect);

		SDL_SetRenderDrawColor(pRenderer, pHealthBox->mHealthLeftBox.mColor.r, pHealthBox->mHealthLeftBox.mColor.g, pHealthBox->mHealthLeftBox.mColor.b, pHealthBox->mHealthLeftBox.mColor.a);
		SDL_FRect healthRect = {	pHealthBox->mHealthLeftBox.mpCurHitbox->getTopLeft().getX() * mScreen.mGameScreenToGameLevelChunkRatio ,
									pHealthBox->mHealthLeftBox.mpCurHitbox->getTopLeft().getY() * mScreen.mGameScreenToGameLevelChunkRatio,
									pHealthBox->mHealthLeftBox.mpCurHitbox->getWidth() * mScreen.mGameScreenToGameLevelChunkRatio,
									pHealthBox->mHealthLeftBox.mpCurHitbox->getHeight() * mScreen.mGameScreenToGameLevelChunkRatio };
		SDL_RenderFillRect(pRenderer, &healthRect);

		//text
		for (int i = 0; i < (*(pHealthBox->mHealthText.mpCurTextures)).size(); i++)
		{
			const Hitbox& curLineBox = (*(pHealthBox->mHealthText.mpCurLineHitboxes))[i];
			SDL_FRect destinationText = {	curLineBox.getTopLeft().getX() * mScreen.mGameScreenToGameLevelChunkRatio ,
											curLineBox.getTopLeft().getY() * mScreen.mGameScreenToGameLevelChunkRatio,
											curLineBox.getWidth() * mScreen.mGameScreenToGameLevelChunkRatio,
											curLineBox.getHeight() * mScreen.mGameScreenToGameLevelChunkRatio };
			SDL_RenderTexture(pRenderer, (*(pHealthBox->mHealthText.mpCurTextures))[i], NULL, &destinationText);
		}
	}
}

bool MenuManager::shouldUpdateTextBoxShowState(EGameState curState, bool forceUpdate)
{	
	//Mini Game Option Boxes have a whenToShowList 
	//bool didGameStateChange = ()
	bool didMiniGameStateChange = (mMiniGameStateManagerData.mLastFrameStateEnum != mMiniGameStateManagerData.mCurStateEnum);
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
		switch (pCurImageBox->mID)
		{
		case ETextBoxID_TAKE_DAMAGE_SCREEN:
			pCurImageBox->mShow = mWorldData.mPlayer.takingDamage();
			break;
		case ETextBoxID_PROJECTILE_UI:
			if (curProjectileBoxIndex == -1)
			{
				curProjectileBoxIndex = count;
			}
			break;
		case ETextBoxID_DOUBLE_JUMP_UI:
			pCurImageBox->mShow = pCurLevel->mDoubleJumpAllowed;
			break;
		case ETextBoxID_SLASH_UI:
			
			pCurImageBox->mShow = pCurLevel->mSlashAllowed;
			break;
		default:
			break;
		}
	}

	//projectiles
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

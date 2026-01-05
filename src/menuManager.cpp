#include "menuManager.h"


MenuManager::MenuManager(ScreenObject& screen, SettingsManager& settingsManager, FontSizeChart& fontSizeChart ) 
		: mScreen(screen), mSettingsManager(settingsManager), mFontSizeChart(fontSizeChart) { ; }

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



void MenuManager::renderMenus() { printBoxes(); }

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
			printBlock(*(UIBlock*)pElem);
			break;
		case EUIClass_BOX:
			pBox = (UIBox*)pElem;
			switch (pBox->mBoxType)
			{
			case EUIBoxClass_TEXTBOX:
				printTextBox(*(TextBox*)pBox);
				break;

			case EUIBoxClass_IMAGEBOX:
				printImageBox(*(ImageBox*)pBox);
				break;

			case EUIBoxClass_SHAPEBOX:
				printShapeBox(*(ShapeBox*)pBox);
				break;

			case EUIBoxClass_HEALTHBOX:

				printHealthBox(*(HealthBox*)pBox);
				break;
			}
			break;
		}
		
	}
}

void MenuManager::printBlock(const UIBlock& block)
{
	SDL_Renderer* pRenderer = mScreen.mpRenderer;
	SDL_Color curBackgroundColor = block.mBackgroundColor;
	if (curBackgroundColor.a != 0)
	{
		SDL_SetRenderDrawColor(pRenderer, curBackgroundColor.r, curBackgroundColor.g, curBackgroundColor.b, curBackgroundColor.a);

		const Hitbox& box = block.mHitbox;
		SDL_FRect rect = { box.getTopLeft().getX()  * mScreen.mGameScreenToGameLevelChunkRatio,
							box.getTopLeft().getY() * mScreen.mGameScreenToGameLevelChunkRatio,
							box.getWidth()  * mScreen.mGameScreenToGameLevelChunkRatio,
							box.getHeight() * mScreen.mGameScreenToGameLevelChunkRatio };
		SDL_RenderFillRect(pRenderer, &rect);
	}
	pRenderer = nullptr;
}

void MenuManager::printTextBox(const TextBox& textBox)
{
	SDL_Renderer* pRenderer = mScreen.mpRenderer;
	SDL_Color curTextBoxColor = textBox.getTextBoxColor();
	SDL_Color curOutlineColor = textBox.getOutlineColor();

	const Hitbox& hitbox = *textBox.mpCurHitbox;
	// outline box
	if (textBox.mOutlineWidth != 0 && curOutlineColor.a != 0)
	{
		SDL_SetRenderDrawColor(pRenderer, curOutlineColor.r, curOutlineColor.g, curOutlineColor.b, curOutlineColor.a);
		SDL_FRect rect = { hitbox.getTopLeft().getX() * mScreen.mGameScreenToGameLevelChunkRatio ,
							 hitbox.getTopLeft().getY() * mScreen.mGameScreenToGameLevelChunkRatio,
							 hitbox.getWidth() * mScreen.mGameScreenToGameLevelChunkRatio,
							 hitbox.getHeight() * mScreen.mGameScreenToGameLevelChunkRatio };
		SDL_RenderFillRect(pRenderer, &rect);
	}

	// background box
	if (curTextBoxColor.a != 0)
	{
		SDL_SetRenderDrawColor(pRenderer, curTextBoxColor.r, curTextBoxColor.g, curTextBoxColor.b, curTextBoxColor.a);
		SDL_FRect rect = { (hitbox.getTopLeft().getX() + textBox.mOutlineWidth) * mScreen.mGameScreenToGameLevelChunkRatio ,
							(hitbox.getTopLeft().getY() + textBox.mOutlineWidth) * mScreen.mGameScreenToGameLevelChunkRatio,
							(hitbox.getWidth() - 2 * textBox.mOutlineWidth) * mScreen.mGameScreenToGameLevelChunkRatio,
							(hitbox.getHeight() - 2 * textBox.mOutlineWidth) * mScreen.mGameScreenToGameLevelChunkRatio };
		SDL_RenderFillRect(pRenderer, &rect);
	}

	// text
	for (int i = 0; i < (*(textBox.mpCurTextures)).size(); i++)
	{
		const Hitbox& curLineBox = (*(textBox.mpCurLineHitboxes))[i];
		SDL_FRect destinationText = { curLineBox.getTopLeft().getX() * mScreen.mGameScreenToGameLevelChunkRatio ,
										curLineBox.getTopLeft().getY() * mScreen.mGameScreenToGameLevelChunkRatio,
										curLineBox.getWidth() * mScreen.mGameScreenToGameLevelChunkRatio,
										curLineBox.getHeight() * mScreen.mGameScreenToGameLevelChunkRatio };
		SDL_RenderTexture(pRenderer, (*(textBox.mpCurTextures))[i], NULL, &destinationText);
	}
	pRenderer = nullptr;
}

void MenuManager::printImageBox(const ImageBox& imageBox)
{
	SDL_Renderer* pRenderer = mScreen.mpRenderer;
	const Hitbox& box = *imageBox.mpCurHitbox;
	SDL_FRect rect = { box.getTopLeft().getX() * mScreen.mGameScreenToGameLevelChunkRatio ,
						box.getTopLeft().getY() * mScreen.mGameScreenToGameLevelChunkRatio,
						box.getWidth() * mScreen.mGameScreenToGameLevelChunkRatio,
						box.getHeight() * mScreen.mGameScreenToGameLevelChunkRatio };
	int rotation = imageBox.mRotation;

	SDL_RenderTextureRotated(pRenderer, imageBox.mImageObject.getTexture(), NULL, &rect, rotation, NULL, SDL_FLIP_NONE);
}

void MenuManager::printShapeBox(const ShapeBox& shapeBox)
{
	SDL_Renderer* pRenderer = mScreen.mpRenderer;
	if (shapeBox.mColor.a != 0)
	{
		SDL_FRect rect;
		const Hitbox& hitbox = *shapeBox.mpCurHitbox;
		SDL_SetRenderDrawColor(pRenderer, shapeBox.mColor.r, shapeBox.mColor.g, shapeBox.mColor.b, shapeBox.mColor.a);
		switch (shapeBox.mShapeType)
		{
		case EShapeBoxClass_CIRCLE:
			drawCircle(shapeBox.mColor, hitbox.getCenter(), hitbox.getWidth() / 2, mScreen);
			break;
		case EShapeBoxClass_RECT:
			rect = { hitbox.getTopLeft().getX() * mScreen.mGameScreenToGameLevelChunkRatio ,
						hitbox.getTopLeft().getY() * mScreen.mGameScreenToGameLevelChunkRatio,
						hitbox.getWidth() * mScreen.mGameScreenToGameLevelChunkRatio,
						hitbox.getHeight() * mScreen.mGameScreenToGameLevelChunkRatio };
			SDL_RenderFillRect(pRenderer, &rect);
			break;
		default:
			SDL_assert(false);
		}
	}
}

void MenuManager::printHealthBox(const HealthBox& healthBox)
{
	printShapeBox(healthBox.mBoundingBox);
	printShapeBox(healthBox.mHealthLeftBox);
	printTextBox(healthBox.mHealthText);
}
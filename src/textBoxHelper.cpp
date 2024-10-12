#include "textBoxHelper.h"


TextBox::TextBox(TextBoxPreset dataStorage, ETextBoxFunction textBoxFunction, TextBoxPositionInfo positionInfo, const char* fileName, int standardSize, int selectedSize, TextBoxColorInfo colorInfo)
{
	mClassType		= EUIBoxClass_TEXTBOX;
	mDataStorage	= dataStorage;
	mType			= dataStorage.mType;
	mFunction		= textBoxFunction;

	mPositionAlign	= positionInfo.mPositionAlign;
	mMaxWidth		= positionInfo.mMaxWidth;
	mMaxHeight		= positionInfo.mMaxHeight;

	//hitbox
	int x = 0;
	int y = 0;
	switch (mPositionAlign)
	{
	case ETextBoxPositionAlign_LEFT:
		x = positionInfo.mPosition.getX();
		y = positionInfo.mPosition.getY();
		break;
	case ETextBoxPositionAlign_CENTER:
		x = positionInfo.mPosition.getX() - (positionInfo.mMaxWidth / 2);
		y = positionInfo.mPosition.getY() - (positionInfo.mMaxHeight / 2);
		break;
	case ETextBoxPositionAlign_RIGHT:
		//TODO check validity
		x = positionInfo.mPosition.getX() - positionInfo.mMaxWidth;
		y = positionInfo.mPosition.getY();
		break;
	default:
		SDL_assert(false);
		break;
	}

	mHitbox					= Hitbox(x, positionInfo.mMaxWidth + x, y, positionInfo.mMaxHeight + y);
	mMargins				= positionInfo.mMargins;

	mFileName				= fileName;
	mStandardFontSize		= standardSize;
	mSelectedFontSize		= selectedSize;
	mFullString				= dataStorage.mMessage;
	mTextAlign				= positionInfo.mTextAlign;

	mIsHighlighted			 = false;
	mStandardTextColor		 = colorInfo.mStandardTextColor;
	mHighlightedTextColor	 = colorInfo.mHighlightedTextColor;
	mStandardTextBoxColor    = colorInfo.mStandardTextBoxColor;
	mHighlightedTextBoxColor = colorInfo.mHighlightedTextBoxColor;
	updateTextLines(dataStorage.mMessage, 0);

}

TextBox::~TextBox()
{
	mFileName = nullptr;
	mpStandardFont = nullptr;
	mpHighlightedFont = nullptr;

	for (SDL_Texture* pTexture : mpStandardTextures)
	{
		SDL_DestroyTexture(pTexture);
	}
	mpStandardTextures.clear();

	for (SDL_Texture* pTexture : mpHighlightedTextures)
	{
		SDL_DestroyTexture(pTexture);
	}
	mpHighlightedTextures.clear();

	for (SDL_Texture* pTexture : mpCurTextures)
	{
		SDL_DestroyTexture(pTexture);
	}
	mpCurTextures.clear();
}

void TextBox::updateMessage(SDL_Renderer* pRenderer, std::string textMessage, int textIncrease)
{
	mSetUp				= true;
	mFullString			= textMessage;
	calcMaxFontSizeGivenText();
	updateTextLines(textMessage, textIncrease);
	setTextBoxTexture(pRenderer, textIncrease);
	updateHitbox();

}

void TextBox::calcMaxFontSizeGivenText()
{
	int curFontWidth;
	int curFontHeight;

	mMaxFontSizeGivenText = 2;
	int stringLength = (int)mFullString.length();

	for (int fontSize = 8; fontSize < 72; fontSize += 2)
	{
		curFontWidth = stringLength * fontSize;
		curFontHeight = (int)(fontSize * (1.0f / mStandardFontX2YRatio));


		int howManyLinesCouldWeHave = mMaxHeight / curFontHeight;
		int maxLineWidthWrapped = howManyLinesCouldWeHave * mMaxWidth;
		if (curFontWidth < maxLineWidthWrapped)
		{
			mMaxFontSizeGivenText = fontSize;
			continue;
		}
	}
}

int TextBox::getMaxFontSizeGivenText()
{
	return mMaxFontSizeGivenText;
}

void TextBox::setBoxGroupMaxFontSize(int groupMaxFontSize)
{
	mMaxFontSizeGivenBoxGroup = groupMaxFontSize;
}

void TextBox::updateTextLines(std::string text, int textIncrease)
{
	int length = mMaxWidth;
	if (mIsHighlighted)
	{
		length = mMaxWidth / (mSelectedFontSize + textIncrease);
	}
	else
	{
		length = mMaxWidth / (mStandardFontSize + textIncrease);
	}
	mTextLines = tokenizeByStringLength(text, length);
}

void TextBox::updateTexture(SDL_Renderer* pRenderer, int textIncrease)
{
	setTextBoxTexture(pRenderer, textIncrease);
	updateHitbox();
}

void TextBox::updateTexture(SDL_Renderer* pRenderer)
{
	setTextBoxTexture(pRenderer, 0);
	updateHitbox();
}

void TextBox::setTextBoxTexture(SDL_Renderer* pRenderer, int textIncrease)
{
	int newRequestedFontWidth = mSelectedFontSize + textIncrease;

	if ((mMaxFontSizeGivenText != 0 and newRequestedFontWidth > mMaxFontSizeGivenText) || (mMaxFontSizeGivenBoxGroup != 0 and newRequestedFontWidth > mMaxFontSizeGivenBoxGroup))
	{
		SDL_assert(0);
		return; //We can not do it so leave the current textures alone.
	}

	for (SDL_Texture* pTexture : mpStandardTextures)
	{
		SDL_DestroyTexture(pTexture);
	}
	mpStandardTextures.clear();

	for (SDL_Texture* pTexture : mpHighlightedTextures)
	{
		SDL_DestroyTexture(pTexture);
	}
	mpHighlightedTextures.clear();

	mpStandardFont	  = TTF_OpenFont(mFileName, (mStandardFontSize + textIncrease));
	mpHighlightedFont = TTF_OpenFont(mFileName, (mSelectedFontSize + textIncrease));

	for (int countLine = 0; countLine < mTextLines.size(); countLine++)
	{
		SDL_Surface* standardSurface = TTF_RenderUTF8_Blended(mpStandardFont, mTextLines[countLine].c_str(), mStandardTextColor);
		mpStandardTextures.push_back(SDL_CreateTextureFromSurface(pRenderer, standardSurface));
		SDL_DestroySurface(standardSurface);

		SDL_Surface* highlightedSurface = TTF_RenderUTF8_Blended(mpHighlightedFont, mTextLines[countLine].c_str(), mHighlightedTextColor);
		mpHighlightedTextures.push_back(SDL_CreateTextureFromSurface(pRenderer, highlightedSurface));
		SDL_DestroySurface(highlightedSurface);
	}

	if (mIsHighlighted)
	{
		mpCurTextures = mpHighlightedTextures;
	}
	else
	{
		mpCurTextures = mpStandardTextures;
	}
}

void TextBox::changeIsHighlighted(bool isHighlighted)
{
	if (isHighlighted)
	{
		mpCurTextures = mpHighlightedTextures;
		mIsHighlighted = true;
		return;
	}
	mpCurTextures = mpStandardTextures;
	mIsHighlighted = false;
}

std::vector<SDL_Texture*> TextBox::getTextBoxTexture()
{
	return mpCurTextures;
}

SDL_Point TextBox::getTextRenderSize(int line) const
{
	SDL_Point size;
	SDL_QueryTexture(mpCurTextures[line], NULL, NULL, &size.x, &size.y);
	return size;
}

SDL_Color TextBox::getTextBoxColor() const
{
	if (mIsHighlighted)
	{
		return mHighlightedTextBoxColor;
	}
	else
	{
		return mStandardTextBoxColor;
	}
}

void TextBox::updateHitbox()
{
	switch (mPositionAlign)
	{
	case ETextBoxPositionAlign_LEFT:
	{
		int x = mHitbox.getTopLeft().getX();
		int y = mHitbox.getTopLeft().getY();
		int maxWidth = 0;
		int totalHeight = 0;

		//int curX;
		int curY = mHitbox.getTopLeft().getY();
		mCurLineHitboxes.clear();
		for (int count = 0; count < mTextLines.size(); count++)
		{
			SDL_Point size = getTextRenderSize(count);
			maxWidth = std::max(maxWidth, size.x);
			totalHeight += size.y;
			Hitbox curHitbox = Hitbox(x, x + size.x, curY, curY + size.y);
			curY += size.y;
			mCurLineHitboxes.push_back(curHitbox);
		}
		if (x < 0)
		{
			x = 0;
		}
		if (y < 0)
		{
			y = 0;
		}
		mHitbox = Hitbox(x, x + maxWidth, y, y + totalHeight);
		break;
	}
	case ETextBoxPositionAlign_CENTER:
	{
		int maxWidth = 0;
		int totalHeight = 0;

		mCurLineHitboxes.clear();
		std::vector <SDL_Point> lineSizes;
		for (int count = 0; count < (int)mTextLines.size(); count++)
		{
			SDL_Point size = getTextRenderSize(count);
			maxWidth = std::max(maxWidth, size.x);
			lineSizes.push_back(size);
			totalHeight += size.y;
		}
		int x = mHitbox.getCenter().getX() - (maxWidth / 2);
		int y = mHitbox.getCenter().getY() - (totalHeight / 2);

		int curY = y;
		for (int count = 0; count < mTextLines.size(); count++)
		{
			SDL_Point size = lineSizes[count];
			int curX = (maxWidth - size.x) / 2 + x;
			Hitbox curHitbox = Hitbox(curX, curX + size.x, y, y + size.y);
			curY += size.y;
			mCurLineHitboxes.push_back(curHitbox);
		}
		mHitbox = Hitbox(x, x + maxWidth, y, y + totalHeight);
		break;
	}
	case ETextBoxPositionAlign_RIGHT:
	{
		//TODO check validity
		int y = mHitbox.getTopLeft().getY();
		int x2 = mHitbox.getBottomRight().getX();
		int maxWidth = 0;
		int totalHeight = y;

		//int curX;
		int curY = mHitbox.getTopLeft().getY();
		mCurLineHitboxes.clear();
		for (int count = 0; count < mTextLines.size(); count++)
		{
			SDL_Point size = getTextRenderSize(count);
			maxWidth = std::max(maxWidth, size.x);
			totalHeight += size.y;
			Hitbox curHitbox = Hitbox(x2 - size.x, x2, curY, curY + size.y);
			curY += size.y;
			mCurLineHitboxes.push_back(curHitbox);
		}
		if (y < 0)
		{
			y = 0;
		}
		mHitbox = Hitbox(x2 - maxWidth, x2, y, y + totalHeight);
		break;
	}
	default:
		SDL_assert(false);
		break;
	}

}

void TextBox::shiftHitbox(Vect2 shiftTopLeft)
{
	mHitbox.updateTopLeft(shiftTopLeft);
	for (int count = 0; count < mCurLineHitboxes.size(); count++)
	{
		mCurLineHitboxes[count].updateTopLeft(shiftTopLeft);
	}
}


ImageBox::ImageBox(ImageBoxPreset preset, ImageBoxPositionInfo positionInfo, std::string fileName)
{
	mClassType		= EUIBoxClass_IMAGEBOX;
	mImageObject	= ImageObject(fileName, positionInfo.mMaxWidth, positionInfo.mMaxHeight, EHowToDetermineWidthHeight_GET_BEST_IMAGE_RATIO);
	mPositionAlign	= positionInfo.mPositionAlign;

	//hitbox
	int x = 0;
	int y = 0;
	switch (mPositionAlign)
	{
	case ETextBoxPositionAlign_LEFT:
		x = positionInfo.mPosition.getX();
		y = positionInfo.mPosition.getY();
		break;
	case ETextBoxPositionAlign_CENTER:
		x = positionInfo.mPosition.getX() - (mImageObject.getIdealImageWidth() / 2);
		y = positionInfo.mPosition.getY() - (mImageObject.getIdealImageHeight() / 2);
		break;
	case ETextBoxPositionAlign_RIGHT:
		//check validity
		x = positionInfo.mPosition.getX() - mImageObject.getIdealImageWidth();
		y = positionInfo.mPosition.getY();
		break;
	default:
		SDL_assert(false);
		break;
	}

	mHitbox        = Hitbox(x, mImageObject.getIdealImageWidth() + x, y, mImageObject.getIdealImageHeight() + y);
	mMargins       = positionInfo.mMargins;

	mRotation      = positionInfo.mRotation;
	mShow          = preset.mAutoShow;
	mID            = preset.mID;
}

void ImageBox::shiftHitbox(Vect2 shiftTopLeft)
{
	mHitbox.updateTopLeft(shiftTopLeft);
}

void ImageBox::updateHitbox(float ratio)
{
	int width  = (int)(mHitbox.getWidth()  * ratio);
	int height = (int)(mHitbox.getHeight() * ratio);
	switch (mPositionAlign)
	{
	case ETextBoxPositionAlign_LEFT:
	{
		int x = mHitbox.getTopLeft().getX();
		int y = mHitbox.getTopLeft().getY();

		mHitbox = Hitbox(x, x + width, y, y + height);
		break;
	}
	case ETextBoxPositionAlign_CENTER:
	{
		int x = mHitbox.getCenter().getX() - (width / 2);
		int y = mHitbox.getCenter().getY() - (height / 2);

		mHitbox = Hitbox(x, x + width, y, y + height);
		break;
	}
	case ETextBoxPositionAlign_RIGHT:
	{
		//TODO check validity
		int y = mHitbox.getTopLeft().getY();
		int x2 = mHitbox.getBottomRight().getX();

		if (y < 0)
		{
			y = 0;
		}
		mHitbox = Hitbox(x2 - width, x2, y, y + height);
		break;
	}
	default:
		break;
	}
}


UIBlock::~UIBlock()
{
	for (UIBlock* subBlock : mpSubBlocks)
	{
		subBlock = nullptr;
	}
	for (UIBox* curBox : mpCurBoxes)
	{
		curBox = nullptr;
	}
}

void UIBlock::updateBlocks(int marginIncrease)
{
	//figure out all block widths and heights
	adjustBlocksWidthHeight();

	int x = 0;
	int y = 0;

	switch (mGrowthDirectionHorizontal)
	{
	case EDirection_RIGHT:
		x = mStartingPositionCenter.getX() + (mMargins.mLeft + marginIncrease);
		break;
	case EDirection_LEFT:
		x = mStartingPositionCenter.getX() - mHitbox.getWidth() - (mMargins.mRight + marginIncrease);
		break;
	default:
		SDL_assert(false);
		break;
	}

	switch (mGrowthDirectionVertical)
	{
	case EDirection_DOWN:
		y = mStartingPositionCenter.getY() + (mMargins.mTop + marginIncrease);
		break;
	case EDirection_UP:
		y = mStartingPositionCenter.getY() - mHitbox.getHeight() - (mMargins.mBottom + marginIncrease);
		break;
	default:
		SDL_assert(false);
		break;
	}

	mHitbox.setTopLeft(Vect2(x, y));

	//figure out block locations
	moveSubBlocks(marginIncrease);
}

void UIBlock::setMaxForBoxes()
{
	for (int count = 0; count < mpSubBlocks.size(); count++)
	{
		mpSubBlocks[count]->setMaxForBoxes();
	}

	for (int count = 0; count < mpCurBoxes.size(); count++)
	{
		UIBox* pCurBox = mpCurBoxes[count];
		if (pCurBox->mHitbox.getWidth() > mHitbox.getWidth())
		{
			pCurBox->mHitbox.setWidth(mHitbox.getWidth());
		}

		if (pCurBox->mHitbox.getHeight() > mHitbox.getHeight())
		{
			pCurBox->mHitbox.setHeight(mHitbox.getHeight());
		}

	}

}

int UIBlock::calcMaxFontSizeForBlock()
{
	int maxFontSize = 72; // Max font size is 72

	for (UIBox* pCurBox : mpCurBoxes)
	{
		TextBox* pTextBox = dynamic_cast<TextBox*>(pCurBox);
		if (pTextBox)
		{
			int textMax = pTextBox->getMaxFontSizeGivenText();
			if(textMax != 0)
				maxFontSize = std::min(maxFontSize, textMax);
		}
	}

	for (UIBlock* pCurBlock : mpSubBlocks)
	{
		int textMax = pCurBlock->calcMaxFontSizeForBlock();
		if (textMax != 0)
			maxFontSize = std::min(maxFontSize, textMax);
	}

	return maxFontSize;
}

void UIBlock::setMaxFontSizeForBlock(int maxTextSize)
{

	for (UIBox* pCurBox : mpCurBoxes)
	{
		TextBox* pTextBox = dynamic_cast<TextBox*>(pCurBox);
		if (pTextBox)
		{
			pTextBox->setBoxGroupMaxFontSize(maxTextSize);

		}
	}

	for (UIBlock* pCurBlock : mpSubBlocks)
	{
		pCurBlock->setMaxFontSizeForBlock(maxTextSize);
	}
}



//MASTER / HEAD BLOCK
BlockAlignElementsVertically::BlockAlignElementsVertically(Hitbox hitbox, EDirection directionH, EDirection directionV, Edges margins)
{
	constructHeadBlock(hitbox, directionH, directionV, margins);
}

BlockAlignElementsVertically::BlockAlignElementsVertically(Hitbox hitbox, EDirection directionH, EDirection directionV, Edges margins, 
		std::string name)
{
	constructHeadBlock(hitbox, directionH, directionV, margins);
	mName = name;
}

void BlockAlignElementsVertically::constructHeadBlock(Hitbox hitbox, EDirection directionH, EDirection directionV, Edges margins)
{
	mIsHeadBlock = true;
	mGrowthDirectionHorizontal = directionH;
	mGrowthDirectionVertical   = directionV;
	constructBlock(hitbox, ETextBoxPositionAlign_LEFT, directionV, margins);
}

//SUB BLOCKS
BlockAlignElementsVertically::BlockAlignElementsVertically(int maxWidth, int maxHeight, ETextBoxPositionAlign positionAlign, 
		EDirection direction, Edges margins)
{
	constructBlock(Hitbox(0, maxWidth, 0, maxHeight), positionAlign, direction, margins);

}

BlockAlignElementsVertically::BlockAlignElementsVertically(int maxWidth, int maxHeight, ETextBoxPositionAlign positionAlign, 
		EDirection direction, Edges margins, std::string name)
{
	constructBlock(Hitbox(0, maxWidth, 0, maxHeight), positionAlign, direction, margins);
	mName = name;

}

void BlockAlignElementsVertically::constructBlock(Hitbox hitbox, ETextBoxPositionAlign positionAlign, EDirection direction, Edges margins)
{
	mHitbox = hitbox;
	mPositionAlign = positionAlign;
	switch (mPositionAlign)
	{
	case ETextBoxPositionAlign_CENTER:
		mStartingPositionCenter = Vect2(hitbox.getCenter().getX(), hitbox.getTopLeft().getY());
		break;
	case ETextBoxPositionAlign_LEFT:
		mStartingPositionCenter = Vect2(hitbox.getTopLeft().getX(), hitbox.getTopLeft().getY());
		break;
	default:
		SDL_assert(false);
		break;
	}

	if ((direction != EDirection_UP) and (direction != EDirection_DOWN))
	{
		SDL_assert(false);
	}
	mGrowthDirection = direction;
	mMargins = margins;
}

void BlockAlignElementsVertically::adjustBlocksWidthHeight()
{
	if (mpSubBlocks.size() > 0)
	{
		int maxWidth = 0;
		int height = 0;
		for (UIBlock* pCurBlock : mpSubBlocks)
		{
			pCurBlock->adjustBlocksWidthHeight();
			maxWidth = std::max(pCurBlock->mHitbox.getWidth(), maxWidth);
			height += pCurBlock->mHitbox.getHeight();
		}
		mHitbox.setWidth(maxWidth);
		mHitbox.setHeight(height);
	}

	if (mpCurBoxes.size() > 0)
	{
		int maxWidth = 0;
		int height   = 0;

		for (UIBox* pCurTextBox : mpCurBoxes)
		{
			maxWidth = std::max(pCurTextBox->mHitbox.getWidth(), maxWidth);
			height  += pCurTextBox->mHitbox.getHeight();
		}

		int updatedX = 0;
		int updatedY = 0;
		switch (mPositionAlign)
		{
		case ETextBoxPositionAlign_CENTER:
			updatedX = mHitbox.getCenter().getX() - (maxWidth / 2);
			updatedY = mHitbox.getCenter().getY() - (height / 2);
			break;
		case ETextBoxPositionAlign_LEFT:
			updatedX = mHitbox.getTopLeft().getX();
			updatedY = mHitbox.getTopLeft().getY();
			break;
		default:
			SDL_assert(false);
			break;
		}
		
		mHitbox.setHeight(height);
		mHitbox.setWidth(maxWidth);
		mHitbox.setTopLeft(Vect2(updatedX, updatedY));
	}
}

void BlockAlignElementsVertically::moveBoxes(int marginIncrease)
{
	if (mGrowthDirection == EDirection_DOWN)
	{
		//top to bottom
		UIBox* pLastTextBox = mpCurBoxes[0];
		int x = mHitbox.getTopLeft().getX() + (pLastTextBox->mMargins.mLeft + marginIncrease);
		int y = mHitbox.getTopLeft().getY() + (pLastTextBox->mMargins.mTop  + marginIncrease);
		int difWidth = 0;
		if (mPositionAlign == ETextBoxPositionAlign_CENTER)
		{
			difWidth = mHitbox.getWidth() - pLastTextBox->mHitbox.getWidth();
		}
		int changeX = x - pLastTextBox->mHitbox.getTopLeft().getX() + (difWidth / 2); 
		int changeY = y - pLastTextBox->mHitbox.getTopLeft().getY();
		pLastTextBox->shiftHitbox(Vect2(changeX, changeY));
		for (int count = 1; count < mpCurBoxes.size(); count++)
		{
			UIBox* pCurTextBox = mpCurBoxes[count];
			x = mHitbox.getTopLeft().getX() + ((pCurTextBox->mMargins.mLeft + pLastTextBox->mMargins.mRight) + 2 * marginIncrease);
			y = pLastTextBox->mHitbox.getBottomRight().getY() + ((pLastTextBox->mMargins.mBottom + pCurTextBox->mMargins.mTop) + 2 * marginIncrease);
			int difWidth = 0;
			if (mPositionAlign == ETextBoxPositionAlign_CENTER)
			{
				difWidth = mHitbox.getWidth() - pCurTextBox->mHitbox.getWidth();
			}
			changeX = x - pCurTextBox->mHitbox.getTopLeft().getX() + (difWidth / 2);
			changeY = y - pCurTextBox->mHitbox.getTopLeft().getY();
			pCurTextBox->shiftHitbox(Vect2(changeX, changeY));
			pLastTextBox = pCurTextBox;
		}
	}
	else 
	{
		//Bottom to top
		UIBox* pLastTextBox = mpCurBoxes[mpCurBoxes.size() - 1];
		int x = mHitbox.getTopLeft().getX()     + (pLastTextBox->mMargins.mLeft + marginIncrease);
		int y = mHitbox.getBottomRight().getY() - pLastTextBox->mHitbox.getHeight() - (pLastTextBox->mMargins.mBottom + marginIncrease);
		int difWidth = 0;
		if (mPositionAlign == ETextBoxPositionAlign_CENTER)
		{
			difWidth = mHitbox.getWidth() - pLastTextBox->mHitbox.getWidth();
		}
		int changeX = x - pLastTextBox->mHitbox.getTopLeft().getX() + (difWidth / 2);
		int changeY = y - pLastTextBox->mHitbox.getTopLeft().getY();
		pLastTextBox->shiftHitbox(Vect2(changeX, changeY));
		for (int count = (int)mpCurBoxes.size() - 2; count > -1; count--)
		{
			UIBox* pCurTextBox = mpCurBoxes[count];
			x = mHitbox.getTopLeft().getX() + (pCurTextBox->mMargins.mLeft + marginIncrease);
			y = pLastTextBox->mHitbox.getTopLeft().getY() - pCurTextBox->mHitbox.getHeight() - ((pCurTextBox->mMargins.mBottom + pLastTextBox->mMargins.mTop) + 2 * marginIncrease);
			int difWidth = 0;
			if (mPositionAlign == ETextBoxPositionAlign_CENTER)
			{
				difWidth = mHitbox.getWidth() - pCurTextBox->mHitbox.getWidth();
			}
			changeX = x - pCurTextBox->mHitbox.getTopLeft().getX() + (difWidth / 2);
			changeY = y - pCurTextBox->mHitbox.getTopLeft().getY();
			pCurTextBox->shiftHitbox(Vect2(changeX, changeY));
			pLastTextBox = pCurTextBox;

		}
	}
}

void BlockAlignElementsVertically::moveSubBlocks(int marginIncrease)
{
	if (mpSubBlocks.size() > 0)
	{
		if (mGrowthDirection == EDirection_DOWN)
		{
			//top to bottom
			UIBlock* pLastBlock = mpSubBlocks[0];
			int x = (pLastBlock->mMargins.mLeft + marginIncrease) + mHitbox.getTopLeft().getX();
			int y = (pLastBlock->mMargins.mTop  + marginIncrease) + mHitbox.getTopLeft().getY();
			pLastBlock->mHitbox.setTopLeft(Vect2(x, y));
			pLastBlock->moveSubBlocks(marginIncrease);
			for (int count = 1; count < mpSubBlocks.size(); count++)
			{
				UIBlock* pCurBlock = mpSubBlocks[count];
				x = (pCurBlock->mMargins.mLeft + marginIncrease)+ mHitbox.getTopLeft().getX();
				y = pLastBlock->mHitbox.getBottomRight().getY() + ((pCurBlock->mMargins.mTop + pLastBlock->mMargins.mBottom) + 2 * marginIncrease);
				pCurBlock->mHitbox.setTopLeft(Vect2(x, y));
				pCurBlock->moveSubBlocks(marginIncrease);
				pLastBlock = pCurBlock;
			}
		}
		else
		{
			//bottom to top
			UIBlock* pLastBlock = mpSubBlocks[mpSubBlocks.size() - 1];
			int x = (pLastBlock->mMargins.mLeft + marginIncrease) + mHitbox.getTopLeft().getX();
			int y = -(pLastBlock->mMargins.mBottom + marginIncrease) + mHitbox.getBottomRight().getY() - pLastBlock->mHitbox.getHeight();
			pLastBlock->mHitbox.setTopLeft(Vect2(x, y));
			pLastBlock->moveSubBlocks(marginIncrease);
			for (int count = (int)mpSubBlocks.size() - 2; count > -1; count--)
			{
				UIBlock* pCurBlock = mpSubBlocks[count];
				x = (pCurBlock->mMargins.mLeft + marginIncrease) + mHitbox.getTopLeft().getX();
				y = pLastBlock->mHitbox.getTopLeft().getY() - ((pCurBlock->mMargins.mBottom + pLastBlock->mMargins.mTop) + 2 * marginIncrease) - pCurBlock->mHitbox.getHeight();
				pCurBlock->mHitbox.setTopLeft(Vect2(x, y));
				pCurBlock->moveSubBlocks(marginIncrease);
				pLastBlock = pCurBlock;
			}
		}
	}
	if (mpCurBoxes.size() > 0)
	{
		moveBoxes(marginIncrease);
	}
}



//MASTER / HEAD BLOCK
BlockAlignElementsHorizontally::BlockAlignElementsHorizontally(Hitbox hitbox, EDirection directionH, EDirection directionV, Edges margins)
{
	constructHeadBlock(hitbox, directionH, directionV, margins);
}

BlockAlignElementsHorizontally::BlockAlignElementsHorizontally(Hitbox hitbox, EDirection directionH, EDirection directionV, Edges margins, 
		std::string name)
{
	constructHeadBlock(hitbox, directionH, directionV, margins);
	mName = name;
}

void BlockAlignElementsHorizontally::constructHeadBlock(Hitbox hitbox, EDirection directionH, EDirection directionV, Edges margins)
{
	mIsHeadBlock = true;
	mGrowthDirectionHorizontal = directionH;
	mGrowthDirectionVertical   = directionV;
	constructBlock(hitbox, ETextBoxPositionAlign_LEFT, directionH, margins);
}

//SUB BLOCKS
BlockAlignElementsHorizontally::BlockAlignElementsHorizontally(int maxWidth, int maxHeight, ETextBoxPositionAlign positionAlign, 
		EDirection direction, Edges margins)
{
	constructBlock(Hitbox(0, maxWidth, 0, maxHeight), positionAlign, direction, margins);
}

BlockAlignElementsHorizontally::BlockAlignElementsHorizontally(int maxWidth, int maxHeight, ETextBoxPositionAlign positionAlign, 
	EDirection direction, Edges margins, std::string name)
{
	constructBlock(Hitbox(0, maxWidth, 0, maxHeight), positionAlign, direction, margins);
	mName = name;
}

void BlockAlignElementsHorizontally::constructBlock(Hitbox hitbox, ETextBoxPositionAlign positionAlign, EDirection direction, Edges margins)
{
	mHitbox = hitbox;
	mPositionAlign = positionAlign;
	switch (positionAlign)
	{
	case ETextBoxPositionAlign_CENTER:
		mStartingPositionCenter = Vect2(hitbox.getTopLeft().getX(), hitbox.getCenter().getY());
		break;
	case ETextBoxPositionAlign_LEFT:
		mStartingPositionCenter = Vect2(hitbox.getTopLeft().getX(), hitbox.getTopLeft().getY());
		break;
	default:
		SDL_assert(false);
		break;
	}
	
	if ((direction != EDirection_LEFT) and (direction != EDirection_RIGHT))
	{
		SDL_assert(false);
	}
	mGrowthDirection = direction;
	mMargins = margins;
}

void BlockAlignElementsHorizontally::adjustBlocksWidthHeight()
{
	if (mpSubBlocks.size() > 0)
	{
		int maxHeight = 0;
		int width = 0;
		for (UIBlock* pCurBlock : mpSubBlocks)
		{
			pCurBlock->adjustBlocksWidthHeight();
			maxHeight = std::max(pCurBlock->mHitbox.getHeight(), maxHeight);
			width += pCurBlock->mHitbox.getWidth();
		}
		mHitbox.setWidth(width);
		mHitbox.setHeight(maxHeight);
	}

	if (mpCurBoxes.size() > 0)
	{
		int maxHeight = 0;
		int width     = 0;

		for (UIBox* pCurTextBox : mpCurBoxes)
		{
			maxHeight = std::max(pCurTextBox->mHitbox.getHeight(), maxHeight);
			width += pCurTextBox->mHitbox.getWidth();
		}

		int updatedX = 0;
		int updatedY = 0;
		switch (mPositionAlign)
		{
		case ETextBoxPositionAlign_CENTER:
			updatedX = mHitbox.getCenter().getX() - (width / 2);
			updatedY = mHitbox.getCenter().getY() - (maxHeight / 2);
			break;
		case ETextBoxPositionAlign_LEFT:
			updatedX = mHitbox.getTopLeft().getX();
			updatedY = mHitbox.getTopLeft().getY();
			break;
		default:
			SDL_assert(false);
			break;
		}
		
		mHitbox.setWidth(width);
		mHitbox.setHeight(maxHeight);
		mHitbox.setTopLeft(Vect2(updatedX, updatedY));
	}
}

void BlockAlignElementsHorizontally::moveBoxes(int marginIncrease)
{
	if (mGrowthDirection == EDirection_RIGHT)
	{
		//left to right
		UIBox* pLastTextBox = mpCurBoxes[0];
		int x = mHitbox.getTopLeft().getX() + (pLastTextBox->mMargins.mLeft + marginIncrease);
		int y = mHitbox.getTopLeft().getY() + (pLastTextBox->mMargins.mTop  + marginIncrease);
		int difHeight = 0;
		if (mPositionAlign == ETextBoxPositionAlign_CENTER)
		{
			difHeight  = mHitbox.getHeight() - pLastTextBox->mHitbox.getHeight();
		}
		int changeX = x - pLastTextBox->mHitbox.getTopLeft().getX();
		int changeY = y - pLastTextBox->mHitbox.getTopLeft().getY() + (difHeight / 2);
		pLastTextBox->shiftHitbox(Vect2(changeX, changeY));
		for (int count = 1; count < mpCurBoxes.size(); count++)
		{
			UIBox* pCurTextBox = mpCurBoxes[count];
			y = mHitbox.getTopLeft().getY() + ((pCurTextBox->mMargins.mTop + pLastTextBox->mMargins.mBottom) + 2 * marginIncrease);
			x = pLastTextBox->mHitbox.getBottomRight().getX() + ((pLastTextBox->mMargins.mRight + pCurTextBox->mMargins.mLeft) + 2 * marginIncrease);
			int difHeight = 0;
			if (mPositionAlign == ETextBoxPositionAlign_CENTER)
			{
				difHeight  = mHitbox.getHeight() - pCurTextBox->mHitbox.getHeight();
			}
			int changeX = x - pCurTextBox->mHitbox.getTopLeft().getX();
			int changeY = y - pCurTextBox->mHitbox.getTopLeft().getY() + (difHeight / 2);
			pCurTextBox->shiftHitbox(Vect2(changeX, changeY));
			pLastTextBox = pCurTextBox;
		}
	}
	else 
	{
		//Right to left
		UIBox* pLastTextBox = mpCurBoxes[mpCurBoxes.size() - 1];
		int x = mHitbox.getBottomRight().getX() - pLastTextBox->mHitbox.getWidth() - ((pLastTextBox->mMargins.mRight) + marginIncrease);
		int y = mHitbox.getTopLeft().getY()     + (pLastTextBox->mMargins.mTop + marginIncrease);
		int difHeight = 0;
		if (mPositionAlign == ETextBoxPositionAlign_CENTER)
		{
			difHeight = mHitbox.getHeight() - pLastTextBox->mHitbox.getHeight();
		}
		int changeX = x - pLastTextBox->mHitbox.getTopLeft().getX();
		int changeY = y - pLastTextBox->mHitbox.getTopLeft().getY() + (difHeight / 2);
		pLastTextBox->shiftHitbox(Vect2(changeX, changeY));
		for (int count = (int)mpCurBoxes.size() - 2; count > -1; count--)
		{
			UIBox* pCurTextBox = mpCurBoxes[count];
			y = mHitbox.getTopLeft().getY() + ((pCurTextBox->mMargins.mTop + pLastTextBox->mMargins.mBottom) + 2 * marginIncrease);
			x = pLastTextBox->mHitbox.getTopLeft().getX() - pCurTextBox->mHitbox.getWidth() - ((pCurTextBox->mMargins.mRight + pLastTextBox->mMargins.mLeft) + 2 * marginIncrease);
			int difHeight = 0;
			if (mPositionAlign == ETextBoxPositionAlign_CENTER)
			{
				difHeight = mHitbox.getHeight() - pCurTextBox->mHitbox.getHeight();
			}
			changeX = x - pCurTextBox->mHitbox.getTopLeft().getX();
			changeY = y - pCurTextBox->mHitbox.getTopLeft().getY() + (difHeight / 2);
			pCurTextBox->shiftHitbox(Vect2(changeX, changeY));
			pLastTextBox = pCurTextBox;

		}
	}
}

void BlockAlignElementsHorizontally::moveSubBlocks(int marginIncrease)
{
	if (mpSubBlocks.size() > 0)
	{
		if (mGrowthDirection == EDirection_RIGHT)
		{
			//left to right
			UIBlock* pLastBlock = mpSubBlocks[0];
			int x = (pLastBlock->mMargins.mLeft + marginIncrease) + mHitbox.getTopLeft().getX();
			int y = (pLastBlock->mMargins.mTop  + marginIncrease) + mHitbox.getTopLeft().getY();
			pLastBlock->mHitbox.setTopLeft(Vect2(x, y));
			pLastBlock->moveSubBlocks(marginIncrease);
			for (int count = 1; count < mpSubBlocks.size(); count++)
			{
				UIBlock* pCurBlock = mpSubBlocks[count];
				y = (pCurBlock->mMargins.mTop + marginIncrease) + mHitbox.getTopLeft().getY();
				x = pLastBlock->mHitbox.getBottomRight().getX() + ((pCurBlock->mMargins.mLeft + pLastBlock->mMargins.mRight) + 2 * marginIncrease);
				pCurBlock->mHitbox.setTopLeft(Vect2(x, y));
				pCurBlock->moveSubBlocks(marginIncrease);
				pLastBlock = pCurBlock;
			}
		}
		else
		{
			//right to left
			UIBlock* pLastBlock = mpSubBlocks[mpSubBlocks.size() - 1];
			int x = (pLastBlock->mMargins.mLeft + marginIncrease) + mHitbox.getBottomRight().getX() - pLastBlock->mHitbox.getWidth() - (pLastBlock->mMargins.mRight + marginIncrease);
			int y = (pLastBlock->mMargins.mTop  + marginIncrease) + mHitbox.getTopLeft().getY();
			pLastBlock->mHitbox.setTopLeft(Vect2(x, y));
			pLastBlock->moveSubBlocks(marginIncrease);
			for (int count = (int)mpSubBlocks.size() - 2; count > -1; count--)
			{
				UIBlock* pCurBlock = mpSubBlocks[count];
				y = (pCurBlock->mMargins.mTop + marginIncrease) + mHitbox.getTopLeft().getY();
				x = pLastBlock->mHitbox.getTopLeft().getX() - ((pCurBlock->mMargins.mRight + pLastBlock->mMargins.mLeft) + 2 * marginIncrease) - pCurBlock->mHitbox.getWidth();
				pCurBlock->mHitbox.setTopLeft(Vect2(x, y));
				pCurBlock->moveSubBlocks(marginIncrease);
				pLastBlock = pCurBlock;
			}
		}
	}
	if (mpCurBoxes.size() > 0)
	{
		//BOXES
		moveBoxes(marginIncrease);
	}
}
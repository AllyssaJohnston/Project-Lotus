#include "uiBox.h"

UIBox::UIBox(const UIBoxData data) : mData(data) { ; }

UIBox::~UIBox()
{
	mpCurHitbox = nullptr;
}



TextBox::TextBox(const TextBoxPreset preset, ETextBoxFunction textBoxFunction, TextBoxPositionInfo positionInfo, const char* fileName, TextBoxSizeInfo sizeInfo,
		TextBoxColorInfo colorInfo) : UIBox(preset.mData), mFunction(textBoxFunction), mMessage(preset.mMessage),
		mMaxWidth(positionInfo.mMaxWidth), mMaxHeight(positionInfo.mMaxHeight), mFontFile(fileName), mStandardFontSize(sizeInfo.mStandardFontSize), 
		mHighlightedFontSize(sizeInfo.mHighlightedFontSize), mOutlineWidth(sizeInfo.mOutlineWidth), 
		mStandardTextBoxColor(colorInfo.mStandardTextBoxColor), mHighlightedTextBoxColor(colorInfo.mHighlightedTextBoxColor), 
		mOutlineColor(colorInfo.mOutlineColor), mHighlightedOutlineColor(colorInfo.mHighlightedOutlineColor)
{
	mClassType		= EUIBoxClass_TEXTBOX;
	mPositionAlign	= positionInfo.mPositionAlign;
	
	// create hitbox
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
		x = positionInfo.mPosition.getX() - positionInfo.mMaxWidth;
		y = positionInfo.mPosition.getY();
		break;
	default:
		SDL_assert(false);
		break;
	}

	mStandardHitbox			= Hitbox(x, positionInfo.mMaxWidth + x, y, positionInfo.mMaxHeight + y);
	mHighlightedHitbox		= Hitbox(x, positionInfo.mMaxWidth + x, y, positionInfo.mMaxHeight + y);
	mpCurHitbox				= &mStandardHitbox;
	mMargins				= positionInfo.mMargins;

	mTextAlign				= positionInfo.mTextAlign;

	mIsHighlighted			 = false;
	mStandardTextColor		 = { colorInfo.mStandardTextColor.r, colorInfo.mStandardTextColor.g, colorInfo.mStandardTextColor.b };
	mHighlightedTextColor	 = { colorInfo.mHighlightedTextColor.r, colorInfo.mHighlightedTextColor.g, colorInfo.mHighlightedTextColor.b };
	
	mTextLines.push_back(mMessage);
}

TextBox::~TextBox()
{
	mFontFile = nullptr;
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

	mpCurTextures = nullptr;
	mpCurHitbox = nullptr;
}

void TextBox::updateMessage(SDL_Renderer* pRenderer, FontSizeChart& fontSizeChart, const std::string textMessage)
{
	mMessage = textMessage;
	calcMaxFontSizeGivenText(fontSizeChart);
	if (DEMO == 0 && mHighlightedFontSize > mMaxFontSizeGivenText)
	{
		std::cout << "Font size too big: " + textMessage + "\n max size: " + std::to_string(mMaxFontSizeGivenText) + " requested size: " + std::to_string(mHighlightedFontSize)<<"\n";
	}
	if (mHighlightedFontSize > mMaxFontSizeGivenText)
	{
		mHighlightedFontSize = mMaxFontSizeGivenText;
		mStandardFontSize = mMaxFontSizeGivenText;
	}

	updateTextLines(textMessage, fontSizeChart);
	setTextBoxTexture(pRenderer);
	updateHitboxes();
	mSetUp = true;
}

void TextBox::calcMaxFontSizeGivenText(FontSizeChart& fontSizeChart)
{
	int totalChars = (int)mMessage.length();
	
	mMaxFontSizeGivenText = fontSizeChart.mMinFontSize;
	std::map<int, SDL_Point>& fontChart = fontSizeChart.mFontChart[mFontFile];
	for (std::map<int, SDL_Point>::iterator iter =  fontChart.begin(); iter != fontChart.end(); ++iter)
	{
		int curFontWidth = totalChars * iter->second.x;
		int curFontHeight = iter->second.y;

		int numCharsPerLine = mMaxWidth / iter->second.x;
		int numLinesNeeded = (int)ceil(totalChars / numCharsPerLine);
		if ((numLinesNeeded * curFontHeight) <= mMaxHeight)
		{
			// good font size
			mMaxFontSizeGivenText = iter->first;
		}
		else 
		{
			// too big, leave as is
			break;
		}
	}
}

int TextBox::getMaxFontSizeGivenText() const { return mMaxFontSizeGivenText; }

void TextBox::updateTextLines(const std::string text, FontSizeChart& fontSizeChart)
{
	int curFontSize = mIsHighlighted ? mHighlightedFontSize : mStandardFontSize;
	int numCharsPerLine = mMaxWidth / fontSizeChart.mFontChart[mFontFile][curFontSize].x;
	SDL_assert(numCharsPerLine > 0);
	mTextLines = tokenizeByStringLength(text, numCharsPerLine);
}

void TextBox::updateTexture(SDL_Renderer* pRenderer)
{
	setTextBoxTexture(pRenderer);
	updateHitboxes();
}

void TextBox::setTextBoxTexture(SDL_Renderer* pRenderer)
{
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

	mpStandardFont	  = TTF_OpenFont(mFontFile, mStandardFontSize);
	mpHighlightedFont = TTF_OpenFont(mFontFile, mHighlightedFontSize);

	for (int countLine = 0; countLine < mTextLines.size(); countLine++)
	{
		std::string curLine = mTextLines[countLine] + " "; // spacing to fix aligning issues
		SDL_Surface* standardSurface = TTF_RenderUTF8_Blended(mpStandardFont, curLine.c_str(), mStandardTextColor);
		mpStandardTextures.push_back(SDL_CreateTextureFromSurface(pRenderer, standardSurface));
		SDL_DestroySurface(standardSurface);

		SDL_Surface* highlightedSurface = TTF_RenderUTF8_Blended(mpHighlightedFont, curLine.c_str(), mHighlightedTextColor);
		mpHighlightedTextures.push_back(SDL_CreateTextureFromSurface(pRenderer, highlightedSurface));
		SDL_DestroySurface(highlightedSurface);
	}

	SDL_assert(mTextLines.size() == mpStandardTextures.size());

	mpCurTextures = mIsHighlighted? &mpHighlightedTextures : &mpStandardTextures;
}

void TextBox::changeIsHighlighted(bool isHighlighted)
{
	if (isHighlighted)
	{
		mpCurTextures = &mpHighlightedTextures;
		mpCurLineHitboxes = &mCurLineHighlightedHitboxes;
		mpCurHitbox = &mHighlightedHitbox;
	}
	else
	{
		mpCurTextures = &mpStandardTextures;
		mpCurLineHitboxes = &mCurLineStandardHitboxes;
		mpCurHitbox = &mStandardHitbox;
	}
	mIsHighlighted = isHighlighted;
}

bool TextBox::getIsHighlighted() const { return mIsHighlighted; }

std::vector<SDL_Texture*> TextBox::getTextBoxTexture() const { return *mpCurTextures; }

SDL_Point TextBox::getTextRenderSize(int line) const { return getTextRenderSize(line, mIsHighlighted); }

SDL_Point TextBox::getTextRenderSize(int line, bool isHighlighted) const
{
	SDL_Point size = SDL_Point();
	if (isHighlighted) 
	{
		SDL_QueryTexture(mpHighlightedTextures[line], NULL, NULL, &size.x, &size.y);
	}
	else 
	{
		SDL_QueryTexture(mpStandardTextures[line], NULL, NULL, &size.x, &size.y);
	}
	
	return size;
}

SDL_Color TextBox::getTextBoxColor() const { return mIsHighlighted ? mHighlightedTextBoxColor : mStandardTextBoxColor; }

void TextBox::updateHitboxes() 
{
	updateHitboxesInternal(false, mStandardHitbox, mCurLineStandardHitboxes);
	updateHitboxesInternal(true, mHighlightedHitbox, mCurLineHighlightedHitboxes);
	mpCurLineHitboxes =  mIsHighlighted ? &mCurLineHighlightedHitboxes : &mCurLineStandardHitboxes;
	mpCurHitbox = mIsHighlighted ? &mHighlightedHitbox : &mStandardHitbox;
}

void TextBox::updateHitboxesInternal(bool isHighlighted, Hitbox& hitbox, std::vector<Hitbox>& hitboxes )
{
	switch (mPositionAlign)
	{
	case ETextBoxPositionAlign_LEFT:
	{
		int x = hitbox.getTopLeft().getX() - mOutlineWidth;
		int y = hitbox.getTopLeft().getY() - mOutlineWidth;
		int maxWidth = 0;
		int totalHeight = 0;

		int curY = hitbox.getTopLeft().getY();
		hitboxes.clear();
		for (int count = 0; count < (int)mTextLines.size(); count++)
		{
			SDL_Point size = getTextRenderSize(count, isHighlighted);
			size.x += mOutlineWidth * 2;
			size.y += mOutlineWidth * 2;
			maxWidth = std::max(maxWidth, size.x);
			totalHeight += size.y;
			Hitbox curHitbox = Hitbox(x, x + size.x, curY, curY + size.y);
			curY += size.y;
			hitboxes.push_back(curHitbox);
		}
		if (x < 0)
		{
			x = 0;
		}
		if (y < 0)
		{
			y = 0;
		}
		hitbox = Hitbox(x, x + maxWidth, y, y + totalHeight);
		break;
	}
	case ETextBoxPositionAlign_CENTER:
	{
		int maxWidth = 0;
		int totalHeight = 0;

		hitboxes.clear();
		std::vector <SDL_Point> lineSizes;
		for (int count = 0; count < (int)mTextLines.size(); count++)
		{
			SDL_Point size = getTextRenderSize(count, isHighlighted);
			maxWidth = std::max(maxWidth, size.x);
			lineSizes.push_back(size);
			totalHeight += size.y;
		}
		int x = hitbox.getCenter().getX() - (maxWidth / 2); // left
		int y = hitbox.getCenter().getY() - (totalHeight / 2); // top

		int curY = y;
		for (int count = 0; count < (int)mTextLines.size(); count++)
		{
			SDL_Point size = lineSizes[count];
			size.x += mOutlineWidth * 2;
			size.y += mOutlineWidth * 2;
			int curX = (maxWidth - size.x) / 2 + x;
			Hitbox curHitbox = Hitbox(curX, curX + size.x, curY, curY + size.y);
			curY += size.y;
			hitboxes.push_back(curHitbox);
		}
		hitbox = Hitbox(x, x + maxWidth, y, y + totalHeight);
		break;
	}
	case ETextBoxPositionAlign_RIGHT:
	{
		int y = hitbox.getTopLeft().getY() - mOutlineWidth;
		int x2 = hitbox.getBottomRight().getX() + mOutlineWidth;
		int maxWidth = 0;
		int totalHeight = y;

		int curY = hitbox.getTopLeft().getY();
		hitboxes.clear();
		for (int count = 0; count < (int)mTextLines.size(); count++)
		{
			SDL_Point size = getTextRenderSize(count, isHighlighted);
			size.x += mOutlineWidth * 2;
			size.y += mOutlineWidth * 2;
			maxWidth = std::max(maxWidth, size.x);
			totalHeight += size.y;
			Hitbox curHitbox = Hitbox(x2 - size.x, x2, curY, curY + size.y);
			curY += size.y;
			hitboxes.push_back(curHitbox);
		}
		if (y < 0)
		{
			y = 0;
		}
		hitbox = Hitbox(x2 - maxWidth, x2, y, y + totalHeight);
		break;
	}
	default:
		SDL_assert(false);
		break;
	}

}

void TextBox::shiftHitbox(const Vect2 shiftTopLeft)
{
	mStandardHitbox.updateTopLeft(shiftTopLeft);
	mHighlightedHitbox.updateTopLeft(shiftTopLeft);

	for (int count = 0; count < mCurLineStandardHitboxes.size(); count++)
	{
		mCurLineStandardHitboxes[count].updateTopLeft(shiftTopLeft);
		mCurLineHighlightedHitboxes[count].updateTopLeft(shiftTopLeft);
	}
}



ImageBox::ImageBox(const ImageBoxPreset preset, const ImageBoxPositionInfo positionInfo, const std::string fileName) : UIBox(preset.mData)
{
	mClassType		= EUIBoxClass_IMAGEBOX;
	mImageObject	= ImageObject(fileName, positionInfo.mMaxWidth, positionInfo.mMaxHeight, EHowToDetermineWidthHeight_GET_BEST_IMAGE_RATIO);
	mPositionAlign	= positionInfo.mPositionAlign;

	// create hitbox
	int x = 0;
	int y = 0;
	switch (mPositionAlign)
	{
	case ETextBoxPositionAlign_LEFT:
		x = positionInfo.mPosition.getX();
		y = positionInfo.mPosition.getY();
		break;
	case ETextBoxPositionAlign_CENTER:
		x = positionInfo.mPosition.getX() - int(mImageObject.getHalfIdealImageWidth());
		y = positionInfo.mPosition.getY() - int(mImageObject.getHalfIdealImageHeight());
		break;
	case ETextBoxPositionAlign_RIGHT:
		x = positionInfo.mPosition.getX() - mImageObject.mIdealImageWidth;
		y = positionInfo.mPosition.getY();
		break;
	default:
		SDL_assert(false);
		break;
	}

	mpCurHitbox    = new Hitbox(x, mImageObject.mIdealImageWidth + x, y, mImageObject.mIdealImageHeight + y);
	mMargins       = positionInfo.mMargins;

	mRotation      = positionInfo.mRotation;
}

void ImageBox::shiftHitbox(const Vect2 shiftTopLeft) { mpCurHitbox->updateTopLeft(shiftTopLeft); }




ShapeBox::ShapeBox(const ShapeBoxPreset preset, const TextBoxPositionInfo positionInfo, const SDL_Color color) : UIBox(preset.mData), mShapeType(preset.mType), mColor(color)
{
	mClassType = EUIBoxClass_SHAPEBOX;
	mPositionAlign = positionInfo.mPositionAlign;

	// create hitbox
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
		x = positionInfo.mPosition.getX() - positionInfo.mMaxWidth;
		y = positionInfo.mPosition.getY();
		break;
	default:
		SDL_assert(false);
		break;
	}

	mpCurHitbox = new Hitbox(x, positionInfo.mMaxWidth + x, y, positionInfo.mMaxHeight + y);
	mMargins = positionInfo.mMargins;
}

void ShapeBox::shiftHitbox(const Vect2 shiftTopLeft) { mpCurHitbox->updateTopLeft(shiftTopLeft); }



HealthBox::HealthBox(const HealthBoxPreset preset, const TextBoxPositionInfo positionInfo, const char* font, int textSize, 
		const SDL_Color healthColor, const SDL_Color backgroundColor, const SDL_Color textColor) : UIBox(preset.mData),
		mBoundingBox(ShapeBox(ShapeBoxPreset(EShapeBoxClass_RECT), positionInfo, backgroundColor)), 
		mHealthLeftBox(ShapeBox(ShapeBoxPreset(EShapeBoxClass_RECT), positionInfo, healthColor)),
		mHealthText(TextBox(StandardTextBoxPreset("temp"), ETextBoxFunction_NO_FUNCTION, positionInfo, font, textSize, textColor)), 
		mMaxWidth(positionInfo.mMaxWidth)
{ 
	mPositionAlign = positionInfo.mPositionAlign;
	mClassType = EUIBoxClass_HEALTHBOX;
	mpCurHitbox = mBoundingBox.mpCurHitbox;
	mMargins = positionInfo.mMargins;
}

void HealthBox::shiftHitbox(const Vect2 shiftTopLeft) 
{
	mBoundingBox.shiftHitbox(shiftTopLeft);
	mHealthLeftBox.shiftHitbox(shiftTopLeft);
	mHealthText.shiftHitbox(shiftTopLeft);
}

void HealthBox::updateMessage(SDL_Renderer* pRenderer, FontSizeChart& fontSizeChart, const std::string updatedMessage, float curRatio)
{
	bool haveSetUp = mHealthText.mSetUp;
	mHealthText.updateMessage(pRenderer, fontSizeChart, updatedMessage);
	mHealthLeftBox.mpCurHitbox->setWidth((int)std::max(mMaxWidth * curRatio, 1.0f));
	if (!haveSetUp)
	{
		mHealthLeftBox.mpCurHitbox->setHeight(mHealthText.mpCurHitbox->getHeight());
		mBoundingBox.mpCurHitbox->setHeight(mHealthText.mpCurHitbox->getHeight());
	}
}
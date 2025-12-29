#include "uiBox.h"

UIBox::UIBox(const UIBoxData data) : mData(data) 
{ 
	mClassType = EUIClass_BOX;
}

UIBox::~UIBox()
{
	mpCurHitbox = nullptr;
}

Hitbox& UIBox::getHitbox() { return *mpCurHitbox; }

bool UIBox::isActive() { return mShow; }


TextBox::TextBox(const TextBoxPreset preset, ETextBoxFunction textBoxFunction, UIPositionInfo positionInfo, const char* fileName, TextBoxSizeInfo sizeInfo,
		TextBoxColorInfo colorInfo) : UIBox(preset.mData), mFunction(textBoxFunction), mMessage(preset.mMessage),
		mFontFile(fileName), mStandardFontSize(sizeInfo.mStandardFontSize), mHighlightedFontSize(sizeInfo.mHighlightedFontSize), 
		mOutlineWidth(sizeInfo.mOutlineWidth), mStandardTextBoxColor(colorInfo.mStandardTextBoxColor), 
		mHighlightedTextBoxColor(colorInfo.mHighlightedTextBoxColor), mDisabledTextBoxColor(colorInfo.mDisabledTextBoxColor), 
		mHighlightedDisabledTextBoxColor(colorInfo.mHighlightedDisabledTextBoxColor),
		mOutlineColor(colorInfo.mOutlineColor), mHighlightedOutlineColor(colorInfo.mHighlightedOutlineColor), 
		mDisabledOutlineColor(colorInfo.mDisabledOutlineColor)
{
	mBoxType = EUIBoxClass_TEXTBOX;
	mPositionAlignH	= positionInfo.mPositionAlignH;
	mPositionAlignV	= positionInfo.mPositionAlignV;
	
	
	// create hitbox
	int x = positionInfo.mPosition.getX();
	int y = positionInfo.mPosition.getY();

	mStandardHitbox			= Hitbox(x, positionInfo.mMaxWidth + x, y, positionInfo.mMaxHeight + y);
	mHighlightedHitbox		= Hitbox(x, positionInfo.mMaxWidth + x, y, positionInfo.mMaxHeight + y);
	mpCurHitbox				= &mStandardHitbox;
	mMaxWidth				= positionInfo.mMaxWidth;
	mMaxHeight				= positionInfo.mMaxHeight;
	mMargins				= positionInfo.mMargins;

	mStandardTextColor		 = { colorInfo.mStandardTextColor.r,		colorInfo.mStandardTextColor.g,		colorInfo.mStandardTextColor.b };
	mHighlightedTextColor	 = { colorInfo.mHighlightedTextColor.r,		colorInfo.mHighlightedTextColor.g,	colorInfo.mHighlightedTextColor.b };
	mDisabledTextColor		 = { colorInfo.mDisabledTextColor.r,		colorInfo.mDisabledTextColor.g,		colorInfo.mDisabledTextColor.b };
	
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

	for (SDL_Texture* pTexture : mpDisabledTextures)
	{
		SDL_DestroyTexture(pTexture);
	}
	mpDisabledTextures.clear();


	mpCurTextures = nullptr;
	mpCurHitbox = nullptr;
	mpCurLineHitboxes = nullptr;
}

void TextBox::updateMessage(SDL_Renderer* pRenderer, FontSizeChart& fontSizeChart, const std::string textMessage)
{
	mMessage = textMessage;
	calcMaxFontSizeGivenText(fontSizeChart);
	if (DEBUG && mHighlightedFontSize > mMaxFontSizeGivenText)
	{
		std::cout << "Font size too big: " + textMessage + "\n max size: " + std::to_string(mMaxFontSizeGivenText) + " requested size: " + std::to_string(mHighlightedFontSize)<<"\n";
	}
	if (mHighlightedFontSize > mMaxFontSizeGivenText)
	{
		mHighlightedFontSize = mMaxFontSizeGivenText;
		mStandardFontSize = mMaxFontSizeGivenText;
	}

	updateTextLines(textMessage, fontSizeChart);
	setTexture(pRenderer);
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
	int curFontSize = getIsHighlighted() ? mHighlightedFontSize : mStandardFontSize;
	int numCharsPerLine = mMaxWidth / fontSizeChart.mFontChart[mFontFile][curFontSize].x;
	SDL_assert(numCharsPerLine > 0);
	mTextLines = tokenizeByStringLength(text, numCharsPerLine);
}

void TextBox::updateTexture(SDL_Renderer* pRenderer)
{
	setTexture(pRenderer);
	updateHitboxes();
}

void TextBox::setTexture(SDL_Renderer* pRenderer)
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

	for (SDL_Texture* pTexture : mpDisabledTextures)
	{
		SDL_DestroyTexture(pTexture);
	}
	mpDisabledTextures.clear();

	mpStandardFont	  = TTF_OpenFont(mFontFile, mStandardFontSize);
	mpHighlightedFont = TTF_OpenFont(mFontFile, mHighlightedFontSize);

	for (int countLine = 0; countLine < mTextLines.size(); countLine++)
	{
		std::string curLine = mTextLines[countLine] + " "; // spacing to fix alignment issues
		
		SDL_Surface* standardSurface = TTF_RenderUTF8_Blended(mpStandardFont, curLine.c_str(), mStandardTextColor);
		mpStandardTextures.push_back(SDL_CreateTextureFromSurface(pRenderer, standardSurface));
		SDL_DestroySurface(standardSurface);

		SDL_Surface* highlightedSurface = TTF_RenderUTF8_Blended(mpHighlightedFont, curLine.c_str(), mHighlightedTextColor);
		mpHighlightedTextures.push_back(SDL_CreateTextureFromSurface(pRenderer, highlightedSurface));
		SDL_DestroySurface(highlightedSurface);

		SDL_Surface* disabledSurface = TTF_RenderUTF8_Blended(mpStandardFont, curLine.c_str(), mDisabledTextColor);
		mpDisabledTextures.push_back(SDL_CreateTextureFromSurface(pRenderer, disabledSurface));
		SDL_DestroySurface(disabledSurface);
	}

	SDL_assert(mTextLines.size() == mpStandardTextures.size() && mTextLines.size() == mpDisabledTextures.size() && mpHighlightedTextures.size() >= mTextLines.size());

	mpCurTextures = &getTexturesForState();
}


void TextBox::changeState(ETextBoxState state)
{
	switch(state)
	{
	case ETextBoxState_HIGHLIGHTED:
		mHighlighted		= true;
		mpCurTextures		= &mpHighlightedTextures;
		mpCurLineHitboxes	= &mCurLineHighlightedHitboxes;
		mpCurHitbox			= &mHighlightedHitbox;
		break;
	case ETextBoxState_DISABLED:
		mHighlighted		= false;
		mpCurTextures		= &mpDisabledTextures;
		mpCurLineHitboxes	= &mCurLineStandardHitboxes;
		mpCurHitbox			= &mStandardHitbox;
		break;
	case ETextBoxState_DISABLED_AND_HIGHLIGHTED:
		mHighlighted		= true;
		mpCurTextures		= &mpDisabledTextures;
		mpCurLineHitboxes	= &mCurLineStandardHitboxes;
		mpCurHitbox			= &mStandardHitbox;
		break;
	default:
		mHighlighted		= false;
		mpCurTextures		= &mpStandardTextures;
		mpCurLineHitboxes	= &mCurLineStandardHitboxes;
		mpCurHitbox			= &mStandardHitbox;
		break;
	}
	mState = state;
}

void TextBox::changeIsHighlighted(bool highlighted)
{
	if (highlighted)
	{
		switch (mState)
		{
		case ETextBoxState_DISABLED:
			changeState(ETextBoxState_DISABLED_AND_HIGHLIGHTED);
			break;
		case ETextBoxState_NORMAL:
			changeState(ETextBoxState_HIGHLIGHTED);
			break;
		default:
			break;
		}
	}
	else
	{
		switch (mState)
		{
		case ETextBoxState_DISABLED_AND_HIGHLIGHTED:
			changeState(ETextBoxState_DISABLED);
			break;
		case ETextBoxState_HIGHLIGHTED:
			changeState(ETextBoxState_NORMAL);
			break;
		default:
			break;
		}
	}
}

void TextBox::changeIsDisabled(bool disabled)
{
	if (disabled)
	{
		switch (mState)
		{
		case ETextBoxState_NORMAL:
			changeState(ETextBoxState_DISABLED);
			break;
		case ETextBoxState_HIGHLIGHTED:
			changeState(ETextBoxState_DISABLED_AND_HIGHLIGHTED);
			break;
		default:
			break;
		}
	}
	else
	{
		switch (mState)
		{
		case ETextBoxState_DISABLED:
			changeState(ETextBoxState_NORMAL);
			break;
		case ETextBoxState_DISABLED_AND_HIGHLIGHTED:
			changeState(ETextBoxState_HIGHLIGHTED);
			break;
		default:
			break;
		}
	}
}


bool TextBox::getIsHighlighted() const { return mHighlighted; }

std::vector<SDL_Texture*> TextBox::getTextBoxTexture() const { return *mpCurTextures; }

SDL_Point TextBox::getTextRenderSize(int line) const { return getTextRenderSize(line, mHighlighted); }

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

SDL_Color TextBox::getTextBoxColor() const 
{
	switch (mState)
	{
	case ETextBoxState_HIGHLIGHTED: return mHighlightedTextBoxColor;
	case ETextBoxState_NORMAL: return mStandardTextBoxColor;
	case ETextBoxState_DISABLED: return mDisabledTextBoxColor;
	case ETextBoxState_DISABLED_AND_HIGHLIGHTED: return mHighlightedDisabledTextBoxColor;
	default:
		SDL_assert(false);
	}
}

SDL_Color TextBox::getTextColor() const
{
	switch (mState)
	{
	case ETextBoxState_HIGHLIGHTED: return mHighlightedTextColor;
	case ETextBoxState_NORMAL: return mStandardTextColor;
	case ETextBoxState_DISABLED:
	case ETextBoxState_DISABLED_AND_HIGHLIGHTED: 
		return mDisabledTextColor;
	default:
		SDL_assert(false);
	}
}

SDL_Color TextBox::getOutlineColor() const
{
	switch (mState)
	{
	case ETextBoxState_HIGHLIGHTED: return mHighlightedOutlineColor;
	case ETextBoxState_NORMAL: return mOutlineColor;
	case ETextBoxState_DISABLED: 
	case ETextBoxState_DISABLED_AND_HIGHLIGHTED: 
		return mDisabledOutlineColor;
	default:
		SDL_assert(false);
	}
}


void TextBox::updateHitboxes() 
{
	updateHitboxesInternal(false, mStandardHitbox, mCurLineStandardHitboxes);
	updateHitboxesInternal(true, mHighlightedHitbox, mCurLineHighlightedHitboxes);
	mpCurLineHitboxes =  &getCurLineHitboxesForState();
	mpCurHitbox = &getCurHitboxForState();
}

void TextBox::updateHitboxesInternal(bool isHighlighted, Hitbox& hitbox, std::vector<Hitbox>& hitboxes )
{
	int maxWidth = 0;
	int totalHeight = 0;

	int x;
	int y;

	hitboxes.clear();
	std::vector <SDL_Point> lineSizes;
	for (int count = 0; count < (int)mTextLines.size(); count++)
	{
		SDL_Point size = getTextRenderSize(count, isHighlighted);
		maxWidth = std::max(maxWidth, size.x);
		lineSizes.push_back(size);
		totalHeight += size.y;
	}

	switch (mPositionAlignH)
	{
	case EUIPositionAlign_LEFT:
		x = hitbox.getTopLeft().getX();
		break;
	case EUIPositionAlign_CENTER:
		x = hitbox.getCenter().getX() - (maxWidth / 2);
		break;
	case EUIPositionAlign_RIGHT:
		x = hitbox.getBottomRight().getX() - maxWidth;
		break;
	}
	switch (mPositionAlignV)
	{
	case EUIPositionAlign_TOP:
		y = hitbox.getTopLeft().getY();
		break;
	case EUIPositionAlign_CENTER:
		y = hitbox.getCenter().getY() - (totalHeight / 2);
		break;
	case EUIPositionAlign_BOTTOM:
		y = hitbox.getBottomRight().getX() - totalHeight;
		break;
	}

	
	int curY = y;
	for (const SDL_Point& size : lineSizes)
	{
		int curX = x;
		switch (mPositionAlignH)
		{
		case EUIPositionAlign_LEFT:
			curX = x;
			break;
		case EUIPositionAlign_CENTER:
			curX = x + (maxWidth - size.x) / 2;
			break;
		case EUIPositionAlign_RIGHT:
			curX = x - size.x;
			break;
		}
		Hitbox curHitbox = Hitbox(curX, curX + size.x, curY, curY + size.y);
		curY += size.y;
		hitboxes.push_back(curHitbox);
	}
	hitbox = Hitbox(x, x + maxWidth, y, y + totalHeight);
	
}

void TextBox::updatePosFromBlockSpace(const Hitbox& blockSpace)
{
	Vect2 shiftTopLeft = getUpdatedPosFromBlockSpace(blockSpace) - mpCurHitbox->getTopLeft();

	mStandardHitbox.updateTopLeft(shiftTopLeft);
	mHighlightedHitbox.updateTopLeft(shiftTopLeft);

	for (int count = 0; count < mCurLineStandardHitboxes.size(); count++)
	{
		mCurLineStandardHitboxes[count].updateTopLeft(shiftTopLeft);
		mCurLineHighlightedHitboxes[count].updateTopLeft(shiftTopLeft);
	}
}


Hitbox& TextBox::getCurHitboxForState() 
{
	switch (mState)
	{
	case ETextBoxState_HIGHLIGHTED: return mHighlightedHitbox;
	case ETextBoxState_DISABLED:
	case ETextBoxState_DISABLED_AND_HIGHLIGHTED:
	default: 
		return mStandardHitbox;
	}
}

std::vector<Hitbox>& TextBox::getCurLineHitboxesForState()
{
	switch (mState)
	{
	case ETextBoxState_HIGHLIGHTED: return mCurLineHighlightedHitboxes;
	case ETextBoxState_DISABLED: 
	case ETextBoxState_DISABLED_AND_HIGHLIGHTED:
	default: 
		return mCurLineStandardHitboxes;
	}
}

std::vector<SDL_Texture*>& TextBox::getTexturesForState()
{
	switch (mState)
	{
	case ETextBoxState_HIGHLIGHTED: return mpHighlightedTextures;
	case ETextBoxState_DISABLED: return mpDisabledTextures;
	case ETextBoxState_DISABLED_AND_HIGHLIGHTED: return mpDisabledTextures;
	default: return mpStandardTextures;
	}
}



ImageBox::ImageBox(const ImageBoxPreset preset, const UIPositionInfo positionInfo, const std::string fileName) : UIBox(preset.mData)
{
	mBoxType = EUIBoxClass_IMAGEBOX;
	mImageObject	= ImageObject(fileName, positionInfo.mMaxWidth, positionInfo.mMaxHeight, EHowToDetermineWidthHeight_GET_BEST_IMAGE_RATIO);
	mPositionAlignH	= positionInfo.mPositionAlignH;
	mPositionAlignV = positionInfo.mPositionAlignV;

	// create hitbox
	int x = positionInfo.mPosition.getX();
	int y = positionInfo.mPosition.getY();

	mpCurHitbox    = new Hitbox(x, mImageObject.mIdealImageWidth + x, y, mImageObject.mIdealImageHeight + y);
	mMargins       = positionInfo.mMargins;

	mRotation      = positionInfo.mRotation;
}

void ImageBox::updatePosFromBlockSpace(const Hitbox& blockSpace) { mpCurHitbox->setTopLeft(getUpdatedPosFromBlockSpace(blockSpace)); }

void ImageBox::setTexture(SDL_Renderer* pRenderer) { mImageObject.setUpTexture(pRenderer); }


ShapeBox::ShapeBox(const ShapeBoxPreset preset, const UIPositionInfo positionInfo, const SDL_Color color) : UIBox(preset.mData), mShapeType(preset.mType), mColor(color)
{
	mBoxType = EUIBoxClass_SHAPEBOX;
	mPositionAlignH = positionInfo.mPositionAlignH;
	mPositionAlignV = positionInfo.mPositionAlignV;

	// create hitbox
	int x = positionInfo.mPosition.getX();
	int y = positionInfo.mPosition.getY();

	mpCurHitbox = new Hitbox(x, positionInfo.mMaxWidth + x, y, positionInfo.mMaxHeight + y);
	mMargins = positionInfo.mMargins;
}

void ShapeBox::updatePosFromBlockSpace(const Hitbox& blockSpace) { mpCurHitbox->setTopLeft(getUpdatedPosFromBlockSpace(blockSpace)); }



HealthBox::HealthBox(const HealthBoxPreset preset, const UIPositionInfo positionInfo, const char* font, int textSize, const SDL_Color healthColor, 
		const SDL_Color backgroundColor, const SDL_Color textColor) : UIBox(preset.mData), 
		mBoundingBox(ShapeBox(ShapeBoxPreset(EShapeBoxClass_RECT), positionInfo, backgroundColor)), 
		mHealthLeftBox(ShapeBox(ShapeBoxPreset(EShapeBoxClass_RECT), positionInfo, healthColor)),
		mHealthText(TextBox(StandardTextBoxPreset("temp"), ETextBoxFunction_NO_FUNCTION, positionInfo, font, TextBoxSizeInfo(textSize), TextBoxColorInfo(textColor)))
{ 
	mPositionAlignH = positionInfo.mPositionAlignH;
	mPositionAlignV = positionInfo.mPositionAlignV;
	mBoxType = EUIBoxClass_HEALTHBOX;
	mpCurHitbox = mBoundingBox.mpCurHitbox;
	mMargins = positionInfo.mMargins;
	mMaxWidth = positionInfo.mMaxWidth;
	mMaxHeight = positionInfo.mMaxHeight;
}

void HealthBox::updatePosFromBlockSpace(const Hitbox& blockSpace)
{
	Vect2 shiftTopLeft = getUpdatedPosFromBlockSpace(blockSpace) - mBoundingBox.getHitbox().getTopLeft();

	mBoundingBox.getHitbox().updateTopLeft(shiftTopLeft);
	mHealthLeftBox.getHitbox().updateTopLeft(shiftTopLeft);
	mHealthText.updatePosFromBlockSpace(blockSpace);
}

void HealthBox::updateRatio(float curRatio) { mHealthLeftBox.mpCurHitbox->setWidth((int)std::max(mMaxWidth * curRatio, 1.0f)); }

void HealthBox::updateMessage(SDL_Renderer* pRenderer, FontSizeChart& fontSizeChart, const std::string updatedMessage, float curRatio)
{
	bool haveSetUp = mHealthText.mSetUp;
	mHealthText.updateMessage(pRenderer, fontSizeChart, updatedMessage);
	updateRatio(curRatio);
	if (!haveSetUp)
	{
		mHealthLeftBox.mpCurHitbox->setHeight(mHealthText.mpCurHitbox->getHeight());
		mBoundingBox.mpCurHitbox->setHeight(mHealthText.mpCurHitbox->getHeight());
	}
	mBoundingBox.mShow = mShow;
	mHealthLeftBox.mShow = mShow;
	mHealthText.mShow = mShow;
}


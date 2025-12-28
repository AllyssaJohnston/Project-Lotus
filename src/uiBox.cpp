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
		mHighlightedTextBoxColor(colorInfo.mHighlightedTextBoxColor), mOutlineColor(colorInfo.mOutlineColor),
		mHighlightedOutlineColor(colorInfo.mHighlightedOutlineColor)
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
	mpCurLineHitboxes = nullptr;
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
	int curFontSize = mIsHighlighted ? mHighlightedFontSize : mStandardFontSize;
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
	mBoundingBox.mShow = mShow;
	mHealthLeftBox.mShow = mShow;
	mHealthText.mShow = mShow;
}


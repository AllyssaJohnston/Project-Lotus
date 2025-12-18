#include "textBoxHelperClass.h"


TextBoxData::TextBoxData(EGameStatBoxValueToDisplay gameStatToDisplay, int combatCharacterIndex, bool showDuringAllCharacters, ECharacterStatBoxValueToDisplay characterStatToDisplay,
	int attackNum, ETextBoxType type, std::vector <EMiniGameState>& miniGameStateWhenToShowList) : mGameStatToDisplay(gameStatToDisplay), mCombatCharacterIndex(combatCharacterIndex), 
	mShowDuringAllCharacters(showDuringAllCharacters), mCharacterStatToDisplay(characterStatToDisplay), mAttackNum(attackNum), mType(type), mMiniGameStateWhenToShowList(miniGameStateWhenToShowList) { ; }


TextBoxPositionInfo::TextBoxPositionInfo(Vect2 position, ETextBoxPositionAlign positionAlign, ETextBoxTextAlign textAlign, int maxWidth, int maxHeight, Edges margins)
	: mPosition(position), mPositionAlign(positionAlign), mTextAlign(textAlign), mMaxWidth(maxWidth), mMaxHeight(maxHeight), mMargins(margins) { ; }

TextBoxPositionInfo::TextBoxPositionInfo(ETextBoxPositionAlign positionAlign, ETextBoxTextAlign textAlign, int maxWidth, int maxHeight, Edges margins) 
	: mPositionAlign(positionAlign), mTextAlign(textAlign), mMaxWidth(maxWidth), mMaxHeight(maxHeight), mMargins(margins)
{
	if (mPositionAlign == ETextBoxPositionAlign_LEFT)
	{
		mPosition = Vect2(0, 0);
	}
	else if (mPositionAlign == ETextBoxPositionAlign_CENTER)
	{
		mPosition = Vect2(maxWidth / 2, maxHeight / 2);
	}
	else 
	{
		SDL_assert(false);
	}
}

TextBoxPositionInfo::TextBoxPositionInfo(Vect2 position, ETextBoxPositionAlign positionAlign, ETextBoxTextAlign textAlign, int maxWidth, int maxHeight) 
	: mPosition(position), mPositionAlign(positionAlign), mTextAlign(textAlign), mMaxWidth(maxWidth), mMaxHeight(maxHeight)
{	
	mMargins = Edges();
}


TextBoxSizeInfo::TextBoxSizeInfo(int standardSize, int highlightSize, int outlineWidth) : mStandardFontSize(standardSize), mHighlightedFontSize(highlightSize), mOutlineWidth(outlineWidth) { ; }

TextBoxSizeInfo::TextBoxSizeInfo(int standardSize, int highlightSize) :mStandardFontSize(standardSize), mHighlightedFontSize(highlightSize) { ; }

TextBoxSizeInfo::TextBoxSizeInfo(int standardSize) : mStandardFontSize(standardSize), mHighlightedFontSize(standardSize) { ; }

TextBoxColorInfo::TextBoxColorInfo(SDL_Color standardTextColor, SDL_Color highlightedTextColor, SDL_Color standardTextBoxColor, SDL_Color highlightedTextBoxColor, 
		SDL_Color outlineColor, SDL_Color highlightedOutlineColor)
		: mStandardTextColor(standardTextColor), mHighlightedTextColor(highlightedTextColor), mStandardTextBoxColor(standardTextBoxColor), mHighlightedTextBoxColor(highlightedTextBoxColor),
		mOutlineColor(outlineColor), mHighlightedOutlineColor(highlightedOutlineColor) { ; }

TextBoxColorInfo::TextBoxColorInfo(SDL_Color standardTextColor, SDL_Color highlightedTextColor, SDL_Color standardTextBoxColor, SDL_Color highlightedTextBoxColor) 
	: mStandardTextColor(standardTextColor), mHighlightedTextColor(highlightedTextColor), mStandardTextBoxColor(standardTextBoxColor), mHighlightedTextBoxColor(highlightedTextBoxColor) { ; }

TextBoxColorInfo::TextBoxColorInfo(SDL_Color standardTextColor, SDL_Color standardTextBoxColor, SDL_Color highlightedTextBoxColor)
	: mStandardTextColor(standardTextColor), mHighlightedTextColor(standardTextColor), mStandardTextBoxColor(standardTextBoxColor), mHighlightedTextBoxColor(highlightedTextBoxColor) { ; }

TextBoxColorInfo::TextBoxColorInfo(SDL_Color standardTextColor, SDL_Color standardTextBoxColor)
	: mStandardTextColor(standardTextColor), mHighlightedTextColor(standardTextColor), mStandardTextBoxColor(standardTextBoxColor), mHighlightedTextBoxColor(standardTextBoxColor) { ; }

TextBoxColorInfo::TextBoxColorInfo(SDL_Color standardTextColor) : mStandardTextColor(standardTextColor), mHighlightedTextColor(standardTextColor)
{
	mStandardTextBoxColor = { 0, 0, 0, 0 };
	mHighlightedTextBoxColor = { 0, 0, 0, 0 };
}


ImageBoxPositionInfo::ImageBoxPositionInfo(Vect2 position, int rotation, ETextBoxPositionAlign positionAlign, int maxWidth, int maxHeight, Edges margins)
	: mPosition(position), mRotation(rotation), mPositionAlign(positionAlign), mMaxWidth(maxWidth), mMaxHeight(maxHeight), mMargins(margins) { ; }

ImageBoxPositionInfo::ImageBoxPositionInfo(int rotation, ETextBoxPositionAlign positionAlign, int maxWidth, int maxHeight, Edges margins)
	: mRotation(rotation), mPositionAlign(positionAlign), mMaxWidth(maxWidth), mMaxHeight(maxHeight), mMargins(margins)
{
	if (mPositionAlign == ETextBoxPositionAlign_LEFT)
	{
		mPosition = Vect2(0, 0);
	}
	else if (mPositionAlign == ETextBoxPositionAlign_CENTER)
	{
		mPosition = Vect2(maxWidth, maxHeight);
	}
	else 
	{
		SDL_assert(false);
	}
}

ImageBoxPositionInfo::ImageBoxPositionInfo(Vect2 position, int rotation, ETextBoxPositionAlign positionAlign, int maxWidth, int maxHeight)
	: mPosition(position), mRotation(rotation), mPositionAlign(positionAlign), mMaxWidth(maxWidth), mMaxHeight(maxHeight) 
{
	mMargins = Edges();
}


void FontSizeChart::createFontChart(const char* fontName, SDL_Renderer* pRenderer)
{
	for (int i = mMinFontSize; i <= mMaxFontSize; i++)
	{
		TTF_Font* font = TTF_OpenFont(fontName, i);
		SDL_Texture* texture = (SDL_CreateTextureFromSurface(pRenderer, TTF_RenderUTF8_Blended(font, "B", { 0, 0, 0, 1 })));
		SDL_Point size;
		SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);
		mFontChart[fontName][i] = size;
	}
}
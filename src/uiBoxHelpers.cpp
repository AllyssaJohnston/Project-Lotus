#include "uiBoxHelpers.h"


UIBoxData::UIBoxData(EGameStatBoxValueToDisplay gameStatToDisplay, int combatCharacterIndex, bool showDuringAllCharacters, ECharacterStatBoxValueToDisplay characterStatToDisplay,
	int attackNum, EMiniGameCombatAttackCategoryType attackCategory, EUIBoxType type, std::vector <EMiniGameState>& miniGameStateWhenToShowList) : mGameStatToDisplay(gameStatToDisplay), mCombatCharacterIndex(combatCharacterIndex),
	mShowDuringAllCharacters(showDuringAllCharacters), mCharacterStatToDisplay(characterStatToDisplay), mAttackNum(attackNum), mAttackCategory(attackCategory), mType(type), mMiniGameStateWhenToShowList(miniGameStateWhenToShowList) { ; }



TextBoxPositionInfo::TextBoxPositionInfo(ETextBoxPositionAlign positionAlignH, ETextBoxPositionAlign positionAlignV, int maxWidth, int maxHeight, Edges margins)
	: mPositionAlignH(positionAlignH), mPositionAlignV(positionAlignV), mMaxWidth(maxWidth), mMaxHeight(maxHeight), mMargins(margins)
{
	int x = 0;
	int y = 0;
	if (mPositionAlignH == ETextBoxPositionAlign_CENTER)
	{
		x = maxWidth / 2;
	}
	if (mPositionAlignV == ETextBoxPositionAlign_CENTER)
	{
		y = maxHeight / 2;
	}

	mPosition = Vect2(x, y);
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


ImageBoxPositionInfo::ImageBoxPositionInfo(int rotation, ETextBoxPositionAlign positionAlignH, ETextBoxPositionAlign positionAlignV, int maxWidth, int maxHeight, Edges margins)
	: mRotation(rotation), mPositionAlignH(positionAlignH), mPositionAlignV(positionAlignV), mMaxWidth(maxWidth), mMaxHeight(maxHeight), mMargins(margins)
{
	int x = 0;
	int y = 0;
	if (mPositionAlignH == ETextBoxPositionAlign_LEFT)
	{
		x = 0;
	}
	else if (mPositionAlignH == ETextBoxPositionAlign_CENTER)
	{
		x = maxWidth / 2;
	}
	else if (mPositionAlignH == ETextBoxPositionAlign_RIGHT)
	{
		x = maxWidth;
	}
	else 
	{
		SDL_assert(false);
	}
	if (mPositionAlignV == ETextBoxPositionAlign_TOP)
	{
		y = 0;
	}
	else if (mPositionAlignV == ETextBoxPositionAlign_CENTER)
	{
		y = maxHeight /2;
	}
	else if (mPositionAlignV == ETextBoxPositionAlign_BOTTOM)
	{
		y = maxHeight;
	}
	else 
	{
		SDL_assert(false);
	}
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


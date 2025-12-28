#include "uiBoxHelpers.h"


UIBoxData::UIBoxData(EUIBoxValueToDisplay gameStatToDisplay, int combatCharacterIndex, bool showDuringAllCharacters, int attackNum, EMiniGameCombatAttackCategoryType attackCategory, 
	EUIBoxType type, std::vector <EMiniGameState>& miniGameStateWhenToShowList) : mGameStatToDisplay(gameStatToDisplay), mCombatCharacterIndex(combatCharacterIndex),
	mShowDuringAllCharacters(showDuringAllCharacters), mAttackNum(attackNum), mAttackCategory(attackCategory), mType(type), mMiniGameStateWhenToShowList(miniGameStateWhenToShowList) { ; }



UIPositionInfo::UIPositionInfo(EUIPositionAlign positionAlignH, EUIPositionAlign positionAlignV, Hitbox hitbox, Edges margins)
	: mPositionAlignH(positionAlignH), mPositionAlignV(positionAlignV), mMaxWidth(hitbox.getWidth()), mMaxHeight(hitbox.getHeight()), mMargins(margins)
{ mPosition = hitbox.getTopLeft() + Vect2(margins.mLeft, margins.mTop) - Vect2(margins.mRight, margins.mBottom); }

UIPositionInfo::UIPositionInfo(EUIPositionAlign positionAlignH, EUIPositionAlign positionAlignV, int maxWidth, int maxHeight, Edges margins)
	: mPositionAlignH(positionAlignH), mPositionAlignV(positionAlignV), mMaxWidth(maxWidth), mMaxHeight(maxHeight), mMargins(margins)
	{ mPosition = Vect2(0, 0); }

UIPositionInfo::UIPositionInfo(int rotation, EUIPositionAlign positionAlignH, EUIPositionAlign positionAlignV, Hitbox hitbox, Edges margins)
	: mRotation(rotation), mPositionAlignH(positionAlignH), mPositionAlignV(positionAlignV), mMaxWidth(hitbox.getWidth()), mMaxHeight(hitbox.getHeight()), mMargins(margins)
	{ mPosition = hitbox.getTopLeft() + Vect2(margins.mLeft, margins.mTop) - Vect2(margins.mRight, margins.mBottom); }

UIPositionInfo::UIPositionInfo(int rotation, EUIPositionAlign positionAlignH, EUIPositionAlign positionAlignV, int maxWidth, int maxHeight, Edges margins)
	: mRotation(rotation), mPositionAlignH(positionAlignH), mPositionAlignV(positionAlignV), mMaxWidth(maxWidth), mMaxHeight(maxHeight), mMargins(margins) 
	{ mPosition = Vect2(0, 0); }


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


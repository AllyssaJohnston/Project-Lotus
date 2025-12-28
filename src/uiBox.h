#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "globals.h"
#include "hitbox.h"
#include "uiBoxPresets.h"
#include "image.h"
#include "UIElement.h"
#include "uiboxHelpers.h"



//TODO 1. for scaling text size, have a percentage, and simply force all hitboxes to scale by that amount. Keep margins as is. 
// 2. Then to fix gross text scaling, use font size chart to find an appropiate text size

class UIBox : public UIElement
{
public:
	const UIBoxData			mData;
	bool					mShow			= true;
	Hitbox*					mpCurHitbox		= nullptr;
	EUIBoxClass				mBoxType		= EUIBoxClass_INVALID;

	UIBox(const UIBoxData data);
	virtual ~UIBox();

	virtual Hitbox& getHitbox() override;

	bool isActive() override;
};

class TextBox : public UIBox
{
public:
	ETextBoxFunction	mFunction				= ETextBoxFunction_INVALID;
	std::string			mMessage				= "invalid";

	std::vector<SDL_Texture*>* mpCurTextures	= nullptr;
	std::vector<Hitbox>* mpCurLineHitboxes		= nullptr;

	int	mOutlineWidth = 0;

	bool mSetUp = false;

private:
	ETextBoxState mState = ETextBoxState_NORMAL;
	bool mHighlighted = false;

	const char* mFontFile;
	TTF_Font*	mpStandardFont = nullptr;
	TTF_Font*	mpHighlightedFont = nullptr;
	int			mStandardFontSize = -1;
	int			mHighlightedFontSize = -1;
	int			mMaxFontSizeGivenText = FontSizeChart::mMinFontSize;

	std::vector <std::string> mTextLines;

	Hitbox mStandardHitbox = Hitbox();
	Hitbox mHighlightedHitbox = Hitbox();
	std::vector<Hitbox> mCurLineStandardHitboxes;
	std::vector<Hitbox> mCurLineHighlightedHitboxes;

	std::vector<SDL_Texture*> mpStandardTextures;
	std::vector<SDL_Texture*> mpHighlightedTextures;
	std::vector<SDL_Texture*> mpDisabledTextures;

	SDL_Color mStandardTextColor;
	SDL_Color mHighlightedTextColor;
	SDL_Color mDisabledTextColor;

	SDL_Color mStandardTextBoxColor;
	SDL_Color mHighlightedTextBoxColor;
	SDL_Color mDisabledTextBoxColor;
	SDL_Color mHighlightedDisabledTextBoxColor;

	SDL_Color mOutlineColor;
	SDL_Color mHighlightedOutlineColor;
	SDL_Color mDisabledOutlineColor;

public:

	TextBox(const TextBoxPreset preset, ETextBoxFunction textBoxFunction, const UIPositionInfo positionInfo,
		const char* fileName, const TextBoxSizeInfo sizeInfo, const TextBoxColorInfo colorInfo);

	~TextBox();

	void updateMessage(SDL_Renderer* pRenderer, FontSizeChart& fontSizeChart, const std::string textMessage);

	void calcMaxFontSizeGivenText(FontSizeChart& fontSizeChart);

	int getMaxFontSizeGivenText() const;

	

	void updateTexture(SDL_Renderer* pRenderer);

	void setTexture(SDL_Renderer* pRenderer) override;


	bool getIsHighlighted() const;

	void changeState(ETextBoxState state);

	void changeIsHighlighted(bool highlighted);

	void changeIsDisabled(bool disabled);


	std::vector<SDL_Texture*> getTextBoxTexture() const;

	SDL_Point getTextRenderSize(int line) const;

	SDL_Color getTextBoxColor() const;

	SDL_Color getTextColor() const;

	SDL_Color getOutlineColor() const;


	void updateHitboxes();

	void updatePosFromBlockSpace(const Hitbox& blockSpace) override;

private:
	Hitbox& getCurHitboxForState();

	std::vector<Hitbox>& getCurLineHitboxesForState();

	std::vector<SDL_Texture*>& getTexturesForState();

	void updateTextLines(const std::string text, FontSizeChart& fontSizeChart);

	void updateHitboxesInternal(bool isHighlighted, Hitbox& hitbox, std::vector<Hitbox>& hitboxes);

	SDL_Point getTextRenderSize(int line, bool isHighlighted) const;
};

class ImageBox : public UIBox
{
public:
	int						mRotation		= 0;
	ImageObject				mImageObject;

	ImageBox(const ImageBoxPreset preset, const UIPositionInfo positionInfo, const std::string fileName);

	~ImageBox() { ; }

	void updatePosFromBlockSpace(const Hitbox& blockSpace) override;

	void setTexture(SDL_Renderer* pRenderer) override;
};

class ShapeBox : public UIBox
{
public:
	EShapeBoxClass  mShapeType = EShapeBoxClass_INVALID;
	SDL_Color	    mColor;

	ShapeBox(const ShapeBoxPreset preset, const UIPositionInfo positionInfo, const SDL_Color color);

	void updatePosFromBlockSpace(const Hitbox& blockSpace) override;

	void setTexture(SDL_Renderer* pRenderer) override { ; }
};

class HealthBox : public UIBox
{
public:

	ShapeBox mBoundingBox;
	ShapeBox mHealthLeftBox;
	TextBox	 mHealthText;

	HealthBox(const HealthBoxPreset preset, const UIPositionInfo positionInfo, const char* font, int textSize, const SDL_Color healthColor, const SDL_Color backgroundColor, const SDL_Color textColor);

	void updatePosFromBlockSpace(const Hitbox& blockSpace) override;

	void updateMessage(SDL_Renderer* pRenderer, FontSizeChart& fontSizeChart, const std::string updatedMessage, float curRatio);

	void updateRatio(float curRatio);

	void setTexture(SDL_Renderer* pRenderer) override { ; }
};


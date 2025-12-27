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
	ETextBoxFunction		mFunction			= ETextBoxFunction_INVALID;
	std::string				mMessage			= "invalid";

	Hitbox mStandardHitbox						= Hitbox();
	Hitbox mHighlightedHitbox					= Hitbox();

	const char* mFontFile;
	TTF_Font*	mpStandardFont					= nullptr;
	TTF_Font*	mpHighlightedFont				= nullptr;
	int			mStandardFontSize				= -1;
	int			mHighlightedFontSize			= -1;
	int			mMaxFontSizeGivenText			= FontSizeChart::mMinFontSize;

	std::vector <std::string> mTextLines;

	int mMaxWidth								= -1;
	int mMaxHeight								= -1;

	std::vector<SDL_Texture*> mpStandardTextures;
	std::vector<SDL_Texture*> mpHighlightedTextures;
	std::vector<SDL_Texture*>* mpCurTextures	= nullptr;

	std::vector<Hitbox>* mpCurLineHitboxes		= nullptr;
	std::vector<Hitbox> mCurLineStandardHitboxes;
	std::vector<Hitbox> mCurLineHighlightedHitboxes;

	int		  mOutlineWidth						= 0;
	SDL_Color mStandardTextColor;
	SDL_Color mHighlightedTextColor;
	SDL_Color mStandardTextBoxColor;
	SDL_Color mHighlightedTextBoxColor;
	SDL_Color mOutlineColor;
	SDL_Color mHighlightedOutlineColor;

	bool mSetUp = false;

	TextBox(const TextBoxPreset preset, ETextBoxFunction textBoxFunction, const TextBoxPositionInfo positionInfo,
		const char* fileName, const TextBoxSizeInfo sizeInfo, const TextBoxColorInfo colorInfo);

	~TextBox();

	void updateMessage(SDL_Renderer* pRenderer, FontSizeChart& fontSizeChart, const std::string textMessage);

	void calcMaxFontSizeGivenText(FontSizeChart& fontSizeChart);

	int getMaxFontSizeGivenText() const;

	

	void updateTexture(SDL_Renderer* pRenderer);

	void setTexture(SDL_Renderer* pRenderer) override;

	bool getIsHighlighted() const;

	void changeIsHighlighted(bool isHighlighted);

	std::vector<SDL_Texture*> getTextBoxTexture() const;

	SDL_Point getTextRenderSize(int line) const;

	SDL_Color getTextBoxColor() const;

	void updateHitboxes();

	void updatePosFromBlockSpace(const Hitbox& blockSpace) override;

private:
	bool mIsHighlighted = false;
	

	void updateTextLines(const std::string text, FontSizeChart& fontSizeChart);

	void updateHitboxesInternal(bool isHighlighted, Hitbox& hitbox, std::vector<Hitbox>& hitboxes);

	SDL_Point getTextRenderSize(int line, bool isHighlighted) const;
};

class ImageBox : public UIBox
{
public:
	int						mRotation		= 0;
	ETextBoxPositionAlign	mPositionAlign	= ETextBoxPositionAlign_INVALID;
	ImageObject				mImageObject;

	ImageBox(const ImageBoxPreset preset, const ImageBoxPositionInfo positionInfo, const std::string fileName);

	~ImageBox() { ; }

	void updatePosFromBlockSpace(const Hitbox& blockSpace) override;

	void setTexture(SDL_Renderer* pRenderer) override;
};

class ShapeBox : public UIBox
{
public:
	EShapeBoxClass  mShapeType = EShapeBoxClass_INVALID;
	SDL_Color	    mColor;

	ShapeBox(const ShapeBoxPreset preset, const TextBoxPositionInfo positionInfo, const SDL_Color color);

	void updatePosFromBlockSpace(const Hitbox& blockSpace) override;

	void setTexture(SDL_Renderer* pRenderer) override { ; }
};

class HealthBox : public UIBox
{
public:

	ShapeBox mBoundingBox;
	ShapeBox mHealthLeftBox;
	TextBox	 mHealthText;

	HealthBox(const HealthBoxPreset preset, const TextBoxPositionInfo positionInfo, const char* font, int textSize, const SDL_Color healthColor, const SDL_Color backgroundColor, const SDL_Color textColor);

	void updatePosFromBlockSpace(const Hitbox& blockSpace) override;

	void updateMessage(SDL_Renderer* pRenderer, FontSizeChart& fontSizeChart, const std::string updatedMessage, float curRatio);

	void setTexture(SDL_Renderer* pRenderer) override { ; }

private:
	int mMaxWidth;
};


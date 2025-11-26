#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <string>
#include <vector>
#include <map>
#include "hitboxHelper.h"
#include "textBoxPresets.h"
#include "imageHelper.h"
#include "textboxHelperClass.h"


enum EUIBoxClass
{
	EUIBoxClass_INVALID = -1,
	EUIBoxClass_TEXTBOX,
	EUIBoxClass_IMAGEBOX,
	EUIBoxClass_SHAPEBOX,
	EUIBoxClass_HEALTHBOX,
	EUIBoxClass_MAX
};


//TODO 1. for scaling text size, have a percentage, and simply force all hitboxes to scale by that amount. Keep margins as is. 
// 2. Then to fix gross text scaling, use font size chart to find an appropiate text size

class UIBox
{
public:
	bool					mAutoShow		= true;
	bool					mShow			= true;
	ETextBoxPositionAlign	mPositionAlign	= ETextBoxPositionAlign_CENTER; //only affects x coord
	Hitbox*					mpCurHitbox		= nullptr;
	Edges					mMargins		= Edges();
	EUIBoxClass				mClassType		= EUIBoxClass_INVALID;

	UIBox() { ; }
	virtual ~UIBox();

	virtual void shiftHitbox(Vect2 shiftTopLeft) = 0;
};

class TextBox : public UIBox
{
public:
	ETextBoxType          mType					= ETextBoxType_INVALID;
	ETextBoxFunction      mFunction				= ETextBoxFunction_INVALID;

	Hitbox mStandardHitbox						= Hitbox();
	Hitbox mHighlightedHitbox					= Hitbox();

	const char* mFontFile;
	TTF_Font *	mpStandardFont					= nullptr;
	TTF_Font *	mpHighlightedFont				= nullptr;
	int			mStandardFontSize				= -1;
	int			mHighlightedFontSize			= -1;
	int			mMaxFontSizeGivenText			= FontSizeChart::mMinFontSize;

	std::vector <std::string> mTextLines;
	ETextBoxTextAlign		  mTextAlign		= ETextBoxTextAlign_INVALID;

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

	TextBoxPreset mDataStorage;

	bool mSetUp = false;

	TextBox(TextBoxPreset dataStorage, ETextBoxFunction textBoxFunction, TextBoxPositionInfo positionInfo,
		const char* fileName, TextBoxSizeInfo sizeInfo, TextBoxColorInfo colorInfo);

	~TextBox();

	void updateMessage(SDL_Renderer* pRenderer, FontSizeChart& fontSizeChart, std::string textMessage);

	void calcMaxFontSizeGivenText(FontSizeChart& fontSizeChart);

	int getMaxFontSizeGivenText() const;

	

	void updateTexture(SDL_Renderer* pRenderer);

	void setTextBoxTexture(SDL_Renderer* pRenderer);

	bool getIsHighlighted() const;

	void changeIsHighlighted(bool isHighlighted);

	std::vector<SDL_Texture*> getTextBoxTexture() const;

	SDL_Point getTextRenderSize(int line) const;

	SDL_Color getTextBoxColor() const;

	void updateHitboxes();

	void shiftHitbox(Vect2 shiftTopLeft) override;

private:
	bool mIsHighlighted = false;
	

	void updateTextLines(std::string text, FontSizeChart& fontSizeChart);

	void updateHitboxesInternal(bool isHighlighted, Hitbox& hitbox, std::vector<Hitbox>& hitboxes);

	SDL_Point getTextRenderSize(int line, bool isHighlighted) const;
};

class ImageBox : public UIBox
{
public:
	int						mRotation		= 0;
	ETextBoxPositionAlign	mPositionAlign	= ETextBoxPositionAlign_INVALID;
	ImageObject				mImageObject;
	ETextBoxID				mID				= ETextBoxID_INVALID;

	ImageBox(ImageBoxPreset preset, ImageBoxPositionInfo positionInfo, std::string fileName);

	~ImageBox() { ; }

	void shiftHitbox(Vect2 shiftTopLeft) override;
};

class ShapeBox : public UIBox
{
public:
	EShapeBoxClass mShapeType = EShapeBoxClass_INVALID;
	SDL_Color	   mColor;
	ShapeBoxPreset mDataStorage;

	ShapeBox(ShapeBoxPreset preset, TextBoxPositionInfo positionInfo, SDL_Color color);

	void shiftHitbox(Vect2 shiftTopLeft) override;
};

class HealthBox : public UIBox
{
public:
	ShapeBox mBoundingBox;
	ShapeBox mHealthLeftBox;
	TextBox	 mHealthText;
	int		 mCombatCharacterIndex = -1;

	HealthBox(HealthBoxPreset preset, TextBoxPositionInfo positionInfo, const char* font, int healthSize, SDL_Color healthColor, 
			SDL_Color backgroundColor, SDL_Color textColor);

	void shiftHitbox(Vect2 shiftTopLeft) override;

	void updateMessage(SDL_Renderer* pRenderer, FontSizeChart& fontSizeChart, std::string updatedMessage, float curRatio);

private:
	int mMaxWidth;
};

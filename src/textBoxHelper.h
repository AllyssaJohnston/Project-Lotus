#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <string>
#include <vector>
#include "hitboxHelper.h"
#include "textBoxPresets.h"
#include "imageHelper.h"

enum EUIBoxClass
{
	EUIBoxClass_INVALID = -1,
	EUIBoxClass_TEXTBOX,
	EUIBoxClass_IMAGEBOX,
	EUIBoxClass_MAX
};

class UIBox
{
public:
	ETextBoxPositionAlign	mPositionAlign = ETextBoxPositionAlign_CENTER; //only affects x coord
	Hitbox					mHitbox		 = Hitbox();
	Edges					mMargins		 = Edges();
	EUIBoxClass				mClassType   = EUIBoxClass_INVALID;

	UIBox(){;}

	virtual void shiftHitbox(Vect2 shiftTopLeft){;}
};

class TextBox : public UIBox
{
public:
	ETextBoxType          mType				= ETextBoxType_INVALID;
	ETextBoxFunction      mFunction			= ETextBoxFunction_INVALID;

	const char* mFileName;
	TTF_Font *	mpStandardFont;
	TTF_Font *	mpHighlightedFont;
	int			mStandardFontSize			= -1;
	int			mSelectedFontSize			= -1;
	float		mStandardFontX2YRatio		= .5f;
	int			mMaxFontSizeGivenText		= 0;
	int			mMaxFontSizeGivenBoxGroup	= 0;

	std::string				  mFullString;
	std::vector <std::string> mTextLines;
	ETextBoxTextAlign		  mTextAlign	= ETextBoxTextAlign_INVALID;

	int mMaxWidth							= -1;
	int mMaxHeight							= -1;

	std::vector<SDL_Texture*> mpStandardTextures;
	std::vector<SDL_Texture*> mpHighlightedTextures;
	std::vector<SDL_Texture*> mpCurTextures;

	std::vector<Hitbox> mCurLineHitboxes;

	bool	  mIsHighlighted				= false;
	SDL_Color mStandardTextColor;
	SDL_Color mHighlightedTextColor;
	SDL_Color mStandardTextBoxColor;
	SDL_Color mHighlightedTextBoxColor;

	TextBoxPreset mDataStorage;

	bool mSetUp								= false;

	TextBox(TextBoxPreset dataStorage, ETextBoxFunction textBoxFunction, TextBoxPositionInfo positionInfo, 
			const char* fileName, int standardSize, int selectedSize, TextBoxColorInfo colorInfo);

	~TextBox();

	void updateMessage(SDL_Renderer* pRenderer, std::string textMessage, int textIncrease);

	void calcMaxFontSizeGivenText();

	int getMaxFontSizeGivenText();

	void setBoxGroupMaxFontSize(int groupMaxFontSize);

	void updateTextLines(std::string text, int textIncrease);

	void updateTexture(SDL_Renderer* pRenderer, int textIncrease);
	void updateTexture(SDL_Renderer* pRenderer);

	void setTextBoxTexture(SDL_Renderer* pRenderer, int textIncrease);

	void changeIsHighlighted(bool isHighlighted);

	std::vector<SDL_Texture*> getTextBoxTexture();

	SDL_Point getTextRenderSize(int line) const;

	SDL_Color getTextBoxColor() const;

	void updateHitbox();

	void shiftHitbox(Vect2 shiftTopLeft);

};

class ImageBox : public UIBox
{
public:
	int						mRotation		= 0;
	ETextBoxPositionAlign	mPositionAlign	= ETextBoxPositionAlign_INVALID;
	ImageObject				mImageObject;
	bool					mShow			= false;
	ETextBoxID				mID				= ETextBoxID_INVALID;

	ImageBox(ImageBoxPreset preset, ImageBoxPositionInfo positionInfo, std::string fileName);

	~ImageBox(){};

	void shiftHitbox(Vect2 shiftTopLeft);

	void updateHitbox(float ratio);
	
};

class UIBlock
{
public:
	std::vector<UIBlock*> mpSubBlocks;
	std::vector<UIBox*>   mpCurBoxes;
	std::string mName;
	bool   mIsHeadBlock						= false;
	Hitbox mHitbox;
	Vect2  mStartingPositionCenter;
	Edges  mMargins;
	ETextBoxPositionAlign mPositionAlign	= ETextBoxPositionAlign_INVALID;
	EDirection mGrowthDirection				= EDirection_INVALID;
	//for head block
	EDirection mGrowthDirectionHorizontal	= EDirection_INVALID;
	EDirection mGrowthDirectionVertical		= EDirection_INVALID;

	~UIBlock();

	virtual void adjustBlocksWidthHeight(){}

	virtual void moveBoxes(int marginIncrease){}

	virtual void moveSubBlocks(int marginIncrease){}

	void updateBlocks(int marginIncrease);

	void setMaxForBoxes();

	int calcMaxFontSizeForBlock();

	void setMaxFontSizeForBlock(int maxTextSize);

};

class BlockAlignElementsVertically : public UIBlock
{
public:
	//HEAD BLOCK
	BlockAlignElementsVertically(Hitbox hitbox, EDirection directionH, EDirection directionV, Edges margins);
	BlockAlignElementsVertically(Hitbox hitbox, EDirection directionH, EDirection directionV, Edges margins, std::string name);

	void constructHeadBlock(Hitbox hitbox, EDirection directionH, EDirection directionV, Edges margins);
	
	//SUB BLOCKS
	BlockAlignElementsVertically(int maxWidth, int maxHeight, ETextBoxPositionAlign positionAlign, EDirection direction, Edges margins);
	BlockAlignElementsVertically(int maxWidth, int maxHeight, ETextBoxPositionAlign positionAlign, EDirection direction, Edges margins, 
			std::string name);

	void constructBlock(Hitbox hitbox, ETextBoxPositionAlign positionAlign, EDirection direction, Edges margins);
	
	//TODO cut off textboxes after max dimensiosn
	void adjustBlocksWidthHeight();

	void moveBoxes(int marginIncrease);

	void moveSubBlocks(int marginIncrease);
};

class BlockAlignElementsHorizontally : public UIBlock
{
public:
	//MASTER BLOCK
	BlockAlignElementsHorizontally(Hitbox hitbox, EDirection directionH, EDirection directionV, Edges margins);
	BlockAlignElementsHorizontally(Hitbox hitbox, EDirection directionH, EDirection directionV, Edges margins, std::string name);

	void constructHeadBlock(Hitbox hitbox, EDirection directionH, EDirection directionV, Edges margins);

	//SUB BLOCKS
	BlockAlignElementsHorizontally(int maxWidth, int maxHeight, ETextBoxPositionAlign positionAlign, EDirection direction, Edges margins);
	BlockAlignElementsHorizontally(int maxWidth, int maxHeight, ETextBoxPositionAlign positionAlign, EDirection direction, Edges margins, 
			std::string name);

	void constructBlock(Hitbox hitbox, ETextBoxPositionAlign positionAlign, EDirection direction, Edges margins);

	//TODO cut after max
	void adjustBlocksWidthHeight();

	void moveBoxes(int marginIncrease);

	void moveSubBlocks(int marginIncrease);
};




/*
class BlockAlignElementsHorizontally : public UIBlock
{
public:
	//MASTER BLOCK
	BlockAlignElementsHorizontally(Hitbox hitbox, EDirection directionH, EDirection directionV, Edges margins)
	{
		constructHeadBlock(hitbox, directionH, directionV, margins);
	}
	BlockAlignElementsHorizontally(Hitbox hitbox, EDirection directionH, EDirection directionV, Edges margins, std::string name)
	{
		constructHeadBlock(hitbox, directionH, directionV, margins);
		mName = name;
	}

	void constructHeadBlock(Hitbox hitbox, EDirection directionH, EDirection directionV, Edges margins)
	{
		mIsHeadBlock = true;
		mGrowthDirectionHorizontal = directionH;
		mGrowthDirectionVertical   = directionV;
		constructBlock(hitbox, ETextBoxPositionAlign_LEFT, directionH, margins);
	}

	//SUB BLOCKS
	BlockAlignElementsHorizontally(int maxWidth, int maxHeight, ETextBoxPositionAlign positionAlign, EDirection direction, Edges margins)
	{
		constructBlock(Hitbox(0, maxWidth, 0, maxHeight), positionAlign, direction, margins);
	}
	BlockAlignElementsHorizontally(int maxWidth, int maxHeight, ETextBoxPositionAlign positionAlign, EDirection direction, Edges margins, std::string name)
	{
		constructBlock(Hitbox(0, maxWidth, 0, maxHeight), positionAlign, direction, margins);
		mName = name;
	}

	void constructBlock(Hitbox hitbox, ETextBoxPositionAlign positionAlign, EDirection direction, Edges margins)
	{
		mHitbox = hitbox;
		mPositionAlign = positionAlign;
		if (positionAlign == ETextBoxPositionAlign_CENTER)
		{
			mStartingPositionCenter = Vect2(hitbox.getTopLeft().getX(), hitbox.getCenter().getY());
		}
		else if (positionAlign == ETextBoxPositionAlign_LEFT)
		{
			mStartingPositionCenter = Vect2(hitbox.getTopLeft().getX(), hitbox.getTopLeft().getY());
		}
		if ((direction != EDirection_LEFT) and (direction != EDirection_RIGHT))
		{
			SDL_assert(false);
		}
		mGrowthDirection = direction;
		mMargins = margins;
	}

	//TODO cut after max
	void adjustBlocksWidthHeight()
	{
		if (mpSubBlocks.size() > 0)
		{
			int maxHeight = 0;
			int width = 0;
			for (int count = 0; count < mpSubBlocks.size(); count++)
			{
				mpSubBlocks[count]->adjustBlocksWidthHeight();
				UIBlock* pCurBlock = mpSubBlocks[count];
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

			for (int count = 0; count < mpCurBoxes.size(); count++)
			{
				UIBox* pCurTextBox = mpCurBoxes[count];
				maxHeight = std::max(pCurTextBox->mHitbox.getHeight(), maxHeight);
				width += pCurTextBox->mHitbox.getWidth();
			} */
			//width represents just widths of textboxes
			//initialWidth represents widths of the block without imageboxes

			//float changeWidth = float(width) / float(initialWidth);

			/*for (int count = 0; count < mpCurBoxes.size(); count++)
			{
				UIBox* pCurTextBox = mpCurBoxes[count];
				if (pCurTextBox->mClassType == EUIBoxClass_IMAGEBOX)
				{
					((ImageBox*)pCurTextBox)->updateHitbox(changeWidth);
					width += pCurTextBox->mHitbox.getWidth();
				}
			}*/
			//imageboxes have been scaled according to how much the textboxes scaled
			//width represents width of all boxes
			/*
			int updatedX = 0;
			int updatedY = 0;
			if (mPositionAlign == ETextBoxPositionAlign_CENTER)
			{
				updatedX = mHitbox.getCenter().getX() - (width / 2);
				updatedY = mHitbox.getCenter().getY() - (maxHeight / 2);
			}
			else if (mPositionAlign == ETextBoxPositionAlign_LEFT)
			{
				updatedX = mHitbox.getTopLeft().getX();
				updatedY = mHitbox.getTopLeft().getY();
			}
			mHitbox.setWidth(width);
			mHitbox.setHeight(maxHeight);
			mHitbox.setTopLeft(Vect2(updatedX, updatedY));
		}
	}

	void moveBoxes(int marginIncrease)
	{
		//BOXES
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

	void moveSubBlocks(int marginIncrease)
	{
		if (mpSubBlocks.size() > 0)
		{
			//BLOCKS
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
};
*/
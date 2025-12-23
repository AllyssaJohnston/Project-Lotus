#pragma once
#include "uiBox.h"

class UIBlock
{
public:
	std::vector<UIBlock*> mpSubBlocks;
	std::vector<UIBox*> mpAllBoxes;
	std::string mName;
	bool   mIsHeadBlock						= false;
	Hitbox mHitbox;
	bool   mFillWidth;
	bool   mFillHeight;
	int	   mMaxWidth						= 1;
	int	   mMaxHeight						= 1;
	Vect2  mStartingPositionCenter;
	Edges  mMargins;
	ETextBoxPositionAlign mPositionAlign	= ETextBoxPositionAlign_INVALID;
	SDL_Color mBackgroundColor				= { 0, 0, 0, 0 };
	EDirection mGrowthDirection				= EDirection_INVALID;
	// for head block
	EDirection mGrowthDirectionHorizontal	= EDirection_INVALID;
	EDirection mGrowthDirectionVertical		= EDirection_INVALID;

	~UIBlock();

	virtual void adjustBlocksWidthHeight() = 0;

	virtual void moveBoxes() = 0;

	virtual void moveSubBlocks() = 0;

	void updateBlocks();

	void setMaxForBoxes();

	void setAllTextures(SDL_Renderer* pRenderer);

	std::vector<UIBox*> getAllBoxes();

	void getAllBlocksInternal(std::vector<UIBlock*>& list);

protected:
	void setAllTexturesInternal(SDL_Renderer* pRenderer);

	void getAllBoxesInternal(std::vector<UIBox*>& list);

	int getIndexOfFirstCurBox();

	int getIndexOfLastCurBox();

};

class BlockAlignElementsVertically : public UIBlock
{
public:
	// HEAD BLOCK
	BlockAlignElementsVertically(Hitbox hitbox, ETextBoxPositionAlign positionAlign, EDirection directionH, EDirection directionV, bool fillWidth, bool fillHeight, 
			Edges margins, SDL_Color backgroundColor = { 0, 0, 0, 0 }, std::string name = "");
	
	// SUB BLOCKS
	BlockAlignElementsVertically(int maxWidth, int maxHeight, ETextBoxPositionAlign positionAlign, EDirection direction, bool fillWidth, bool fillHeight, 
			Edges margins, SDL_Color backgroundColor = { 0, 0, 0, 0 }, std::string name = "");

private:
	void constructBlock(Hitbox hitbox, ETextBoxPositionAlign positionAlign, EDirection direction, bool fillWidth, bool fillHeight, Edges margins, SDL_Color backgroundColor);

	// TODO cut off textboxes after max dimensiosn
	void adjustBlocksWidthHeight();

	void moveBoxes();

	void moveSubBlocks();
};

class BlockAlignElementsHorizontally : public UIBlock
{
public:
	// HEAD BLOCK
	BlockAlignElementsHorizontally(Hitbox hitbox, ETextBoxPositionAlign positionAlign, EDirection directionH, EDirection directionV, bool fillWidth, bool fillHeight, 
			Edges margins, SDL_Color backgroundColor = { 0, 0, 0, 0 }, std::string name = "");

	// SUB BLOCKS
	BlockAlignElementsHorizontally(int maxWidth, int maxHeight, ETextBoxPositionAlign positionAlign, EDirection direction,
			bool fillWidth, bool fillHeight, Edges margins, SDL_Color backgroundColor = { 0, 0, 0, 0 }, std::string name = "");

private:
	void constructBlock(Hitbox hitbox, ETextBoxPositionAlign positionAlign, EDirection direction, bool fillWidth, bool fillHeight, Edges margins, SDL_Color backgroundColor);

	// TODO cut after max
	void adjustBlocksWidthHeight();

	void moveBoxes();

	void moveSubBlocks();
};


class BlockAlignElementsGrid : public UIBlock
{
public:
	// HEAD BLOCK
	BlockAlignElementsGrid(Hitbox hitbox, ETextBoxPositionAlign positionAlign, bool limitByRows, int limit, bool fillWidth, bool fillHeight, Edges margins, 
			SDL_Color backgroundColor = { 0, 0, 0, 0 }, std::string name = "");

	// SUB BLOCKS
	BlockAlignElementsGrid(int maxWidth, int maxHeight, ETextBoxPositionAlign positionAlign, bool limitByRows, int limit, bool fillWidth, bool fillHeight, Edges margins,
		SDL_Color backgroundColor = { 0, 0, 0, 0 }, std::string name = "");

private:
	bool mLimitByRows;
	int mLimit = -1;
	int mNumRows = -1;
	int mNumCols = -1;

	std::vector<int> mRowHeights;
	std::vector<int> mColWidths;

	void constructBlock(Hitbox hitbox, ETextBoxPositionAlign positionAlign, bool limitByRows, int limit, bool fillWidth, bool fillHeight, Edges margins, SDL_Color backgroundColor);

	// TODO cut after max
	void adjustBlocksWidthHeight();

	void moveBoxes();

	void moveSubBlocks();

	void updateNumRowsCols();
};
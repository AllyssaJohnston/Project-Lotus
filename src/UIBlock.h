#pragma once
#include "uiBox.h"
#include "uiElement.h"

class UIBlock : public UIElement
{
public:
	std::string mName = "";
	std::vector<UIElement*>					mpSubElems; // more blocks or ui boxes
	bool   mIsHeadBlock						= false;
	Hitbox mHitbox;										// for entire block
	bool   mFillWidth						= false;
	bool   mFillHeight						= false;
	int	   mMaxWidth						= 1;
	int	   mMaxHeight						= 1;
	Vect2  mStartingPositionCenter;
	ETextBoxPositionAlign mPositionAlign	= ETextBoxPositionAlign_INVALID;
	SDL_Color mBackgroundColor				= { 0, 0, 0, 0 };
	EDirection mGrowthDirection				= EDirection_INVALID;
	// for head block
	EDirection mGrowthDirectionHorizontal	= EDirection_INVALID;
	EDirection mGrowthDirectionVertical		= EDirection_INVALID;
	
	~UIBlock();

	Hitbox& getHitbox() override;

	virtual void adjustBlocksWidthHeight() = 0;

	virtual void moveElems() = 0;

	void updateBlocks();

	void setMaxSize();

	bool isActive() override;

	void setTexture(SDL_Renderer* pRenderer) override;

	std::vector<UIBox*> getAllBoxes();

	std::vector<UIElement*> getAllElems();

	void getAllElems(std::vector<UIElement*>& list);

protected:

	void getAllBoxesInternal(std::vector<UIBox*>& list);

	void getAllElemsInternal(std::vector<UIElement*>& list);

	int getIndexOfFirstActiveElem();

	int getIndexOfLastActiveElem();

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
	void adjustBlocksWidthHeight() override;

	void moveElems() override;
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
	void adjustBlocksWidthHeight() override;

	void moveElems() override;
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
	void adjustBlocksWidthHeight() override;

	void moveElems() override;

	void updateNumRowsCols();
};
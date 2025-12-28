#pragma once
#include "uiBox.h"
#include "uiElement.h"

class UIBlock : public UIElement
{
public:
	std::string mName = "";
	std::vector<UIElement*>					mpSubElems; // more blocks or ui boxes
	Hitbox mHitbox;										// for entire block
	
	SDL_Color mBackgroundColor				= { 0, 0, 0, 0 };
	

private:
	bool mLimitByRows;
	int  mLimit = -1;
	int  mNumRows = -1;
	int  mNumCols = -1;
	std::vector<int> mRowHeights;
	std::vector<int> mColWidths;

	int  mSpacing = 0;

	bool   mFillWidth = false;
	bool   mFillHeight = false;

	Vect2  mAnchorPoint;

	EDirection mGrowthDirectionHorizontal = EDirection_INVALID;
	EDirection mGrowthDirectionVertical = EDirection_INVALID;

public:
	// HEAD BLOCK
	UIBlock(Hitbox hitbox, EUIPositionAlign positionAlignH, EUIPositionAlign positionAlignV, EDirection growthDirectionH, EDirection growthDirectionV,
		bool limitByRows, int limit, bool fillWidth, bool fillHeight, Edges margins, int spacing, SDL_Color backgroundColor = { 0, 0, 0, 0 }, std::string name = "");

	// SUB BLOCKS
	UIBlock(int maxWidth, int maxHeight, EUIPositionAlign positionAlignH, EUIPositionAlign positionAlignV, EDirection growthDirectionH, EDirection growthDirectionV,
		bool limitByRows, int limit, bool fillWidth, bool fillHeight, Edges margins, int spacing, SDL_Color backgroundColor = { 0, 0, 0, 0 }, std::string name = "");

	~UIBlock();

	Hitbox& getHitbox() override;

	void adjustBlocksWidthHeight();

	void moveElems();

	void updateBlocks();

	void setMaxSize();

	bool isActive() override;

	void setTexture(SDL_Renderer* pRenderer) override;

	std::vector<UIBox*> getAllBoxes();

	std::vector<UIElement*> getAllElems();

	void getAllElems(std::vector<UIElement*>& list);

private:
	void constructBlock(Hitbox hitbox, EUIPositionAlign positionAlignH, EUIPositionAlign positionAlignV, EDirection growthDirectionH, EDirection growthDirectionV,
		bool limitByRows, int limit, bool fillWidth, bool fillHeight, Edges margins, int spacing, SDL_Color backgroundColor);

	void updatePosFromBlockSpace(const Hitbox& blockSpace) override;

	void getAllBoxesInternal(std::vector<UIBox*>& list);

	void getAllElemsInternal(std::vector<UIElement*>& list);

	int getIndexOfFirstActiveElem();

	int getIndexOfLastActiveElem();

	void updateNumRowsCols();
};
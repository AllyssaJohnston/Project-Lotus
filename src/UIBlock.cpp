#include "UIBlock.h"

UIBlock::~UIBlock()
{
	for (UIElement* pElem : mpSubElems)
	{
		if (pElem->mClassType == EUIClass_BLOCK)
		{
			((UIBlock*)pElem)->~UIBlock();
			delete pElem;
			pElem = nullptr;
		}
		else // BOX
		{
			pElem = nullptr;
		}
	}
	mpSubElems.clear();
}

Hitbox& UIBlock::getHitbox() { return mHitbox; }

// only called on the head blocks in menu page
void UIBlock::updateBlocks()
{
	// figure out all block widths and heights
	adjustBlocksWidthHeight();

	int x = 0;
	int y = 0;

	switch (mGrowthDirectionHorizontal)
	{
	case EDirection_RIGHT:
		x = mStartingPositionCenter.getX() + mMargins.mLeft;
		break;
	case EDirection_LEFT:
		x = mStartingPositionCenter.getX() - mHitbox.getWidth() - mMargins.mRight;
		break;
	case EDirection_LEFT_AND_RIGHT:
		x = mStartingPositionCenter.getX() - mHitbox.getWidth() /2;
		break;
	default:
		SDL_assert(false);
		break;
	}

	switch (mGrowthDirectionVertical)
	{
	case EDirection_DOWN:
		y = mStartingPositionCenter.getY() + mMargins.mTop;
		break;
	case EDirection_UP:
		y = mStartingPositionCenter.getY() - mHitbox.getHeight() - mMargins.mBottom;
		break;
	case EDirection_UP_AND_DOWN:
		y = mStartingPositionCenter.getY() - mHitbox.getHeight() / 2;
		break;
	default:
		SDL_assert(false);
		break;
	}

	mHitbox.setTopLeft(Vect2(x, y));

	// figure out block locations
	moveElems();
}

void UIBlock::setMaxSize()
{
	for (UIElement* pElem : mpSubElems)
	{
		if (pElem->mClassType == EUIClass_BLOCK)
		{
			((UIBlock*)pElem)->setMaxSize();
		}
		if (pElem->isActive())
		{
			if (pElem->getHitbox().getWidth() > mHitbox.getWidth())
			{
				pElem->getHitbox().setWidth(mHitbox.getWidth());
			}

			if (pElem->getHitbox().getHeight() > mHitbox.getHeight())
			{
				pElem->getHitbox().setHeight(mHitbox.getHeight());
			}
		}
		
	}
}

bool UIBlock::isActive() { return mHitbox.getWidth() > 0 || mHitbox.getHeight() > 0; }

void UIBlock::constructBlock(Hitbox hitbox, ETextBoxPositionAlign positionAlignH, ETextBoxPositionAlign positionAlignV, EDirection direction, bool fillWidth, bool fillHeight, Edges margins, SDL_Color backgroundColor)
{
	mClassType = EUIClass_BLOCK;
	mHitbox = hitbox;
	mMaxWidth = hitbox.getWidth();
	mMaxHeight = hitbox.getHeight();
	mPositionAlignH = positionAlignH;
	mPositionAlignV = positionAlignV;
	int startX;
	int startY;
	switch (mPositionAlignH)
	{
	case ETextBoxPositionAlign_CENTER:
		startX = hitbox.getCenter().getX();
		break;
	case ETextBoxPositionAlign_LEFT:
		startX = hitbox.getTopLeft().getX();
		break;
	default:
		SDL_assert(false);
		break;
	}
	switch (mPositionAlignV)
	{
	case ETextBoxPositionAlign_CENTER:
		startY = hitbox.getCenter().getY();
		break;
	case ETextBoxPositionAlign_TOP:
		startY = hitbox.getTopLeft().getY();
		break;
	default:
		SDL_assert(false);
		break;
	}
	mStartingPositionCenter = Vect2(startX, startY);

	mGrowthDirection = direction;
	mMargins = margins;
	mBackgroundColor = backgroundColor;
	mFillWidth = fillWidth;
	mFillHeight = fillHeight;
}

void UIBlock::updatePosFromBlockSpace(const Hitbox& blockSpace) { mHitbox.setTopLeft(getUpdatedPosFromBlockSpace(blockSpace)); }

void UIBlock::setTexture(SDL_Renderer* pRenderer)
{
	for (UIElement* pElem : mpSubElems)
	{
		pElem->setTexture(pRenderer);
	}
}

std::vector<UIBox*> UIBlock::getAllBoxes()
{
	std::vector<UIBox*> list;
	getAllBoxesInternal(list);
	return list;
}

std::vector<UIElement*> UIBlock::getAllElems()
{
	std::vector<UIElement*> list;
	getAllElemsInternal(list);
	return list;
}

void UIBlock::getAllElems(std::vector<UIElement*>& list) { getAllElemsInternal(list); }

void UIBlock::getAllBoxesInternal(std::vector<UIBox*>& list) 
{
	for (UIElement* pElem : mpSubElems)
	{
		if (pElem->mClassType == EUIClass_BLOCK)
		{
			((UIBlock*)pElem)->getAllBoxesInternal(list);
		}
		else // BOX
		{
			list.push_back((UIBox*)pElem);
		}
	}
}

void UIBlock::getAllElemsInternal(std::vector<UIElement*>& list)
{
	list.push_back(this);
	for (UIElement* pElem : mpSubElems)
	{
		if (pElem->mClassType == EUIClass_BLOCK)
		{
			((UIBlock*)pElem)->getAllElemsInternal(list);
		}
		else // BOX
		{
			list.push_back((UIBox*)pElem);
		}
	}
}

int UIBlock::getIndexOfFirstActiveElem()
{
	for (int i = 0; i < (int)mpSubElems.size(); i++) 
	{
		if (mpSubElems[i]->isActive())
		{
			return i;
		}
	}
	return -1;
}

int UIBlock::getIndexOfLastActiveElem()
{
	for (int i = (int)mpSubElems.size() - 1; i > - 1; i--)
	{
		if (mpSubElems[i]->isActive())
		{
			return i;
		}
	}
	return -1;
}



// HEAD BLOCK
BlockAlignElementsVertically::BlockAlignElementsVertically(Hitbox hitbox, ETextBoxPositionAlign positionAlignH, ETextBoxPositionAlign positionAlignV, EDirection directionH, EDirection directionV,
		bool fillWidth, bool fillHeight, Edges margins, SDL_Color backgroundColor, std::string name) 
{
	mIsHeadBlock = true;
	mGrowthDirectionHorizontal = directionH;
	mGrowthDirectionVertical = directionV;
	constructBlock(hitbox, positionAlignH, positionAlignV, directionV, fillWidth, fillHeight, margins, backgroundColor);
	if ((directionH != EDirection_LEFT) and (directionH != EDirection_RIGHT) && (directionH != EDirection_LEFT_AND_RIGHT))
	{
		SDL_assert(false);
	}
	if ((directionV != EDirection_UP) and (directionV != EDirection_DOWN) && (directionV != EDirection_UP_AND_DOWN))
	{
		SDL_assert(false);
	}
	mName = name;
}

// SUB BLOCKS
BlockAlignElementsVertically::BlockAlignElementsVertically(int maxWidth, int maxHeight, ETextBoxPositionAlign positionAlignH, ETextBoxPositionAlign positionAlignV,
		EDirection direction, bool fillWidth, bool fillHeight, Edges margins, SDL_Color backgroundColor, std::string name)
{
	constructBlock(Hitbox(0, maxWidth, 0, maxHeight), positionAlignH, positionAlignV, direction, fillWidth, fillHeight, margins, backgroundColor);
	if ((direction != EDirection_UP) and (direction != EDirection_DOWN) && (direction != EDirection_UP_AND_DOWN))
	{
		SDL_assert(false);
	}
	mName = name;
}

void BlockAlignElementsVertically::adjustBlocksWidthHeight()
{
	if (mpSubElems.size() > 0)
	{
		int maxWidth = mFillWidth ? mMaxWidth : 0; // TODO cut after max
		int height = 0;
		for (UIElement* pElem : mpSubElems)
		{
			if (pElem->mClassType == EUIClass_BLOCK)
			{
				((UIBlock*)pElem)->adjustBlocksWidthHeight();
			}

			if (!pElem->isActive())
			{
				continue;
			}

			int curWidth = pElem->getHitbox().getWidth();
			if (curWidth != 0)
			{
				curWidth += pElem->mMargins.mLeft + pElem->mMargins.mRight;
			}
			int curHeight = pElem->getHitbox().getHeight();
			if (curHeight != 0)
			{
				curHeight += pElem->mMargins.mTop + pElem->mMargins.mBottom;
			}

			maxWidth = std::max(curWidth, maxWidth);
			height += curHeight;
		}

		height = mFillHeight ? std::max(mMaxHeight, height) : height;

		mHitbox.setWidth(std::max(maxWidth, 0));
		mHitbox.setHeight(std::max(height, 0));
	}
}

void BlockAlignElementsVertically::moveElems()
{
	if (mGrowthDirection == EDirection_DOWN or mGrowthDirection == EDirection_UP_AND_DOWN)
	{
		// top to bottom
		int i = getIndexOfFirstActiveElem();
		if (i == -1)
		{
			return;
		}
		UIElement* pLastElem = mpSubElems[i];
		
		int x1 = mHitbox.getTopLeft().getX() + pLastElem->mMargins.mLeft;
		int x2 = mHitbox.getBottomRight().getX() - pLastElem->mMargins.mRight;
		int y1 = mHitbox.getTopLeft().getY() + pLastElem->mMargins.mTop;
		int y2 = y1 + pLastElem->getHitbox().getHeight();
		Hitbox blockSpace = Hitbox(x1, x2, y1, y2);
		
		pLastElem->updatePosFromBlockSpace(blockSpace);
		if (pLastElem->mClassType == EUIClass_BLOCK)
		{
			((UIBlock*)pLastElem)->moveElems();
		}

		for (int count = i + 1; count < mpSubElems.size(); count++)
		{
			UIElement* pCurElem = mpSubElems[count];
			if (!pCurElem->isActive())
			{
				continue;
			}

			x1 = mHitbox.getTopLeft().getX() + pCurElem->mMargins.mLeft;
			x2 = mHitbox.getBottomRight().getX() - pCurElem->mMargins.mRight;
			y1 = pLastElem->getHitbox().getBottomRight().getY() + pLastElem->mMargins.mBottom + pCurElem->mMargins.mTop;
			y2 = y1 + pCurElem->getHitbox().getHeight();
			Hitbox blockSpace = Hitbox(x1, x2, y1, y2);

			pCurElem->updatePosFromBlockSpace(blockSpace);
			if (pCurElem->mClassType == EUIClass_BLOCK)
			{
				((UIBlock*)pCurElem)->moveElems();
			}
			
			pLastElem = pCurElem;
		}
	}
	else 
	{
		// Bottom to top
		int i = getIndexOfLastActiveElem();
		if (i == -1)
		{
			return;
		}
		UIElement* pLastElem = mpSubElems[i];
	
		int x1 = mHitbox.getTopLeft().getX() + pLastElem->mMargins.mLeft;
		int x2 = mHitbox.getBottomRight().getX() - pLastElem->mMargins.mRight;
		int y2 = mHitbox.getBottomRight().getY() - pLastElem->mMargins.mBottom;
		int y1 = y2 - pLastElem->getHitbox().getHeight();
		Hitbox blockSpace = Hitbox(x1, x2, y1, y2);

		pLastElem->updatePosFromBlockSpace(blockSpace);
		if (pLastElem->mClassType == EUIClass_BLOCK)
		{
			((UIBlock*)pLastElem)->moveElems();
		}

		for (int count = i - 1; count > -1; count--)
		{
			UIElement* pCurElem = mpSubElems[count];
			if (!pCurElem->isActive())
			{
				continue;
			}
			
			int x1 = mHitbox.getTopLeft().getX() + pCurElem->mMargins.mLeft;
			int x2 = mHitbox.getBottomRight().getX() - pCurElem->mMargins.mRight;
			int y2 = pLastElem->getHitbox().getTopLeft().getY() - pLastElem->mMargins.mTop - pCurElem->mMargins.mBottom;
			int y1 = y2 - pCurElem->getHitbox().getHeight();
			Hitbox blockSpace = Hitbox(x1, x2, y1, y2);

			pCurElem->updatePosFromBlockSpace(blockSpace);
			if (pCurElem->mClassType == EUIClass_BLOCK)
			{
				((UIBlock*)pCurElem)->moveElems();
			}
			
			pLastElem = pCurElem;
		}
	}
}



// HEAD BLOCK
BlockAlignElementsHorizontally::BlockAlignElementsHorizontally(Hitbox hitbox, ETextBoxPositionAlign positionAlignH, ETextBoxPositionAlign positionAlignV, EDirection directionH, EDirection directionV, 
		bool fillWidth, bool fillHeight, Edges margins, SDL_Color backgroundColor, std::string name) 
{
	mIsHeadBlock = true;
	mGrowthDirectionHorizontal = directionH;
	mGrowthDirectionVertical = directionV;
	constructBlock(hitbox, positionAlignH, positionAlignV, directionH, fillWidth, fillHeight, margins, backgroundColor);
	if ((directionH != EDirection_LEFT) and (directionH != EDirection_RIGHT) && (directionH != EDirection_LEFT_AND_RIGHT))
	{
		SDL_assert(false);
	}
	if ((directionV != EDirection_UP) and (directionV != EDirection_DOWN) && (directionV != EDirection_UP_AND_DOWN))
	{
		SDL_assert(false);
	}
	mName = name;
}

// SUB BLOCKS
BlockAlignElementsHorizontally::BlockAlignElementsHorizontally(int maxWidth, int maxHeight, ETextBoxPositionAlign positionAlignH, ETextBoxPositionAlign positionAlignV, 
		EDirection direction, bool fillWidth, bool fillHeight, Edges margins, SDL_Color backgroundColor, std::string name)
{
	constructBlock(Hitbox(0, maxWidth, 0, maxHeight), positionAlignH, positionAlignV, direction, fillWidth, fillHeight, margins, backgroundColor);
	if ((direction != EDirection_LEFT) and (direction != EDirection_RIGHT) && (direction != EDirection_LEFT_AND_RIGHT))
	{
		SDL_assert(false);
	}
	mName = name;
}


void BlockAlignElementsHorizontally::adjustBlocksWidthHeight()
{
	if (mpSubElems.size() > 0)
	{
		int maxHeight = mFillHeight ? mMaxHeight : 0; // TODO cut after max
		int width = 0;
		
		for (UIElement* pElem : mpSubElems)
		{
			if (pElem->mClassType == EUIClass_BLOCK)
			{
				((UIBlock*)pElem)->adjustBlocksWidthHeight();
			}

			if (!pElem->isActive())
			{
				continue;
			}

			int curWidth = pElem->getHitbox().getWidth();
			if (curWidth != 0)
			{
				curWidth += pElem->mMargins.mLeft + pElem->mMargins.mRight;
			}
			int curHeight = pElem->getHitbox().getHeight();
			if (curHeight != 0)
			{
				curHeight += pElem->mMargins.mTop + pElem->mMargins.mBottom;
			}


			maxHeight = std::max(curHeight, maxHeight);
			width += curWidth;
		}

		width = mFillWidth ? std::max(mMaxWidth, width) : width;

		mHitbox.setWidth(std::max(width, 0));
		mHitbox.setHeight(std::max(maxHeight, 0));
	}
}

void BlockAlignElementsHorizontally::moveElems()
{
	if (mGrowthDirection == EDirection_RIGHT or mGrowthDirection == EDirection_LEFT_AND_RIGHT)
	{
		// left to right
		int i = getIndexOfFirstActiveElem();
		if (i == -1)
		{
			return;
		}
		UIElement* pLastElem = mpSubElems[i];

		int x1 = mHitbox.getTopLeft().getX() + pLastElem->mMargins.mLeft;
		int x2 = x1 + pLastElem->getHitbox().getWidth();
		int y1 = mHitbox.getTopLeft().getY() + pLastElem->mMargins.mTop;
		int y2 = mHitbox.getBottomRight().getY() - pLastElem->mMargins.mBottom;
		Hitbox blockSpace = Hitbox(x1, x2, y1, y2);

		pLastElem->updatePosFromBlockSpace(blockSpace);
		if (pLastElem->mClassType == EUIClass_BLOCK)
		{
			((UIBlock*)pLastElem)->moveElems();
		}

		for (int count = i + 1; count < mpSubElems.size(); count++)
		{
			UIElement* pCurElem = mpSubElems[count];
			if (!pCurElem->isActive())
			{
				continue;
			}
			
			int x1 = pLastElem->getHitbox().getBottomRight().getX() + pLastElem->mMargins.mRight + pCurElem->mMargins.mLeft;
			int x2 = x1 + pCurElem->getHitbox().getWidth();
			int y1 = mHitbox.getTopLeft().getY() + pCurElem->mMargins.mTop;
			int y2 = mHitbox.getBottomRight().getY() - pCurElem->mMargins.mBottom;
			Hitbox blockSpace = Hitbox(x1, x2, y1, y2);

			pCurElem->updatePosFromBlockSpace(blockSpace);
			if (pCurElem->mClassType == EUIClass_BLOCK)
			{
				((UIBlock*)pCurElem)->moveElems();
			}

			pLastElem = pCurElem;
		}
	}
	else 
	{
		// Right to left
		int i = getIndexOfLastActiveElem();
		if (i == -1)
		{
			return;
		}
		UIElement* pLastElem = mpSubElems[i];

		int x2 = mHitbox.getBottomRight().getX() - pLastElem->mMargins.mLeft;
		int x1 = x2 - pLastElem->getHitbox().getWidth();
		int y1 = mHitbox.getTopLeft().getY() + pLastElem->mMargins.mTop;
		int y2 = mHitbox.getBottomRight().getY() - pLastElem->mMargins.mBottom;
		Hitbox blockSpace = Hitbox(x1, x2, y1, y2);

		pLastElem->updatePosFromBlockSpace(blockSpace);
		if (pLastElem->mClassType == EUIClass_BLOCK)
		{
			((UIBlock*)pLastElem)->moveElems();
		}
		
		for (int count = i - 1; count > -1; count--) 
		{
			UIElement* pCurElem = mpSubElems[count];
			if (!pCurElem->isActive())
			{
				continue;
			}

			x2 = pLastElem->getHitbox().getTopLeft().getX() - pLastElem->mMargins.mLeft - pCurElem->mMargins.mRight;
			x1 = x2 - pCurElem->getHitbox().getWidth();
			y1 = mHitbox.getTopLeft().getY() + pLastElem->mMargins.mTop;
			y2 = mHitbox.getBottomRight().getY() - pLastElem->mMargins.mBottom;
			
			Hitbox blockSpace = Hitbox(x1, x2, y1, y2);

			pCurElem->updatePosFromBlockSpace(blockSpace);
			if (pCurElem->mClassType == EUIClass_BLOCK)
			{
				((UIBlock*)pCurElem)->moveElems();
			}
			
			pLastElem = pCurElem;
		}
	}
}



// MASTER BLOCK
BlockAlignElementsGrid::BlockAlignElementsGrid(Hitbox hitbox, ETextBoxPositionAlign positionAlignH, ETextBoxPositionAlign positionAlignV, bool limitByRows, int limit, 
	bool fillWidth, bool fillHeight, Edges margins, int spacing, SDL_Color backgroundColor, std::string name)
{
	mIsHeadBlock = true;
	mName = name;
	constructBlock(hitbox, positionAlignH, positionAlignV, limitByRows, limit, fillWidth, fillHeight, margins, spacing, backgroundColor);
}

// MASTER BLOCK
BlockAlignElementsGrid::BlockAlignElementsGrid(int maxWidth, int maxHeight, ETextBoxPositionAlign positionAlignH, ETextBoxPositionAlign positionAlignV, bool limitByRows, int limit, 
	bool fillWidth, bool fillHeight, Edges margins, int spacing, SDL_Color backgroundColor, std::string name)
{
	mIsHeadBlock = false;
	mName = name;
	constructBlock(Hitbox(0, maxWidth, 0, maxHeight), positionAlignH, positionAlignV, limitByRows, limit, fillWidth, fillHeight, margins, spacing, backgroundColor);
}

void BlockAlignElementsGrid::constructBlock(Hitbox hitbox, ETextBoxPositionAlign positionAlignH, ETextBoxPositionAlign positionAlignV, bool limitByRows, int limit,
	bool fillWidth, bool fillHeight, Edges margins, int spacing, SDL_Color backgroundColor)
{
	mClassType = EUIClass_BLOCK;
	mGrowthDirectionHorizontal = EDirection_RIGHT;
	mGrowthDirectionVertical = EDirection_DOWN;
	mHitbox = hitbox;
	mMaxWidth = hitbox.getWidth();
	mMaxHeight = hitbox.getHeight();

	mLimitByRows = limitByRows;
	mLimit = limit;

	mPositionAlignH = positionAlignH;
	mPositionAlignV = positionAlignV;
	mStartingPositionCenter = hitbox.getTopLeft();

	mMargins = margins;
	mSpacing = spacing;
	mBackgroundColor = backgroundColor;
	mFillWidth = fillWidth;
	mFillHeight = fillHeight;
}

// TODO cut after max
void BlockAlignElementsGrid::adjustBlocksWidthHeight() 
{
	// update num rows
	mRowHeights.clear();
	mColWidths.clear();
	updateNumRowsCols();

	for (int i = 0; i < (int)mpSubElems.size(); i++)
	{
		UIElement* pElem = mpSubElems[i];
		int x = i / mNumCols;
		int y = i - (x * mNumCols);

		if (pElem->mClassType == EUIClass_BLOCK)
		{
			((UIBlock*)pElem)->adjustBlocksWidthHeight();
		}

		int curWidth = 0;
		int curHeight = 0;
		if (pElem->isActive())
		{
			curWidth = pElem->getHitbox().getWidth();
			if (curWidth != 0)
			{
				curWidth += pElem->mMargins.mLeft + pElem->mMargins.mRight + (i != (int)mpSubElems.size() - 1 ? mSpacing : 0);
			}
			curHeight = pElem->getHitbox().getHeight();
			if (curHeight != 0)
			{
				curHeight += pElem->mMargins.mTop + pElem->mMargins.mBottom + (i != (int)mpSubElems.size() - 1 ? mSpacing : 0);
			}
		}

		if (mRowHeights.size() <= x) 
		{
			mRowHeights.push_back(curHeight);
		}
		else 
		{
			mRowHeights[x] = std::max(mRowHeights[x], curHeight);
		}

		if (mColWidths.size() <= y)
		{
			mColWidths.push_back(curWidth);
		}
		else
		{
			mColWidths[y] = std::max(mColWidths[y], curWidth);
		}
	}

	for (int i = 0; i < (int)mpSubElems.size(); i++)
	{
		UIElement* pElem = mpSubElems[i];
		int x = i / mNumCols;
		int y = i - (x * mNumCols);
	}

	int width = 0;
	int height = 0;

	for (int curWidth : mColWidths) 
	{
		width += curWidth;
	}
	for (int curHeight : mRowHeights)
	{
		height += curHeight;
	}

	width	= mFillWidth ? std::max(mMaxWidth, width) : width;
	height	= mFillHeight ? std::max(mMaxHeight, height) : height;

	mHitbox.setWidth(std::max(width, 0));
	mHitbox.setHeight(std::max(height, 0));
}

void BlockAlignElementsGrid::moveElems() 
{
	// left to right
	int i = getIndexOfFirstActiveElem();
	if (i == -1)
	{
		return;
	}
	UIElement* pLastElem = mpSubElems[i];

	int curRow = i / mNumCols;
	int curCol = i - (curRow * mNumCols);

	// box margins are taken into account for colWidths and rowHeights
	int x1 = mHitbox.getTopLeft().getX();
	for (int c = 0; c < curCol; c++)
	{
		x1 += mColWidths[c];
	}
	int y1 = mHitbox.getTopLeft().getY();
	for (int r = 0; r < curRow; r++)
	{
		y1 += mRowHeights[r];
	}
	
	int x2 = x1 + mColWidths[curCol];
	int y2 = y1 + mRowHeights[curRow];
	
	Hitbox blockSpace = Hitbox(x1 + pLastElem->mMargins.mLeft, x2 - pLastElem->mMargins.mRight, y1 + pLastElem->mMargins.mTop, y2 - pLastElem->mMargins.mBottom);

	pLastElem->updatePosFromBlockSpace(blockSpace);
	if (pLastElem->mClassType == EUIClass_BLOCK)
	{
		((UIBlock*)pLastElem)->moveElems();
	}

	for (int count = i + 1; count < mpSubElems.size(); count++)
	{
		UIElement* pCurElem = mpSubElems[count];
		if (!pCurElem->isActive())
		{
			continue;
		}
		
		curRow = count / mNumCols;
		curCol = count - (curRow * mNumCols);

		if (curCol == 0)
		{
			x1 = mHitbox.getTopLeft().getX();
			y1 = mHitbox.getTopLeft().getY();
			for (int r = 0; r < curRow; r++)
			{
				y1 += mRowHeights[r];
			}
		}
		else 
		{
			x1 = mHitbox.getTopLeft().getX();
			for (int c = 0; c < curCol; c++)
			{
				x1 += mColWidths[c];
			}
		}

		x2 = x1 + mColWidths[curCol];
		y2 = y1 + mRowHeights[curRow];
		
		Hitbox blockSpace = Hitbox(x1 + pCurElem->mMargins.mLeft, x2 - pCurElem->mMargins.mRight, y1 + pCurElem->mMargins.mTop, y2 - pCurElem->mMargins.mBottom);

		pCurElem->updatePosFromBlockSpace(blockSpace);
		if (pCurElem->mClassType == EUIClass_BLOCK)
		{
			((UIBlock*)pCurElem)->moveElems();
		}

		pLastElem = pCurElem;
	}
}

void BlockAlignElementsGrid::updateNumRowsCols() 
{
	if (mLimitByRows)
	{
		mNumRows = mLimit;
		mNumCols = (int)ceil((double)mpSubElems.size() / (double)mNumRows);
	}
	else
	{
		mNumCols = mLimit;
		mNumRows = (int)ceil((double)mpSubElems.size() / (double)mNumCols);
	}
}
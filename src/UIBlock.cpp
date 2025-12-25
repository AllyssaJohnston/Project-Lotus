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
BlockAlignElementsVertically::BlockAlignElementsVertically(Hitbox hitbox, ETextBoxPositionAlign positionAlign, EDirection directionH, EDirection directionV,
		bool fillWidth, bool fillHeight, Edges margins, SDL_Color backgroundColor, std::string name) 
{
	mIsHeadBlock = true;
	mGrowthDirectionHorizontal = directionH;
	mGrowthDirectionVertical = directionV;
	constructBlock(hitbox, positionAlign, directionV, fillWidth, fillHeight, margins, backgroundColor);
	mName = name;
}

// SUB BLOCKS
BlockAlignElementsVertically::BlockAlignElementsVertically(int maxWidth, int maxHeight, ETextBoxPositionAlign positionAlign, 
		EDirection direction, bool fillWidth, bool fillHeight, Edges margins, SDL_Color backgroundColor, std::string name)
{
	constructBlock(Hitbox(0, maxWidth, 0, maxHeight), positionAlign, direction, fillWidth, fillHeight, margins, backgroundColor);
	mName = name;
}

void BlockAlignElementsVertically::constructBlock(Hitbox hitbox, ETextBoxPositionAlign positionAlign, EDirection direction, bool fillWidth, bool fillHeight, Edges margins, SDL_Color backgroundColor)
{
	mClassType = EUIClass_BLOCK;
	mHitbox = hitbox;
	mMaxWidth = hitbox.getWidth();
	mMaxHeight = hitbox.getHeight();
	mPositionAlign = positionAlign;
	switch (mPositionAlign)
	{
	case ETextBoxPositionAlign_CENTER:
		if (direction == EDirection_UP_AND_DOWN) {
			mStartingPositionCenter = hitbox.getCenter();
			break;
		}
		mStartingPositionCenter = Vect2(hitbox.getCenter().getX(), hitbox.getTopLeft().getY());
		break;
	case ETextBoxPositionAlign_LEFT:
		mStartingPositionCenter = hitbox.getTopLeft();
		break;
	default:
		SDL_assert(false);
		break;
	}

	if ((direction != EDirection_UP) and (direction != EDirection_DOWN))
	{
		SDL_assert(false);
	}
	mGrowthDirection = direction;
	mMargins = margins;
	mBackgroundColor = backgroundColor;
	mFillWidth = fillWidth;
	mFillHeight = fillHeight;
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

		int updatedX = 0;
		int updatedY = 0;
		switch (mPositionAlign)
		{
		case ETextBoxPositionAlign_CENTER:
			updatedX = mHitbox.getCenter().getX() - (maxWidth / 2);
			updatedY = mHitbox.getCenter().getY() - (height / 2);
			break;
		case ETextBoxPositionAlign_LEFT:
			updatedX = mHitbox.getTopLeft().getX();
			updatedY = mHitbox.getTopLeft().getY();
			break;
		default:
			SDL_assert(false);
			break;
		}

		mHitbox.setWidth(std::max(maxWidth, 0));
		mHitbox.setHeight(std::max(height, 0));
		mHitbox.setTopLeft(Vect2(updatedX, updatedY));
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

		// if left aligned, take into account all left margins, ignore right margins
		// if center aligned, start in the center and THEN push right by the left magin AND push left by the right margain
		// if right aligned, ignore left margins, take into account the right margins
		int leftMargin = (mPositionAlign == ETextBoxPositionAlign_RIGHT) ? 0 : pLastElem->mMargins.mLeft;
		int rightMargin = (mPositionAlign == ETextBoxPositionAlign_LEFT) ? 0 : pLastElem->mMargins.mRight;
		
		int x = mHitbox.getTopLeft().getX() + leftMargin - rightMargin;
		int y = mHitbox.getTopLeft().getY() + pLastElem->mMargins.mTop;
		
		int centering = (mPositionAlign == ETextBoxPositionAlign_CENTER) ? (mHitbox.getWidth() - pLastElem->getHitbox().getWidth()) / 2 : 0;
		int changeX = x - pLastElem->getHitbox().getTopLeft().getX() + centering;
		int changeY = y - pLastElem->getHitbox().getTopLeft().getY();

		if (pLastElem->mClassType == EUIClass_BLOCK)
		{
			pLastElem->getHitbox().setTopLeft(Vect2(x, y));
			((UIBlock*)pLastElem)->moveElems();
		}
		else // BOX
		{
			((UIBox*)pLastElem)->shiftHitbox(Vect2(changeX, changeY));
		}
		
		for (int count = i + 1; count < mpSubElems.size(); count++)
		{
			UIElement* pCurElem = mpSubElems[count];
			if (!pCurElem->isActive())
			{
				continue;
			}
			
			leftMargin = (mPositionAlign == ETextBoxPositionAlign_RIGHT) ? 0 : pCurElem->mMargins.mLeft;
			rightMargin = (mPositionAlign == ETextBoxPositionAlign_LEFT) ? 0 : pCurElem->mMargins.mRight;

			x = mHitbox.getTopLeft().getX() + leftMargin - rightMargin;
			y = pLastElem->getHitbox().getBottomRight().getY() + pLastElem->mMargins.mBottom + pCurElem->mMargins.mTop;
			
			centering = (mPositionAlign == ETextBoxPositionAlign_CENTER) ? (mHitbox.getWidth() - pCurElem->getHitbox().getWidth()) / 2 : 0;
			changeX = x - pCurElem->getHitbox().getTopLeft().getX() + centering;
			changeY = y - pCurElem->getHitbox().getTopLeft().getY();

			if (pCurElem->mClassType == EUIClass_BLOCK)
			{
				pCurElem->getHitbox().setTopLeft(Vect2(x, y));
				((UIBlock*)pCurElem)->moveElems();
			}
			else // BOX
			{
				((UIBox*)pCurElem)->shiftHitbox(Vect2(changeX, changeY));
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
		
		// if left aligned, take into account all left margins, ignore right margins
		// if center aligned, start in the center and THEN push right by the left magin AND push left by the right margain
		// if right aligned, ignore left margins, take into account the right margins
		int leftMargin = (mPositionAlign == ETextBoxPositionAlign_RIGHT) ? 0 : pLastElem->mMargins.mLeft;
		int rightMargin = (mPositionAlign == ETextBoxPositionAlign_LEFT) ? 0 : pLastElem->mMargins.mRight;

		int x = mHitbox.getTopLeft().getX() + leftMargin - rightMargin;
		int y = mHitbox.getBottomRight().getY() - pLastElem->getHitbox().getHeight() - pLastElem->mMargins.mBottom;
		
		int centering = (mPositionAlign == ETextBoxPositionAlign_CENTER) ? (mHitbox.getWidth() - pLastElem->getHitbox().getWidth()) / 2 : 0;
		int changeX = x - pLastElem->getHitbox().getTopLeft().getX() + centering;
		int changeY = y - pLastElem->getHitbox().getTopLeft().getY();

		if (pLastElem->mClassType == EUIClass_BLOCK)
		{
			pLastElem->getHitbox().setTopLeft(Vect2(x, y));
			((UIBlock*)pLastElem)->moveElems();
		}
		else // BOX
		{
			((UIBox*)pLastElem)->shiftHitbox(Vect2(changeX, changeY));
		}

		for (int count = i - 1; count > -1; count--)
		{
			UIElement* pCurElem = mpSubElems[count];
			if (!pCurElem->isActive())
			{
				continue;
			}
			

			leftMargin = (mPositionAlign == ETextBoxPositionAlign_RIGHT) ? 0 : pCurElem->mMargins.mLeft;
			rightMargin = (mPositionAlign == ETextBoxPositionAlign_LEFT) ? 0 : pCurElem->mMargins.mRight;

			x = mHitbox.getTopLeft().getX() + leftMargin - rightMargin;
			y = pLastElem->getHitbox().getTopLeft().getY() - pCurElem->getHitbox().getHeight() -
				(pCurElem->mMargins.mBottom + pLastElem->mMargins.mTop);
			
			centering = (mPositionAlign == ETextBoxPositionAlign_CENTER) ? (mHitbox.getWidth() - pCurElem->getHitbox().getWidth()) / 2 : 0;
			changeX = x - pCurElem->getHitbox().getTopLeft().getX() + centering;
			changeY = y - pCurElem->getHitbox().getTopLeft().getY();

			if (pCurElem->mClassType == EUIClass_BLOCK)
			{
				pCurElem->getHitbox().setTopLeft(Vect2(x, y));
				((UIBlock*)pCurElem)->moveElems();
			}
			else // BOX
			{
				((UIBox*)pCurElem)->shiftHitbox(Vect2(changeX, changeY));
			}
			
			pLastElem = pCurElem;
		}
	}
}



// HEAD BLOCK
BlockAlignElementsHorizontally::BlockAlignElementsHorizontally(Hitbox hitbox, ETextBoxPositionAlign positionAlign, EDirection directionH, EDirection directionV, 
		bool fillWidth, bool fillHeight, Edges margins, SDL_Color backgroundColor, std::string name) 
{
	mIsHeadBlock = true;
	mGrowthDirectionHorizontal = directionH;
	mGrowthDirectionVertical = directionV;
	constructBlock(hitbox, positionAlign, directionH, fillWidth, fillHeight, margins, backgroundColor);
	mName = name;
}

// SUB BLOCKS
BlockAlignElementsHorizontally::BlockAlignElementsHorizontally(int maxWidth, int maxHeight, ETextBoxPositionAlign positionAlign, 
		EDirection direction, bool fillWidth, bool fillHeight, Edges margins, SDL_Color backgroundColor, std::string name)
{
	constructBlock(Hitbox(0, maxWidth, 0, maxHeight), positionAlign, direction, fillWidth, fillHeight, margins, backgroundColor);
	mName = name;
}

void BlockAlignElementsHorizontally::constructBlock(Hitbox hitbox, ETextBoxPositionAlign positionAlign, EDirection direction, bool fillWidth, bool fillHeight, 
		Edges margins, SDL_Color backgroundColor)
{
	mClassType = EUIClass_BLOCK;
	mHitbox = hitbox;
	mMaxWidth = hitbox.getWidth();
	mMaxHeight = hitbox.getHeight();
	mPositionAlign = positionAlign;
	switch (positionAlign)
	{
	case ETextBoxPositionAlign_CENTER:
		if (direction == EDirection_LEFT_AND_RIGHT) {
			mStartingPositionCenter = hitbox.getCenter();
			break;
		}
		mStartingPositionCenter = Vect2(hitbox.getTopLeft().getX(), hitbox.getCenter().getY());
		break;
	case ETextBoxPositionAlign_LEFT:
		mStartingPositionCenter = hitbox.getTopLeft();
		break;
	default:
		SDL_assert(false);
		break;
	}
	
	if ((direction != EDirection_LEFT) and (direction != EDirection_RIGHT) and (direction != EDirection_LEFT_AND_RIGHT))
	{
		SDL_assert(false);
	}
	mGrowthDirection = direction;
	mMargins = margins;
	mBackgroundColor = backgroundColor;
	mFillWidth = fillWidth;
	mFillHeight = fillHeight;
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

		int updatedX = 0;
		int updatedY = 0;
		switch (mPositionAlign)
		{
		case ETextBoxPositionAlign_CENTER:
			updatedX = mHitbox.getCenter().getX() - (width / 2);
			updatedY = mHitbox.getCenter().getY() - (maxHeight / 2);
			break;
		case ETextBoxPositionAlign_LEFT:
			updatedX = mHitbox.getTopLeft().getX();
			updatedY = mHitbox.getTopLeft().getY();
			break;
		default:
			SDL_assert(false);
			break;
		}

		mHitbox.setWidth(std::max(width, 0));
		mHitbox.setHeight(std::max(maxHeight, 0));
		mHitbox.setTopLeft(Vect2(updatedX, updatedY));
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

		int x = mHitbox.getTopLeft().getX() + pLastElem->mMargins.mLeft;
		int y = mHitbox.getTopLeft().getY() + pLastElem->mMargins.mTop - pLastElem->mMargins.mBottom;

		int centering = (mPositionAlign == ETextBoxPositionAlign_CENTER) ? (mHitbox.getHeight() - pLastElem->getHitbox().getHeight()) / 2 : 0;
		int changeX = x - pLastElem->getHitbox().getTopLeft().getX();
		int changeY = y - pLastElem->getHitbox().getTopLeft().getY() + centering;

		if (pLastElem->mClassType == EUIClass_BLOCK)
		{
			pLastElem->getHitbox().setTopLeft(Vect2(x, y));
			((UIBlock*)pLastElem)->moveElems();
		}
		else // BOX
		{
			((UIBox*)pLastElem)->shiftHitbox(Vect2(changeX, changeY));
		}

		for (int count = i + 1; count < mpSubElems.size(); count++)
		{
			UIElement* pCurElem = mpSubElems[count];
			if (!pCurElem->isActive())
			{
				continue;
			}
			
			y = mHitbox.getTopLeft().getY() + pCurElem->mMargins.mTop - pLastElem->mMargins.mBottom;
			x = pLastElem->getHitbox().getBottomRight().getX() + pLastElem->mMargins.mRight + pCurElem->mMargins.mLeft;
			
			centering = (mPositionAlign == ETextBoxPositionAlign_CENTER) ? (mHitbox.getHeight() - pCurElem->getHitbox().getHeight()) / 2 : 0;
			int changeX = x - pCurElem->getHitbox().getTopLeft().getX();
			int changeY = y - pCurElem->getHitbox().getTopLeft().getY() + centering;

			if (pCurElem->mClassType == EUIClass_BLOCK)
			{
				pCurElem->getHitbox().setTopLeft(Vect2(x, y));
				((UIBlock*)pCurElem)->moveElems();
			}
			else // BOX
			{
				((UIBox*)pCurElem)->shiftHitbox(Vect2(changeX, changeY));
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
		int x = mHitbox.getBottomRight().getX() - pLastElem->getHitbox().getWidth() - pLastElem->mMargins.mRight;
		int y = mHitbox.getTopLeft().getY() + pLastElem->mMargins.mTop - pLastElem->mMargins.mBottom;

		int centering = (mPositionAlign == ETextBoxPositionAlign_CENTER) ? (mHitbox.getHeight() - pLastElem->getHitbox().getHeight()) / 2 : 0;
		int changeX = x - pLastElem->getHitbox().getTopLeft().getX();
		int changeY = y - pLastElem->getHitbox().getTopLeft().getY() + centering;

		if (pLastElem->mClassType == EUIClass_BLOCK)
		{
			pLastElem->getHitbox().setTopLeft(Vect2(x, y));
			((UIBlock*)pLastElem)->moveElems();
		}
		else // BOX
		{
			((UIBox*)pLastElem)->shiftHitbox(Vect2(changeX, changeY));
		}

		for (int count = i - 1; count > -1; count--) 
		{
			UIElement* pCurElem = mpSubElems[count];
			if (!pCurElem->isActive())
			{
				continue;
			}

			y = mHitbox.getTopLeft().getY() + pCurElem->mMargins.mTop - pLastElem->mMargins.mBottom;
			x = pLastElem->getHitbox().getTopLeft().getX() - pCurElem->getHitbox().getWidth() - (pCurElem->mMargins.mRight + pLastElem->mMargins.mLeft);
			
			centering = (mPositionAlign == ETextBoxPositionAlign_CENTER) ? (mHitbox.getHeight() - pCurElem->getHitbox().getHeight()) / 2 : 0;
			changeX = x - pCurElem->getHitbox().getTopLeft().getX();
			changeY = y - pCurElem->getHitbox().getTopLeft().getY() + centering;
			
			if (pCurElem->mClassType == EUIClass_BLOCK)
			{
				pCurElem->getHitbox().setTopLeft(Vect2(x, y));
				((UIBlock*)pCurElem)->moveElems();
			}
			else // BOX
			{
				((UIBox*)pCurElem)->shiftHitbox(Vect2(changeX, changeY));
			}

			pLastElem = pCurElem;
		}
	}
}

// MASTER BLOCK
BlockAlignElementsGrid::BlockAlignElementsGrid(Hitbox hitbox, ETextBoxPositionAlign positionAlign, bool limitByRows, int limit, bool fillWidth, bool fillHeight,
	Edges margins, SDL_Color backgroundColor, std::string name)
{
	mIsHeadBlock = true;
	mName = name;
	constructBlock(hitbox, positionAlign, limitByRows, limit, fillWidth, fillHeight, margins, backgroundColor);
}

// MASTER BLOCK
BlockAlignElementsGrid::BlockAlignElementsGrid(int maxWidth, int maxHeight, ETextBoxPositionAlign positionAlign, bool limitByRows, int limit, bool fillWidth, bool fillHeight,
	Edges margins, SDL_Color backgroundColor, std::string name)
{
	mIsHeadBlock = false;
	mName = name;
	constructBlock(Hitbox(0, maxWidth, 0, maxHeight), positionAlign, limitByRows, limit, fillWidth, fillHeight, margins, backgroundColor);
}

void BlockAlignElementsGrid::constructBlock(Hitbox hitbox, ETextBoxPositionAlign positionAlign, bool limitByRows, int limit,
	bool fillWidth, bool fillHeight, Edges margins, SDL_Color backgroundColor)
{
	mClassType = EUIClass_BLOCK;
	mGrowthDirectionHorizontal = EDirection_RIGHT;
	mGrowthDirectionVertical = EDirection_DOWN;
	mHitbox = hitbox;
	mMaxWidth = hitbox.getWidth();
	mMaxHeight = hitbox.getHeight();

	mLimitByRows = limitByRows;
	mLimit = limit;

	mPositionAlign = positionAlign;
	mStartingPositionCenter = hitbox.getTopLeft();

	mMargins = margins;
	mBackgroundColor = backgroundColor;
	mFillWidth = fillWidth;
	mFillHeight = fillHeight;
}

// TODO cut after max
void BlockAlignElementsGrid::adjustBlocksWidthHeight() 
{
	mRowHeights.clear();
	mColWidths.clear();

	// update num rows
	updateNumRowsCols();

	if (mpSubElems.size() > 0)
	{
		int width = 0;
		int height = 0;
		
		int i = 0;
		for (UIElement* pElem : mpSubElems)
		{
			int x = i / mNumCols;
			int y = i - (x * mNumCols);

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

			i++;
		}

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

		int updatedX = 0;
		int updatedY = 0;
		switch (mPositionAlign)
		{
		case ETextBoxPositionAlign_CENTER:
			updatedX = mHitbox.getCenter().getX() - (width / 2);
			updatedY = mHitbox.getCenter().getY() - (height / 2);
			break;
		case ETextBoxPositionAlign_LEFT:
			updatedX = mHitbox.getTopLeft().getX();
			updatedY = mHitbox.getTopLeft().getY();
			break;
		default:
			SDL_assert(false);
			break;
		}

		mHitbox.setWidth(std::max(width, 0));
		mHitbox.setHeight(std::max(height, 0));
		mHitbox.setTopLeft(Vect2(updatedX, updatedY));
	}
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

	int x = mHitbox.getTopLeft().getX();
	int y = mHitbox.getTopLeft().getY();
	

	int centeringX = (mPositionAlign == ETextBoxPositionAlign_CENTER) ? (mColWidths[curCol]  - pLastElem->getHitbox().getWidth()) / 2 : 0;
	int centeringY = (mPositionAlign == ETextBoxPositionAlign_CENTER) ? (mRowHeights[curRow] - pLastElem->getHitbox().getHeight()) / 2 : 0;
	int changeX = x + pLastElem->mMargins.mLeft + centeringX - pLastElem->getHitbox().getTopLeft().getX();
	int changeY = y + pLastElem->mMargins.mTop + centeringY - pLastElem->getHitbox().getTopLeft().getY();
	
	if (pLastElem->mClassType == EUIClass_BLOCK)
	{
		pLastElem->getHitbox().setTopLeft(Vect2(x, y));
		((UIBlock*)pLastElem)->moveElems();
	}
	else // BOX
	{
		((UIBox*)pLastElem)->shiftHitbox(Vect2(changeX, changeY));
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
			x = mHitbox.getTopLeft().getX();
			y = mRowHeights[0] + mHitbox.getTopLeft().getY();
			for (int r = 1; r < curRow; r++)
			{
				y += mRowHeights[r];
			}
		}
		else 
		{
			x = mColWidths[0] + mHitbox.getTopLeft().getX();
			for (int c = 1; c < curCol; c++)
			{
				x += mColWidths[c];
			}
		}
		
		centeringX = (mPositionAlign == ETextBoxPositionAlign_CENTER) ? (mColWidths[curCol] - pCurElem->getHitbox().getWidth()) / 2 : 0;
		centeringY = (mPositionAlign == ETextBoxPositionAlign_CENTER) ? (mRowHeights[curRow] - pCurElem->getHitbox().getHeight()) / 2 : 0;
		int changeX = x + pCurElem->mMargins.mLeft + centeringX - pCurElem->getHitbox().getTopLeft().getX();
		int changeY = y + pCurElem->mMargins.mTop + centeringY - pCurElem->getHitbox().getTopLeft().getY();

		if (pCurElem->mClassType == EUIClass_BLOCK)
		{
			pCurElem->getHitbox().setTopLeft(Vect2(x, y));
			((UIBlock*)pCurElem)->moveElems();
		}
		else // BOX
		{
			((UIBox*)pCurElem)->shiftHitbox(Vect2(changeX, changeY));
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
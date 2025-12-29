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

UIBlock::UIBlock(Hitbox hitbox, EUIPositionAlign positionAlignH, EUIPositionAlign positionAlignV, EDirection growthDirectionH, EDirection growthDirectionV,
	bool limitByRows, int limit, bool fillWidth, bool fillHeight, Edges margins, int spacing, SDL_Color backgroundColor, std::string name)
{
	mName = name;
	constructBlock(hitbox, positionAlignH, positionAlignV, growthDirectionH, growthDirectionV, limitByRows, limit, fillWidth, fillHeight, margins, spacing, backgroundColor);
}

UIBlock::UIBlock(int maxWidth, int maxHeight, EUIPositionAlign positionAlignH, EUIPositionAlign positionAlignV,
	EDirection growthDirectionH, EDirection growthDirectionV, bool limitByRows, int limit, bool fillWidth, bool fillHeight, Edges margins, int spacing,
	SDL_Color backgroundColor, std::string name)
{
	mName = name;
	constructBlock(Hitbox(0, maxWidth, 0, maxHeight), positionAlignH, positionAlignV, growthDirectionH, growthDirectionV, limitByRows, limit, fillWidth, fillHeight, margins, spacing, backgroundColor);
}

void UIBlock::constructBlock(Hitbox hitbox, EUIPositionAlign positionAlignH, EUIPositionAlign positionAlignV, EDirection growthDirectionH, EDirection growthDirectionV,
	bool limitByRows, int limit, bool fillWidth, bool fillHeight, Edges margins, int spacing, SDL_Color backgroundColor)
{
	mClassType = EUIClass_BLOCK;
	mGrowthDirectionHorizontal = growthDirectionH;
	mGrowthDirectionVertical = growthDirectionV;
	mHitbox = hitbox;
	mMaxWidth = hitbox.getWidth();
	mMaxHeight = hitbox.getHeight();

	mLimitByRows = limitByRows;
	mLimit = limit;

	mPositionAlignH = positionAlignH;
	mPositionAlignV = positionAlignV;
	int startX = 0;
	int startY = 0;
	switch (mPositionAlignH)
	{
	case EUIPositionAlign_LEFT:
		startX = hitbox.getTopLeft().getX();
		break;
	case EUIPositionAlign_CENTER:
		startX = hitbox.getCenter().getX();
		break;
	case EUIPositionAlign_RIGHT:
		startX = hitbox.getBottomRight().getX();
		break;
	default:
		SDL_assert(false);
		break;
	}
	switch (mPositionAlignV)
	{
	case EUIPositionAlign_TOP:
		startY = hitbox.getTopLeft().getY();
		break;
	case EUIPositionAlign_CENTER:
		startY = hitbox.getCenter().getY();
		break;
	case EUIPositionAlign_BOTTOM:
		startY = hitbox.getBottomRight().getY();
		break;
	default:
		SDL_assert(false);
		break;
	}
	mAnchorPoint = Vect2(startX, startY);

	mMargins = margins;
	mSpacing = spacing;
	mBackgroundColor = backgroundColor;
	mFillWidth = fillWidth;
	mFillHeight = fillHeight;
}


Hitbox& UIBlock::getHitbox() { return mHitbox; }

// TODO cut after max
void UIBlock::adjustBlocksWidthHeight()
{
	// update num rows
	mRowHeights.clear();
	mColWidths.clear();
	updateNumRowsCols();

	int firstActive = -1;
	int lastActive = -1;

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
			if (firstActive == -1)
			{
				firstActive = i;
			}
			lastActive = i;

			curWidth = pElem->getHitbox().getWidth();
			if (curWidth != 0)
			{
				curWidth += pElem->mMargins.mLeft + pElem->mMargins.mRight + mSpacing;
			}
			curHeight = pElem->getHitbox().getHeight();
			if (curHeight != 0)
			{
				curHeight += pElem->mMargins.mTop + pElem->mMargins.mBottom + mSpacing;
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

	int totalWidth = 0;
	for (int curWidth : mColWidths)
	{
		totalWidth += curWidth;
	}
	int totalHeight = 0;
	for (int curHeight : mRowHeights)
	{
		totalHeight += curHeight;
	}

	if (mFillWidth && firstActive != -1 && lastActive != -1)
	{
		if (totalWidth > mMaxWidth)
		{
			totalWidth = mMaxWidth;
		}
		int difWidth = mMaxWidth - totalWidth;
		mColWidths[firstActive / mNumRows] += difWidth / 2;
		mColWidths[lastActive / mNumRows] += difWidth / 2;
		totalWidth = mMaxWidth;
	}
	if (mFillHeight && firstActive != -1 && lastActive != -1)
	{
		if (totalHeight > mMaxHeight)
		{
			totalHeight = mMaxHeight;
		}
		int difHeight = mMaxHeight - totalHeight;
		mRowHeights[firstActive / mNumCols] += difHeight / 2;
		mRowHeights[lastActive / mNumCols] += difHeight / 2;
		totalHeight = mMaxHeight;
	}

	mHitbox.setWidth(std::max(totalWidth, 0));
	mHitbox.setHeight(std::max(totalHeight, 0));
}

void UIBlock::moveElems()
{
	// left to right
	int i = getIndexOfFirstActiveElem();
	if (i == -1)
	{
		return;
	}
	UIElement* pLastElem = mpSubElems[i];

	int lastRow;
	int lastCol;
	int curRow = i / mNumCols;
	int curCol = i - (curRow * mNumCols);

	// box margins are taken into account for colWidths and rowHeights
	int x1;
	int x2;
	int y1;
	int y2;
	if (mGrowthDirectionHorizontal == EDirection_RIGHT)
	{
		x1 = mHitbox.getTopLeft().getX();
		for (int c = 0; c < curCol; c++)
		{
			x1 += mColWidths[c];
		}
		x2 = x1 + mColWidths[curCol];
	}
	else
	{
		int totalWidth = 0;
		for (int c = curCol + 1; c < (int)mColWidths.size(); c++)
		{
			totalWidth += mColWidths[c];
		}
		x2 = mHitbox.getBottomRight().getX() - totalWidth;
		x1 = x2 - mColWidths[curCol];
	}

	if (mGrowthDirectionVertical == EDirection_DOWN)
	{
		y1 = mHitbox.getTopLeft().getY();
		for (int r = 0; r < curRow; r++)
		{
			y1 += mRowHeights[r];
		}
		y2 = y1 + mRowHeights[curRow];
	}
	else
	{
		int totalHeight = 0;
		for (int r = curRow + 1; r < (int)mRowHeights.size(); r++)
		{
			totalHeight += mRowHeights[r];
		}
		y2 = mHitbox.getBottomRight().getY() - totalHeight;
		y1 = y2 - mRowHeights[curRow];
	}
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

		lastRow = curRow;
		lastCol = curCol;
		curRow = count / mNumCols;
		curCol = count - (curRow * mNumCols);

		if (curCol == 0)
		{
			// next row
			// x is the leftmost it can be
			if (mGrowthDirectionHorizontal == EDirection_RIGHT)
			{
				x1 = mHitbox.getTopLeft().getX();
				x2 = x1 + mColWidths[0];
			}
			else // left
			{
				int totalWidth = 0;
				for (int c = curCol + 1; c < (int)mColWidths.size(); c++)
				{
					totalWidth += mColWidths[c];
				}
				x2 = mHitbox.getBottomRight().getX() - totalWidth;
				x1 = x2 - mColWidths[curCol];
			}

			// shift the new y pos
			if (mGrowthDirectionVertical == EDirection_DOWN)
			{
				y1 += mRowHeights[lastRow];
				y2 = y1 + mRowHeights[curRow];
			}
			else // up
			{
				y2 += mRowHeights[curRow];
				y1 = y2 - mRowHeights[curRow];
			}
		}
		else
		{
			// next col
			if (mGrowthDirectionHorizontal == EDirection_RIGHT)
			{
				x1 += mColWidths[lastCol];
				x2 = x1 + mColWidths[curCol];
			}
			else // left
			{
				x2 += mColWidths[curCol];
				x1 = x2 - mColWidths[curCol];
			}
		}
		Hitbox blockSpace = Hitbox(x1 + pCurElem->mMargins.mLeft, x2 - pCurElem->mMargins.mRight, y1 + pCurElem->mMargins.mTop, y2 - pCurElem->mMargins.mBottom);

		pCurElem->updatePosFromBlockSpace(blockSpace);
		if (pCurElem->mClassType == EUIClass_BLOCK)
		{
			((UIBlock*)pCurElem)->moveElems();
		}

		pLastElem = pCurElem;
	}
}

// only called on the head blocks in menu page
void UIBlock::updateBlocks()
{
	// figure out all block widths and heights
	adjustBlocksWidthHeight();

	int x = mAnchorPoint.getX();
	int y = mAnchorPoint.getY();

	if (mPositionAlignH == EUIPositionAlign_CENTER)
	{
		x -= mHitbox.getWidth() / 2;
	}
	if (mPositionAlignV == EUIPositionAlign_CENTER)
	{
		y -= mHitbox.getHeight() / 2;
	}

	switch (mGrowthDirectionHorizontal)
	{
	case EDirection_RIGHT:
		x += mMargins.mLeft;
		break;
	case EDirection_LEFT:
		x -= mHitbox.getWidth() + mMargins.mRight;
		break;
	default:
		SDL_assert(false);
		break;
	}

	switch (mGrowthDirectionVertical)
	{
	case EDirection_DOWN:
		y += mMargins.mTop;
		break;
	case EDirection_UP:
		y -= mHitbox.getHeight() + mMargins.mBottom;
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

void UIBlock::updateNumRowsCols()
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
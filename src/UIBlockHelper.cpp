#include "UIBlockHelper.h"


UIBlock::~UIBlock()
{
	for (UIBlock* subBlock : mpSubBlocks)
	{
		subBlock->~UIBlock();
		delete subBlock;
		subBlock = nullptr;
	}
	for (UIBox* curBox : mpAllBoxes)
	{
		curBox = nullptr;
	}
}

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
	moveSubBlocks();
}

void UIBlock::setMaxForBoxes()
{
	for (UIBlock* pBlock : mpSubBlocks)
	{
		pBlock->setMaxForBoxes();
	}

	for (UIBox* pCurBox : mpAllBoxes)
	{
		if (pCurBox->mShow) 
		{
			if (pCurBox->mpCurHitbox->getWidth() > mHitbox.getWidth())
			{
				pCurBox->mpCurHitbox->setWidth(mHitbox.getWidth());
			}

			if (pCurBox->mpCurHitbox->getHeight() > mHitbox.getHeight())
			{
				pCurBox->mpCurHitbox->setHeight(mHitbox.getHeight());
			}
		}
		
	}
}

void UIBlock::setAllTextures(SDL_Renderer* pRenderer)
{
	for (UIBlock* pBlock : mpSubBlocks)
	{
		pBlock->setAllTextures(pRenderer);
	}
	setAllTexturesInternal(pRenderer);
}

void UIBlock::setAllTexturesInternal(SDL_Renderer* pRenderer)
{
	for (UIBox* pBox : mpAllBoxes)
	{
		if (pBox->mClassType == EUIBoxClass_TEXTBOX) {
			((TextBox*)pBox)->updateTexture(pRenderer);
		}
		else if (pBox->mClassType == EUIBoxClass_IMAGEBOX)
		{
			((ImageBox*)pBox)->mImageObject.setUpTexture(pRenderer);
		}
	}
}

std::vector<UIBox*> UIBlock::getAllBoxes()
{
	std::vector<UIBox*> list;
	getAllBoxesInternal(list);
	return list;
}

void UIBlock::getAllBoxesInternal(std::vector<UIBox*>& list) 
{
	for (UIBox* pBox : mpAllBoxes) 
	{
		list.push_back(pBox);
	}
	for (UIBlock* pBlock : mpSubBlocks) 
	{
		pBlock->getAllBoxesInternal(list);
	}
}

void UIBlock::getAllBlocksInternal(std::vector<UIBlock*>& list)
{
	list.push_back(this);
	for (UIBlock* pBlock : mpSubBlocks) 
	{
		pBlock->getAllBlocksInternal(list);
	}
}

int UIBlock::getIndexOfFirstCurBox()
{
	for (int i = 0; i < mpAllBoxes.size(); i++) 
	{
		if (mpAllBoxes[i]->mShow) 
		{
			return i;
		}
	}
	return -1;
}

int UIBlock::getIndexOfLastCurBox()
{
	for (int i = (int)mpAllBoxes.size() - 1; i > - 1; i--)
	{
		if (mpAllBoxes[i]->mShow)
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
	if (mpSubBlocks.size() > 0)
	{
		int maxWidth = mFillWidth ? mMaxWidth : 0; // TODO cut after max
		int height = 0;
		for (UIBlock* pCurBlock : mpSubBlocks)
		{
			pCurBlock->adjustBlocksWidthHeight();
			maxWidth = std::max(pCurBlock->mHitbox.getWidth() + pCurBlock->mMargins.mLeft + pCurBlock->mMargins.mRight, maxWidth);
			height += pCurBlock->mHitbox.getHeight() + pCurBlock->mMargins.mTop + pCurBlock->mMargins.mBottom;
		}

		height = mFillHeight ? std::max(mMaxHeight, height) : height;

		mHitbox.setWidth(std::max(maxWidth, 1));
		mHitbox.setHeight(std::max(height, 1));
	}

	else if (mpAllBoxes.size() > 0)
	{
		int maxWidth = mFillWidth ? mMaxWidth : 0; // TODO cut after max
		int height = 0;

		for (UIBox* pCurBox : mpAllBoxes)
		{
			if (pCurBox->mShow)
			{
				maxWidth = std::max(pCurBox->mpCurHitbox->getWidth() + pCurBox->mMargins.mLeft + pCurBox->mMargins.mRight, maxWidth);
				height += pCurBox->mpCurHitbox->getHeight() + pCurBox->mMargins.mTop + pCurBox->mMargins.mBottom;
			}
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

		mHitbox.setWidth(std::max(maxWidth, 1));
		mHitbox.setHeight(std::max(height, 1));
		mHitbox.setTopLeft(Vect2(updatedX, updatedY));
	}
}

void BlockAlignElementsVertically::moveBoxes()
{
	if (mGrowthDirection == EDirection_DOWN or mGrowthDirection == EDirection_UP_AND_DOWN)
	{
		// top to bottom
		int i = getIndexOfFirstCurBox();
		if (i == -1)
		{
			return;
		}
		UIBox* pLastTextBox = mpAllBoxes[i];

		// if left aligned, take into account all left margins, ignore right margins
		// if center aligned, start in the center and THEN push right by the left magin AND push left by the right margain
		// if right aligned, ignore left margins, take into account the right margins
		int leftMargin = (mPositionAlign == ETextBoxPositionAlign_RIGHT) ? 0 : pLastTextBox->mMargins.mLeft;
		int rightMargin = (mPositionAlign == ETextBoxPositionAlign_LEFT) ? 0 : pLastTextBox->mMargins.mRight;
		
		int x = mHitbox.getTopLeft().getX() + leftMargin - rightMargin;
		int y = mHitbox.getTopLeft().getY() + pLastTextBox->mMargins.mTop;
		
		int centering = (mPositionAlign == ETextBoxPositionAlign_CENTER) ? (mHitbox.getWidth() - pLastTextBox->mpCurHitbox->getWidth()) / 2 : 0;
		int changeX = x - pLastTextBox->mpCurHitbox->getTopLeft().getX() + centering;
		int changeY = y - pLastTextBox->mpCurHitbox->getTopLeft().getY();
		pLastTextBox->shiftHitbox(Vect2(changeX, changeY));
		for (int count = i + 1; count < mpAllBoxes.size(); count++)
		{
			if (!mpAllBoxes[count]->mShow)
			{
				continue;
			}
			UIBox* pCurTextBox = mpAllBoxes[count];

			leftMargin = (mPositionAlign == ETextBoxPositionAlign_RIGHT) ? 0 : pCurTextBox->mMargins.mLeft;
			rightMargin = (mPositionAlign == ETextBoxPositionAlign_LEFT) ? 0 : pCurTextBox->mMargins.mRight;

			x = mHitbox.getTopLeft().getX() + leftMargin - rightMargin;
			y = pLastTextBox->mpCurHitbox->getBottomRight().getY() + pLastTextBox->mMargins.mBottom + pCurTextBox->mMargins.mTop;
			
			centering = (mPositionAlign == ETextBoxPositionAlign_CENTER) ? (mHitbox.getWidth() - pCurTextBox->mpCurHitbox->getWidth()) / 2 : 0;
			changeX = x - pCurTextBox->mpCurHitbox->getTopLeft().getX() + centering;
			changeY = y - pCurTextBox->mpCurHitbox->getTopLeft().getY();
			pCurTextBox->shiftHitbox(Vect2(changeX, changeY));
			
			pLastTextBox = pCurTextBox;
		}
	}
	else 
	{
		// Bottom to top
		int i = getIndexOfLastCurBox();
		if (i == -1)
		{
			return;
		}
		UIBox* pLastTextBox = mpAllBoxes[i];
		
		// if left aligned, take into account all left margins, ignore right margins
		// if center aligned, start in the center and THEN push right by the left magin AND push left by the right margain
		// if right aligned, ignore left margins, take into account the right margins

		int leftMargin = (mPositionAlign == ETextBoxPositionAlign_RIGHT) ? 0 : pLastTextBox->mMargins.mLeft;
		int rightMargin = (mPositionAlign == ETextBoxPositionAlign_LEFT) ? 0 : pLastTextBox->mMargins.mRight;

		int x = mHitbox.getTopLeft().getX() + leftMargin - rightMargin;
		int y = mHitbox.getBottomRight().getY() - pLastTextBox->mpCurHitbox->getHeight() - pLastTextBox->mMargins.mBottom;
		
		int centering = (mPositionAlign == ETextBoxPositionAlign_CENTER) ? (mHitbox.getWidth() - pLastTextBox->mpCurHitbox->getWidth()) / 2 : 0;
		int changeX = x - pLastTextBox->mpCurHitbox->getTopLeft().getX() + centering;
		int changeY = y - pLastTextBox->mpCurHitbox->getTopLeft().getY();
		pLastTextBox->shiftHitbox(Vect2(changeX, changeY));
		for (int count = i - 1; count > -1; count--) // int count = i  - 1
		{
			if (!mpAllBoxes[count]->mShow)
			{
				continue;
			}
			UIBox* pCurTextBox = mpAllBoxes[count];

			leftMargin = (mPositionAlign == ETextBoxPositionAlign_RIGHT) ? 0 : pCurTextBox->mMargins.mLeft;
			rightMargin = (mPositionAlign == ETextBoxPositionAlign_LEFT) ? 0 : pCurTextBox->mMargins.mRight;

			x = mHitbox.getTopLeft().getX() + leftMargin - rightMargin;
			y = pLastTextBox->mpCurHitbox->getTopLeft().getY() - pCurTextBox->mpCurHitbox->getHeight() - 
				(pCurTextBox->mMargins.mBottom + pLastTextBox->mMargins.mTop);
			
			centering = (mPositionAlign == ETextBoxPositionAlign_CENTER) ? (mHitbox.getWidth() - pCurTextBox->mpCurHitbox->getWidth()) / 2 : 0;
			changeX = x - pCurTextBox->mpCurHitbox->getTopLeft().getX() + centering;
			changeY = y - pCurTextBox->mpCurHitbox->getTopLeft().getY();
			pCurTextBox->shiftHitbox(Vect2(changeX, changeY));
			
			pLastTextBox = pCurTextBox;
		}
	}
}

void BlockAlignElementsVertically::moveSubBlocks()
{
	if (mpSubBlocks.size() > 0)
	{
		if (mGrowthDirection == EDirection_DOWN or mGrowthDirection == EDirection_UP_AND_DOWN)
		{
			// top to bottom
			UIBlock* pLastBlock = mpSubBlocks[0];
			int x = pLastBlock->mMargins.mLeft + mHitbox.getTopLeft().getX();
			int y = pLastBlock->mMargins.mTop + mHitbox.getTopLeft().getY();
			pLastBlock->mHitbox.setTopLeft(Vect2(x, y));
			pLastBlock->moveSubBlocks();
			for (int count = 1; count < mpSubBlocks.size(); count++)
			{
				UIBlock* pCurBlock = mpSubBlocks[count];
				x = pCurBlock->mMargins.mLeft + mHitbox.getTopLeft().getX();
				y = pLastBlock->mHitbox.getBottomRight().getY() + pCurBlock->mMargins.mTop + pLastBlock->mMargins.mBottom;
				pCurBlock->mHitbox.setTopLeft(Vect2(x, y));
				pCurBlock->moveSubBlocks();
				pLastBlock = pCurBlock;
			}
		}
		else
		{
			// bottom to top
			UIBlock* pLastBlock = mpSubBlocks[mpSubBlocks.size() - 1];
			int x = pLastBlock->mMargins.mLeft + mHitbox.getTopLeft().getX();
			int y = -pLastBlock->mMargins.mBottom + mHitbox.getBottomRight().getY() - pLastBlock->mHitbox.getHeight();
			pLastBlock->mHitbox.setTopLeft(Vect2(x, y));
			pLastBlock->moveSubBlocks();
			for (int count = (int)mpSubBlocks.size() - 2; count > -1; count--)
			{
				UIBlock* pCurBlock = mpSubBlocks[count];
				x = pCurBlock->mMargins.mLeft + mHitbox.getTopLeft().getX();
				y = pLastBlock->mHitbox.getTopLeft().getY() - (pCurBlock->mMargins.mBottom + pLastBlock->mMargins.mTop) - pCurBlock->mHitbox.getHeight();
				pCurBlock->mHitbox.setTopLeft(Vect2(x, y));
				pCurBlock->moveSubBlocks();
				pLastBlock = pCurBlock;
			}
		}
	}
	if (mpAllBoxes.size() > 0)
	{
		moveBoxes();
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
	if (mpSubBlocks.size() > 0)
	{
		int maxHeight = mFillHeight ? mMaxHeight : 0; // TODO cut after max
		int width = 0;
		
		for (UIBlock* pCurBlock : mpSubBlocks)
		{
			pCurBlock->adjustBlocksWidthHeight();
			maxHeight = std::max(pCurBlock->mHitbox.getHeight() + pCurBlock->mMargins.mTop + pCurBlock->mMargins.mBottom, maxHeight);
			width += pCurBlock->mHitbox.getWidth() + pCurBlock->mMargins.mLeft + pCurBlock->mMargins.mRight;
		}

		width = mFillWidth ? std::max(mMaxWidth, width) : width;

		mHitbox.setWidth(std::max(width, 1));
		mHitbox.setHeight(std::max(maxHeight, 1));
	}

	else if (mpAllBoxes.size() > 0)
	{
		int maxHeight = mFillHeight ? mMaxHeight : 0; // TODO cut after max
		int width = 0;

		for (UIBox* pCurBox : mpAllBoxes)
		{
			if (pCurBox->mShow)
			{
				maxHeight = std::max(pCurBox->mpCurHitbox->getHeight() + pCurBox->mMargins.mTop + pCurBox->mMargins.mBottom, maxHeight);
				width += pCurBox->mpCurHitbox->getWidth() + pCurBox->mMargins.mLeft + pCurBox->mMargins.mRight;
			}
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

		mHitbox.setWidth(std::max(width, 1));
		mHitbox.setHeight(std::max(maxHeight, 1));
		mHitbox.setTopLeft(Vect2(updatedX, updatedY));
	}
}

void BlockAlignElementsHorizontally::moveBoxes()
{
	if (mGrowthDirection == EDirection_RIGHT or mGrowthDirection == EDirection_LEFT_AND_RIGHT)
	{
		// left to right
		int i = getIndexOfFirstCurBox();
		if (i == -1) 
		{
			return;
		}
		UIBox* pLastTextBox = mpAllBoxes[i];

		int x = mHitbox.getTopLeft().getX() + pLastTextBox->mMargins.mLeft;
		int y = mHitbox.getTopLeft().getY() + pLastTextBox->mMargins.mTop - pLastTextBox->mMargins.mBottom;

		int centering = (mPositionAlign == ETextBoxPositionAlign_CENTER) ? (mHitbox.getHeight() - pLastTextBox->mpCurHitbox->getHeight()) / 2 : 0;
		int changeX = x - pLastTextBox->mpCurHitbox->getTopLeft().getX();
		int changeY = y - pLastTextBox->mpCurHitbox->getTopLeft().getY() + centering;
		pLastTextBox->shiftHitbox(Vect2(changeX, changeY));
		for (int count = i + 1; count < mpAllBoxes.size(); count++)
		{
			if (!mpAllBoxes[count]->mShow)
			{
				continue;
			}
			UIBox* pCurTextBox = mpAllBoxes[count];
			y = mHitbox.getTopLeft().getY() + pCurTextBox->mMargins.mTop - pLastTextBox->mMargins.mBottom;
			x = pLastTextBox->mpCurHitbox->getBottomRight().getX() + pLastTextBox->mMargins.mRight + pCurTextBox->mMargins.mLeft;
			
			centering = (mPositionAlign == ETextBoxPositionAlign_CENTER) ? (mHitbox.getHeight() - pCurTextBox->mpCurHitbox->getHeight()) / 2 : 0;
			int changeX = x - pCurTextBox->mpCurHitbox->getTopLeft().getX();
			int changeY = y - pCurTextBox->mpCurHitbox->getTopLeft().getY() + centering;
			pCurTextBox->shiftHitbox(Vect2(changeX, changeY));
			pLastTextBox = pCurTextBox;
		}
	}
	else 
	{
		// Right to left
		int i = getIndexOfLastCurBox();
		if (i == -1)
		{
			return;
		}
		UIBox* pLastTextBox = mpAllBoxes[i]; 
		int x = mHitbox.getBottomRight().getX() - pLastTextBox->mpCurHitbox->getWidth() - pLastTextBox->mMargins.mRight;
		int y = mHitbox.getTopLeft().getY() + pLastTextBox->mMargins.mTop - pLastTextBox->mMargins.mBottom;

		int centering = (mPositionAlign == ETextBoxPositionAlign_CENTER) ? (mHitbox.getHeight() - pLastTextBox->mpCurHitbox->getHeight()) / 2 : 0;
		int changeX = x - pLastTextBox->mpCurHitbox->getTopLeft().getX();
		int changeY = y - pLastTextBox->mpCurHitbox->getTopLeft().getY() + centering;
		pLastTextBox->shiftHitbox(Vect2(changeX, changeY));
		for (int count = i - 1; count > -1; count--) 
		{
			if (!mpAllBoxes[count]->mShow)
			{
				continue;
			}
			UIBox* pCurTextBox = mpAllBoxes[count];
			y = mHitbox.getTopLeft().getY() + pCurTextBox->mMargins.mTop - pLastTextBox->mMargins.mBottom;
			x = pLastTextBox->mpCurHitbox->getTopLeft().getX() - pCurTextBox->mpCurHitbox->getWidth() - (pCurTextBox->mMargins.mRight + pLastTextBox->mMargins.mLeft);
			
			centering = (mPositionAlign == ETextBoxPositionAlign_CENTER) ? (mHitbox.getHeight() - pCurTextBox->mpCurHitbox->getHeight()) / 2 : 0;
			changeX = x - pCurTextBox->mpCurHitbox->getTopLeft().getX();
			changeY = y - pCurTextBox->mpCurHitbox->getTopLeft().getY() + centering;
			pCurTextBox->shiftHitbox(Vect2(changeX, changeY));
			pLastTextBox = pCurTextBox;

		}
	}
}

void BlockAlignElementsHorizontally::moveSubBlocks()
{
	if (mpSubBlocks.size() > 0)
	{
		if (mGrowthDirection == EDirection_RIGHT or mGrowthDirection == EDirection_LEFT_AND_RIGHT)
		{
			// left to right
			UIBlock* pLastBlock = mpSubBlocks[0];
			int x = pLastBlock->mMargins.mLeft + mHitbox.getTopLeft().getX();
			int y = pLastBlock->mMargins.mTop - pLastBlock->mMargins.mBottom + mHitbox.getTopLeft().getY();
			pLastBlock->mHitbox.setTopLeft(Vect2(x, y));
			pLastBlock->moveSubBlocks();
			for (int count = 1; count < mpSubBlocks.size(); count++) // count at 1
			{
				UIBlock* pCurBlock = mpSubBlocks[count];
				y = pCurBlock->mMargins.mTop - pLastBlock->mMargins.mBottom + mHitbox.getTopLeft().getY();
				x = pLastBlock->mHitbox.getBottomRight().getX() + pCurBlock->mMargins.mLeft + pLastBlock->mMargins.mRight;
				pCurBlock->mHitbox.setTopLeft(Vect2(x, y));
				pCurBlock->moveSubBlocks();
				pLastBlock = pCurBlock;
			}
		}
		else
		{
			// right to left
			UIBlock* pLastBlock = mpSubBlocks[mpSubBlocks.size() - 1];
			int x = pLastBlock->mMargins.mLeft + mHitbox.getBottomRight().getX() - pLastBlock->mHitbox.getWidth() - pLastBlock->mMargins.mRight;
			int y = pLastBlock->mMargins.mTop - pLastBlock->mMargins.mBottom + mHitbox.getTopLeft().getY();
			pLastBlock->mHitbox.setTopLeft(Vect2(x, y));
			pLastBlock->moveSubBlocks();
			for (int count = (int)mpSubBlocks.size() - 2; count > -1; count--)
			{
				UIBlock* pCurBlock = mpSubBlocks[count];
				y = pCurBlock->mMargins.mTop - pLastBlock->mMargins.mBottom  + mHitbox.getTopLeft().getY();
				x = pLastBlock->mHitbox.getTopLeft().getX() - (pCurBlock->mMargins.mRight + pLastBlock->mMargins.mLeft) - pCurBlock->mHitbox.getWidth();
				pCurBlock->mHitbox.setTopLeft(Vect2(x, y));
				pCurBlock->moveSubBlocks();
				pLastBlock = pCurBlock;
			}
		}
	}
	if (mpAllBoxes.size() > 0)
	{
		// BOXES
		moveBoxes();
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

	if (mpSubBlocks.size() > 0)
	{
		int width = 0;
		int height = 0;
		
		int i = 0;
		for (UIBlock* pCurBlock : mpSubBlocks)
		{
			int x = i / mNumCols;
			int y = i - (x * mNumCols);

			pCurBlock->adjustBlocksWidthHeight();
			
			int curWidth	= pCurBlock->mHitbox.getWidth()  + pCurBlock->mMargins.mLeft + pCurBlock->mMargins.mRight;
			int curHeight	= pCurBlock->mHitbox.getHeight() + pCurBlock->mMargins.mTop + pCurBlock->mMargins.mBottom;

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

		width = mFillWidth ? std::max(mMaxWidth, width) : width;
		height = mFillHeight ? std::max(mMaxHeight, height) : height;

		mHitbox.setWidth(std::max(width, 1));
		mHitbox.setHeight(std::max(height, 1));
	}

	else if (mpAllBoxes.size() > 0)
	{
		int width = 0;
		int height = 0;

		int i = 0;
		for (UIBox* pCurBox : mpAllBoxes)
		{
			int x = i / mNumCols;
			int y = i - (x * mNumCols);

			int curWidth	= pCurBox->mpCurHitbox->getWidth() + pCurBox->mMargins.mLeft + pCurBox->mMargins.mRight;
			int curHeight	= pCurBox->mpCurHitbox->getHeight() + pCurBox->mMargins.mTop + pCurBox->mMargins.mBottom;

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

			for (int curWidth : mColWidths)
			{
				width += curWidth;
			}
			for (int curHeight : mRowHeights)
			{
				height += curHeight;
			}

			width = mFillWidth ? std::max(mMaxWidth, width) : width;
			height = mFillHeight ? std::max(mMaxHeight, height) : height;
			i++;
		}

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

		mHitbox.setWidth(std::max(width, 1));
		mHitbox.setHeight(std::max(height, 1));
		mHitbox.setTopLeft(Vect2(updatedX, updatedY));
	}
}

void BlockAlignElementsGrid::moveBoxes() 
{
	// left to right
	UIBox* pLastTextBox = mpAllBoxes[0];

	int curRow = 0;
	int curCol = 0;

	int x = mHitbox.getTopLeft().getX();
	int y = mHitbox.getTopLeft().getY();
	

	int centeringX = (mPositionAlign == ETextBoxPositionAlign_CENTER) ? (mColWidths[curCol]  - pLastTextBox->mpCurHitbox->getWidth()) / 2 : 0;
	int centeringY = (mPositionAlign == ETextBoxPositionAlign_CENTER) ? (mRowHeights[curRow] - pLastTextBox->mpCurHitbox->getHeight()) / 2 : 0;
	int changeX = x + pLastTextBox->mMargins.mLeft + centeringX - pLastTextBox->mpCurHitbox->getTopLeft().getX();
	int changeY = y + pLastTextBox->mMargins.mTop + centeringY - pLastTextBox->mpCurHitbox->getTopLeft().getY();
	pLastTextBox->shiftHitbox(Vect2(changeX, changeY));
	for (int count = 1; count < mpAllBoxes.size(); count++)
	{
		if (!mpAllBoxes[count]->mShow)
		{
			continue;
		}
		UIBox* pCurTextBox = mpAllBoxes[count];
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
		
		centeringX = (mPositionAlign == ETextBoxPositionAlign_CENTER) ? (mColWidths[curCol] - pCurTextBox->mpCurHitbox->getWidth()) / 2 : 0;
		centeringY = (mPositionAlign == ETextBoxPositionAlign_CENTER) ? (mRowHeights[curRow] - pCurTextBox->mpCurHitbox->getHeight()) / 2 : 0;
		int changeX = x + pCurTextBox->mMargins.mLeft + centeringX - pCurTextBox->mpCurHitbox->getTopLeft().getX();
		int changeY = y + pCurTextBox->mMargins.mTop + centeringY - pCurTextBox->mpCurHitbox->getTopLeft().getY();
		pCurTextBox->shiftHitbox(Vect2(changeX, changeY));
		pLastTextBox = pCurTextBox;
	}
}

void BlockAlignElementsGrid::moveSubBlocks() 
{
	if (mpSubBlocks.size() > 0)
	{
		// left to right && top to bottom
		UIBlock* pLastBlock = mpSubBlocks[0];
		
		int curRow = 0;
		int curCol = 0;

		int x = mHitbox.getTopLeft().getX();
		int y = mHitbox.getTopLeft().getY();
		pLastBlock->mHitbox.setTopLeft(Vect2(x + pLastBlock->mMargins.mLeft, y + pLastBlock->mMargins.mTop));
		pLastBlock->moveSubBlocks();
		for (int count = 1; count < mpSubBlocks.size(); count++) // count at 1
		{
			UIBlock* pCurBlock = mpSubBlocks[count];
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

			pCurBlock->mHitbox.setTopLeft(Vect2(x + pCurBlock->mMargins.mLeft, y + pCurBlock->mMargins.mTop));
			pCurBlock->moveSubBlocks();
			pLastBlock = pCurBlock;
		}
	}
	if (mpAllBoxes.size() > 0)
	{
		// BOXES
		moveBoxes();
	}
}

void BlockAlignElementsGrid::updateNumRowsCols() 
{
	if (mLimitByRows)
	{
		mNumRows = mLimit;
		if (mpSubBlocks.size() > 0)
		{
			mNumCols = (int)ceil((double)mpSubBlocks.size() / (double)mNumRows);
		}
		else
		{
			mNumCols = (int)ceil((double)mpAllBoxes.size() / (double)mNumRows);
		}
	}
	else
	{
		mNumCols = mLimit;
		if (mpSubBlocks.size() > 0)
		{
			mNumRows = (int)ceil((double)mpSubBlocks.size() / (double)mNumCols);
		}
		else
		{
			mNumRows = (int)ceil((double)mpAllBoxes.size() / (double)mNumCols);
		}
	}
}
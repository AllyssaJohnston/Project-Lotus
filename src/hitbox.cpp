#include "hitbox.h"
#include <cmath>
#include <cassert>

HitboxEdges::HitboxEdges(EEntityEdgeType top, EEntityEdgeType bottom, EEntityEdgeType left, EEntityEdgeType right)
{
	mTop	= top;
	mBottom = bottom;
	mLeft	= left;
	mRight	= right;
}

Edges::Edges(int top, int bottom, int left, int right) :mTop(top), mBottom(bottom), mLeft(left), mRight(right) { ; }

Vect2::Vect2(int x, int y) : mX(x), mY(y) { ; }

Vect2::Vect2(float x, int y) : mX((int)x), mY(y) { ; }

Vect2::Vect2(int x, float y) : mX(x), mY((int)y) { ; }

Vect2::Vect2(float x, float y) : mX((int)x), mY((int)y) { ; }


Vect2& Vect2::operator+=(const Vect2& vect)
{
	mX += vect.mX;
	mY += vect.mY;
	return *this;
}

Vect2& Vect2::operator-=(const Vect2& vect)
{
	mX -= vect.mX;
	mY -= vect.mY;
	return *this;
}

Vect2 Vect2::operator+(const Vect2& vect) const { return Vect2(mX + vect.mX, mY + vect.mY); }

Vect2 Vect2::operator-(const Vect2& vect) const { return Vect2(mX - vect.mX, mY - vect.mY); }

Vect2& Vect2::operator*=(int a)
{
	mX *= a;
	mY *= a;
	return *this;
}

Vect2& Vect2::operator/=(int div)
{
	assert(div != 0);
	mX /= div;
	mY /= div;
	return *this;
}

bool Vect2::operator==(Vect2& vect) { return (mX == vect.mX) && (mY == vect.mY); }

bool Vect2::operator!=(Vect2& vect) { return !(*this==vect); }


int Vect2::getX() const { return mX; }

int Vect2::getY() const { return mY; }

void Vect2::changeX(int xDelta) { mX += xDelta; }

void Vect2::changeY(int yDelta) { mY += yDelta; }

void Vect2::setX(int x) { mX = x; }

void Vect2::setY(int y) { mY = y; }



CoordsX1Y1WidthHeight::CoordsX1Y1WidthHeight(int x1, int y1, int w, int h) : mX1(x1), mY1(y1), mWidth(w), mHeight(h) {}



CoordsX1X2Y1Y2::CoordsX1X2Y1Y2(int x1, int x2, int y1, int y2) : mX1(x1), mX2(x2), mY1(y1), mY2(y2) {}

CoordsX1X2Y1Y2::CoordsX1X2Y1Y2(int x1, int x2, int y1, int y2, int shiftX, int shiftY) : CoordsX1X2Y1Y2(x1 + shiftX, x2 + shiftX, y1 + shiftY, y2 + shiftY) {}



bool rangeOverlap(int start1, int end1, int start2, int end2) { return !(end1 < start2 or end2 < start1); }

int rangeOverlapDistance(int start1, int end1, int start2, int end2)
{
	int extent1 = (end1 - start1) / 2;
	int center1 = start1 + extent1;
	int extent2 = (end2 - start2) / 2;
	int center2 = start2 + extent2;
	int distCenters = center2 - center1;
	if (abs(distCenters) < extent1 + extent2)
	{
		return extent1 + extent2 - abs(distCenters);
	}
	else
	{
		return 0;
	}
}

int getDistanceBetweenPoints(Vect2 point1, Vect2 point2)
{
	int distanceX = point1.getX() - point2.getX();
	int distanceY = point1.getY() - point2.getY();
	int overallDistance = (int)pow(pow(distanceX, 2) + pow(distanceY, 2), .5f);
	return overallDistance;
}




Hitbox::Hitbox(int x1, int x2, int y1, int y2) 
{
	mHalfWidthHeight = Vect2( (x2-x1) / 2, (y2-y1) / 2 );
	mCenterPos = Vect2(x1, y1);
	mCenterPos += mHalfWidthHeight;

	checkCoordsValidity();
}

Hitbox::Hitbox(Vect2 topLeftInput, int widthInput, int heightInput) 
{
	mHalfWidthHeight = Vect2(widthInput / 2, heightInput / 2);
	mCenterPos = topLeftInput;
	mCenterPos += mHalfWidthHeight;

	checkCoordsValidity();
}

Hitbox::Hitbox(Vect2 topLeftInput, Vect2 bottomRightInput) 
{
	mHalfWidthHeight = Vect2((bottomRightInput.getX() - topLeftInput.getX()) / 2, (bottomRightInput.getY() - topLeftInput.getY()) / 2);
	mCenterPos = topLeftInput;
	mCenterPos += mHalfWidthHeight;

	checkCoordsValidity();
}

Hitbox::Hitbox(CoordsX1Y1WidthHeight coords)
{
	mHalfWidthHeight = Vect2((coords.mWidth / 2), (coords.mHeight / 2));
	mCenterPos = Vect2(coords.mX1 + mHalfWidthHeight.getX(), coords.mY1 + mHalfWidthHeight.getY());
}

Hitbox::Hitbox() { checkCoordsValidity(); }

void Hitbox::checkCoordsValidity() 
{
	if (mHalfWidthHeight.getX() < 0 or mHalfWidthHeight.getY() < 0)
	{
		SDL_assert(false);
	}
}

bool Hitbox::overlap(const Hitbox& otherHitbox) const
{
	int x1 = getTopLeft().getX();
	int x2 = getBottomRight().getX();
	int otherX1 = otherHitbox.getTopLeft().getX();
	int otherX2 = otherHitbox.getBottomRight().getX();

	int y1 = getTopLeft().getY();
	int y2 = getBottomRight().getY();
	int otherY1 = otherHitbox.getTopLeft().getY();
	int otherY2 = otherHitbox.getBottomRight().getY();

	bool xOverlap = rangeOverlap(x1, x2, otherX1, otherX2);
	bool yOverlap = rangeOverlap(y1, y2, otherY1, otherY2);

	return (xOverlap and yOverlap);
}

EBoxSide Hitbox::separate(Hitbox& otherHitbox, bool doSeparate)
{
	int xDelta = rangeOverlapDistance(getTopLeft().getX(), getBottomRight().getX(), otherHitbox.getTopLeft().getX(), otherHitbox.getBottomRight().getX());
	int yDelta = rangeOverlapDistance(getTopLeft().getY(), getBottomRight().getY(), otherHitbox.getTopLeft().getY(), otherHitbox.getBottomRight().getY());
	if (abs(xDelta) < abs(yDelta))
	{
		if (getCenter().getX() < otherHitbox.getCenter().getX())
		{
			if (doSeparate)
			{
				updateTopLeftX(-xDelta);
			}
			return EBoxSide_LEFT;
		}
		else
		{
			if (doSeparate)
			{
				updateTopLeftX(xDelta);
			}
			return EBoxSide_RIGHT;
		}
	}
	else
	{
		if (getCenter().getY() < otherHitbox.getCenter().getY())
		{
			if (doSeparate)
			{
				updateTopLeftY(-yDelta);
			}
			return EBoxSide_TOP;
		}
		else
		{
			if (doSeparate)
			{
				updateTopLeftY(yDelta);
			}
			return EBoxSide_BOTTOM;
		}
	}
}

Vect2 Hitbox::getTopLeft() const { return mCenterPos - mHalfWidthHeight; }

Vect2 Hitbox::getCenter() const { return mCenterPos; }

Vect2 Hitbox::getBottomRight() const { return mCenterPos + mHalfWidthHeight; }

int Hitbox::getWidth() const { return mHalfWidthHeight.getX() * 2; }

int Hitbox::getHeight() const { return mHalfWidthHeight.getY() * 2; }

void Hitbox::updateTopLeft(Vect2 changeTopLeft)  { mCenterPos += changeTopLeft; }

void Hitbox::updateTopLeftX(int x) { mCenterPos.changeX(x); }

void Hitbox::updateTopLeftY(int y) { mCenterPos.changeY(y); }

void Hitbox::setTopLeft(Vect2 newTopLeft)
{
	mCenterPos = newTopLeft;
	mCenterPos += mHalfWidthHeight;
}

void Hitbox::setTopLeftX(int x)
{
	int y = mCenterPos.getY();
	mCenterPos = Vect2(x + mHalfWidthHeight.getX(), y);
}

void Hitbox::setTopLeftY(int y)
{
	int x = mCenterPos.getX();
	mCenterPos = Vect2(x, y + mHalfWidthHeight.getY());
}

void Hitbox::setCenter(Vect2 newCenter) { mCenterPos = newCenter; }

void Hitbox::setWidth(int w)
{
	if (w < 0)
	{
		SDL_assert(false);
	}
	else
	{
		Vect2 topLeft = getTopLeft();
		mHalfWidthHeight.setX(w / 2);
		mCenterPos.setX(topLeft.getX() + mHalfWidthHeight.getX());
	}
}

void Hitbox::setHeight(int h)
{
	if (h < 0)
	{
		SDL_assert(false);
	}
	else
	{
		Vect2 topLeft = getTopLeft();
		mHalfWidthHeight.setY(h / 2);
		mCenterPos.setY(topLeft.getY() + mHalfWidthHeight.getY());
	}
}



EDirection returnOppositeDirection(EDirection givenDirection)
{
	switch (givenDirection) {
		case EDirection_LEFT:
			return EDirection_RIGHT;
			break;
		case EDirection_RIGHT:
			return EDirection_LEFT;
			break;
		case EDirection_UP:
			return EDirection_DOWN;
			break;
		case EDirection_DOWN:
			return EDirection_UP;
			break;
		default:
			return EDirection_NONE;
	}
}



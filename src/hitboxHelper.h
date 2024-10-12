#pragma once
#include <string>
#include <SDL3/SDL.h>
#include "helperClass.h"


enum EBoxSide
{
	EBoxSide_INVALID = -1,
	EBoxSide_TOP,
	EBoxSide_BOTTOM,
	EBoxSide_LEFT,
	EBoxSide_RIGHT,
	EBoxSide_MAX,
};

struct HitboxEdges
{
	EEntityEdgeType mTop;
	EEntityEdgeType mBottom;
	EEntityEdgeType mLeft;
	EEntityEdgeType mRight;

	HitboxEdges(EEntityEdgeType top, EEntityEdgeType bottom, EEntityEdgeType left, EEntityEdgeType right);

	HitboxEdges();
};

//For menus
struct Edges
{
	int mTop    = 0;
	int mBottom = 0;
	int mLeft   = 0;
	int mRight  = 0;

	Edges(){}

	Edges(int top, int bottom, int left, int right)
	{
		mTop    = top;
		mBottom = bottom;
		mLeft   = left;
		mRight  = right;
	}
};

class Vect2
{
private:
	int mX;
	int mY;

public:
	Vect2();
	Vect2(int x, int y);

	Vect2& operator+=(const Vect2& vect);
	Vect2& operator-=(const Vect2& vect);
	Vect2& operator/=(int);

	Vect2 operator+(const Vect2& vect) const;
	Vect2 operator-(const Vect2& vect) const;

	bool operator==(Vect2 vect);

	int getX() const;

	int getY() const;

	void changeX(int xDelta);

	void changeY(int yDelta);

	void setX(int x);

	void setY(int y);

};


class CoordsX1Y1WidthHeight
{
public:
	int mX1; //TODO, is there a way to make this 'const' after declaration??
	int mY1;
	int mWidth;
	int mHeight;


public:
	CoordsX1Y1WidthHeight(int x1Input, int y1Input, int widthInput, int heightInput);

	CoordsX1Y1WidthHeight();

};

class CoordsX1X2Y1Y2
{
private:
	int mX1;
	int mX2;
	int mY1;
	int mY2;


public:
	CoordsX1X2Y1Y2(int x1, int x2, int y1, int y2);

	CoordsX1X2Y1Y2(int x1, int x2, int y1, int y2, int shiftX, int shiftY);

	CoordsX1X2Y1Y2();

	int getX1() const;

	int getX2() const;
	
	int getY1() const;

	int getY2() const;

	int getWidth() const;

	int getHeight() const;
};



bool rangeOverlap(int start1, int end1, int start2, int end2);

int rangeOverlapDistance(int start1, int end1, int start2, int end2);

int getDistanceBetweenPoints(Vect2 point1, Vect2 point2);



class Hitbox 
{
private:
	Vect2 mCenterPos;
	Vect2 mHalfWidthHeight;

public:
	Hitbox(int x1, int x2, int y1, int y2);

	Hitbox(Vect2 topLeftInput, int widthInput, int heightInput);

	Hitbox(Vect2 topLeftInput, Vect2 bottomRightInput);

	Hitbox(CoordsX1Y1WidthHeight coords);

	Hitbox();

	void checkCoordsValidity();

	bool overlap(Hitbox otherHitbox) const;
	EBoxSide separate(Hitbox & otherHitbox, bool doSeparate);

	Vect2 getTopLeft() const;
	Vect2 getCenter() const;
	Vect2 getBottomRight() const;

	int getWidth() const;
	int getHeight() const;

	void updateTopLeft(Vect2 newTopLeft);
	void updateTopLeftX(int x);
	void updateTopLeftY(int y);

	void setTopLeft(Vect2 newTopLeft);
	void setTopLeftX(int x);
	void setTopLeftY(int y);

	void setCenter(Vect2 newCenter);

	void setWidth(int w);
	void setHeight(int h);
};

EDirection returnOppositeDirection(EDirection givenDirection);
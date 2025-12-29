#pragma once
#include <SDL3/SDL.h>
#include "helpers.h"


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
	EEntityEdgeType mTop	= EEntityEdgeType_NEUTRAL;;
	EEntityEdgeType mBottom = EEntityEdgeType_NEUTRAL;;
	EEntityEdgeType mLeft	= EEntityEdgeType_NEUTRAL;;
	EEntityEdgeType mRight	= EEntityEdgeType_NEUTRAL;;

	HitboxEdges() { ; }

	HitboxEdges(EEntityEdgeType top, EEntityEdgeType bottom, EEntityEdgeType left, EEntityEdgeType right);
};

// For menus
struct Edges
{
	int mTop    = 0;
	int mBottom = 0;
	int mLeft   = 0;
	int mRight  = 0;

	Edges(){}

	Edges(int top, int bottom, int left, int right);
};

class Vect2
{
private:
	int mX = 0;
	int mY = 0;

public:
	Vect2() { ; }
	Vect2(int x, int y);
	Vect2(float x, int y);
	Vect2(int x, float y);
	Vect2(float x, float y);

	Vect2& operator+=(const Vect2& vect);
	Vect2& operator-=(const Vect2& vect);
	Vect2& operator*=(int);
	Vect2& operator/=(int);

	Vect2 operator+(const Vect2& vect) const;
	Vect2 operator-(const Vect2& vect) const;

	bool operator==(Vect2& vect);
	bool operator!=(Vect2& vect);

	int getX() const;
	int getY() const;

	void changeX(int xDelta);
	void changeY(int yDelta);

	void setX(int x);
	void setY(int y);
};


struct CoordsX1Y1WidthHeight
{
	int mX1 = 0; 
	int mY1 = 0;
	int mWidth = 0;
	int mHeight = 0;

	CoordsX1Y1WidthHeight(int x1Input, int y1Input, int widthInput, int heightInput);
	CoordsX1Y1WidthHeight() { ; }
};

struct CoordsX1X2Y1Y2
{
	int mX1 = 0;
	int mX2 = 0;
	int mY1 = 0;
	int mY2 = 0;

	CoordsX1X2Y1Y2(int x1, int x2, int y1, int y2);
	CoordsX1X2Y1Y2(int x1, int x2, int y1, int y2, int shiftX, int shiftY);
	CoordsX1X2Y1Y2() { ; }
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

	bool overlap(const Hitbox& otherHitbox) const;
	EBoxSide separate(Hitbox & otherHitbox, bool doSeparate);

	Vect2 getTopLeft() const;
	Vect2 getCenter() const;
	Vect2 getBottomRight() const;

	int getWidth() const;
	int getHeight() const;

	void updateTopLeft(Vect2 changeTopLeft);
	void updateTopLeftX(int changeX);
	void updateTopLeftY(int changeY);

	void setTopLeft(Vect2 newTopLeft);
	void setTopLeftX(int x);
	void setTopLeftY(int y);

	void setCenter(Vect2 newCenter);

	void setWidth(int w);
	void setHeight(int h);
};

EDirection returnOppositeDirection(EDirection givenDirection);


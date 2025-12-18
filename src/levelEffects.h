#pragma once
#include "hitboxHelper.h"

class CircleEffect
{

private:
    Vect2     mCenter;
    Vect2     mLimitTopLeft;
    Vect2     mLimitBottomRight;
public:

    Vect2     mCurCenter;
    SDL_Color mColor;
    int       mRadius = -1;

    int       mNumTicks = 5;
    int const mSpaceBetweenTicks = 5;
    int       mCurSpaceBetweenTicks = mSpaceBetweenTicks;

    int        mCurXMove = 5;
    EDirection mCurDirectionX = EDirection_LEFT;
    int        mCurYMove = 5;
    EDirection mCurDirectionY = EDirection_DOWN;


    CircleEffect(Vect2 center, SDL_Color color, int radius, Vect2 limitTopLeft, Vect2 limitBottomRight);

    void move();
};
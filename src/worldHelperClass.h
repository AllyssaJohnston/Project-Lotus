#pragma once
#include "hitboxHelper.h"

class CanGoToNextLevelResults
{
public:
	int  mNextWorldNumber   = -1;
	int  mNextLevelNumber   = -1;
	bool mCanGoToNextLevel  = false;
	
	CanGoToNextLevelResults(int nextWorldNumber, int nextLevelNumber, bool canGoToNextLevel);
};

class LevelInfo
{
public:
	bool mDoubleJumpAllowed                 = false;
	bool mThrowProjectileAllowed            = false;
	bool mThrowDownwardProjectileAllowed    = false;
	bool mSlashAllowed                      = false;
	bool mMustKillAllEnemies                = false;

	LevelInfo(bool doubleJumpAllowed, bool throwProjectileAllowed, bool throwDownwardProjectileAllowed, bool slashAllowed, 
            bool mustKillAllEnemies);
};

class CircleEffect
{

private:
    Vect2     mCenter;
    Vect2     mLimitTopLeft;
    Vect2     mLimitBottomRight;
public:
 
    Vect2     mCurCenter;
    SDL_Color mColor;
    int       mRadius               = -1;

    int       mNumTicks             = 5;
    int const mSpaceBetweenTicks    = 5;
    int       mCurSpaceBetweenTicks = mSpaceBetweenTicks;

    int        mCurXMove            = 5;
    EDirection mCurDirectionX       = EDirection_LEFT;
    int        mCurYMove            = 5;
    EDirection mCurDirectionY       = EDirection_DOWN;


    CircleEffect(Vect2 center, SDL_Color color, int radius, Vect2 limitTopLeft, Vect2 limitBottomRight);

    void move();
};
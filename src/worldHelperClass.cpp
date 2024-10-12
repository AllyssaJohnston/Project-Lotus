#include "worldHelperClass.h"

CanGoToNextLevelResults::CanGoToNextLevelResults(int nextWorldNumber, int nextLevelNumber, bool canGoToNextLevel)
{
	mNextWorldNumber  = nextWorldNumber;
	mNextLevelNumber  = nextLevelNumber;
	mCanGoToNextLevel = canGoToNextLevel;
}

LevelInfo::LevelInfo(bool doubleJumpAllowed, bool throwProjectileAllowed, bool throwDownwardProjectileAllowed, bool slashAllowed, 
		bool mustKillAllEnemies)
{
	mDoubleJumpAllowed				= doubleJumpAllowed;
	mThrowProjectileAllowed			= throwProjectileAllowed;
	mThrowDownwardProjectileAllowed = throwDownwardProjectileAllowed;
	mSlashAllowed					= slashAllowed;
	mMustKillAllEnemies				= mustKillAllEnemies;
}

CircleEffect::CircleEffect(Vect2 center, SDL_Color color, int radius, Vect2 limitTopLeft, Vect2 limitBottomRight)
{
	mCenter    = center;
	mCurCenter = center;
	mColor     = color;

	mRadius    = radius;
	mLimitTopLeft     = limitTopLeft;
	mLimitBottomRight = limitBottomRight;
}

void CircleEffect::move()
{
    mCurSpaceBetweenTicks -= 1;
    if (mCurSpaceBetweenTicks <= 0)
    {
        mNumTicks -= 1;
        if (mNumTicks <= 0)
        {
            //update move
            bool flipX = (rand() % 10) < 1;
            if (flipX)
            {
                if (mCurDirectionX == EDirection_LEFT)
                {
                    mCurDirectionX = EDirection_RIGHT;
                }
                else
                {
                    mCurDirectionX = EDirection_LEFT;
                }
            }
            bool flipY = (rand() % 10) < 1;
            if (flipY)
            {
                if (mCurDirectionY == EDirection_UP)
                {
                    mCurDirectionY = EDirection_DOWN;
                }
                else
                {
                    mCurDirectionY = EDirection_UP;
                }
            }

            mCurXMove = rand() % 2;
            mCurYMove = rand() % 2;
            mNumTicks = rand() % 10;
        }

        //move
        if (mCurDirectionX == EDirection_LEFT)
        {
            mCurCenter.changeX(-mCurXMove);
        }
        else
        {
            mCurCenter.changeX(mCurXMove);
        }
        if (mCurDirectionY == EDirection_UP)
        {
            mCurCenter.changeY(-mCurYMove);
        }
        else
        {
            mCurCenter.changeY(mCurYMove);
        }

        //collisions
        if (mCurCenter.getX() < mLimitTopLeft.getX())
        {
            mCurCenter.setX(mLimitTopLeft.getX());
            if (mCurDirectionX == EDirection_LEFT)
            {
                mCurDirectionX = EDirection_RIGHT;
            }
        }
        else if (mCurCenter.getY() < mLimitTopLeft.getY())
        {
            mCurCenter.setY(mLimitTopLeft.getY());
            if (mCurDirectionY == EDirection_UP)
            {
                mCurDirectionY = EDirection_DOWN;
            }
        }

        if (mCurCenter.getX() > mLimitBottomRight.getX())
        {
            mCurCenter.setX(mLimitBottomRight.getX());
            if (mCurDirectionX == EDirection_RIGHT)
            {
                mCurDirectionX = EDirection_LEFT;
            }
        }
        else if (mCurCenter.getY() > mLimitBottomRight.getY())
        {
            mCurCenter.setY(mLimitBottomRight.getY());
            if (mCurDirectionY == EDirection_DOWN)
            {
                mCurDirectionY = EDirection_UP;
            }
        }

        mCurSpaceBetweenTicks = mSpaceBetweenTicks;
    }
}
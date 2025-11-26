#pragma once
#include "hitboxHelper.h"

#define GO_TO_MAIN_WORLD -1

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

enum ELevelType
{
    ELevelType_INVALID = -1,
    ELevelType_PLATFORMING,
    ELevelType_MINI_GAME,
    ELevelType_MAX
};

struct LevelData
{
    ELevelType mType = ELevelType_INVALID;
    int mWorldNumber = -1;
    int mLevelNumber = -1;

    LevelData(ELevelType type, int worldNumber, int levelNumber);

    LevelData(int worldNumber, int levelNumber);

    LevelData(int levelNumber);
};

class CanGoToNextLevelResults
{
public:
    LevelData* mpNextLevelData = nullptr;
    bool mCanGoToNextLevel = false;

    CanGoToNextLevelResults() { ; }

    CanGoToNextLevelResults(LevelData* pNextLevelData);
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

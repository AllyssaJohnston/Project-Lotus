#pragma once
#include "hitbox.h"

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
    int mStageNumber = -1;

    LevelData(ELevelType type, int worldNumber, int levelNumber, int stageNumber);

    // platforming
    LevelData(int worldNumber, int levelNumber);

    // mini game
    LevelData(int worldNumber, int levelNumber, int stageNumber);
};

class CanGoToNextLevelResults
{
public:
    LevelData* mpNextLevelData = nullptr;
    bool mCanGoToNextLevel = false;

    CanGoToNextLevelResults() { ; }

    CanGoToNextLevelResults(LevelData* pNextLevelData);
};



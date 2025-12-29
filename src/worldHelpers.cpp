#include "worldHelpers.h"

CanGoToNextLevelResults::CanGoToNextLevelResults(LevelData* pNextLevelData) : mpNextLevelData(pNextLevelData) {  mCanGoToNextLevel = true; }

LevelInfo::LevelInfo(bool doubleJumpAllowed, bool throwProjectileAllowed, bool throwDownwardProjectileAllowed, bool slashAllowed, 
		bool mustKillAllEnemies)
{
	mDoubleJumpAllowed				= doubleJumpAllowed;
	mThrowProjectileAllowed			= throwProjectileAllowed;
	mThrowDownwardProjectileAllowed = throwDownwardProjectileAllowed;
	mSlashAllowed					= slashAllowed;
	mMustKillAllEnemies				= mustKillAllEnemies;
}

LevelData::LevelData(ELevelType type, int worldNumber, int levelNumber, int stageNumber) : mType(type), mWorldNumber(worldNumber), mLevelNumber(levelNumber), mStageNumber(stageNumber) { ; }

LevelData::LevelData(int worldNumber, int levelNumber) :  mWorldNumber(worldNumber), mLevelNumber(levelNumber) {  mType = ELevelType_PLATFORMING;  }

LevelData::LevelData(int worldNumber, int levelNumber, int stageNumber) : mWorldNumber(worldNumber), mLevelNumber(levelNumber), mStageNumber(stageNumber)
        { mType = ELevelType_MINI_GAME; }


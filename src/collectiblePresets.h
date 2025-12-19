#pragma once
#include "SDL3/SDL.h"
#include "vector"
#include "helperClass.h"
#include "imageHelper.h"
#include "hitboxHelper.h"
#include "entityPresets.h"
#include "worldHelperClass.h"

struct CCollectiblePreset : public EntityPreset { CCollectiblePreset(); };

struct CKeyPreset : public CCollectiblePreset { CKeyPreset(); };

struct CSavePointPreset : public CCollectiblePreset { CSavePointPreset(); };
static CSavePointPreset gStaticSavePointPreset;

struct CLotusCollectiblePreset : public CCollectiblePreset { CLotusCollectiblePreset(); };
static CLotusCollectiblePreset gStaticLotusCollectiblePreset;

struct CEndOfLevelPreset : public CCollectiblePreset
{
	CEndOfLevelPreset(int worldType);

	virtual void setUpAnimationPresets(int worldType);
};

struct CMiniGameLevelPreset : public CCollectiblePreset
{
	LevelData nextLevelData;
	CMiniGameLevelPreset(EEntityCharacterTypes enemyType, LevelData givenNextLevelData);

	void setUpAnimationPresets(EEntityCharacterTypes enemyType);
};
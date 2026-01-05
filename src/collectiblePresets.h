#pragma once
#include "entityPresets.h"
#include "worldHelpers.h"


struct CCollectiblePreset : public EntityPreset { CCollectiblePreset(); };

struct CKeyPreset : public CCollectiblePreset { CKeyPreset(); };

struct CSavePointPreset : public CCollectiblePreset { CSavePointPreset(); };
static CSavePointPreset gStaticSavePointPreset;

struct CLotusCollectiblePreset : public CCollectiblePreset { CLotusCollectiblePreset(); };
static CLotusCollectiblePreset gStaticLotusCollectiblePreset;

struct CEndOfLevelPreset : public CCollectiblePreset { CEndOfLevelPreset(bool vertical); };

struct CMiniGameLevelPreset : public CCollectiblePreset
{
	LevelData nextLevelData;
	CMiniGameLevelPreset(EEntityCharacterTypes enemyType, LevelData givenNextLevelData);

	void setUpAnimationPresets(EEntityCharacterTypes enemyType);
};
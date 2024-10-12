#pragma once
#include "SDL3/SDL.h"
#include "vector"
#include "helperClass.h"
#include "imageHelper.h"
#include "hitboxHelper.h"
#include "entityPresets.h"

struct CCollectiblePreset : public CCharacterPreset
{
	CCollectiblePreset();
};

struct CKeyPreset : public CCollectiblePreset
{
	CKeyPreset();
};

struct CSavePointPreset : public CCollectiblePreset
{
	CSavePointPreset();
};
static CSavePointPreset gStaticSavePointPreset;

struct CLotusCollectiblePreset : public CCollectiblePreset
{
	CLotusCollectiblePreset();
};
static CLotusCollectiblePreset gStaticLotusCollectiblePreset;

struct CEndOfLevelPreset : public CCollectiblePreset
{
	CEndOfLevelPreset(int worldType);

	void setUpAnimationPresets(int worldType);
};

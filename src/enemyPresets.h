#pragma once
#include "helperClass.h"
#include "hitboxHelper.h"
#include "movementHelperClass.h"
#include "imageHelper.h"
#include "entityPresets.h"
#include <SDL3/SDL.h>

struct EEnemyPreset : public CCharacterPreset
{
	EEntityMovementPath mProjectilePath		= EEntityMovementPath_HORIZONTAL; 
	int					mProjectileInterval = 0; //interval for shooting projectiles

	EEnemyPreset();
};


//EARTH ENEMIES

//DRAGUAR BLOSSOM
struct ERatPreset : public EEnemyPreset
{
	ERatPreset();
};

//SEEDLING
struct EFastRatPreset : public EEnemyPreset
{
	EFastRatPreset();
};

//SHROOM VIPER
struct EBouncyRatPreset : public EEnemyPreset
{
	EBouncyRatPreset();
};

//FROCODILE
struct EFrogPreset : public EEnemyPreset
{
	EFrogPreset();
};

//FROG SHROOM
struct EBouncyFrogPreset : public EEnemyPreset
{
	EBouncyFrogPreset();
};

//SPIDERIG
struct EHidingRatPreset : public EEnemyPreset
{
	EHidingRatPreset();
};




//FIRE ENEMIES

//FIRMUS
struct EFireRatPreset : public EEnemyPreset
{
	EFireRatPreset();
};

//FIERLINE
struct EFireBatVPreset : public EEnemyPreset
{
	EFireBatVPreset();
};

//FIRIKIN
struct EFireFrogPreset : public EEnemyPreset
{
	EFireFrogPreset();
};

//SCORPUS
struct ESnailPreset : public EEnemyPreset
{
	ESnailPreset();
};

//GEM BUG
struct ESpikeySnailPreset : public EEnemyPreset
{
	ESpikeySnailPreset();
};

//GEM WALKER
struct ESpikeySnailChameleonPreset : public EEnemyPreset
{
	ESpikeySnailChameleonPreset();
};



//WATER ENEMIES

//GLATA
struct EBatHPreset : public EEnemyPreset
{
	EBatHPreset();
};

//SERPHISH
struct EBatVPreset : public EEnemyPreset
{
	EBatVPreset();
};

//JELISH
struct EBatDPreset : public EEnemyPreset
{
	EBatDPreset();
};

//SHELL BEAST
struct EHidingSnailPreset : public EEnemyPreset
{
	EHidingSnailPreset();
};

//SPINE SHELL
struct EGrabbingSnailPreset : public EEnemyPreset
{
	EGrabbingSnailPreset();
};



//AIR ENEMIES

//MOTHA
struct EWalkingFrogPreset : public EEnemyPreset
{
	EWalkingFrogPreset();
};

//SWOOPER
struct ESlowBatHPreset : public EEnemyPreset
{
	ESlowBatHPreset();
};

//FELIGA
struct EProjectileBatVPreset : public EEnemyPreset
{
	EProjectileBatVPreset();
};

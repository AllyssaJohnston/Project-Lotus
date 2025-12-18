#pragma once
#include <SDL3/SDL.h>
#include "helperClass.h"
#include "hitboxHelper.h"
#include "movementHelperClass.h"
#include "imageHelper.h"
#include "entityPresets.h"


struct EEnemyPreset : public EntityPreset
{
	EEntityMovementPath mProjectilePath		= EEntityMovementPath_HORIZONTAL; 
	int					mProjectileInterval = 0; // interval for shooting projectiles

	EEnemyPreset();
};


// EARTH ENEMIES
struct ERatPreset : public EEnemyPreset { ERatPreset(); };					// DRAGUAR BLOSSOM
struct EFastRatPreset : public EEnemyPreset { EFastRatPreset(); };			// SEEDLING
struct EBouncyRatPreset : public EEnemyPreset { EBouncyRatPreset(); };		// SHROOM VIPER
struct EFrogPreset : public EEnemyPreset { EFrogPreset(); };				// FROCODILE
struct EBouncyFrogPreset : public EEnemyPreset { EBouncyFrogPreset(); };	// FROG SHROOM
struct EHidingRatPreset : public EEnemyPreset { EHidingRatPreset(); };		// SPIDERIG


//FIRE ENEMIES
struct EFireRatPreset : public EEnemyPreset { EFireRatPreset(); };								// FIRMUS
struct EFireBatVPreset : public EEnemyPreset { EFireBatVPreset(); };							// FIERLINE
struct EFireFrogPreset : public EEnemyPreset { EFireFrogPreset(); };							// FIRIKIN
struct ESnailPreset : public EEnemyPreset { ESnailPreset(); };									// SCORPUS
struct ESpikeySnailPreset : public EEnemyPreset { ESpikeySnailPreset(); };						// GEM BUG
struct ESpikeySnailChameleonPreset : public EEnemyPreset { ESpikeySnailChameleonPreset(); };	// GEM WALKER


//WATER ENEMIES
struct EBatHPreset : public EEnemyPreset { EBatHPreset(); };					// GLATA
struct EBatVPreset : public EEnemyPreset { EBatVPreset(); };					// SERPHISH
struct EBatDPreset : public EEnemyPreset { EBatDPreset(); };					// JELISH
struct EHidingSnailPreset : public EEnemyPreset { EHidingSnailPreset(); };		// SHELL BEAST
struct EGrabbingSnailPreset : public EEnemyPreset { EGrabbingSnailPreset(); };	// SPINE SHELL



//AIR ENEMIES
struct EWalkingFrogPreset : public EEnemyPreset { EWalkingFrogPreset(); };			// MOTHA
struct ESlowBatHPreset : public EEnemyPreset { ESlowBatHPreset(); };				// SWOOPER
struct EProjectileBatVPreset : public EEnemyPreset { EProjectileBatVPreset(); };	//FELIGA

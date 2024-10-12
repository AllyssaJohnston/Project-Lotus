#pragma once
#include "helperClass.h"
#include "EntityPresets.h"

struct PPlatformPreset : public CCharacterPreset
{
	std::vector <AnimationPreset> mHorizontalAnimationPresets;
	std::vector <AnimationPreset> mVerticalAnimationPresets;
	HitboxEdges					  mHiddenHitboxEdges;
	bool						  mPrintViaChunk = true;
	bool						  mSplice        = false;
	EHowToDetermineWidthHeight    mHowToDetermineWidthHeight = EHowToDetermineWidthHeight_CHUNK_IMAGES;

	PPlatformPreset();
};

struct PStandardPreset : public PPlatformPreset
{
	PStandardPreset(int worldNumber);

	void setUpAnimationPresets(EWorldType worldType);
};

struct PSolidPreset : public PPlatformPreset
{
	PSolidPreset(int worldNumber);

	void setUpAnimationPresets(EWorldType worldType);
};

struct PStickyPreset : public PPlatformPreset
{
	PStickyPreset();
};

struct PIcyPreset : public PPlatformPreset
{
	PIcyPreset();
};

struct PMagneticPreset: public PPlatformPreset
{
	PMagneticPreset();
};

struct PCrumblingPreset : public PPlatformPreset
{
	HitboxEdges mHiddenHitboxEdges;

	PCrumblingPreset();
};

struct PSpikePreset : public PPlatformPreset
{
	PSpikePreset();
};

struct PLavaPreset : public PPlatformPreset
{
	PLavaPreset();
};

struct PElectricPreset : public PPlatformPreset
{
	PElectricPreset();
};

struct PBouncyPreset : public PPlatformPreset
{
	PBouncyPreset();
};

struct PWallJumpablePreset : public PPlatformPreset
{
	PWallJumpablePreset();
};


struct PGatePreset : public PPlatformPreset 
{
	PGatePreset(int worldNumber);

	void setUpAnimationPresets(EWorldType worldType);
};

struct PTargetGatePreset : public PGatePreset 
{
	PTargetGatePreset(int worldNumber);

	void setUpAnimationPresets(EWorldType worldType);
};

struct PPressureOperatedGatePreset : public PGatePreset
{
	PPressureOperatedGatePreset();
};


struct PTargetPreset : public PPlatformPreset
{
	PTargetPreset(int worldNumber);

	void setUpAnimationPresets(EWorldType worldType);
};


struct PCratePreset : public PPlatformPreset
{
	PCratePreset();
};

struct PMagneticCratePreset : public PCratePreset
{
	PMagneticCratePreset();
};

struct PArmoredCratePreset : public PCratePreset
{
	PArmoredCratePreset();
};


struct PPressurePlatePreset : public PPlatformPreset
{
	PPressurePlatePreset();
};


struct PMovingPreset : public PPlatformPreset
{
	PMovingPreset(int worldNumber);

	void setUpAnimationPresets(EWorldType worldType);
};


struct PAreaEffectPlatformPreset : public PPlatformPreset
{
	std::vector <AnimationPreset> mUpAnimationPresets;
	std::vector <AnimationPreset> mDownAnimationPresets;
	std::vector <AnimationPreset> mLeftAnimationPresets;
	std::vector <AnimationPreset> mRightAnimationPresets;
	std::vector <AnimationPreset> mAreaEffectUpAnimationPresets;
	std::vector <AnimationPreset> mAreaEffectDownAnimationPresets;
	std::vector <AnimationPreset> mAreaEffectLeftAnimationPresets;
	std::vector <AnimationPreset> mAreaEffectRightAnimationPresets;
	int mAreaEffectMovement = 0;

	PAreaEffectPlatformPreset();

};

struct PWindGustPlatformPreset : public PAreaEffectPlatformPreset
{
	PWindGustPlatformPreset();

	void setUpAnimationPresets();
};
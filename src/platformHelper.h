#pragma once
#include <string>
#include "helperClass.h"
#include "imageHelper.h"
#include "hitboxHelper.h"
#include "platformPresets.h"
#include "entityHelper.h"
#include "movementHelper.h"

class Platform : public Entity
{
protected:
	CoordsX1X2Y1Y2		mCoords;
	PPlatformPreset*	mpPreset	= nullptr;
	bool				mIsMoveable = false;

	Platform();

public:
	bool mPrintViaChunk				= false;
	bool mSplice					= false;
	int  mCodeNumber				= -1;
	EHowToDetermineWidthHeight mHowToDetermineWidthHeight = EHowToDetermineWidthHeight_INVALID;

	Platform(CoordsX1X2Y1Y2 coordsInput, PPlatformPreset* preset);

	~Platform();

	void setUpPlatformBaseStats(CoordsX1X2Y1Y2 coordsInput, PPlatformPreset* preset);


	virtual bool isMoveable() const;


	virtual void hide();

	virtual void activate();

	virtual void startCrumble();


	PPlatformPreset* getPreset() const;


	virtual void setCheckpointStats() override;


	virtual void preTick() override;

	virtual void tick() override;

	virtual void postTick() override;

	virtual void died() override;

};

class Crate : public Platform
{
public:

	Crate(CoordsX1X2Y1Y2 coordsInput, PCratePreset* preset);

	~Crate();

	void tick() override;

	void postTick() override;

	void setCheckpointStats() override;
};

class Crumbling : public Platform
{
private: 
	HitboxEdges  mStartingHitboxEdges;
	HitboxEdges  mCheckpointHitboxEdges;
	HitboxEdges  mHiddenHitboxEdges;
	bool mIsCrumbling			= false;
	bool mCheckpointIsCrumbling = false;

public:
	Crumbling(CoordsX1X2Y1Y2 coordsInput, PCrumblingPreset* preset);

	~Crumbling();

	void hide() override;

	void startCrumble();

	void resetStats() override;

	void resetToCheckpoint() override;

	void setCheckpointStats() override;

	void postTick() override;
};

class Gate : public Platform
{
private: 
	HitboxEdges  mStartingHitboxEdges;
	HitboxEdges  mCheckpointHitboxEdges;
	HitboxEdges  mHiddenHitboxEdges;
	int          mFramesSinceUnhidden = 0;

public:
	Gate(CoordsX1X2Y1Y2 coordsInput, PGatePreset* preset);
	Gate(CoordsX1X2Y1Y2 coordsInput, PGatePreset* preset, int codeNumber);

	~Gate();

	void setUpGate(CoordsX1X2Y1Y2 coordsInput, PGatePreset* preset);

	void preTick() override;

	void hide() override;

	void resetStats() override;

	void resetToCheckpoint() override;

	void setCheckpointStats() override;

};

class Target : public Platform
{
private: 
	HitboxEdges  mStartingHitboxEdges;
	HitboxEdges  mCheckpointHitboxEdges;
	HitboxEdges  mHiddenHitboxEdges;

public:
	Target(CoordsX1X2Y1Y2 coordsInput, PTargetPreset* preset);

	~Target();

	void hide() override;

	void resetStats() override;

	void resetToCheckpoint() override;

	void setCheckpointStats() override;
};

class PressurePlate : public Platform
{
private: 
	int mFramesSinceUnactivated = -1;

public:

	PressurePlate(CoordsX1X2Y1Y2 coordsInput, PPressurePlatePreset* preset, int codeNumber);
	
	~PressurePlate();

	void preTick() override;

	void activate() override;

	void resetStats() override;

	void resetToCheckpoint() override;

	void setCheckpointStats() override;
};

class MovingPlatform : public Platform
{
public:
	
	MovingPlatform(CoordsX1X2Y1Y2 coordsInput, PMovingPreset* preset, EEntityMovementPath movementPath);

	~MovingPlatform();
};

class AreaEffectPlatform : public Platform
{
public:
	//mMovementManager for actual platform
	Hitbox		mAreaEffectHitbox; //Hitbox for area effect
	EDirection	mEffectDirection	= EDirection_INVALID;
	int			mAreaEffectMovement	= -1;

	//mAnimationManager platform animation
	AnimationManager mAreaEffectAnimationManager; //animation for effect

	AreaEffectPlatform(CoordsX1X2Y1Y2 hitboxCoords, CoordsX1X2Y1Y2 areaEffectCoords, EDirection effectDirection, 
			PAreaEffectPlatformPreset* preset);

	~AreaEffectPlatform();
};
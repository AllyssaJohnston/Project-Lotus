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


	virtual void setCheckpointStats();


	virtual void preTick();

	virtual void tick();

	virtual void postTick();

};

class Crate : public Platform
{
public:

	Crate(CoordsX1X2Y1Y2 coordsInput, PCratePreset* preset);

	~Crate();

	void tick();

	void postTick();

	void setCheckpointStats();
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

	void hide();

	void startCrumble();

	void resetStats();

	void resetToCheckpoint();

	void setCheckpointStats();

	void postTick();
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

	void preTick();

	void hide();

	void resetStats();

	void resetToCheckpoint();

	void setCheckpointStats();

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

	void hide();

	void resetStats();

	void resetToCheckpoint();

	void setCheckpointStats();
};

class PressurePlate : public Platform
{
private: 
	int mFramesSinceUnactivated = -1;

public:

	PressurePlate(CoordsX1X2Y1Y2 coordsInput, PPressurePlatePreset* preset, int codeNumber);
	
	~PressurePlate();

	void preTick();

	void activate();

	void resetStats();

	void resetToCheckpoint();

	void setCheckpointStats();
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
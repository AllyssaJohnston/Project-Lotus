#pragma once
#include <string>
#include "platformPresets.h"
#include "entity.h"

class Platform : public Entity
{
protected:
	CoordsX1X2Y1Y2		mCoords;
	bool				mIsMoveable = false;

	Platform() { ; } // subclasses are in charge of their own setup

public:
	bool mPrintViaChunk				= false;
	bool mSplice					= false;
	int  mCodeNumber				= -1;
	EHowToDetermineWidthHeight mHowToDetermineWidthHeight = EHowToDetermineWidthHeight_INVALID;

	Platform(const CoordsX1X2Y1Y2 coordsInput, const PPlatformPreset& preset);

	~Platform();

	void setUpPlatformBaseStats(const CoordsX1X2Y1Y2& coordsInput, const PPlatformPreset& preset);


	virtual bool isMoveable() const;


	virtual void hide();

	virtual void activate();

	virtual void startCrumble();


	virtual void setCheckpointStats() override;


	virtual void preTick() override;

	virtual void tick() override;

	virtual void postTick() override;

	virtual void died() override;

};

class Crate : public Platform
{
public:

	Crate(const CoordsX1X2Y1Y2 coordsInput, const PCratePreset& preset);

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
	Crumbling(const CoordsX1X2Y1Y2 coordsInput, const PCrumblingPreset& preset);

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
	Gate(const CoordsX1X2Y1Y2 coordsInput, const PGatePreset& preset);
	Gate(const CoordsX1X2Y1Y2 coordsInput, const PGatePreset& preset, int codeNumber);

	~Gate();

	void setUpGate(const CoordsX1X2Y1Y2& coordsInput, const PGatePreset& preset);

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
	Target(const CoordsX1X2Y1Y2 coordsInput, const PTargetPreset& preset);

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

	PressurePlate(const CoordsX1X2Y1Y2 coordsInput, const PPressurePlatePreset& preset, int codeNumber);
	
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
	
	MovingPlatform(const CoordsX1X2Y1Y2 coordsInput, const PMovingPreset& preset, EEntityMovementPath movementPath);

	~MovingPlatform();
};

class AreaEffectPlatform : public Platform
{
public:
	Hitbox		mAreaEffectHitbox; // Hitbox for area effect only
	EDirection	mEffectDirection	= EDirection_INVALID;
	int			mAreaEffectMovement	= -1;

	AnimationManager mAreaEffectAnimationManager; // animation for area effect only

	AreaEffectPlatform(const CoordsX1X2Y1Y2 hitboxCoords, const CoordsX1X2Y1Y2 areaEffectCoords, EDirection effectDirection,
			const PAreaEffectPlatformPreset& preset);

	~AreaEffectPlatform();
};
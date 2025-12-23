#include "platform.h"

Platform::Platform(const CoordsX1X2Y1Y2 coordsInput, const PPlatformPreset& preset)
{
	setUpPlatformBaseStats(coordsInput, preset);

	int width  = coordsInput.mX2 - coordsInput.mX1;
	int height = coordsInput.mY2 - coordsInput.mY1;
	mMovementManager.setupMovementManager(Vect2(coordsInput.mX1, coordsInput.mY1), preset, width, height);
	std::vector <AnimationPreset> animationPresets;
	if (height > width)
	{
		animationPresets = preset.mVerticalAnimationPresets;
	}
	else
	{
		animationPresets = preset.mHorizontalAnimationPresets;
	}

	if (animationPresets.size() > 0)
	{
		mAnimationManager.setupAnimationManager(animationPresets,   width + preset.mExtraWidth, height + preset.mExtraHeight, mHowToDetermineWidthHeight);
	}
}

Platform::~Platform() { Entity::~Entity(); }

void Platform::setUpPlatformBaseStats(const CoordsX1X2Y1Y2& coordsInput, const PPlatformPreset& preset)
{
	setUpBaseStats(preset);
	mAmAlive = true;
	mMovementEffect   = preset.mMovementEffect;
	mCoords		      = coordsInput;
	mPrintViaChunk    = preset.mPrintViaChunk;
	mSplice           = preset.mSplice;
	mHowToDetermineWidthHeight = preset.mHowToDetermineWidthHeight;

	if ((!mPrintViaChunk 	and mHowToDetermineWidthHeight == EHowToDetermineWidthHeight_CHUNK_IMAGES) 	or (mPrintViaChunk 	and mHowToDetermineWidthHeight != EHowToDetermineWidthHeight_CHUNK_IMAGES))
	{
		SDL_assert(false);
	}
	if ((!mSplice 			and mHowToDetermineWidthHeight == EHowToDetermineWidthHeight_SPLICE) 		or (mSplice 		and mHowToDetermineWidthHeight != EHowToDetermineWidthHeight_SPLICE))
	{
		SDL_assert(false);
	}
}


bool Platform::isMoveable() const { return mIsMoveable; }


void Platform::hide() { ; }

void Platform::activate() { ; }

void Platform::startCrumble() { ; }


void Platform::setCheckpointStats()
{
	mMovementManager.setCheckpointPosition();
}


void Platform::preTick()
{
	Entity::preTick();
	mMovementManager.preTick();

}

void Platform::tick()
{
	Entity::tick();
	mMovementManager.calcMovement();
}

void Platform::postTick()
{
	Entity::postTick();
	mMovementManager.postTick();
	if (mMovementManager.getCurMovementState() == EMovementStateIndex_JUMPING)
	{

	}
	else
	{
		mMovementManager.setMovementStateToCharacterMode();
	}
}

void Platform::died() 
{ 
	if (mEntityType == EEntityType_NON_STATIC) 
	{
		Entity::died();
	}
	else 
	{
		mAmAlive = true;
	}
}



Crate::Crate(const CoordsX1X2Y1Y2 coordsInput, const PCratePreset& preset) : Platform()
{
	setUpPlatformBaseStats(coordsInput, preset);
	int width = coordsInput.mX2 - coordsInput.mX1;
	int height = coordsInput.mY2 - coordsInput.mY1;
	mMovementManager.setupMovementManager(Vect2(coordsInput.mX1, coordsInput.mY1), preset, width, height);
	mAnimationManager.setupAnimationManager(preset.mHorizontalAnimationPresets, width + preset.mExtraWidth, height + preset.mExtraHeight, mHowToDetermineWidthHeight);
	mIsMoveable = true;
}

Crate::~Crate() { Platform::~Platform(); }

void Crate::tick()
{
	Entity::tick();
	if (mMovementManager.isOnGround())
	{
		if (mMovementManager.inGroundCharacteristics(EEntityCharacteristicsTypes_SLIPPERY))
		{
			// on slippery
			mMovementManager.getMovementStates()[EMovementStateIndex_WALKING]->autoMove();
		}
	}
	mMovementManager.calculateXDirection();
	mMovementManager.calcMovement();
}

void Crate::postTick() 
{ 
	Entity::postTick();
	mMovementManager.postTick();
}

void Crate::setCheckpointStats()
{
	mCheckpointAmAlive = mAmAlive;
	mMovementManager.setCheckpointPosition();
}





Crumbling::Crumbling(const CoordsX1X2Y1Y2 coordsInput, const PCrumblingPreset& preset) : Platform()
{
	setUpPlatformBaseStats(coordsInput, preset);
	mCheckpointIsVisible   = true;

	int width			   = coordsInput.mX2 - coordsInput.mX1;
	int height		   	   = coordsInput.mY2 - coordsInput.mY1;
	mMovementManager.setupMovementManager(Vect2(coordsInput.mX1, coordsInput.mY1), preset, width, height);

	mStartingHitboxEdges   = preset.mHitboxEdges;
	mHiddenHitboxEdges	   = preset.mHiddenHitboxEdges;

	mAnimationManager.setupAnimationManager(preset.mHorizontalAnimationPresets, width + preset.mExtraWidth, height + preset.mExtraHeight, mHowToDetermineWidthHeight);
}

Crumbling::~Crumbling() { Platform::~Platform(); }

void Crumbling::hide()
{
	mMovementManager.permSetHitboxEdges(mHiddenHitboxEdges);
	mIsVisible = false;
}

void Crumbling::startCrumble()
{
	mIsCrumbling = true;
}

void Crumbling::resetStats()
{
	Platform::resetStats();
	mIsVisible						= mStartingIsVisible;
	mIsCrumbling                    = false;
	mMovementManager.permSetHitboxEdges(mStartingHitboxEdges);
	mAnimationManager.restartCurAnimation();
}

void Crumbling::resetToCheckpoint()
{
	Platform::resetToCheckpoint();
	mIsVisible   = mCheckpointIsVisible;
	mIsCrumbling = mCheckpointIsCrumbling;
	mMovementManager.permSetHitboxEdges(mCheckpointHitboxEdges);
	mAnimationManager.restartCurAnimation();
}

void Crumbling::setCheckpointStats()
{
	Platform::setCheckpointStats();
	mMovementManager.setCheckpointPosition();
	mCheckpointIsVisible   = mIsVisible;
	mCheckpointIsCrumbling = mIsCrumbling;
	mCheckpointHitboxEdges = mMovementManager.getHitboxEdges();
}

void Crumbling::postTick()
{
	Platform::postTick();
	if (mIsCrumbling)
	{
		mAnimationManager.updateAnimation(EAnimationType_PLAY);
	}
	if (mAnimationManager.isCurAnimationFinished())
	{
		hide();
	}
}





Gate::Gate(const CoordsX1X2Y1Y2 coordsInput, const PGatePreset& preset) : Platform()
{ 
	setUpGate(coordsInput, preset); 
}

Gate::Gate(const CoordsX1X2Y1Y2 coordsInput, const PGatePreset& preset, int codeNumber) : Platform()
{
	setUpGate(coordsInput, preset);
	mCodeNumber = codeNumber;
}

Gate::~Gate() { Platform::~Platform();}

void Gate::setUpGate(const CoordsX1X2Y1Y2& coordsInput, const PGatePreset& preset)
{
	setUpPlatformBaseStats(coordsInput, preset);
	mCheckpointIsVisible = true;
	int width			 = coordsInput.mX2 - coordsInput.mX1;
	int height			 = coordsInput.mY2 - coordsInput.mY1;
	mMovementManager.setupMovementManager(Vect2(coordsInput.mX1, coordsInput.mY1), preset, width, height);
	mStartingHitboxEdges = preset.mHitboxEdges;
	mHiddenHitboxEdges	 = preset.mHiddenHitboxEdges;

	mAnimationManager.setupAnimationManager(preset.mVerticalAnimationPresets, width + preset.mExtraWidth, height + preset.mExtraHeight, mHowToDetermineWidthHeight);
}

void Gate::preTick()
{
	Platform::preTick();
	if (mEntityCharacterType == EEntityCharacterTypes_P_PRESSURE_OPERATED_GATE)
	{
		if (mIsVisible == false)
		{
			mFramesSinceUnhidden += 1;
			if (mFramesSinceUnhidden == 3)
			{
				mFramesSinceUnhidden = 0;
				mIsVisible = true;
				mMovementManager.permSetHitboxEdges(mStartingHitboxEdges);

			}
		}
	}
}

void Gate::hide()
{
	mMovementManager.permSetHitboxEdges(mHiddenHitboxEdges);
	mIsVisible				= false;
	mFramesSinceUnhidden	= 0;
}

void Gate::resetStats()
{
	Platform::resetStats();
	mIsVisible = mStartingIsVisible;
	mMovementManager.permSetHitboxEdges(mStartingHitboxEdges);
	mFramesSinceUnhidden = 0;
}

void Gate::resetToCheckpoint()
{
	Platform::resetToCheckpoint();
	mIsVisible	= mCheckpointIsVisible;
	mMovementManager.permSetHitboxEdges(mCheckpointHitboxEdges);
	mFramesSinceUnhidden = 0;
}

void Gate::setCheckpointStats()
{
	Platform::setCheckpointStats();
	mCheckpointIsVisible   = mIsVisible;
	mCheckpointHitboxEdges = mMovementManager.getHitboxEdges();
}





Target::Target(const CoordsX1X2Y1Y2 coordsInput, const PTargetPreset& preset) : Platform()
{
	setUpPlatformBaseStats(coordsInput, preset);

	mCheckpointIsVisible = true;
	int width			 = coordsInput.mX2 - coordsInput.mX1;
	int height			 = coordsInput.mY2 - coordsInput.mY1;
	mMovementManager.setupMovementManager(Vect2(coordsInput.mX1, coordsInput.mY1), preset, width, height);
	mStartingHitboxEdges = preset.mHitboxEdges;
	mHiddenHitboxEdges	 = preset.mHiddenHitboxEdges;

	mAnimationManager.setupAnimationManager(preset.mHorizontalAnimationPresets, width + preset.mExtraWidth, height + preset.mExtraHeight, mHowToDetermineWidthHeight);
}

Target::~Target() { Platform::~Platform(); }

void Target::hide()
{
	mMovementManager.permSetHitboxEdges(mHiddenHitboxEdges);
	mIsVisible = false;
}

void Target::resetStats()
{
	Platform::resetStats();
	mIsVisible = mStartingIsVisible;
	mMovementManager.permSetHitboxEdges(mStartingHitboxEdges);
}

void Target::resetToCheckpoint()
{
	Platform::resetToCheckpoint();
	mIsVisible = mCheckpointIsVisible;
	mMovementManager.permSetHitboxEdges(mCheckpointHitboxEdges);
}

void Target::setCheckpointStats()
{
	Platform::setCheckpointStats();
	mMovementManager.setCheckpointPosition();
	mCheckpointIsVisible   = mIsVisible;
	mCheckpointHitboxEdges = mMovementManager.getHitboxEdges();
}





PressurePlate::PressurePlate(const CoordsX1X2Y1Y2 coordsInput, const PPressurePlatePreset& preset, int codeNumber) : Platform()
{
	setUpPlatformBaseStats(coordsInput, preset);

	mCodeNumber = codeNumber;
	mCheckpointIsVisible = true;
	int width			 = coordsInput.mX2 - coordsInput.mX1;
	int height			 = coordsInput.mY2 - coordsInput.mY1;
	mMovementManager.setupMovementManager(Vect2(coordsInput.mX1, coordsInput.mY1), preset, width, height);

	mPrintViaChunk = false;
	mAnimationManager.setupAnimationManager(preset.mHorizontalAnimationPresets, width + preset.mExtraWidth, height + preset.mExtraHeight, mHowToDetermineWidthHeight);

	mFramesSinceUnactivated = 0;
}

PressurePlate::~PressurePlate() { Platform::~Platform(); }

void PressurePlate::preTick()
{
	Platform::preTick();
	if (mIsActivated == true)
	{
		mFramesSinceUnactivated += 1;
		if (mFramesSinceUnactivated == 3)
		{
			mFramesSinceUnactivated = 0;
			mIsActivated = false;
		}
	}
}

void PressurePlate::activate()
{
	mIsActivated = true;
}

void PressurePlate::resetStats()
{
	Platform::resetStats();
	mIsActivated            = mStartingIsActivated;
	mFramesSinceUnactivated = 0;
}

void PressurePlate::resetToCheckpoint()
{
	Platform::resetToCheckpoint();
	mIsActivated            = mCheckpointIsActivated;
	mFramesSinceUnactivated = 0;
}

void PressurePlate::setCheckpointStats()
{
	Platform::setCheckpointStats();
	mCheckpointIsActivated = mIsActivated;
}




MovingPlatform::MovingPlatform(const CoordsX1X2Y1Y2 coordsInput, const PMovingPreset& preset, EEntityMovementPath movementPath) : Platform(coordsInput, preset)
{
	setUpPlatformBaseStats(coordsInput, preset);

	int width  = coordsInput.mX2 - coordsInput.mX1;
	int height = coordsInput.mY2 - coordsInput.mY1;

	EDirection curDirection = EDirection_LEFT;
	if (movementPath == EEntityMovementPath_VERTICAL)
	{
		curDirection = EDirection_DOWN;
	}

	mMovementManager.setupMovementManager(Vect2(coordsInput.mX1, coordsInput.mY1), preset, movementPath, curDirection, width, height);
}

MovingPlatform::~MovingPlatform() { Platform::~Platform(); }




AreaEffectPlatform::AreaEffectPlatform(const CoordsX1X2Y1Y2 hitboxCoords, const CoordsX1X2Y1Y2 areaEffectCoords, EDirection effectDirection,
		const PAreaEffectPlatformPreset& preset) : Platform()
{
	setUpPlatformBaseStats(hitboxCoords, preset);

	int width  = hitboxCoords.mX2 - hitboxCoords.mX1;
	int height = hitboxCoords.mY2 - hitboxCoords.mY1;
	int areaEffectWidth  = areaEffectCoords.mX2 - areaEffectCoords.mX1;
	int areaEffectHeight = areaEffectCoords.mY2 - areaEffectCoords.mY1;
	mAreaEffectHitbox = Hitbox(Vect2(areaEffectCoords.mX1, areaEffectCoords.mY1), areaEffectWidth, areaEffectHeight);

	mAreaEffectMovement  = preset.mAreaEffectMovement;
	mEffectDirection = effectDirection;

	std::vector <AnimationPreset>  animationPresets;
	std::vector <AnimationPreset> areaEffectAnimationPresets;

	switch (effectDirection)
	{
	case EDirection_LEFT:
		animationPresets			= preset.mLeftAnimationPresets;
		areaEffectAnimationPresets	= preset.mAreaEffectLeftAnimationPresets;
		break;
	case EDirection_RIGHT:
		animationPresets			= preset.mRightAnimationPresets;
		areaEffectAnimationPresets	= preset.mAreaEffectRightAnimationPresets;
		break;
	case EDirection_UP:
		animationPresets			= preset.mUpAnimationPresets;
		areaEffectAnimationPresets	= preset.mAreaEffectUpAnimationPresets;
		break;
	case EDirection_DOWN:
		animationPresets			= preset.mDownAnimationPresets;
		areaEffectAnimationPresets	= preset.mAreaEffectDownAnimationPresets;
		break;
	default:
		SDL_assert(false);
		break;
	}

	mAnimationManager.setupAnimationManager(			animationPresets,			width,				height,				mHowToDetermineWidthHeight);
	mAreaEffectAnimationManager.setupAnimationManager(	areaEffectAnimationPresets, areaEffectWidth,	areaEffectHeight,	mHowToDetermineWidthHeight);
}

AreaEffectPlatform::~AreaEffectPlatform() { Platform::~Platform(); }
#include "platformHelper.h"

Platform::Platform(CoordsX1X2Y1Y2 coordsInput, PPlatformPreset* preset)
{
	setUpPlatformBaseStats(coordsInput, preset);

	int width  = coordsInput.getX2() - coordsInput.getX1();
	int height = coordsInput.getY2() - coordsInput.getY1();
	mMovementManager.setupMovementManager(Vect2(coordsInput.getX1(), coordsInput.getY1()), preset, width, height);
	std::vector <AnimationPreset> animationPresets;
	if (height > width)
	{
		animationPresets = preset->mVerticalAnimationPresets;
	}
	else
	{
		animationPresets = preset->mHorizontalAnimationPresets;
	}

	if (animationPresets.size() > 0)
	{
		mAnimationManager.setupAnimationManager(animationPresets,   width + preset->mExtraWidth, height + preset->mExtraHeight, mHowToDetermineWidthHeight);
	}
}

Platform::Platform()
{
	mpPreset = nullptr;
}

Platform::~Platform()
{
	mpPreset = nullptr;
	Entity::~Entity();
}

void Platform::setUpPlatformBaseStats(CoordsX1X2Y1Y2 coordsInput, PPlatformPreset* preset)
{
	setUpBaseStats(preset);
	mMovementEffect   = preset->mMovementEffect;
	mCoords		      = coordsInput;
	mpPreset	      = preset;
	mPrintViaChunk    = preset->mPrintViaChunk;
	mSplice           = preset->mSplice;
	mHowToDetermineWidthHeight = preset->mHowToDetermineWidthHeight;

	if ((false == mPrintViaChunk 	and mHowToDetermineWidthHeight == EHowToDetermineWidthHeight_CHUNK_IMAGES) 	or (true == mPrintViaChunk 	and mHowToDetermineWidthHeight != EHowToDetermineWidthHeight_CHUNK_IMAGES))
	{
		SDL_assert(false);
	}
	if ((false == mSplice 			and mHowToDetermineWidthHeight == EHowToDetermineWidthHeight_SPLICE) 		or (true == mSplice 		and mHowToDetermineWidthHeight != EHowToDetermineWidthHeight_SPLICE))
	{
		SDL_assert(false);
	}
}


bool Platform::isMoveable() const
{
	return mIsMoveable;
}


void Platform::hide()
{
	return;
}

void Platform::activate()
{
	return;
}

void Platform::startCrumble()
{
	return;
}


PPlatformPreset* Platform::getPreset() const
{
	return mpPreset;
}


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
	mMovementManager.updateMovement();
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





Crate::Crate(CoordsX1X2Y1Y2 coordsInput, PCratePreset* preset) : Platform()
{
	setUpPlatformBaseStats(coordsInput, preset);
	int width = coordsInput.getX2() - coordsInput.getX1();
	int height = coordsInput.getY2() - coordsInput.getY1();
	mMovementManager.setupMovementManager(Vect2(coordsInput.getX1(), coordsInput.getY1()), preset, width, height);
	mAnimationManager.setupAnimationManager(preset->mHorizontalAnimationPresets, width + preset->mExtraWidth, height + preset->mExtraHeight, mHowToDetermineWidthHeight);
	mIsMoveable = true;
}

Crate::~Crate()
{
	Platform::~Platform();
}

void Crate::tick()
{
	Entity::tick();
	if (mMovementManager.getPositionData().mOnGround)
	{
		for (int count = 0; count < mMovementManager.getPositionData().mCurGroundCharacteristics.size(); count++)
		{
			if (mMovementManager.getPositionData().mCurGroundCharacteristics[count] == EEntityCharacteristicsTypes_SLIPPERY)
			{
				mMovementManager.getMovementStates()[EMovementStateIndex_WALKING]->autoMove();
			}
		}
	}
	mMovementManager.updateMovement();

}

void Crate::postTick()
{
	//mMovementManager.calculateXDirection();
	Platform::postTick();
}

void Crate::setCheckpointStats()
{
	mCheckpointAmAlive = mAmAlive;
	mMovementManager.setCheckpointPosition();
}





Crumbling::Crumbling(CoordsX1X2Y1Y2 coordsInput, PCrumblingPreset* preset) : Platform()
{
	setUpPlatformBaseStats(coordsInput, preset);
	mCheckpointIsVisible   = true;

	int width			   = coordsInput.getX2() - coordsInput.getX1();
	int height		   	   = coordsInput.getY2() - coordsInput.getY1();
	mMovementManager.setupMovementManager(Vect2(coordsInput.getX1(), coordsInput.getY1()), preset, width, height);

	mStartingHitboxEdges   = preset->mHitboxEdges;
	mHiddenHitboxEdges	   = preset->mHiddenHitboxEdges;

	mAnimationManager.setupAnimationManager(preset->mHorizontalAnimationPresets, width + preset->mExtraWidth, height + preset->mExtraHeight, mHowToDetermineWidthHeight);
}

Crumbling::~Crumbling()
{
	Platform::~Platform();
}

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





Gate::Gate(CoordsX1X2Y1Y2 coordsInput, PGatePreset* preset) : Platform()
{
	setUpGate(coordsInput, preset);
}

Gate::Gate(CoordsX1X2Y1Y2 coordsInput, PGatePreset* preset, int codeNumber) : Platform()
{
	setUpGate(coordsInput, preset);
	mCodeNumber = codeNumber;
}

Gate::~Gate()
{
	Platform::~Platform();
}

void Gate::setUpGate(CoordsX1X2Y1Y2 coordsInput, PGatePreset* preset)
{
	setUpPlatformBaseStats(coordsInput, preset);
	mCheckpointIsVisible = true;
	int width			 = coordsInput.getX2() - coordsInput.getX1();
	int height			 = coordsInput.getY2() - coordsInput.getY1();
	mMovementManager.setupMovementManager(Vect2(coordsInput.getX1(), coordsInput.getY1()), preset, width, height);
	mStartingHitboxEdges = preset->mHitboxEdges;
	mHiddenHitboxEdges	 = preset->mHiddenHitboxEdges;

	mAnimationManager.setupAnimationManager(preset->mVerticalAnimationPresets, width + preset->mExtraWidth, height + preset->mExtraHeight, mHowToDetermineWidthHeight);
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





Target::Target(CoordsX1X2Y1Y2 coordsInput, PTargetPreset* preset) : Platform()
{
	setUpPlatformBaseStats(coordsInput, preset);

	mCheckpointIsVisible = true;
	int width			 = coordsInput.getX2() - coordsInput.getX1();
	int height			 = coordsInput.getY2() - coordsInput.getY1();
	mMovementManager.setupMovementManager(Vect2(coordsInput.getX1(), coordsInput.getY1()), preset, width, height);
	mStartingHitboxEdges = preset->mHitboxEdges;
	mHiddenHitboxEdges	 = preset->mHiddenHitboxEdges;

	mAnimationManager.setupAnimationManager(preset->mHorizontalAnimationPresets, width + preset->mExtraWidth, height + preset->mExtraHeight, mHowToDetermineWidthHeight);

}

Target::~Target()
{
	Platform::~Platform();
}

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





PressurePlate::PressurePlate(CoordsX1X2Y1Y2 coordsInput, PPressurePlatePreset* preset, int codeNumber) : Platform()
{
	setUpPlatformBaseStats(coordsInput, preset);

	mCodeNumber = codeNumber;
	mCheckpointIsVisible = true;
	int width			 = coordsInput.getX2() - coordsInput.getX1();
	int height			 = coordsInput.getY2() - coordsInput.getY1();
	mMovementManager.setupMovementManager(Vect2(coordsInput.getX1(), coordsInput.getY1()), preset, width, height);

	mPrintViaChunk = false;
	mAnimationManager.setupAnimationManager(preset->mHorizontalAnimationPresets, width + preset->mExtraWidth, height + preset->mExtraHeight, mHowToDetermineWidthHeight);

	mFramesSinceUnactivated = 0;
}

PressurePlate::~PressurePlate()
{
	Platform::~Platform();
}

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




MovingPlatform::MovingPlatform(CoordsX1X2Y1Y2 coordsInput, PMovingPreset* preset, EEntityMovementPath movementPath) : 
		Platform(coordsInput, preset)
{
	setUpPlatformBaseStats(coordsInput, preset);

	int width  = coordsInput.getX2() - coordsInput.getX1();
	int height = coordsInput.getY2() - coordsInput.getY1();

	EDirection curDirection = EDirection_LEFT;
	if (movementPath == EEntityMovementPath_VERTICAL)
	{
		curDirection = EDirection_DOWN;
	}

	mMovementManager.setupMovementManager(Vect2(coordsInput.getX1(), coordsInput.getY1()), preset, movementPath, curDirection, width, height);
}

MovingPlatform::~MovingPlatform()
{
	Platform::~Platform();
}




AreaEffectPlatform::AreaEffectPlatform(CoordsX1X2Y1Y2 hitboxCoords, CoordsX1X2Y1Y2 areaEffectCoords, EDirection effectDirection, 
		PAreaEffectPlatformPreset* preset) : Platform(hitboxCoords, preset)
{
	setUpPlatformBaseStats(hitboxCoords, preset);

	int width  = hitboxCoords.getX2() - hitboxCoords.getX1();
	int height = hitboxCoords.getY2() - hitboxCoords.getY1();
	int areaEffectWidth  = areaEffectCoords.getX2() - areaEffectCoords.getX1();
	int areaEffectHeight = areaEffectCoords.getY2() - areaEffectCoords.getY1();
	mAreaEffectHitbox = Hitbox(Vect2(areaEffectCoords.getX1(), areaEffectCoords.getY1()), areaEffectWidth, areaEffectHeight);

	mAreaEffectMovement  = preset->mAreaEffectMovement;
	mEffectDirection = effectDirection;

	std::vector <AnimationPreset>  animationPresets;
	std::vector <AnimationPreset> areaEffectAnimationPresets;

	switch (effectDirection)
	{
	case EDirection_LEFT:
		animationPresets			= preset->mLeftAnimationPresets;
		areaEffectAnimationPresets	= preset->mAreaEffectLeftAnimationPresets;
		break;
	case EDirection_RIGHT:
		animationPresets			= preset->mRightAnimationPresets;
		areaEffectAnimationPresets	= preset->mAreaEffectRightAnimationPresets;
		break;
	case EDirection_UP:
		animationPresets			= preset->mUpAnimationPresets;
		areaEffectAnimationPresets	= preset->mAreaEffectUpAnimationPresets;
		break;
	case EDirection_DOWN:
		animationPresets			= preset->mDownAnimationPresets;
		areaEffectAnimationPresets	= preset->mAreaEffectDownAnimationPresets;
		break;
	default:
		SDL_assert(false);
		break;
	}

	mAnimationManager.setupAnimationManager(			animationPresets,			width,				height,				mHowToDetermineWidthHeight);
	mAreaEffectAnimationManager.setupAnimationManager(	areaEffectAnimationPresets, areaEffectWidth,	areaEffectHeight,	mHowToDetermineWidthHeight);
}

AreaEffectPlatform::~AreaEffectPlatform()
{
	Platform::~Platform();
}
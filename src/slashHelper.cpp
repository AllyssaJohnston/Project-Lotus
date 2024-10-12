#include "slashHelper.h"

EntityDistance::EntityDistance()
{
	Entity* mpEntity = nullptr;
	int mDistance    = INT_MAX;
}

EntityDistance::EntityDistance(Entity* pEntity, Entity* otherEntity, Hitbox slashHitbox, int degrees)
{
	mpEntity = pEntity;
	int height = slashHitbox.getHeight();
	float radians = degToRad(degrees);
	mDistance = (int)sin(radians) * height;
}

EntityDistance::~EntityDistance()
{
	mpEntity = nullptr;
}



SlashManager::SlashManager(MovementManager& playerMovementManager) : mPlayerMovementManager(playerMovementManager)
{
	mTimeOfLastSlash = std::chrono::high_resolution_clock::now();
	mAnimationManager.setupAnimationManager(PlayerProjectilePreset(EEntityMovementPath_HORIZONTAL).mAnimationPresets, EHowToDetermineWidthHeight_GET_BEST_IMAGE_RATIO);
	mImageObjectHitbox.setupImageObject("blue.bmp",		 mHitbox.getWidth(), mHitbox.getHeight(), EHowToDetermineWidthHeight_USE_WIDTH_AND_HEIGHT_INPUT);
	mImageObjectImageHitbox.setupImageObject("blue.bmp", mAnimationManager.getCurImage()->getIdealImageWidth(), mAnimationManager.getCurImage()->getIdealImageHeight(), EHowToDetermineWidthHeight_USE_WIDTH_AND_HEIGHT_INPUT);
}

void SlashManager::startSlash()
{
	mCurSlash = true;
	const auto deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - mTimeOfLastSlash);
	mTimeOfLastSlash = std::chrono::high_resolution_clock::now();
	if ((deltaTime.count() / pow(10, 9)) < 1.5)
	{
		//less than 1.5 secs, flip slash path direction
		mCurPathDirection =  returnOppositeDirection(mCurPathDirection);
	}
	else
	{
		mCurPathDirection = EDirection_DOWN;
	}
	mCurSlashDirection = mPlayerMovementManager.getCurFacingDirection();

	if (mCurPathDirection == EDirection_UP)
	{
		if (mCurSlashDirection == EDirection_LEFT)
		{
			mCurRotation = 270;
		}
		else
		{
			mCurRotation = -90;
		}
	}
	else
	{
		mCurRotation = 90;
	}

	updateCurCenterPoint();
}

void SlashManager::updateCurCenterPoint()
{
	int xStartCoord = mPlayerMovementManager.getHitbox().getBottomRight().getX() + mSpaceFromPlayer;
	int yStartCoord = mPlayerMovementManager.getHitbox().getCenter().getY() - 20;
	if (mCurSlashDirection == EDirection_LEFT)
	{
		xStartCoord = mPlayerMovementManager.getHitbox().getTopLeft().getX() - mSpaceFromPlayer;
	}
	mCenterOfRotation = Vect2(xStartCoord, yStartCoord);
	int xCoord = xStartCoord + (int)(mSlashLength * cos(degToRad(mCurRotation)));
	int yCoord = yStartCoord - (int)(mSlashLength * sin(degToRad(mCurRotation)));
	mHitbox.setCenter(Vect2(xCoord, yCoord));
}

void SlashManager::tick()
{
	if (mCurSlash)
	{
		mRotationTickCountDown -= 1;
		if (mRotationTickCountDown <= 0)
		{
			mRotationTickCountDown = mRotationTickCountDownInterval;
			updateCurCenterPoint();

			//update rotation
			if		(mCurSlashDirection == EDirection_LEFT  and mCurPathDirection == EDirection_DOWN) 
			{
				//90 to 270
				mCurRotation += mRotationInterval;
				if (mCurRotation > 270)
				{
					mCurSlash = false;
				}
			}
			else if (mCurSlashDirection == EDirection_LEFT and mCurPathDirection == EDirection_UP)
			{
				//270 to 90
				mCurRotation -= mRotationInterval;
				if (mCurRotation < 90)
				{
					mCurSlash = false;
				}
			}
			else if  (mCurSlashDirection == EDirection_RIGHT and mCurPathDirection == EDirection_UP)
			{
				//-90 to 90
				mCurRotation += mRotationInterval;
				if (mCurRotation > 90)
				{
					mCurSlash = false;
				}
			}
			else // (mCurSlashDirection == EDirection_RIGHT  and mCurPathDirection == EDirection_DOWN) 
			{
				//90 to -90
				mCurRotation -= mRotationInterval;
				if (mCurRotation < -90)
				{
					mCurSlash = false;
				}
			}
		}

	}
}
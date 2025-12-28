#include "slashManager.h"

int getEntityDistance(Entity& entity, const Entity& otherEntity, const Hitbox& slashHitbox, int degrees)
{
	int height = slashHitbox.getHeight();
	float radians = (float)degToRad(degrees);
	return (int)sin(radians) * height;
}

EntityDistance::EntityDistance(Entity& entity, int distance) : mEntity(entity), mDistance(distance) { ; }

SlashManager::SlashManager()
{
	mTimeOfLastSlash = std::chrono::high_resolution_clock::now();
	mAnimationManager.setupAnimationManager(PlayerProjectilePreset(EEntityMovementPath_HORIZONTAL).mAnimationPresets, EHowToDetermineWidthHeight_GET_BEST_IMAGE_RATIO);
	if (DEBUG)
	{
		mImageObjectHitbox.setupImageObject("blue.bmp", mHitbox.getWidth(), mHitbox.getHeight(), EHowToDetermineWidthHeight_USE_WIDTH_AND_HEIGHT_INPUT);
		mImageObjectImageHitbox.setupImageObject("pink.bmp", mAnimationManager.getCurImage()->mIdealImageWidth, mAnimationManager.getCurImage()->mIdealImageHeight, EHowToDetermineWidthHeight_USE_WIDTH_AND_HEIGHT_INPUT);
	}
}

void SlashManager::startSlash(MovementManager& playerMovementManager)
{
	mCurSlash = true;
	const auto deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - mTimeOfLastSlash);
	mTimeOfLastSlash = std::chrono::high_resolution_clock::now();
	if ((deltaTime.count() / pow(10, 9)) < 1.5)
	{
		// less than 1.5 secs, flip slash path direction
		mCurPathDirection =  returnOppositeDirection(mCurPathDirection);
	}
	else
	{
		mCurPathDirection = EDirection_DOWN;
	}
	mCurSlashDirection = playerMovementManager.getCurFacingDirection();

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

	updateCurCenterPoint(playerMovementManager);
}

void SlashManager::updateCurCenterPoint(MovementManager& playerMovementManager)
{
	int xStartCoord = playerMovementManager.getHitbox().getBottomRight().getX() + mSpaceFromPlayer;
	int yStartCoord = playerMovementManager.getHitbox().getCenter().getY() - 20;
	if (mCurSlashDirection == EDirection_LEFT)
	{
		xStartCoord = playerMovementManager.getHitbox().getTopLeft().getX() - mSpaceFromPlayer;
	}
	mCenterOfRotation = Vect2(xStartCoord, yStartCoord);
	int xCoord = xStartCoord + (int)(mSlashLength * cos(degToRad(mCurRotation)));
	int yCoord = yStartCoord - (int)(mSlashLength * sin(degToRad(mCurRotation)));
	mHitbox.setCenter(Vect2(xCoord, yCoord));
}

void SlashManager::tick(MovementManager& playerMovementManager)
{
	if (mCurSlash)
	{
		mRotationTickCountDown -= 1;
		if (mRotationTickCountDown <= 0)
		{
			mRotationTickCountDown = mRotationTickCountDownInterval;
			updateCurCenterPoint(playerMovementManager);

			// update rotation
			if		(mCurSlashDirection == EDirection_LEFT  and mCurPathDirection == EDirection_DOWN) 
			{
				// 90 to 270
				mCurRotation += mRotationInterval;
				if (mCurRotation > 270)
				{
					mCurSlash = false;
				}
			}
			else if (mCurSlashDirection == EDirection_LEFT and mCurPathDirection == EDirection_UP)
			{
				// 270 to 90
				mCurRotation -= mRotationInterval;
				if (mCurRotation < 90)
				{
					mCurSlash = false;
				}
			}
			else if  (mCurSlashDirection == EDirection_RIGHT and mCurPathDirection == EDirection_UP)
			{
				// -90 to 90
				mCurRotation += mRotationInterval;
				if (mCurRotation > 90)
				{
					mCurSlash = false;
				}
			}
			else // (mCurSlashDirection == EDirection_RIGHT  and mCurPathDirection == EDirection_DOWN) 
			{
				// 90 to -90
				mCurRotation -= mRotationInterval;
				if (mCurRotation < -90)
				{
					mCurSlash = false;
				}
			}
		}

	}
}
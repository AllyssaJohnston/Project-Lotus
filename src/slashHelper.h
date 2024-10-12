#pragma once
#include <SDL3/SDL.h>
#include <cmath>
#include <corecrt_math_defines.h>
#include <chrono>
#include "helperClass.h"
#include "hitboxHelper.h"
#include "entityHelper.h"
#include "projectilePresets.h"

#define degToRad(angleDegrees) ((float)(angleDegrees) * M_PI / 180.0f)
#define radToDeg(angleRadians) ((float)(angleRadians) * 180.0f / M_PI)

struct EntityDistance
{
	Entity* mpEntity = nullptr;
	int mDistance    = INT_MAX;

	EntityDistance();

	EntityDistance(Entity* pEntity, Entity* otherEntity, Hitbox slashHitbox, int degrees);

	~EntityDistance();
};

class SlashManager
{
	MovementManager& mPlayerMovementManager;
	int mSpaceFromPlayer = 10;
public:
	EDirection mCurSlashDirection		= EDirection_RIGHT;
	EDirection mCurPathDirection		= EDirection_DOWN;
	int mCurRotation					= 0;
	int mRotationInterval				= 5;
	int mRotationTickCountDown			= 0;
	int mRotationTickCountDownInterval	= 5;
	bool mCurSlash						= false;
	std::chrono::time_point<std::chrono::steady_clock> mTimeOfLastSlash;

	int mSlashLength = 80;
	Vect2 mCenterOfRotation = Vect2(0, 0);
	Hitbox mHitbox = Hitbox(Vect2(0,0), 10, 10);
	AnimationManager mAnimationManager;
	ImageObject mImageObjectHitbox;
	SDL_Texture* mHitboxTexture; //actual slash hitbox
	ImageObject mImageObjectImageHitbox;
	SDL_Texture* mSlashImageHitboxTexture; //sword hitbox
	int mImageRotation = 180;

	SlashManager(MovementManager& playerMovementManager);

	void startSlash();

	void updateCurCenterPoint();

	void tick();

};
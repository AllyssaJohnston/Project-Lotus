#pragma once
#include <SDL3/SDL.h>
#include <cmath>
#include <corecrt_math_defines.h>
#include <chrono>
#include "helpers.h"
#include "hitbox.h"
#include "movementManager.h"
#include "entity.h"
#include "projectilePresets.h"

#define degToRad(angleDegrees) ((float)(angleDegrees) * M_PI / 180.0f)
#define radToDeg(angleRadians) ((float)(angleRadians) * 180.0f / M_PI)

int getEntityDistance(Entity& entity, const Entity& otherEntity, const Hitbox& slashHitbox, int degrees);

struct EntityDistance
{
	Entity& mEntity;
	int mDistance    = INT_MAX;

	EntityDistance(Entity& entity, int distance);

	~EntityDistance() { ; }
};

class SlashManager
{
	int mSpaceFromPlayer = 10;
public:
	EDirection mCurSlashDirection		= EDirection_RIGHT;
	EDirection mCurPathDirection		= EDirection_DOWN;
	int mCurRotation					= 0;
	int mRotationInterval				= 5;
	int mRotationTickCountDown			= 0;
	int mRotationTickCountDownInterval	= 1;
	bool mCurSlash						= false;
	std::chrono::time_point<std::chrono::steady_clock> mTimeOfLastSlash;

	int mSlashLength						= 80;
	Vect2 mCenterOfRotation					= Vect2(0, 0);
	Hitbox mHitbox							= Hitbox(Vect2(0,0), 10, 10);
	AnimationManager mAnimationManager;
	ImageObject mImageObjectHitbox;
	SDL_Texture* mpHitboxTexture			= nullptr; // actual slash hitbox
	ImageObject mImageObjectImageHitbox;
	SDL_Texture* mpSlashImageHitboxTexture	= nullptr; // sword hitbox
	int mImageRotation						= 180;

	SlashManager();

	void startSlash(MovementManager& playerMovementManager);

	void updateCurCenterPoint(MovementManager& playerMovementManager);

	void tick(MovementManager& playerMovementManager);
};
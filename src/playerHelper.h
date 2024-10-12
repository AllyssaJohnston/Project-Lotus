#pragma once
#include <SDL3/SDL.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <list>
#include "entityHelper.h"
#include "imageHelper.h"
#include "helperClass.h"
#include "movementHelperClass.h"
#include "movementHelper.h"
#include "playerPreset.h"
#include "collectibleHelper.h"

class Player : public Entity
{
private:
	const int mStandardOutfit	= 0;
	const int mDoubleJumpOutfit	= 1;
public:
	int mStartingKeys		= 0;
	int mCheckpointKeys		= 0;
	int mKeys				= 0;

	int mStartingTargets	= 0;
	int mCheckpointTargets	= 0;
	int mTargets			= 0;

	std::vector <Collectible *> mpCurHeldCollectibles;

	int mSwordSlashWidth;
	int mSwordSlashHeight;

	bool mUseHorizontalInputThisFrame = true;

	Player();

	~Player();

	void preTick();

	void tick();

	void postTick();


	void useInput(std::vector <KeyData>& eventVect);


	void updateAnimationManager();


	void resetStats();

	void resetToCheckpoint();

	void setCheckpointStats();


	void updateKeys(int keys);

	void updateTargets(int targets);

	void addHeldCollectible(Collectible* curCollectible);


	void setUpAllTextures(SDL_Renderer* pRenderer);
};
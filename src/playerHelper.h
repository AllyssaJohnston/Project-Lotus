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
#include "globals.h"


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

	std::vector <Collectible*> mpCurHeldCollectibles;

	int mSwordSlashWidth;
	int mSwordSlashHeight;

	bool mUseHorizontalInputThisFrame = true;

	bool mCanWallJump = false;
	bool mCheckpointCanWallJump = false;

	Player();

	~Player();

	void preTick() override;

	void tick() override;

	void postTick() override;


	void useInput(std::vector <KeyData>& eventVect);


	void updateAnimationManager() override;


	void resetStats() override;

	void resetToCheckpoint() override;

	void setCheckpointStats() override;


	void updateKeys(int keys);

	void updateTargets(int targets);

	void addHeldCollectible(Collectible* pCurCollectible);


	void setUpAllTextures(SDL_Renderer* pRenderer);

};
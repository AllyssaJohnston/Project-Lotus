#pragma once
#include <string>
#include "wtypes.h"
#include <SDL3/SDL.h>

struct ScreenObject
{
	// ACTUAL DESKTOP SIZE IN PIXELS
	int mScreenWidth;
	int mScreenHeight;

	// GAME WINDOW SIZE IN PIXELS
	int mGameScreenWidth;
	int mGameScreenHeight;

	// GAME WINDOW SIZE IN GAME UNITS
	int mGameLevelChunkWidth  = 1600;
	int mGameLevelChunkHeight = 900;

	float mGameScreenToGameLevelChunkRatio;

	//w ith toolbar
	SDL_Window* mpWindow = NULL;
	// render space
	SDL_Surface* mpScreenSurface = NULL;

	SDL_Renderer* mpRenderer = NULL;

	ScreenObject();
	
	~ScreenObject();
	
	void getBestScreenSize();
};
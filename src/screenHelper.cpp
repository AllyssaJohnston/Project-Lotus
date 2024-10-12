#include <SDL3/SDL.h>
#include "screenHelper.h"

ScreenObject::ScreenObject()
{
	getBestScreenSize();

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "could not initialize sdl3: %s\n", SDL_GetError());
	}

	mpWindow = SDL_CreateWindow("lotusAdventure_sdl3", mGameScreenWidth, mGameScreenHeight, 0);

	if (mpWindow == NULL)
	{
		fprintf(stderr, "could not create window: %s\n", SDL_GetError());
	}
	mpRenderer = SDL_CreateRenderer(mpWindow, NULL, SDL_RENDERER_ACCELERATED);
	if (mpRenderer == NULL) {
		fprintf(stderr, "could not create renderer: %s\n", SDL_GetError());
	}

	mpScreenSurface = SDL_CreateSurface(mGameScreenWidth, mGameScreenHeight,SDL_PIXELFORMAT_RGBA8888);
	if (mpScreenSurface == NULL) {
		fprintf(stderr, "could not create SURFACE: %s\n", SDL_GetError());
	}
}

ScreenObject::~ScreenObject()
{
	SDL_DestroySurface(mpScreenSurface);
	SDL_DestroyRenderer(mpRenderer);
	SDL_DestroyWindow(mpWindow);
	SDL_Quit();
}

void ScreenObject::getBestScreenSize()
{
	RECT 	   desktopWindow;
	const HWND desktopWindowRef = GetDesktopWindow();
	GetWindowRect(desktopWindowRef, &desktopWindow);
	mScreenWidth = desktopWindow.right; //1920;// desktopWindow.right;
	mScreenHeight = desktopWindow.bottom; //1440;// desktopWindow.bottom;

	float gameRatio1 = mScreenWidth / 16.0f;
	int	  gameWidth1 = mScreenWidth;
	int   gameHeight1 = gameRatio1 * 9;

	float gameRatio2 = mScreenHeight / 9.0f;
	int   gameWidth2 = gameRatio2 * 16;
	int   gameHeight2 = mScreenHeight;

	if (gameWidth2 <= mScreenWidth or gameHeight1 <= mScreenHeight)
	{
		mGameScreenWidth  = gameWidth1  * 2;
		mGameScreenHeight = gameHeight1 * 2;
	}
	else
	{
		mGameScreenWidth  = gameWidth2  * 2;
		mGameScreenHeight = gameHeight2 * 2;
	}

	mGameScreenToGameLevelChunkRatio = (float)mGameScreenWidth / (float)mGameLevelChunkWidth;
}
#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <string>

struct StyleManager
{
	const static char* fontName;
	const static SDL_Color lightPink;
	const static SDL_Color pink;
	const static SDL_Color lightYellow;
	const static SDL_Color white;
	const static SDL_Color oliveGreen;
	const static SDL_Color sunYellow;
	const static SDL_Color red;
	const static SDL_Color navy;
	const static SDL_Color black;
	const static SDL_Color gray;
	const static SDL_Color teal;
	const static SDL_Color hintBlue;
	const static SDL_Color gold;
	const static SDL_Color clear;

	// combat menus
	const static int panelWidth			= 700;
	const static int panelHeight		= 450;
	const static int panelHeadingHeight	= 100;
	const static int panelBodyHeight	= 350;

	const static int headingText = 30;
	const static int optionText = 30;
	const static int detailText = 17;
	const static int bullet = 20;

	StyleManager() { ; }

	~StyleManager();
};

SDL_Color blendColors(SDL_Color* pColor1, SDL_Color* pColor2, float blendPercent);
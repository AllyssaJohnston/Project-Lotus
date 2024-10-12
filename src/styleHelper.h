#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <string>
#include <vector>

class StyleManager
{
public: 
	const char * robotoFontName = "fonts/Roboto-Regular.ttf";
	const static SDL_Color pink;
	const static SDL_Color lightYellow;
	const static SDL_Color white;
	const static SDL_Color oliveGreen;
	const static SDL_Color sunYellow;
	const static SDL_Color red;
	const static SDL_Color navy;
	const static SDL_Color black;
	const static SDL_Color teal;
	const static SDL_Color hintBlue;
	const static SDL_Color clear;


	StyleManager(){}

	~StyleManager();
};
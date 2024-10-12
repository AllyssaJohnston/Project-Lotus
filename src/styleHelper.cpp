#include "styleHelper.h"

const SDL_Color StyleManager::pink			= { 170, 10,  80 };
const SDL_Color StyleManager::lightYellow	= {255, 250, 200};
const SDL_Color StyleManager::white			= {255, 255, 255};
const SDL_Color StyleManager::oliveGreen	= {60, 110, 20};
const SDL_Color StyleManager::sunYellow		= {240, 230, 60};
const SDL_Color StyleManager::red			= {255, 40, 30};
const SDL_Color StyleManager::navy			= {20, 0, 110 };
const SDL_Color StyleManager::black			= {0, 0, 0};
const SDL_Color StyleManager::teal			= {20, 60, 60, 1 };
const SDL_Color StyleManager::hintBlue      = {221, 255, 253};
const SDL_Color StyleManager::clear         = {0, 0, 0, 0};

StyleManager::~StyleManager()
{
	robotoFontName = nullptr;
}
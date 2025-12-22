#include "styleHelper.h"

const char* StyleManager::fontName = "fonts/Roboto-Regular.ttf";
const SDL_Color StyleManager::lightPink     = {245, 108, 169, 1};
const SDL_Color StyleManager::pink			= {170, 10,  80,  1};
const SDL_Color StyleManager::lightYellow	= {255, 250, 200, 1};
const SDL_Color StyleManager::white			= {255, 255, 255, 1};
const SDL_Color StyleManager::oliveGreen	= {60,  110, 20,  1};
const SDL_Color StyleManager::sunYellow		= {240, 230, 60,  1};
const SDL_Color StyleManager::red			= {255, 40,  30,  1};
const SDL_Color StyleManager::navy			= {20,  0,   110, 1};
const SDL_Color StyleManager::black			= {0,   0,   0,   1};
const SDL_Color StyleManager::teal			= {20,  60,  60,  1};
const SDL_Color StyleManager::hintBlue      = {221, 255, 253, 1};
const SDL_Color StyleManager::gold			= {181, 160, 0,   1};
const SDL_Color StyleManager::clear         = {0,   0,   0,   0};

StyleManager::~StyleManager() { fontName = nullptr; }

SDL_Color blendColors(SDL_Color* pColor1, SDL_Color* pColor2, float blendPercent)
{
	SDL_Color newColor;
	newColor.r = Uint8(pColor1->r * (1 - blendPercent)) + Uint8(pColor2->r * blendPercent);
	newColor.g = Uint8(pColor1->g * (1 - blendPercent)) + Uint8(pColor2->g * blendPercent);
	newColor.b = Uint8(pColor1->b * (1 - blendPercent)) + Uint8(pColor2->b * blendPercent);
	return newColor;
}
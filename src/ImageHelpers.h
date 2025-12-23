#pragma once
#include <SDL3/SDL.h>
#include "helpers.h"

float getBestImageRatio(SDL_Surface* surface, int maxWidth, int maxHeight);

enum EImageOffset
{
	EImageOffset_INVALID = -1,
	EImageOffset_PRINT_TOP_LEFT,
	EImageOffset_FULL_UPWARDS,
	EImageOffset_MIDDLE,
	EImageOffset_LEFT_X_MIDDLE_Y,
	EImageOffset_MIDDLE_X_TOP_Y,
	EImageOffset_MAX
};
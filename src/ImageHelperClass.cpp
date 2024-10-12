#include "ImageHelperClass.h"

float getBestImageRatio(SDL_Surface* surface, int maxWidth, int maxHeight)
{
	float imageRatioByWidth  = float(surface->w) / maxWidth;
	float imageRatioByHeight = float(surface->h) / maxHeight;

	float imageHeightByWidthRatio = surface->h / imageRatioByWidth;


	if (imageHeightByWidthRatio > int(maxHeight * 1.05f)) 
	{
		return imageRatioByHeight;
	}
	else
	{
		return imageRatioByWidth;
	}
}
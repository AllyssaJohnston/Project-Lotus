#pragma once
#include "menuHelpers.h"




void drawCircle(const SDL_Color& color, const Vect2& center, int radius, const ScreenObject& screen)
{
	SDL_Renderer* pRenderer = screen.mpRenderer;

	float actualCenterX = center.getX() * screen.mGameScreenToGameLevelChunkRatio;
	float actualCenterY = center.getY() * screen.mGameScreenToGameLevelChunkRatio;
	float actualRadius = radius * screen.mGameScreenToGameLevelChunkRatio;


	for (int degrees = 0; degrees < 360; degrees++)
	{
		for (int curRadius = 0; curRadius < actualRadius; curRadius++)
		{
			float radians = (float)degToRad(degrees);
			float curX = actualCenterX + (cos(radians) * curRadius);
			float curY = actualCenterY + (sin(radians) * curRadius);
			float alpha = (actualRadius - curRadius) / actualRadius;
			SDL_SetRenderDrawColor(pRenderer, color.r, color.g, color.b, Uint8(alpha));
			SDL_RenderPoint(pRenderer, curX, curY);
		}
	}
}


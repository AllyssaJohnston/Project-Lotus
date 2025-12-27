#include "UIElement.h"

Vect2 UIElement::getUpdatedPosFromBlockSpace(const Hitbox& blockSpace)
{
	int x;
	int y;

	switch (mPositionAlignH)
	{
	case ETextBoxPositionAlign_LEFT:
		x = blockSpace.getTopLeft().getX();
		break;
	case ETextBoxPositionAlign_CENTER:
		x = blockSpace.getCenter().getX() - (getHitbox().getWidth() / 2);
		break;
	case ETextBoxPositionAlign_RIGHT:
		x = blockSpace.getBottomRight().getX() - getHitbox().getWidth();
		break;
	default:
		SDL_assert(false);
	}
	switch (mPositionAlignV)
	{
	case ETextBoxPositionAlign_TOP:
		y = blockSpace.getTopLeft().getY();
		break;
	case ETextBoxPositionAlign_CENTER:
		y = blockSpace.getCenter().getY() - (getHitbox().getHeight() / 2);
		break;
	case ETextBoxPositionAlign_BOTTOM:
		y = blockSpace.getBottomRight().getY() - getHitbox().getHeight();
		break;
	default:
		SDL_assert(false);
	}

	return Vect2(x, y);
}
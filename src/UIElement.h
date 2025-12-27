#pragma once
#include <vector>
#include "hitbox.h"
#include "uiBoxHelpers.h"


class UIElement
{
public:
	EUIClass mClassType = EUIClass_ELEMENT;
	ETextBoxPositionAlign	mPositionAlignH = ETextBoxPositionAlign_CENTER; // only affects x coord
	ETextBoxPositionAlign	mPositionAlignV = ETextBoxPositionAlign_CENTER; // only affects y coord
	Edges  mMargins = Edges();

	virtual void updatePosFromBlockSpace(const Hitbox& blockSpace) = 0;

	virtual Hitbox& getHitbox() = 0;

	virtual bool isActive() = 0;

	virtual void setTexture(SDL_Renderer* pRenderer) = 0;

protected:
	// returns the topleft coord the elem should move to according to the given blockspace and the elem's 
	// position alignments
	Vect2 getUpdatedPosFromBlockSpace(const Hitbox& blockSpace);
};

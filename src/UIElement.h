#pragma once
#include <vector>
#include "hitbox.h"
#include "uiBoxHelpers.h"


class UIElement
{
public:
	EUIClass mClassType = EUIClass_ELEMENT;
	EUIPositionAlign mPositionAlignH = EUIPositionAlign_CENTER;
	EUIPositionAlign mPositionAlignV = EUIPositionAlign_CENTER;
	Edges  mMargins = Edges();

protected:
	int	 mMaxWidth = 1;
	int	 mMaxHeight = 1;
	
public:
	// align the element in the given blockspace according to the elem's position alignments
	virtual void updatePosFromBlockSpace(const Hitbox& blockSpace) = 0;

	virtual Hitbox& getHitbox() = 0;

	virtual bool isActive() = 0;

	virtual void setTexture(SDL_Renderer* pRenderer) = 0;

protected:
	// returns the topleft coord the elem should move to according to the given blockspace and the elem's 
	// position alignments
	Vect2 getUpdatedPosFromBlockSpace(const Hitbox& blockSpace);
};

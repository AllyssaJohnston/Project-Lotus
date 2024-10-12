#pragma once
#include "entityQuadTree.h"
#include "helperClass.h"
#include "platformHelper.h"

EntityQuadTree::EntityQuadTree(Hitbox givenHitbox, std::vector<Entity*> pParentEntities)
{
	mHitbox = givenHitbox;
	for (Entity* pEntity : pParentEntities)
	{
		AddItem(pEntity);
	}
	if (mpEntities.size() > mNumPrefferedItems && mpEntities.size() != pParentEntities.size())
	{
		createChildrenQuads();
	}
}

EntityQuadTree::~EntityQuadTree()
{
	if (mHasChildrenQuads)
	{
		mpQuadTopLeft->~EntityQuadTree();
		mpQuadTopRight->~EntityQuadTree();
		mpQuadBottomLeft->~EntityQuadTree();
		mpQuadBottomRight->~EntityQuadTree();
	}
	else
	{
		mpQuadTopLeft		= nullptr;
		mpQuadTopRight		= nullptr;
		mpQuadBottomLeft	= nullptr;
		mpQuadBottomRight	= nullptr;
	}
}

Hitbox EntityQuadTree::getHitbox()
{
	return mHitbox;
}

void EntityQuadTree::SetHitbox(Hitbox hitbox)
{
	mHitbox = hitbox;
}

void EntityQuadTree::AddItem(Entity* pEntityToAdd)
{
	if (mHasChildrenQuads)
	{
		mpQuadTopLeft->AddItem(pEntityToAdd);
		mpQuadTopRight->AddItem(pEntityToAdd);
		mpQuadBottomLeft->AddItem(pEntityToAdd);
		mpQuadBottomRight->AddItem(pEntityToAdd);
		return;
	}

	if (mHitbox.overlap(pEntityToAdd->getMovementManager().getHitbox()))
	{
		addToListIfUnique(mpEntities, pEntityToAdd);
	}
	if (pEntityToAdd->getSubClassType() == EEntitySubClassTypes_AREA_EFFECT)
	{
		AreaEffectPlatform* pAreaEffectPlatform = (AreaEffectPlatform*)pEntityToAdd;
		if (mHitbox.overlap(pAreaEffectPlatform->mAreaEffectHitbox))
		{
			addToListIfUnique(mpEntities, pEntityToAdd);
		}
	}

	if (mpEntities.size() > mNumPrefferedItems)
	{
		createChildrenQuads();
	}
}

void EntityQuadTree::RemoveItem(Entity* pEntityToRemove)
{
	if (mHasChildrenQuads)
	{
		mpQuadTopLeft->RemoveItem(pEntityToRemove);
		mpQuadTopRight->RemoveItem(pEntityToRemove);
		mpQuadBottomLeft->RemoveItem(pEntityToRemove);
		mpQuadBottomRight->RemoveItem(pEntityToRemove);
	}
	else
	{
		for (int i = (int)mpEntities.size() - 1; i > -1; i--)
		{
			if (mpEntities[i] == pEntityToRemove)
			{
				mpEntities.erase(mpEntities.begin() + i);
				//delete mpEntities[i];
			}
		}
	}
}

void EntityQuadTree::getEntitiesInHitbox(std::vector<Entity*>& list, Hitbox givenHitbox)
{
	if (mHasChildrenQuads)
	{
		mpQuadTopLeft->getEntitiesInHitbox(list, givenHitbox);
		mpQuadTopRight->getEntitiesInHitbox(list, givenHitbox);
		mpQuadBottomLeft->getEntitiesInHitbox(list, givenHitbox);
		mpQuadBottomRight->getEntitiesInHitbox(list, givenHitbox);
	}
	else
	{
		for (Entity* pEntityToCheck : mpEntities)
		{
			if (pEntityToCheck->getMovementManager().getHitbox().overlap(givenHitbox))
			{
				addToListIfUnique(list, pEntityToCheck);
			}
			if (pEntityToCheck->getSubClassType() == EEntitySubClassTypes_AREA_EFFECT)
			{
				AreaEffectPlatform* pAreaEffectPlatform = (AreaEffectPlatform*)pEntityToCheck;
				if (mHitbox.overlap(pAreaEffectPlatform->mAreaEffectHitbox))
				{
					addToListIfUnique(mpEntities, pEntityToCheck);
				}
			}
		}
	}
}

void EntityQuadTree::getEntitiesInHitbox(std::vector<Entity*>& list, Hitbox givenHitbox, EEntityClassTypes classType)
{
	if (mHasChildrenQuads)
	{
		mpQuadTopLeft->getEntitiesInHitbox(list, givenHitbox, classType);
		mpQuadTopRight->getEntitiesInHitbox(list, givenHitbox, classType);
		mpQuadBottomLeft->getEntitiesInHitbox(list, givenHitbox, classType);
		mpQuadBottomRight->getEntitiesInHitbox(list, givenHitbox, classType);
	}
	else
	{
		for (Entity* pEntityToCheck : mpEntities)
		{
			if (pEntityToCheck->getClassType() == classType)
			{
				if (pEntityToCheck->getMovementManager().getHitbox().overlap(givenHitbox))
				{
					addToListIfUnique(list, pEntityToCheck);
				}
				if (pEntityToCheck->getSubClassType() == EEntitySubClassTypes_AREA_EFFECT)
				{
					AreaEffectPlatform* pAreaEffectPlatform = (AreaEffectPlatform*)pEntityToCheck;
					if (mHitbox.overlap(pAreaEffectPlatform->mAreaEffectHitbox))
					{
						addToListIfUnique(mpEntities, pEntityToCheck);
					}
				}
			}
		}
	}
}

void EntityQuadTree::createChildrenQuads()
{
	mHasChildrenQuads = true;

	int quadWidth = mHitbox.getWidth() / 2;
	int quadHeight = mHitbox.getHeight() / 2;
	int x1 = mHitbox.getTopLeft().getX();
	int xMid = mHitbox.getTopLeft().getX() + quadWidth;
	int y1 = mHitbox.getTopLeft().getY();
	int yMid = mHitbox.getTopLeft().getY() + quadHeight;


	mpQuadTopLeft		= new EntityQuadTree(Hitbox(CoordsX1Y1WidthHeight(x1,   y1,		quadWidth, quadHeight)), mpEntities);
	mpQuadTopRight		= new EntityQuadTree(Hitbox(CoordsX1Y1WidthHeight(xMid, y1,		quadWidth, quadHeight)), mpEntities);
	mpQuadBottomLeft	= new EntityQuadTree(Hitbox(CoordsX1Y1WidthHeight(x1,	yMid,	quadWidth, quadHeight)), mpEntities);
	mpQuadBottomRight	= new EntityQuadTree(Hitbox(CoordsX1Y1WidthHeight(xMid, yMid,	quadWidth, quadHeight)), mpEntities);

	mpEntities.clear();
}
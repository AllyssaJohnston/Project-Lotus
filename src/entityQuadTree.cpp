#pragma once
#include "entityQuadTree.h"
#include "helperClass.h"
#include "platformHelper.h"

EntityQuadTree::EntityQuadTree(const Hitbox& givenHitbox, std::vector<Entity*>& pParentEntities)
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
		mpEntities.clear(); // don't delete them
		mpQuadTopLeft		= nullptr;
		mpQuadTopRight		= nullptr;
		mpQuadBottomLeft	= nullptr;
		mpQuadBottomRight	= nullptr;
	}
}

Hitbox EntityQuadTree::getHitbox() const { return mHitbox; }

void EntityQuadTree::SetHitbox(const Hitbox& hitbox) { mHitbox = hitbox; }

void EntityQuadTree::AddItem(Entity& entityToAdd)
{
	if (mHasChildrenQuads)
	{
		mpQuadTopLeft->AddItem(entityToAdd);
		mpQuadTopRight->AddItem(entityToAdd);
		mpQuadBottomLeft->AddItem(entityToAdd);
		mpQuadBottomRight->AddItem(entityToAdd);
		return;
	}

	if (mHitbox.overlap(entityToAdd.getMovementManager().getHitbox()))
	{
		addToListIfUnique(mpEntities, &entityToAdd);
	}
	if (entityToAdd.getSubClassType() == EEntitySubClassTypes_AREA_EFFECT)
	{
		AreaEffectPlatform& areaEffectPlatform = (AreaEffectPlatform&)(entityToAdd);
		if (mHitbox.overlap(areaEffectPlatform.mAreaEffectHitbox))
		{
			addToListIfUnique(mpEntities, &entityToAdd);
		}
	}

	if (mpEntities.size() > mNumPrefferedItems)
	{
		createChildrenQuads();
	}
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


void EntityQuadTree::RemoveItem(const Entity& entityToRemove)
{
	if (mHasChildrenQuads)
	{
		mpQuadTopLeft->RemoveItem(entityToRemove);
		mpQuadTopRight->RemoveItem(entityToRemove);
		mpQuadBottomLeft->RemoveItem(entityToRemove);
		mpQuadBottomRight->RemoveItem(entityToRemove);
	}
	else
	{
		std::vector<Entity*>::iterator iter = std::find(mpEntities.begin(), mpEntities.end(), &entityToRemove);
		if (iter != mpEntities.end())
		{
			mpEntities.erase(iter);
		}
		
	}
}

void EntityQuadTree::RemoveItem(const Entity* const pEntityToRemove)
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
		std::vector<Entity*>::iterator iter = std::find(mpEntities.begin(), mpEntities.end(), pEntityToRemove);
		if (iter != mpEntities.end())
		{
			mpEntities.erase(iter);
		}

	}
}

void EntityQuadTree::getEntitiesInHitbox(std::vector<Entity*>& pList, const Hitbox& givenHitbox) const
{
	if (mHasChildrenQuads)
	{
		mpQuadTopLeft->getEntitiesInHitbox(pList, givenHitbox);
		mpQuadTopRight->getEntitiesInHitbox(pList, givenHitbox);
		mpQuadBottomLeft->getEntitiesInHitbox(pList, givenHitbox);
		mpQuadBottomRight->getEntitiesInHitbox(pList, givenHitbox);
	}
	else
	{
		for (Entity* pEntityToCheck : mpEntities)
		{
			if (pEntityToCheck->getMovementManager().getHitbox().overlap(givenHitbox))
			{
				addToListIfUnique(pList, pEntityToCheck);
			}
			if (pEntityToCheck->getSubClassType() == EEntitySubClassTypes_AREA_EFFECT)
			{
				AreaEffectPlatform* pAreaEffectPlatform = (AreaEffectPlatform*)pEntityToCheck;
				if (mHitbox.overlap(pAreaEffectPlatform->mAreaEffectHitbox))
				{
					addToListIfUnique(pList, pEntityToCheck);
				}
			}
		}
	}
}

void EntityQuadTree::getEntitiesInHitbox(std::vector<Entity*>& pList, const Hitbox& givenHitbox, EEntityClassTypes classType) const
{
	if (mHasChildrenQuads)
	{
		mpQuadTopLeft->getEntitiesInHitbox(pList, givenHitbox, classType);
		mpQuadTopRight->getEntitiesInHitbox(pList, givenHitbox, classType);
		mpQuadBottomLeft->getEntitiesInHitbox(pList, givenHitbox, classType);
		mpQuadBottomRight->getEntitiesInHitbox(pList, givenHitbox, classType);
	}
	else
	{
		for (Entity* pEntityToCheck : mpEntities)
		{
			if (pEntityToCheck->getClassType() == classType)
			{
				if (pEntityToCheck->getMovementManager().getHitbox().overlap(givenHitbox))
				{
					addToListIfUnique(pList, pEntityToCheck);
				}
				if (classType == EEntitySubClassTypes_AREA_EFFECT && pEntityToCheck->getSubClassType() == EEntitySubClassTypes_AREA_EFFECT)
				{
					AreaEffectPlatform* pAreaEffectPlatform = (AreaEffectPlatform*)pEntityToCheck;
					if (mHitbox.overlap(pAreaEffectPlatform->mAreaEffectHitbox))
					{
						addToListIfUnique(pList, pEntityToCheck);
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
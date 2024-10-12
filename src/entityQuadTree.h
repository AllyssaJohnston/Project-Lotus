#pragma once
#include "hitboxHelper.h"
#include "entityHelper.h"

class EntityQuadTree
{
private:
	Hitbox mHitbox = Hitbox();
	EntityQuadTree* mpQuadTopLeft		= nullptr;
	EntityQuadTree* mpQuadTopRight		= nullptr;
	EntityQuadTree* mpQuadBottomLeft	= nullptr;
	EntityQuadTree* mpQuadBottomRight	= nullptr;

	bool mHasChildrenQuads = false;
	int mNumPrefferedItems = 6;
	std::vector<Entity*> mpEntities;
	
public:
	EntityQuadTree(){}

	EntityQuadTree(Hitbox givenHitbox, std::vector<Entity*> pParentEntities);
	
	~EntityQuadTree();

	Hitbox getHitbox();

	void SetHitbox(Hitbox hitbox);

	void AddItem(Entity* pEntityToAdd);

	void RemoveItem(Entity* pEntityToRemove);

	void getEntitiesInHitbox(std::vector<Entity*>& list, Hitbox givenHitbox);

	void getEntitiesInHitbox(std::vector<Entity*>& list, Hitbox givenHitbox, EEntityClassTypes classType);

private:
	void createChildrenQuads();

};
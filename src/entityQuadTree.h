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

	EntityQuadTree(const Hitbox& givenHitbox, std::vector<Entity*>& pParentEntities);
	
	~EntityQuadTree();

	Hitbox getHitbox() const;

	void SetHitbox(const Hitbox& hitbox);

	void AddItem(Entity& entityToAdd);
	void AddItem(Entity* pEntityToAdd);

	void RemoveItem(const Entity& entityToRemove);
	void RemoveItem(const Entity* const pEntityToRemove);

	void getEntitiesInHitbox(std::vector<Entity*>& pList, const Hitbox& givenHitbox) const;

	void getEntitiesInHitbox(std::vector<Entity*>& pList, const Hitbox& givenHitbox, EEntityClassTypes classType) const;

private:
	void createChildrenQuads();

};
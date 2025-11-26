#pragma once
#include "entityHelper.h"

class DamageManager
{
public:
	DamageManager(){}

    bool willKillCharacter(const Entity* pEntity, const EEntityEdgeType edgeType) const;

    bool willProjectileKillCharacter(const Entity* projectile, const Entity* pEntity) const;

    void spreadEdges(Entity* pSpreadingEntity, Entity* pEntityToSpreadTo) const;
};
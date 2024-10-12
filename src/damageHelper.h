#pragma once
#include "entityHelper.h"

class DamageManager
{
public:
	DamageManager(){}

    bool willKillCharacter(Entity* pEntity, EEntityEdgeType edgeType);

    bool willProjectileKillCharacter(Entity* projectile, Entity* pEntity);

    void spreadEdges(Entity* pSpreadingEntity, Entity* pEntityToSpreadTo);
};
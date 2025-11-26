#include "damageHelper.h"

bool DamageManager::willKillCharacter(const Entity* pEntity, const EEntityEdgeType edgeType) const
{
    if (pEntity->hasCharacteristic(EEntityCharacteristicsTypes_INVINCIBLE))
    {
        return false;
    }

    return ((edgeType == EEntityEdgeType_HAZARDOUS  and !pEntity->hasCharacteristic(EEntityCharacteristicsTypes_CRUSH_RESISTENT))
         or (edgeType == EEntityEdgeType_SPIKEY     and !pEntity->hasCharacteristic(EEntityCharacteristicsTypes_SPIKE_RESISTENT))
         or (edgeType == EEntityEdgeType_BURNING    and !pEntity->hasCharacteristic(EEntityCharacteristicsTypes_FIRE_RESISTENT))
         or (edgeType == EEntityEdgeType_ELECTRIC   and !pEntity->hasCharacteristic(EEntityCharacteristicsTypes_ELECTRICITY_RESISTENT)));
}

bool DamageManager::willProjectileKillCharacter(const Entity* projectile, const Entity* pEntity) const { return pEntity->mVulnerableToProjectiles; }

void DamageManager::spreadEdges(Entity* pSpreadingEntity, Entity* pEntityToSpreadTo) const
{
    if (pSpreadingEntity->getAmAlive() and pEntityToSpreadTo->getAmAlive() and ( pSpreadingEntity->mSpreadEdges 
        or (pSpreadingEntity->getMovementManager().mHaveStatusEffect and pEntityToSpreadTo->mImmuneToStatusEffects == false )))
    {
        HitboxEdges edges    = pSpreadingEntity->getMovementManager().getHitboxEdges();
        int         interval =  pSpreadingEntity->mSpreadEdgesInterval;
        pEntityToSpreadTo->getMovementManager().setCurHitboxEdges(edges, interval);
    }
}
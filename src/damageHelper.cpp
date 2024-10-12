#include "damageHelper.h"

bool DamageManager::willKillCharacter(Entity* pEntity, EEntityEdgeType edgeType)
{
    if (pEntity->hasCharacteristic(EEntityCharacteristicsTypes_INVINCIBLE))
    {
        return false;
    }

    if      ((edgeType == EEntityEdgeType_HAZARDOUS and (pEntity->hasCharacteristic(EEntityCharacteristicsTypes_CRUSH_RESISTENT) == false))
        or   (edgeType == EEntityEdgeType_SPIKEY    and (pEntity->hasCharacteristic(EEntityCharacteristicsTypes_SPIKE_RESISTENT) == false))
        or   (edgeType == EEntityEdgeType_BURNING   and (pEntity->hasCharacteristic(EEntityCharacteristicsTypes_FIRE_RESISTENT)  == false))
        or   (edgeType == EEntityEdgeType_ELECTRIC  and (pEntity->hasCharacteristic(EEntityCharacteristicsTypes_ELECTRICITY_RESISTENT)  == false)))
    {
        return true;
    }
    return false;
}

bool DamageManager::willProjectileKillCharacter(Entity* projectile, Entity* pEntity)
{
    if (pEntity->mVulnerableToProjectiles)
    {
        return true;
    }
    return false;
}

void DamageManager::spreadEdges(Entity* pSpreadingEntity, Entity* pEntityToSpreadTo)
{
    if (pSpreadingEntity->getAmAlive() and pEntityToSpreadTo->getAmAlive() and ( pSpreadingEntity->mSpreadEdges 
        or (pSpreadingEntity->getMovementManager().mHaveStatusEffect and pEntityToSpreadTo->mImmuneToStatusEffects == false )))
    {
        HitboxEdges edges    = pSpreadingEntity->getMovementManager().getHitboxEdges();
        int         interval =  pSpreadingEntity->mSpreadEdgesInterval;
        pEntityToSpreadTo->getMovementManager().setCurHitboxEdges(edges, interval);
    }
}
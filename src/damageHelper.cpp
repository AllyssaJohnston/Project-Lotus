#include "damageHelper.h"

bool willKillCharacter(const Entity& entity, const EEntityEdgeType edgeType)
{
    if (entity.hasCharacteristic(EEntityCharacteristicsTypes_INVINCIBLE))
    {
        return false;
    }

    return ((edgeType == EEntityEdgeType_HAZARDOUS  and !entity.hasCharacteristic(EEntityCharacteristicsTypes_CRUSH_RESISTENT))
         or (edgeType == EEntityEdgeType_SPIKEY     and !entity.hasCharacteristic(EEntityCharacteristicsTypes_SPIKE_RESISTENT))
         or (edgeType == EEntityEdgeType_BURNING    and !entity.hasCharacteristic(EEntityCharacteristicsTypes_FIRE_RESISTENT))
         or (edgeType == EEntityEdgeType_ELECTRIC   and !entity.hasCharacteristic(EEntityCharacteristicsTypes_ELECTRICITY_RESISTENT)));
}

void spreadEdges(Entity& spreadingEntity, Entity& entityToSpreadTo)
{
    if (spreadingEntity.getAmAlive() and entityToSpreadTo.getAmAlive() and ( spreadingEntity.mSpreadEdges 
        or (spreadingEntity.getMovementManager().mHaveStatusEffect and !entityToSpreadTo.mImmuneToStatusEffects)))
    {
        HitboxEdges edges    = spreadingEntity.getMovementManager().getHitboxEdges();
        int         interval = spreadingEntity.mSpreadEdgesInterval;
        entityToSpreadTo.getMovementManager().setCurHitboxEdges(edges, interval);
    }
}
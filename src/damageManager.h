#pragma once
#include "entity.h"


bool willKillCharacter(const Entity& entity, const EEntityEdgeType edgeType);

void spreadEdges(Entity& spreadingEntity, Entity& entityToSpreadTo);
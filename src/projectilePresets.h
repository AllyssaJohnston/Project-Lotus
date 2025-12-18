#pragma once
#include "entityPresets.h"

struct ProjectilePreset : public EntityPreset { ProjectilePreset(); };

struct PlayerProjectilePreset : public ProjectilePreset { PlayerProjectilePreset(EEntityMovementPath movementPath); };

struct EnemyProjectilePreset : public ProjectilePreset { EnemyProjectilePreset(); };
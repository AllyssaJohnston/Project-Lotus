#pragma once
#include "entityPresets.h"

struct ProjectilePreset : public CCharacterPreset
{
	ProjectilePreset();
};

struct PlayerProjectilePreset : public ProjectilePreset
{
	PlayerProjectilePreset(EEntityMovementPath movementPath);
};

struct EnemyProjectilePreset : public ProjectilePreset
{
	EnemyProjectilePreset();
};
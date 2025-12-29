#pragma once
#include "entityPresets.h"
#include "globals.h"

struct CPlayerPreset : public EntityPreset
{
	int mSwordSlashWidth        = 50;
	int mSwordSlashHeight       = 30;

	CPlayerPreset();
};

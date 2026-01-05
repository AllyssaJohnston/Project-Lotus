#pragma once
#include <string>
#include "combatAttack.h"
#include "combatAttackHelpers.h"
#include "UIBlock.h"
#include "uiBox.h"
#include "gameStateHelpers.h"
#include "miniGameStateHelpers.h"
#include "settingsManager.h"
#include "worldData.h"
#include "screen.h"

enum EMenuPageType
{
	EMenuPageType_INVALID = -1,
	EMenuPageType_MAIN_MENU,
	EMenuPageType_MAIN_GAME_MENU,
	EMenuPageType_MINI_GAME_MENU,
	EMenuPageType_MAX
};

void drawCircle(const SDL_Color& color, const Vect2& center, int radius, const ScreenObject& screen);
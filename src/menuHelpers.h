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


bool shouldShowUIBox(const UIBoxData& data, const MiniGameStateManagerData& managerData, const CombatManager& combatManager);

void drawCircle(const SDL_Color& color, const Vect2& center, int radius, const ScreenObject& screen);


std::string updateGameStatBoxCurTextBoxMessage(const TextBox& textBox, const std::string& curKeys, const WorldData& worldData, const SettingsManager& settingsManager);

std::string updateMiniGameStatBoxCurTextBoxMessage(const TextBox& textBox, const MiniGameStateManagerData& managerData, const MiniGameWorldData& worldData);

std::string updateCharacterStatBoxCurTextBoxMessage(const TextBox& textBox, const MiniGameStateManagerData& managerData, const MiniGameWorldData& worldData);

std::string updateHealthStatBoxCurTextBoxMessage(const HealthBox& healthBox, const MiniGameWorldData& worldData);

float updateHealthStatBoxCurTextBoxRatio(const HealthBox& healthBox, const MiniGameWorldData& worldData);

void printBlock(const ScreenObject& screen, const UIBlock& block);

void printTextBox(const ScreenObject& screen, const TextBox& textBox);

void printImageBox(const ScreenObject& screen, const ImageBox& iamgeBox);

void printShapeBox(const ScreenObject& screen, const ShapeBox& shapeBox);

void printHealthBox(const ScreenObject& screen, const HealthBox& healthBox);
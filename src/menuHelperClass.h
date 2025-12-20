#pragma once
#include <string>
#include "combatAttackHelper.h"
#include "textBoxHelper.h"
#include "gameStateHelperClass.h"
#include "miniGameStateHelperClass.h"
#include "settingsHelper.h"
#include "worldDataHelper.h"

std::string getAttackType(const Attack& attack);

std::string getAttackDamage(const Attack& attack, int characterDamage);

std::string getSpecialEffect(const Attack& attack);

bool shouldShowTextBox(const TextBox& textBox, const MiniGameStateManagerData& data);

bool shouldShowTextBox(const ShapeBox& shapeBox, const MiniGameStateManagerData& data);

void drawCircle(const SDL_Color& color, const Vect2& center, int radius, ScreenObject& screen);


std::string updateGameStatBoxCurTextBoxMessage(const TextBox& textBox, const std::string& curKeys, const WorldData& worldData, const SettingsManager& settingsManager);

std::string updateMiniGameStatBoxCurTextBoxMessage(const TextBox& textBox, const MiniGameStateManagerData& managerData, const MiniGameWorldData& worldData);

std::string updateCharacterStatBoxCurTextBoxMessage(const TextBox& textBox, const MiniGameStateManagerData& managerData, const MiniGameWorldData& worldData);

std::string updateHealthStatBoxCurTextBoxMessage(const HealthBox& healthBox, const MiniGameWorldData& worldData);

float updateHealthStatBoxCurTextBoxRatio(const HealthBox& healthBox, const MiniGameWorldData& worldData);
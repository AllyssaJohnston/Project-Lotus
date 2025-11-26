#pragma once
#include <string>
#include <map>
#include "combatAttackHelper.h"
#include "textBoxHelper.h"
#include "gameStateHelperClass.h"
#include "settingsHelper.h"

std::string getAttackName(Attack attack);

std::string getAttackType(Attack attack);

std::string getAttackDamage(Attack attack, int characterDamage);

std::string getSpecialEffect(Attack attack);

bool shouldShowTextBox(TextBox* pTextBox, MiniGameStateManagerData& data);

bool shouldShowTextBox(ShapeBox* pShapeBox, MiniGameStateManagerData& data);

void drawCircle(SDL_Color color, Vect2 center, int radius, ScreenObject& screen);


std::string updateGameStatBoxCurTextBoxMessage(TextBox* pTextBox, std::string& curKeys, WorldData& worldData, SettingsManager& settingsManager);

std::string updateMiniGameStatBoxCurTextBoxMessage(TextBox* pTextBox, MiniGameStateManagerData& managerData, MiniGameWorldData& worldData);

std::string updateCharacterStatBoxCurTextBoxMessage(TextBox* pTextBox, MiniGameStateManagerData& managerData, MiniGameWorldData& worldData);

std::string updateHealthStatBoxCurTextBoxMessage(HealthBox* pHealthBox, MiniGameWorldData& worldData);

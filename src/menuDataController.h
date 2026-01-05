#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <string>
#include <vector>

#include "helpers.h"

#include "uiBox.h"
#include "UIBlock.h"

#include "settingsManager.h"
#include "screen.h"
#include "worldData.h"
#include "miniGameWorldData.h"

#include "menuManager.h"
#include "menuPage.h"
#include "menuHelpers.h"

class MenuDataController
{
private:
	MenuManager& mMenuManager;
	WorldData& mWorldData;
	MiniGameStateManagerData& mMiniGameStateManagerData;
	MiniGameWorldData& mMiniGameWorldData;
	ScreenObject& mScreen;
	SettingsManager& mSettingsManager;
	KeyboardData& mKeyboardData;
	FontSizeChart& mFontSizeChart;

public:
	MenuDataController(MenuManager& menuManager, WorldData& worldData, MiniGameStateManagerData& miniGameStateManagerData,
		MiniGameWorldData& miniWorldData, ScreenObject& screen, SettingsManager& settingsManager, FontSizeChart& fontSizeChart, KeyboardData& keyboardData);

	void getUpdatedMenuBoxes(EGameState curState, bool forceUpdate);

	void updateUIElements();

private:
	bool shouldShowUIBox(const UIBoxData& data);

	void updateShowStateForCurPageUIBoxes();

	bool shouldUpdateTextBoxShowState(EGameState curState, bool forceUpdate);

	std::string updateGameStatBoxCurTextBoxMessage(const TextBox& textBox);

	std::string updateMiniGameStatBoxCurTextBoxMessage(const TextBox& textBox);

	std::string updateCharacterStatBoxCurTextBoxMessage(const TextBox& textBox);

	std::string updateHealthStatBoxCurTextBoxMessage(const HealthBox& healthBox);

	void updateUIBoxDisable(TextBox& textBox);

	float updateHealthStatBoxCurTextBoxRatio(const HealthBox& healthBox);
};
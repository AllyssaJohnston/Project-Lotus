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

#include "menuPage.h"
#include "menuHelpers.h"




class MenuManager
{
public:
	std::vector <MenuPage*> mpMenuPages;
	MenuPage* mpCurMenuPage		= nullptr;
	MenuPage* mpLastFrameMenuPage	= nullptr;
	
	MenuManager(ScreenObject& screen, WorldData& worldData, SettingsManager& settingsManager, FontSizeChart& fontSizeChart, MiniGameStateManagerData& miniGameStateManagerData, 
			MiniGameWorldData& miniWorldData);
	
	~MenuManager();

	void preTick();

	TextBox* returnMouseTextBox(Vect2& gameUnitsMousePos);

	void setAllTextBoxTextures();

	void setUpBlocks();

	void setCurMenuPage(MenuPage* pNewMenuPage);

	void renderMenus(EGameState curState, bool forceUpdate, std::string& curKeys);

	bool shouldUpdateTextBoxShowState(EGameState curState, bool forceUpdate);

	void updateUIElements();

private:
	ScreenObject& mScreen;
	WorldData& mWorldData;
	SettingsManager& mSettingsManager;
	FontSizeChart& mFontSizeChart;
	MiniGameStateManagerData& mMiniGameStateManagerData;
	MiniGameWorldData& mMiniGameWorldData;

	void getUpdatedMenuBoxes(EGameState curState, bool forceUpdate, std::string& curKeys);

	void printBoxes();
};
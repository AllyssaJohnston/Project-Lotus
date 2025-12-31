#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <string>
#include <vector>

#include "helpers.h"

#include "uiBox.h"
#include "UIBlock.h"

#include "screen.h"

#include "menuPage.h"
#include "menuHelpers.h"


class MenuManager
{
public:
	std::vector <MenuPage*> mpMenuPages;
	MenuPage* mpCurMenuPage			= nullptr;
	MenuPage* mpLastFrameMenuPage	= nullptr;
	
	MenuManager(ScreenObject& screen, SettingsManager& settingsManager, FontSizeChart& fontSizeChart);
	
	~MenuManager();

	void preTick();

	TextBox* returnMouseTextBox(Vect2& gameUnitsMousePos);

	void setAllTextBoxTextures();

	void setUpBlocks();

	void setCurMenuPage(MenuPage* pNewMenuPage);

	void renderMenus();

private:
	ScreenObject& mScreen;
	FontSizeChart& mFontSizeChart;
	SettingsManager& mSettingsManager;

	void printBoxes();

	void printBlock(const UIBlock& block);

	void printTextBox(const TextBox& textBox);

	void printImageBox(const ImageBox& iamgeBox);

	void printShapeBox(const ShapeBox& shapeBox);

	void printHealthBox(const HealthBox& healthBox);
};


#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <string>
#include <vector>
#include "textBoxHelper.h"
#include "helperClass.h"
#include "gameStateHelperClass.h"
#include "settingsHelper.h"
#include "screenHelper.h"
#include "worldDataHelper.h"
#include "miniGameWorldDataHelper.h"
#include "menuHelperClass.h"
#include "UIBlockHelper.h"

enum EMenuPageType
{
	EMenuPageType_INVALID = -1,
	EMenuPageType_MAIN_MENU,
	EMenuPageType_MAIN_GAME_MENU,
	EMenuPageType_MINI_GAME_MENU,
	EMenuPageType_SETTINGS_MENU,
	EMenuPageType_MAX
};

class MenuPage
{
private:
	TextBox* mpCurSelectedTextBox	= nullptr;
	TextBox* mpLastFrameCurTextBox	= nullptr;
	TextBox* mpCurTextBox			= nullptr;
public:
	std::vector <TextBox*>		mpAllSelectableTextBoxes;
	std::vector <TextBox*>		mpAllDisplayOnlyTextBoxes;
	std::vector <ImageBox*>		mpImageBoxes;
	std::vector <ShapeBox*>		mpShapeBoxes;
	std::vector <HealthBox*>	mpHealthBoxes;
	std::vector <UIBlock*>		mpBlocks;

	~MenuPage();

	void preTick();

	void addBox(TextBox* pTextBox, bool selectable, UIBlock* pBlock);
	void addBox(ImageBox* pTextBox, UIBlock* pBlock);
	void addBox(ShapeBox* pTextBox, UIBlock* pBlock);
	void addBox(HealthBox* pTextBox, UIBlock* pBlock);

	void useInput(const std::vector <int>& eventVect);

	void setCurTextBoxIfValid(int count);

	void setCurTextBox(TextBox* pTextBox);

	void setCurSelectedTextBox(TextBox* pTextBox);

	TextBox* getCurTextBox() const;

	TextBox* getCurSelectedTextBox() const;

	bool curTextBoxChange();

	int getCurTextBoxIndex() const;

	std::vector<SDL_Texture*> getCurTextBoxTextures() const;

	std::vector <TextBox*> getAllTextBoxes() const;

	std::vector <TextBox*> getCurTextBoxes() const;

	std::vector <UIBlock*> getAllBlocks() const;

	void updateAllTextBoxShowState(MiniGameStateManagerData& data);

	void setDefaultSelectedBox();

	void adjustBlocks();

	void deleteBlock(UIBlock* pBlock);
};

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
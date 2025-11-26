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

/*class EMenusType(Enum):
    MAIN_MENU                  = 0
    LEVEL_SELECTION_MENU       = 1
    SAVE_MENU                  = 2
    LOAD_MENU                  = 3
    SHOP_MENU                  = 4
    SHOP_ITEM_MENU             = 5
    STATS_MENU                 = 6
    HANDBOOK_MENU              = 7
    SETTINGS_MENU              = 8
    CONTROLS_MENU              = 9
    GAME_PLAY                  = 10
    GAME_LOADING_SCREEN        = 11
    GAME_LOADED_SCREEN         = 12
    MINI_GAME_MENU             = 13*/

class MenuPage
{
private:
	TextBox *	mpCurSelectedTextBox	= nullptr;
	TextBox*	mpLastFrameCurTextBox	= nullptr;
	TextBox *	mpCurTextBox			= nullptr;
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

	void setCurTextBox(TextBox * textBox);

	void setCurSelectedTextBox(TextBox* textBox);

	TextBox* getCurTextBox();

	TextBox* getCurSelectedTextBox();

	bool curTextBoxChange();

	int getCurTextBoxIndex();

	std::vector<SDL_Texture*> getCurTextBoxTextures() const;

	std::vector <TextBox*> getAllTextBoxes();

	std::vector <TextBox*> getCurTextBoxes();

	std::vector <UIBlock*> getAllBlocks();

	void updateAllTextBoxShowState(MiniGameStateManagerData& data);

	void setDefaultSelectedBox();

	void adjustBlocks();

	void deleteBlock(UIBlock* pBlock);
};

class MenuManager
{
public:
	std::vector <MenuPage*> mpMenuPages;
	MenuPage * mpCurMenuPage		= nullptr;
	MenuPage * mpLastFrameMenuPage	= nullptr;
	
	MenuManager(ScreenObject& screen, WorldData& worldData, SettingsManager& settingsManager, FontSizeChart& fontSizeChart, MiniGameStateManagerData& miniGameStateManagerData, 
			MiniGameWorldData& miniWorldData);
	
	~MenuManager();

	void preTick();

	TextBox* returnMouseTextBox(Vect2 gameUnitsMousePos);

	void setAllTextBoxTextures();

	void setUpBlocks();

	void setCurMenuPage(MenuPage* newMenuPage);

	void renderMenus(EGameState curState, bool forceUpdate, std::string curKeys);

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
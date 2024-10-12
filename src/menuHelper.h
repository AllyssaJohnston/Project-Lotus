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
	TextBox * mpCurSelectedTextBox  = nullptr;
	TextBox * mpCurTextBox			= nullptr;
public:
	std::vector <TextBox*>  mpAllSelectableTextBoxes;
	std::vector <TextBox*>  mpCurSelectableTextBoxes;
	std::vector <TextBox*>  mpAllDisplayOnlyTextBoxes;
	std::vector <TextBox*>  mpCurDisplayOnlyTextBoxes;
	std::vector <ImageBox*> mpImageBoxes;
	std::vector <UIBlock*>  mpBlocks;

	~MenuPage()
	{
		mpCurSelectedTextBox = nullptr;
		mpCurTextBox = nullptr;
		for (TextBox* box : mpAllSelectableTextBoxes)
		{
			delete box;
		}
		for (TextBox* box : mpAllDisplayOnlyTextBoxes)
		{
			delete box;
		}
		for (ImageBox* box : mpImageBoxes)
		{
			delete box;
		}
		for (UIBlock* box : mpBlocks)
		{
			delete box;
		}
	}

	void setCurSelectableAndDisplayOnlyTextBoxes(std::vector <TextBox*> curSelectableTextBoxes, std::vector<TextBox*> curDisplayOnlyTextBoxes);

	void useInput(const std::vector <int>& eventVect);

	void setCurTextBoxIfValid(int count);

	void setCurTextBox(TextBox * textBox);

	void setCurSelectedTextBox(TextBox* textBox);

	TextBox* getCurTextBox();

	TextBox* getCurSelectedTextBox();

	int getCurTextBoxIndex();

	std::vector <TextBox*> returnAllTextBoxes();

	std::vector <TextBox*> returnAllCurTextBoxes();

	void adjustBlocks(float textSizeFactor);
};

class MenuManager
{
public:
	std::vector <MenuPage*> mpMenuPages;
	MenuPage * mpCurMenuPage		= nullptr;
	MenuPage * mpLastFrameMenuPage	= nullptr;
	
	MenuManager(){}
	
	~MenuManager();

	void preTick();

	TextBox* returnMouseTextBox(Vect2 gameUnitsMousePos, ScreenObject& screenObject);

	bool shouldShowTextBox(MiniGameStateManagerData& miniGameStateManagerData, TextBox* pTextBox);

	void setAllTextBoxTextures(SDL_Renderer * pRenderer, int textIncrease);

	void setUpBlocks();

	std::vector<SDL_Texture*> getCurTextBoxTextures();

	int getCurTextBoxIndex() const;

	void setCurMenuPage(MenuPage* newMenuPage);

	void renderMenus(ScreenObject& screenObject, GameStateManagerData& gameStateManagerData, MiniGameStateManagerData& miniGameStateManagerData,
				WorldData& worldData, SettingsManager& settingsManager);

	void shouldSetMenuManagerCurSelectableAndDisplayOnlyTextBoxes(GameStateManagerData& gameStateManagerData, MiniGameStateManagerData& miniGameStateManagerData);

	void setMenuManagerCurSelectableAndDisplayOnlyTextBoxes(MiniGameStateManagerData& miniGameStateManagerData);

	std::string updateGameStatBoxCurTextBoxMessage(WorldData& worldData, SettingsManager& settingsManager, TextBox* pTextBox);

	std::string updateCharacterStatBoxCurTextBoxMessage(MiniGameStateManagerData& miniGameStateManagerData, TextBox* pTextBox);

	void updateUIElements(ScreenObject& screenObject, WorldData& worldData);
};
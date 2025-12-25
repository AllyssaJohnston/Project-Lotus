#include "lotusAdventureMenus.h"

int characterStatsBlockNum;
int characterOptionsBlockNum;
int characterOptionsDetailsBlockNum;
int attackPanelBlockNum;
int characterSelectionPanelBlockNum;

void createMenus(MenuManager& menuManager, const ScreenObject& screen, MiniGameWorldData& worldData)
{
	const char* fontFileName = StyleManager::fontName;
	SDL_Color pink		= StyleManager::pink;
	SDL_Color white		= StyleManager::white;
	SDL_Color teal		= StyleManager::teal;
	SDL_Color hintBlue	= StyleManager::hintBlue;
	SDL_Color clear		= StyleManager::clear;

	const int levelChunkWidth	= screen.mGameLevelChunkWidth;
	const int levelChunkHeight	= screen.mGameLevelChunkHeight;

	const bool fill = true;

	//MAIN MENU PAGE
	MenuPage* mainMenuPage = new MenuPage();

	int maxWidth = levelChunkWidth;
	BlockAlignElementsHorizontally* blockH = new BlockAlignElementsHorizontally(Hitbox(CoordsX1Y1WidthHeight(0, 0, maxWidth, 300)), ETextBoxPositionAlign_CENTER, EDirection_LEFT_AND_RIGHT, EDirection_DOWN, !fill, !fill, Edges(200, 0, 0, 0), clear, "head block");
	BlockAlignElementsVertically* blockV = new BlockAlignElementsVertically(maxWidth, 300, ETextBoxPositionAlign_CENTER, EDirection_DOWN, !fill, !fill, Edges(0, 300, 0, 0));
	mainMenuPage->addBox(new ImageBox(ImageBoxPreset(), ImageBoxPositionInfo(0, ETextBoxPositionAlign_LEFT, 1600, 200, Edges()), "Menu/TitleScreen.bmp"), blockV);
	mainMenuPage->addBox(new TextBox(StandardTextBoxPreset("PLAY GAME"),		ETextBoxFunction_PLAY_GAME_BOX,		 TextBoxPositionInfo(ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, maxWidth, 140, Edges(0, 10, 0, 0)), fontFileName, TextBoxSizeInfo(60, 80), TextBoxColorInfo(hintBlue, white, teal, teal)), true, blockV);
	mainMenuPage->addBox(new TextBox(StandardTextBoxPreset("PLAY MINI GAME"),	ETextBoxFunction_PLAY_MINI_GAME_BOX, TextBoxPositionInfo(ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, maxWidth, 140, Edges(0, 0,  0, 0)), fontFileName, TextBoxSizeInfo(60, 80), TextBoxColorInfo(hintBlue, white, teal, teal)), true, blockV);
	blockH->mpSubElems.push_back(blockV);
	
	BlockAlignElementsHorizontally* blockModel = new BlockAlignElementsHorizontally(Hitbox(CoordsX1Y1WidthHeight(0, 900, 600, 600)), ETextBoxPositionAlign_LEFT, EDirection_RIGHT, EDirection_UP, !fill, !fill, Edges(0, 0, 0, 0), clear);
	mainMenuPage->addBox(new ImageBox(ImageBoxPreset(), ImageBoxPositionInfo(0, ETextBoxPositionAlign_LEFT, 600, 600, Edges()), "Menu/TitleScreenModel.bmp"), blockModel);
	
	mainMenuPage->mpBlocks.push_back(blockModel);
	mainMenuPage->mpBlocks.push_back(blockH);


	//debug HUD
	maxWidth = 200;
	int maxHeight = 100;
	BlockAlignElementsVertically* debugHUDBlock = new BlockAlignElementsVertically(Hitbox(CoordsX1Y1WidthHeight(levelChunkWidth, 0, maxWidth, maxHeight)), ETextBoxPositionAlign_LEFT, EDirection_LEFT, EDirection_DOWN, !fill, !fill, Edges());
	mainMenuPage->addBox(new TextBox(GameStatBoxPreset(EGameStatBoxValueToDisplay_CUR_KEYBOARD), ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_CENTER, maxWidth, maxHeight, Edges(3, 0, 0, 0)), fontFileName, TextBoxSizeInfo(12, 12), TextBoxColorInfo(hintBlue)), false, debugHUDBlock);
	mainMenuPage->mpBlocks.push_back(debugHUDBlock);

	menuManager.mpMenuPages.push_back(mainMenuPage);

	//MAIN GAME MENU PAGE
	createMainGameMenu(menuManager, screen);


	//MINI GAME MENU PAGE
	createMiniGameMenu(menuManager, screen, worldData);

	//SETTINGS
	MenuPage* settingsPage = new MenuPage();

	settingsPage->mpAllDisplayOnlyTextBoxes.push_back(new TextBox(StandardTextBoxPreset("SETTINGS"),  ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(Vect2(levelChunkWidth / 2, 200), ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, 1600, 200), fontFileName, TextBoxSizeInfo(60, 80), TextBoxColorInfo(white, teal)));
	settingsPage->mpAllDisplayOnlyTextBoxes.push_back(new TextBox(StandardTextBoxPreset("Text Size"), ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(Vect2(levelChunkWidth / 2, 200), ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, 1600, 200), fontFileName, TextBoxSizeInfo(60, 80), TextBoxColorInfo(white, teal)));
	menuManager.mpMenuPages.push_back(settingsPage);

	menuManager.mpCurMenuPage = menuManager.mpMenuPages[EMenuPageType_MAIN_GAME_MENU];

	menuManager.setAllTextBoxTextures();
	menuManager.setUpBlocks();
}

void createMainGameMenu(MenuManager& menuManager, const ScreenObject& screen)
{
	const SDL_Color& hintBlue	= StyleManager::hintBlue;
	const SDL_Color& clear		= StyleManager::clear;
	const char* fontFileName	= StyleManager::fontName;

	const int levelChunkWidth	= screen.mGameLevelChunkWidth;
	const int levelChunkHeight	= screen.mGameLevelChunkHeight;

	const bool fill = true;


	//MAIN GAME MENU PAGE
	MenuPage* mainGameMenuPage = new MenuPage();

	//Take Damage Screen
	BlockAlignElementsVertically* takeDamageBlock = new BlockAlignElementsVertically(Hitbox(CoordsX1Y1WidthHeight(0, 0, levelChunkWidth, levelChunkHeight)), ETextBoxPositionAlign_LEFT, EDirection_RIGHT, EDirection_DOWN, fill, fill, Edges(), clear, "take damage icon");
	mainGameMenuPage->addBox(new ImageBox(DontAutoShowImageBoxPreset(EUIBoxType_TAKE_DAMAGE_SCREEN), ImageBoxPositionInfo(Vect2(0, 0), 0, ETextBoxPositionAlign_LEFT, 1600, 900), "UIElements/takeDamageScreen.bmp"), takeDamageBlock);
	mainGameMenuPage->mpBlocks.push_back(takeDamageBlock);

	//UPPER HUD
	int maxWidth = 400;
	int maxHeight = 100;
	BlockAlignElementsVertically* upperHUDBlock = new BlockAlignElementsVertically(Hitbox(CoordsX1Y1WidthHeight(0, 0, maxWidth, 450)), ETextBoxPositionAlign_LEFT, EDirection_RIGHT, EDirection_DOWN, !fill, !fill, Edges(8, 0, 4, 0), clear, "main game upper hud block");
	BlockAlignElementsHorizontally* uLine1 = new BlockAlignElementsHorizontally(maxWidth, maxHeight, ETextBoxPositionAlign_CENTER, EDirection_RIGHT, !fill, !fill, Edges());
	mainGameMenuPage->addBox(new ImageBox(ImageBoxPreset(), ImageBoxPositionInfo(0, ETextBoxPositionAlign_LEFT, 80, 80, Edges(0, 0, 0, 0)), "UIElements/HUDUI.bmp"), uLine1);
	mainGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EGameStatBoxValueToDisplay_CUR_LEVEL_NUMBER), ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_CENTER, maxWidth, maxHeight, Edges()), fontFileName, TextBoxSizeInfo(16, 16), TextBoxColorInfo(hintBlue)), false, uLine1);

	BlockAlignElementsGrid* uLine2 = new BlockAlignElementsGrid(maxWidth, maxHeight, ETextBoxPositionAlign_CENTER, true, 4, !fill, !fill, Edges(6, 0, 1, 0), clear, "main game stats");
	mainGameMenuPage->addBox(new ImageBox(ImageBoxPreset(), ImageBoxPositionInfo(0, ETextBoxPositionAlign_CENTER, 20, 20, Edges(0, 0, 0, 0)), "UIElements/keyUI.bmp"), uLine2);
	mainGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EGameStatBoxValueToDisplay_CUR_KEYS), ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_CENTER, maxWidth, maxHeight, Edges(0, 0, 4, 0)), fontFileName, TextBoxSizeInfo(12, 12), TextBoxColorInfo(hintBlue)), false, uLine2);
	mainGameMenuPage->addBox(new ImageBox(ImageBoxPreset(), ImageBoxPositionInfo(0, ETextBoxPositionAlign_CENTER, 20, 20, Edges(3, 0, 0, 0)), "UIElements/targetUI.bmp"), uLine2);
	mainGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EGameStatBoxValueToDisplay_CUR_TARGETS), ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_CENTER, maxWidth, maxHeight, Edges(3, 0, 4, 0)), fontFileName, TextBoxSizeInfo(12, 12), TextBoxColorInfo(hintBlue)), false, uLine2);
	mainGameMenuPage->addBox(new ImageBox(ImageBoxPreset(), ImageBoxPositionInfo(0, ETextBoxPositionAlign_CENTER, 20, 20, Edges(3, 0, 0, 0)), "UIElements/collectibleUI.bmp"), uLine2);
	mainGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EGameStatBoxValueToDisplay_CUR_COLLECTIBLES), ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_CENTER, maxWidth, maxHeight, Edges(3, 0, 4, 0)), fontFileName, TextBoxSizeInfo(12, 12), TextBoxColorInfo(hintBlue)), false, uLine2);
	mainGameMenuPage->addBox(new ImageBox(ImageBoxPreset(), ImageBoxPositionInfo(0, ETextBoxPositionAlign_CENTER, 20, 20, Edges(3, 0, 0, 0)), "UIElements/enemiesUI.bmp"), uLine2);
	mainGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EGameStatBoxValueToDisplay_CUR_ENEMIES_LEFT), ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_CENTER, maxWidth, maxHeight, Edges(3, 0, 4, 0)), fontFileName, TextBoxSizeInfo(12, 12), TextBoxColorInfo(hintBlue)), false, uLine2);

	upperHUDBlock->mpSubElems.push_back(uLine1);
	upperHUDBlock->mpSubElems.push_back(uLine2);
	mainGameMenuPage->mpBlocks.push_back(upperHUDBlock);

	//debug HUD
	maxWidth = 200;
	maxHeight = 100;
	BlockAlignElementsVertically* debugHUDBlock = new BlockAlignElementsVertically(Hitbox(CoordsX1Y1WidthHeight(levelChunkWidth, 0, maxWidth, maxHeight)), ETextBoxPositionAlign_LEFT, EDirection_LEFT, EDirection_DOWN, !fill, !fill, Edges());
	mainGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EGameStatBoxValueToDisplay_CUR_KEYBOARD), ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_CENTER, maxWidth, maxHeight, Edges(3, 0, 0, 0)), fontFileName, TextBoxSizeInfo(12, 12), TextBoxColorInfo(hintBlue)), false, debugHUDBlock);

	mainGameMenuPage->mpBlocks.push_back(debugHUDBlock);

	//LOWER LEFT HUD
	BlockAlignElementsVertically*	lowerLeftHUDBlock	= new BlockAlignElementsVertically(Hitbox(CoordsX1Y1WidthHeight(0, 900, maxWidth, 450)),	ETextBoxPositionAlign_LEFT,		EDirection_RIGHT, EDirection_UP, !fill, !fill, Edges(),				clear, "head block");
	BlockAlignElementsHorizontally* llLine1				= new BlockAlignElementsHorizontally(maxWidth, maxHeight,									ETextBoxPositionAlign_CENTER,	EDirection_RIGHT,				 !fill, !fill, Edges(0, 10, 0, 0),	clear, "slash block");
	mainGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EGameStatBoxValueToDisplay_SLASH_KEY), ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_CENTER, 100, 60, Edges(0, 0, 4, 0)), fontFileName, TextBoxSizeInfo(15, 15), TextBoxColorInfo(hintBlue)), false, llLine1);
	mainGameMenuPage->addBox(new ImageBox(DontAutoShowImageBoxPreset(EUIBoxType_SLASH_UI), ImageBoxPositionInfo(0, ETextBoxPositionAlign_CENTER, 40, 25, Edges(0, 0, 20, 0)), "UIElements/slashUI.bmp"), llLine1);

	BlockAlignElementsHorizontally* llLine2 = new BlockAlignElementsHorizontally(maxWidth, maxHeight, ETextBoxPositionAlign_LEFT, EDirection_RIGHT, !fill, !fill, Edges(0, 2, 0, 0), clear, "double jump block");
	mainGameMenuPage->addBox(new ImageBox(DontAutoShowImageBoxPreset(EUIBoxType_DOUBLE_JUMP_UI), ImageBoxPositionInfo(0, ETextBoxPositionAlign_CENTER, 40, 30, Edges(0, 0, 38, 0)), "UIElements/doubleJumpUI.bmp"), llLine2);

	BlockAlignElementsHorizontally* llLine3 = new BlockAlignElementsHorizontally(	maxWidth, maxHeight, ETextBoxPositionAlign_CENTER, EDirection_RIGHT,	!fill, !fill, Edges(0, 10, 0, 0), clear, "projectile block");
	BlockAlignElementsVertically* llLine4 = new BlockAlignElementsVertically(		maxWidth, maxHeight, ETextBoxPositionAlign_CENTER, EDirection_DOWN,		!fill, !fill, Edges(20, 0, 0, 0), clear, "projectile keys block");
	mainGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EGameStatBoxValueToDisplay_PROJECTILE_HORIZONTAL_KEY),	ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_CENTER, 100, 60, Edges(0,  0, 3, 0)), fontFileName, TextBoxSizeInfo(15, 15), TextBoxColorInfo(hintBlue)), false, llLine4);
	mainGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EGameStatBoxValueToDisplay_PROJECTILE_VERTICAL_KEY),		ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_CENTER, 100, 60, Edges(10, 0, 3, 0)), fontFileName, TextBoxSizeInfo(15, 15), TextBoxColorInfo(hintBlue)), false, llLine4);

	BlockAlignElementsHorizontally* llLine5 = new BlockAlignElementsHorizontally(maxWidth, maxHeight, ETextBoxPositionAlign_LEFT, EDirection_RIGHT, fill, fill, Edges(0, 0, 0, 0), clear, "projectile icons block");
	mainGameMenuPage->addBox(new ImageBox(DontAutoShowImageBoxPreset(EUIBoxType_PROJECTILE_UI), ImageBoxPositionInfo(30, ETextBoxPositionAlign_CENTER, 80, 90, Edges(0, 0, 30, 0)), "UIElements/swordUI.bmp"), llLine5);
	mainGameMenuPage->addBox(new ImageBox(DontAutoShowImageBoxPreset(EUIBoxType_PROJECTILE_UI), ImageBoxPositionInfo(30, ETextBoxPositionAlign_CENTER, 80, 90, Edges(0, 0, 22, 0)), "UIElements/swordUI.bmp"), llLine5);
	mainGameMenuPage->addBox(new ImageBox(DontAutoShowImageBoxPreset(EUIBoxType_PROJECTILE_UI), ImageBoxPositionInfo(30, ETextBoxPositionAlign_CENTER, 80, 90, Edges(0, 0, 22, 0)), "UIElements/swordUI.bmp"), llLine5);
	mainGameMenuPage->addBox(new ImageBox(DontAutoShowImageBoxPreset(EUIBoxType_PROJECTILE_UI), ImageBoxPositionInfo(30, ETextBoxPositionAlign_CENTER, 80, 90, Edges(0, 0, 22, 0)), "UIElements/swordUI.bmp"), llLine5);
	mainGameMenuPage->addBox(new ImageBox(DontAutoShowImageBoxPreset(EUIBoxType_PROJECTILE_UI), ImageBoxPositionInfo(30, ETextBoxPositionAlign_CENTER, 80, 90, Edges(0, 0, 22, 0)), "UIElements/swordUI.bmp"), llLine5);
	mainGameMenuPage->addBox(new ImageBox(DontAutoShowImageBoxPreset(EUIBoxType_PROJECTILE_UI), ImageBoxPositionInfo(30, ETextBoxPositionAlign_CENTER, 80, 90, Edges(0, 0, 22, 0)), "UIElements/swordUI.bmp"), llLine5);
	mainGameMenuPage->addBox(new ImageBox(DontAutoShowImageBoxPreset(EUIBoxType_PROJECTILE_UI), ImageBoxPositionInfo(30, ETextBoxPositionAlign_CENTER, 80, 90, Edges(0, 0, 22, 0)), "UIElements/swordUI.bmp"), llLine5);

	llLine3->mpSubElems.push_back(llLine4);
	llLine3->mpSubElems.push_back(llLine5);

	lowerLeftHUDBlock->mpSubElems.push_back(llLine1);
	lowerLeftHUDBlock->mpSubElems.push_back(llLine2);
	lowerLeftHUDBlock->mpSubElems.push_back(llLine3);

	mainGameMenuPage->mpBlocks.push_back(lowerLeftHUDBlock);

	//LOWER RIGHT HUD
	BlockAlignElementsHorizontally* lowerRightHUDBlock = new BlockAlignElementsHorizontally(Hitbox(CoordsX1Y1WidthHeight(1600, 900, maxWidth, 450)), ETextBoxPositionAlign_LEFT, EDirection_LEFT, EDirection_UP, !fill, !fill, Edges(0, 10, 0, 0), clear, "head block");

	//Movement
	BlockAlignElementsVertically*	lrLine1 = new BlockAlignElementsVertically(  maxWidth, maxHeight, ETextBoxPositionAlign_CENTER, EDirection_DOWN,  !fill, !fill, Edges(0, 0, 0, 10), clear, "Movement Controls Block");
	BlockAlignElementsHorizontally* lrLine2 = new BlockAlignElementsHorizontally(maxWidth, maxHeight, ETextBoxPositionAlign_CENTER, EDirection_RIGHT, !fill, !fill, Edges(0, 0, 32, 0), clear, "Up Movement Controls Block");
	mainGameMenuPage->addBox(new ImageBox(ImageBoxPreset(), ImageBoxPositionInfo(0, ETextBoxPositionAlign_CENTER, 20, 20, Edges(0, 0, 0, 0)), "UIElements/upUI.bmp"), lrLine2);

	BlockAlignElementsHorizontally* lrLine3 = new BlockAlignElementsHorizontally(maxWidth, maxHeight, ETextBoxPositionAlign_CENTER, EDirection_RIGHT, !fill, !fill, Edges(2, 0, 15, 0), clear, "Left Right Movement Controls");
	mainGameMenuPage->addBox(new ImageBox(ImageBoxPreset(), ImageBoxPositionInfo(0, ETextBoxPositionAlign_CENTER, 20, 20, Edges(0, 0, 0,  0)), "UIElements/leftUI.bmp"), lrLine3);
	mainGameMenuPage->addBox(new ImageBox(ImageBoxPreset(), ImageBoxPositionInfo(0, ETextBoxPositionAlign_CENTER, 20, 20, Edges(0, 0, 22, 0)), "UIElements/rightUI.bmp"), lrLine3);

	BlockAlignElementsVertically* lrLine4 = new BlockAlignElementsVertically(maxWidth, maxHeight, ETextBoxPositionAlign_CENTER, EDirection_DOWN, !fill, !fill, Edges(3, 0, 0, 0), clear, "Movement Keys Block");
	mainGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EGameStatBoxValueToDisplay_MOVEMENT_LEFT_KEY),	ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, 300, 60, Edges(0, 0, 0, 0)), fontFileName, TextBoxSizeInfo(12, 12), TextBoxColorInfo(hintBlue)), false, lrLine4);
	mainGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EGameStatBoxValueToDisplay_MOVEMENT_UP_KEY),		ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, 300, 60, Edges(0, 0, 0, 0)), fontFileName, TextBoxSizeInfo(12, 12), TextBoxColorInfo(hintBlue)), false, lrLine4);
	mainGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EGameStatBoxValueToDisplay_MOVEMENT_RIGHT_KEY),	ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, 300, 60, Edges(0, 0, 0, 0)), fontFileName, TextBoxSizeInfo(12, 12), TextBoxColorInfo(hintBlue)), false, lrLine4);

	lrLine1->mpSubElems.push_back(lrLine2);
	lrLine1->mpSubElems.push_back(lrLine3);
	lrLine1->mpSubElems.push_back(lrLine4);

	//resetLevel
	BlockAlignElementsVertically* lrLine5 = new BlockAlignElementsVertically(maxWidth, maxHeight, ETextBoxPositionAlign_CENTER, EDirection_DOWN, !fill, !fill, Edges(20, 0, 0, 10), clear, "Reset Level Block");
	mainGameMenuPage->addBox(new ImageBox(ImageBoxPreset(), ImageBoxPositionInfo(0, ETextBoxPositionAlign_CENTER, 20, 20, Edges(2, 0, 0, 0)), "UIElements/resetLevelUI.bmp"), lrLine5);
	mainGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EGameStatBoxValueToDisplay_LEVEL_RESET_KEY), ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, 100, 60, Edges(3, 0, 0, 0)), fontFileName, TextBoxSizeInfo(12, 12), TextBoxColorInfo(hintBlue)), false, lrLine5);

	//resetCheckpoint
	BlockAlignElementsVertically* lrLine6 = new BlockAlignElementsVertically(maxWidth, maxHeight, ETextBoxPositionAlign_CENTER, EDirection_DOWN, !fill, !fill, Edges(20, 0, 0, 10), clear, "Reset Checkpoint Block");
	mainGameMenuPage->addBox(new ImageBox(ImageBoxPreset(), ImageBoxPositionInfo(0, ETextBoxPositionAlign_CENTER, 20, 20, Edges(2, 0, 0, 0)), "UIElements/resetCheckpointUI.bmp"), lrLine6);
	mainGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EGameStatBoxValueToDisplay_CHECKPOINT_RESET_KEY), ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, 100, 60, Edges(3, 0, 0, 0)), fontFileName, TextBoxSizeInfo(12, 12), TextBoxColorInfo(hintBlue)), false, lrLine6);


	lowerRightHUDBlock->mpSubElems.push_back(lrLine1);
	lowerRightHUDBlock->mpSubElems.push_back(lrLine5);
	lowerRightHUDBlock->mpSubElems.push_back(lrLine6);
	mainGameMenuPage->mpBlocks.push_back(lowerRightHUDBlock);

	menuManager.mpMenuPages.push_back(mainGameMenuPage);
}

void createMiniGameMenu(MenuManager& menuManager, const ScreenObject& screen,  MiniGameWorldData& worldData)
{
	const char* font = StyleManager::fontName;

	MenuPage* miniGameMenuPage = new MenuPage();
	menuManager.mpMenuPages.push_back(miniGameMenuPage);

	characterStatsBlockNum = (int)miniGameMenuPage->mpBlocks.size();
	createMiniGameCharacterStatsMenu(menuManager, screen, worldData);

	const TextBoxColorInfo black			= TextBoxColorInfo(StyleManager::black);
	const TextBoxColorInfo darkPink			= TextBoxColorInfo(StyleManager::pink);
	const TextBoxColorInfo colors			= TextBoxColorInfo(StyleManager::pink,	StyleManager::white,		StyleManager::sunYellow,	StyleManager::sunYellow);
	const TextBoxColorInfo optionBox		= TextBoxColorInfo(StyleManager::white,	StyleManager::sunYellow,	StyleManager::pink,			StyleManager::pink,		StyleManager::white, StyleManager::sunYellow);
	const SDL_Color& white					= StyleManager::white;
	const SDL_Color& clear					= StyleManager::clear;
	const SDL_Color& highlightPanelColor	= StyleManager::sunYellow;
	const SDL_Color& standardPanelColor		= StyleManager::lightPink;

	const int& headingText	= StyleManager::headingText;
	const int& optionText	= StyleManager::optionText;
	const int& detailText	= StyleManager::detailText;
	const int& bullet		= StyleManager::bullet;

	const bool fill = true;
	
	// keyboard debug HUD
	BlockAlignElementsVertically* debugHUDBlock = new BlockAlignElementsVertically(Hitbox(CoordsX1Y1WidthHeight(1200, 0, 200, 50)), ETextBoxPositionAlign_LEFT, EDirection_RIGHT, EDirection_DOWN, !fill, !fill, Edges(), clear, "keyboard block");
	miniGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EGameStatBoxValueToDisplay_CUR_KEYBOARD), ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_CENTER, 200, 50, Edges(3, 0, 0, 0)), font, TextBoxSizeInfo(12), white), false, debugHUDBlock);
	miniGameMenuPage->mpBlocks.push_back(debugHUDBlock);

	const int& panelWidth			= StyleManager::panelWidth;
	const int& panelHeight			= StyleManager::panelHeight;
	const int& panelHeadingHeight	= StyleManager::panelHeadingHeight;
	const int& panelBodyHeight		= StyleManager::panelBodyHeight;


	std::vector <EMiniGameState> whenToShow;
	BlockAlignElementsVertically* characterOptionsBlock			= new BlockAlignElementsVertically(Hitbox(CoordsX1Y1WidthHeight(700, 375, panelWidth, panelHeight)),	ETextBoxPositionAlign_LEFT,		EDirection_RIGHT, EDirection_DOWN,	fill, fill,	Edges(),	standardPanelColor,  "character panel box");
	BlockAlignElementsVertically* characterOptionsHeadingBlock	= new BlockAlignElementsVertically(panelWidth, panelHeadingHeight,										ETextBoxPositionAlign_CENTER,	EDirection_DOWN,					fill, fill,	Edges(),	highlightPanelColor, "character panel heading box");
	BlockAlignElementsVertically* characterOptionsDetailsBlock	= new BlockAlignElementsVertically(panelWidth, panelBodyHeight,											ETextBoxPositionAlign_LEFT,		EDirection_DOWN,					fill, fill,	Edges(),	standardPanelColor,  "character panel details box");
	characterOptionsBlock->mpSubElems.push_back(characterOptionsHeadingBlock);
	characterOptionsBlock->mpSubElems.push_back(characterOptionsDetailsBlock);
	miniGameMenuPage->mpBlocks.push_back(characterOptionsBlock);
	characterOptionsBlockNum = (int) miniGameMenuPage->mpBlocks.size() - 1;
	characterOptionsDetailsBlockNum = (int) characterOptionsBlock->mpSubElems.size() - 1;

	whenToShow = { EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT, EMiniGameState_PLAYER_WAIT_FOR_ACTION_INPUT, EMiniGameState_PLAYER_WAIT_FOR_ATTACK_OPTION_INPUT, EMiniGameState_PLAYER_WAIT_FOR_ATTACK_DIRECTION_INPUT, EMiniGameState_PLAYER_WAIT_FOR_ATTACK_TILE_INPUT, EMiniGameState_PLAYER_WAIT_FOR_ATTACK_CHARACTER_INPUT };
	miniGameMenuPage->addBox(new TextBox(MiniGamePlayerBoxPreset(ECharacterStatBoxValueToDisplay_CHARACTER_NAME, whenToShow), ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, panelWidth, panelHeadingHeight / 2, Edges(18, 8, 0, 0)), font, TextBoxSizeInfo(20), black), false, characterOptionsHeadingBlock);

	

	// SELECT TILE TO MOVE TO
	whenToShow = { EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT };

	miniGameMenuPage->addBox(new TextBox(MiniGameBoxPreset("Select a Tile to Move To", whenToShow), ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, panelWidth, panelHeadingHeight / 2, Edges(0, 10, 0, 0)), font, TextBoxSizeInfo(headingText), darkPink),	false, characterOptionsHeadingBlock);
	
	BlockAlignElementsHorizontally* moveRowBlock = new BlockAlignElementsHorizontally(panelWidth, panelBodyHeight, ETextBoxPositionAlign_CENTER, EDirection_RIGHT, !fill, !fill, Edges(10, 0, 0, 0), standardPanelColor, "movement block");
	miniGameMenuPage->addBox(new ShapeBox(MiniGameCharacterShapeBoxPreset(EShapeBoxClass_CIRCLE,						whenToShow),								TextBoxPositionInfo(ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, bullet,		bullet,					Edges(0, 0,  10, 5)),										white),							moveRowBlock);
	miniGameMenuPage->addBox(new TextBox(MiniGameBoxPreset("Move Style:",												whenToShow), ETextBoxFunction_NO_FUNCTION,	TextBoxPositionInfo(ETextBoxPositionAlign_LEFT,	  ETextBoxTextAlign_CENTER, panelWidth,	panelBodyHeight,		Edges(0, 0,  0,  0)), font, TextBoxSizeInfo(optionText),	white),					false,	moveRowBlock);
	miniGameMenuPage->addBox(new TextBox(MiniGamePlayerBoxPreset(ECharacterStatBoxValueToDisplay_CHARACTER_MOVE_TYPE,	whenToShow), ETextBoxFunction_NO_FUNCTION,	TextBoxPositionInfo(ETextBoxPositionAlign_LEFT,	  ETextBoxTextAlign_CENTER, panelWidth,	panelBodyHeight,		Edges(0, 0,  0,  5)), font, TextBoxSizeInfo(optionText),	white),					false,	moveRowBlock);
	characterOptionsDetailsBlock->mpSubElems.push_back(moveRowBlock);

	// CHOOSE ACTION
	whenToShow = { EMiniGameState_PLAYER_WAIT_FOR_ACTION_INPUT };
	miniGameMenuPage->addBox(new TextBox(MiniGameBoxPreset("Select an Action",  whenToShow), ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_CENTER, panelWidth, panelHeadingHeight / 2, Edges(0, 5, 0, 0)), font, TextBoxSizeInfo(headingText, headingText),	darkPink),	false, characterOptionsHeadingBlock);
	BlockAlignElementsVertically* actionsBlock	= new BlockAlignElementsVertically(panelWidth, panelBodyHeight, ETextBoxPositionAlign_LEFT, EDirection_DOWN, !fill, !fill, Edges(10, 0, 0, 0),	standardPanelColor, "action options block");
	BlockAlignElementsHorizontally* attackOptionRowBlock = new BlockAlignElementsHorizontally(	panelWidth, panelBodyHeight, ETextBoxPositionAlign_CENTER, EDirection_RIGHT, !fill, !fill, Edges(0, 10, 0, 0),	clear, "attack option block");
	miniGameMenuPage->addBox(new ShapeBox(MiniGameCharacterShapeBoxPreset(EShapeBoxClass_CIRCLE,	whenToShow),														TextBoxPositionInfo(ETextBoxPositionAlign_CENTER,	ETextBoxTextAlign_CENTER,	bullet,		bullet,				Edges(0, 0,  10, 10)),																white),					attackOptionRowBlock);
	miniGameMenuPage->addBox(new TextBox(MiniGameBoxPreset("ATTACK",								whenToShow), ETextBoxFunction_ATTACK_CUR_COMBAT_CHARACTER_BOX,		TextBoxPositionInfo(ETextBoxPositionAlign_LEFT,		ETextBoxTextAlign_CENTER,	panelWidth,	panelBodyHeight,	Edges()),				font, TextBoxSizeInfo(optionText,	optionText + 5, 2),		optionBox),	true,		attackOptionRowBlock);
	BlockAlignElementsHorizontally* supportOptionRowBlock = new BlockAlignElementsHorizontally(	panelWidth, panelBodyHeight, ETextBoxPositionAlign_CENTER, EDirection_RIGHT, !fill, !fill, Edges(0, 10, 0, 0),	clear, "support option block");
	miniGameMenuPage->addBox(new ShapeBox(MiniGameCharacterShapeBoxPreset(EShapeBoxClass_CIRCLE,	whenToShow),														TextBoxPositionInfo(ETextBoxPositionAlign_CENTER,	ETextBoxTextAlign_CENTER,	bullet,		bullet,				Edges(0, 0,  10, 10)),																white),					supportOptionRowBlock);
	miniGameMenuPage->addBox(new TextBox(MiniGameBoxPreset("SUPPORT",								whenToShow), ETextBoxFunction_SUPPORT_CUR_COMBAT_CHARACTER_BOX,		TextBoxPositionInfo(ETextBoxPositionAlign_LEFT,		ETextBoxTextAlign_CENTER,	panelWidth,	panelBodyHeight,	Edges()),				font, TextBoxSizeInfo(optionText,	optionText + 5, 2),		optionBox),	true,		supportOptionRowBlock);
	BlockAlignElementsHorizontally* defendOptionRowBlock = new BlockAlignElementsHorizontally(	panelWidth, panelBodyHeight, ETextBoxPositionAlign_CENTER, EDirection_RIGHT, !fill, !fill, Edges(0, 10, 0, 0),	clear, "defend option block");
	miniGameMenuPage->addBox(new ShapeBox(MiniGameCharacterShapeBoxPreset(EShapeBoxClass_CIRCLE,	whenToShow),														TextBoxPositionInfo(ETextBoxPositionAlign_CENTER,	ETextBoxTextAlign_CENTER,	bullet,		bullet,				Edges(0, 0,  10, 10)),																white),					defendOptionRowBlock);
	miniGameMenuPage->addBox(new TextBox(MiniGameBoxPreset("DEFEND",								whenToShow), ETextBoxFunction_DEFEND_CUR_COMBAT_CHARACTER_BOX,		TextBoxPositionInfo(ETextBoxPositionAlign_LEFT,		ETextBoxTextAlign_CENTER,	panelWidth,	panelBodyHeight,	Edges()),				font, TextBoxSizeInfo(optionText,	optionText + 5, 2),		optionBox),	true,		defendOptionRowBlock);
	BlockAlignElementsHorizontally* healOptionRowBlock = new BlockAlignElementsHorizontally(	panelWidth, panelBodyHeight, ETextBoxPositionAlign_CENTER, EDirection_RIGHT, !fill, !fill, Edges(0, 10, 0, 0),	clear,  "heal option block");
	miniGameMenuPage->addBox(new ShapeBox(MiniGameCharacterShapeBoxPreset(EShapeBoxClass_CIRCLE,	whenToShow),														TextBoxPositionInfo(ETextBoxPositionAlign_CENTER,	ETextBoxTextAlign_CENTER,	bullet,		bullet,				Edges(0, 0,  10, 10)),																white),					healOptionRowBlock);
	miniGameMenuPage->addBox(new TextBox(MiniGameBoxPreset("HEAL",									whenToShow), ETextBoxFunction_HEAL_CUR_COMBAT_CHARACTER_BOX,		TextBoxPositionInfo(ETextBoxPositionAlign_LEFT,		ETextBoxTextAlign_CENTER,	panelWidth,	panelBodyHeight,	Edges()),				font, TextBoxSizeInfo(optionText,	optionText + 5, 2),		optionBox),	true,		healOptionRowBlock);
	BlockAlignElementsHorizontally* passOptionRowBlock   = new BlockAlignElementsHorizontally(	panelWidth, panelBodyHeight, ETextBoxPositionAlign_CENTER, EDirection_RIGHT, !fill, !fill, Edges(),				clear,  "pass turn option block");
	miniGameMenuPage->addBox(new ShapeBox(MiniGameCharacterShapeBoxPreset(EShapeBoxClass_CIRCLE,	whenToShow),														TextBoxPositionInfo(ETextBoxPositionAlign_CENTER,	ETextBoxTextAlign_CENTER,	bullet,		bullet,				Edges(0, 0,  10, 10)),																white),					passOptionRowBlock);
	miniGameMenuPage->addBox(new TextBox(MiniGameBoxPreset("PASS TURN",								whenToShow), ETextBoxFunction_PASS_CUR_COMBAT_CHARACTER_TURN_BOX,	TextBoxPositionInfo(ETextBoxPositionAlign_LEFT,		ETextBoxTextAlign_CENTER,	panelWidth,	panelBodyHeight,	Edges()),				font, TextBoxSizeInfo(optionText,	optionText + 5, 2),		optionBox),	true,		passOptionRowBlock);
	actionsBlock->mpSubElems.push_back(attackOptionRowBlock);
	actionsBlock->mpSubElems.push_back(supportOptionRowBlock);
	actionsBlock->mpSubElems.push_back(defendOptionRowBlock);
	actionsBlock->mpSubElems.push_back(healOptionRowBlock);
	actionsBlock->mpSubElems.push_back(passOptionRowBlock);
	characterOptionsDetailsBlock->mpSubElems.push_back(actionsBlock);

	// CHOOSE ATTACK
	whenToShow = { EMiniGameState_PLAYER_WAIT_FOR_ATTACK_OPTION_INPUT };
	miniGameMenuPage->addBox(new TextBox(MiniGameBoxPreset("Choose an Attack", whenToShow), ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_CENTER, panelWidth, panelHeadingHeight / 2, Edges(0, 5, 0, 0)), font, TextBoxSizeInfo(headingText, headingText), darkPink), false, characterOptionsHeadingBlock);
	attackPanelBlockNum = (int)characterOptionsDetailsBlock->mpSubElems.size();
	createMiniGameCharacterAttackPanel(menuManager, screen, worldData); 

	// CHOOSE ATTACK DIRECTION
	whenToShow = { EMiniGameState_PLAYER_WAIT_FOR_ATTACK_DIRECTION_INPUT };
	miniGameMenuPage->addBox(new TextBox(MiniGameBoxPreset("Choose Attack Direction",						whenToShow), ETextBoxFunction_NO_FUNCTION,					TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_CENTER, panelWidth, panelHeadingHeight / 4,	Edges()),			font, TextBoxSizeInfo(headingText - 7),	darkPink),	false, characterOptionsHeadingBlock);
	miniGameMenuPage->addBox(new TextBox(MiniGameBoxPreset("All tiles in the direction will be attacked",	whenToShow), ETextBoxFunction_NO_FUNCTION,					TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_CENTER, panelWidth, panelHeadingHeight / 4,	Edges(0, 8, 0, 0)), font, TextBoxSizeInfo(detailText - 7),	black),		false, characterOptionsHeadingBlock);

	BlockAlignElementsVertically* attackDirectionBlock = new BlockAlignElementsVertically(	panelWidth, panelBodyHeight, ETextBoxPositionAlign_LEFT,	EDirection_DOWN,  !fill, !fill, Edges(), clear, "attack direction ");
	BlockAlignElementsVertically* attackNameBlock		= new BlockAlignElementsVertically(panelWidth, panelBodyHeight,  ETextBoxPositionAlign_LEFT,    EDirection_DOWN,  !fill, !fill, Edges(), clear, "attack name ");
	miniGameMenuPage->addBox(new TextBox(MiniGamePlayerBoxPreset(ECharacterStatBoxValueToDisplay_CHARACTER_CUR_ATTACK_NAME, whenToShow), ETextBoxFunction_NO_FUNCTION,				TextBoxPositionInfo(ETextBoxPositionAlign_LEFT,		ETextBoxTextAlign_CENTER, panelWidth,	panelBodyHeight / 5,	Edges(5, 5,  0,  0)), font, TextBoxSizeInfo(optionText),						white),		false,	attackNameBlock);
	BlockAlignElementsHorizontally* leftOptionRowBlock	= new BlockAlignElementsHorizontally(panelWidth, panelBodyHeight, ETextBoxPositionAlign_CENTER,	EDirection_RIGHT, !fill, !fill, Edges(), clear, "attack direction left");
	miniGameMenuPage->addBox(new ShapeBox(MiniGameCharacterShapeBoxPreset(EShapeBoxClass_CIRCLE,	whenToShow),																	TextBoxPositionInfo(ETextBoxPositionAlign_CENTER,	ETextBoxTextAlign_CENTER, bullet,		bullet,					Edges(0, 0,  10, 5)),															white),				leftOptionRowBlock);
	miniGameMenuPage->addBox(new TextBox(MiniGameDirectionBoxPreset("LEFT",							whenToShow, EDirection_LEFT),	ETextBoxFunction_ATTACK_DIRECTION_BOX,			TextBoxPositionInfo(ETextBoxPositionAlign_LEFT,		ETextBoxTextAlign_CENTER, panelWidth,	panelBodyHeight / 5,	Edges(0, 10, 0,  0)), font, TextBoxSizeInfo(optionText,	optionText + 5, 2),		optionBox), true,	leftOptionRowBlock);
	BlockAlignElementsHorizontally* rightOptionRowBlock = new BlockAlignElementsHorizontally(panelWidth, panelBodyHeight, ETextBoxPositionAlign_CENTER, EDirection_RIGHT, !fill, !fill, Edges(), clear, "attack direction right");
	miniGameMenuPage->addBox(new ShapeBox(MiniGameCharacterShapeBoxPreset(EShapeBoxClass_CIRCLE,	whenToShow),																	TextBoxPositionInfo(ETextBoxPositionAlign_CENTER,	ETextBoxTextAlign_CENTER, bullet,		bullet,					Edges(0, 0,  10, 5)),															white),				rightOptionRowBlock);
	miniGameMenuPage->addBox(new TextBox(MiniGameDirectionBoxPreset("RIGHT",						whenToShow, EDirection_RIGHT),	ETextBoxFunction_ATTACK_DIRECTION_BOX,			TextBoxPositionInfo(ETextBoxPositionAlign_LEFT,		ETextBoxTextAlign_CENTER, panelWidth,	panelBodyHeight / 5,	Edges(0, 10, 0,  0)), font, TextBoxSizeInfo(optionText,	optionText + 5, 2),		optionBox), true,	rightOptionRowBlock);
	BlockAlignElementsHorizontally* upOptionRowBlock	= new BlockAlignElementsHorizontally(panelWidth, panelBodyHeight, ETextBoxPositionAlign_CENTER, EDirection_RIGHT, !fill, !fill, Edges(), clear, "attack direction up");
	miniGameMenuPage->addBox(new ShapeBox(MiniGameCharacterShapeBoxPreset(EShapeBoxClass_CIRCLE,	whenToShow),																	TextBoxPositionInfo(ETextBoxPositionAlign_CENTER,	ETextBoxTextAlign_CENTER, bullet,		bullet,					Edges(0, 0,  10, 5)),															white),				upOptionRowBlock);
	miniGameMenuPage->addBox(new TextBox(MiniGameDirectionBoxPreset("UP",							whenToShow, EDirection_UP),		ETextBoxFunction_ATTACK_DIRECTION_BOX,			TextBoxPositionInfo(ETextBoxPositionAlign_LEFT,		ETextBoxTextAlign_CENTER, panelWidth,	panelBodyHeight / 5,	Edges(0, 10, 0,  0)), font, TextBoxSizeInfo(optionText,	optionText + 5, 2),		optionBox), true,	upOptionRowBlock);
	BlockAlignElementsHorizontally* downOptionRowBlock	= new BlockAlignElementsHorizontally(panelWidth, panelBodyHeight, ETextBoxPositionAlign_CENTER, EDirection_RIGHT, !fill, !fill, Edges(), clear, "attack direction down");
	miniGameMenuPage->addBox(new ShapeBox(MiniGameCharacterShapeBoxPreset(EShapeBoxClass_CIRCLE,	whenToShow),																	TextBoxPositionInfo(ETextBoxPositionAlign_CENTER,	ETextBoxTextAlign_CENTER, bullet,		bullet,					Edges(0, 0,  10, 5)),															white),				downOptionRowBlock);
	miniGameMenuPage->addBox(new TextBox(MiniGameDirectionBoxPreset("DOWN",							whenToShow, EDirection_DOWN),	ETextBoxFunction_ATTACK_DIRECTION_BOX,			TextBoxPositionInfo(ETextBoxPositionAlign_LEFT,		ETextBoxTextAlign_CENTER, panelWidth,	panelBodyHeight / 5,	Edges(0, 0,  0,  0)), font, TextBoxSizeInfo(optionText,	optionText + 5, 2),		optionBox), true,	downOptionRowBlock);
	attackDirectionBlock->mpSubElems.push_back(attackNameBlock);
	attackDirectionBlock->mpSubElems.push_back(leftOptionRowBlock);
	attackDirectionBlock->mpSubElems.push_back(rightOptionRowBlock);
	attackDirectionBlock->mpSubElems.push_back(upOptionRowBlock);
	attackDirectionBlock->mpSubElems.push_back(downOptionRowBlock);
	characterOptionsDetailsBlock->mpSubElems.push_back(attackDirectionBlock);

	// SELECT TILE TO ATTACK
	whenToShow = { EMiniGameState_PLAYER_WAIT_FOR_ATTACK_TILE_INPUT };
	miniGameMenuPage->addBox(new TextBox(MiniGameBoxPreset("Select a Tile to Attack",										whenToShow), ETextBoxFunction_NO_FUNCTION,	TextBoxPositionInfo(ETextBoxPositionAlign_LEFT,		ETextBoxTextAlign_CENTER, panelWidth, panelHeadingHeight / 2,	Edges(0, 5,  0,  0)), font, TextBoxSizeInfo(headingText),	darkPink),				false,	characterOptionsHeadingBlock);
	BlockAlignElementsHorizontally* attackTilesBlock = new BlockAlignElementsHorizontally(panelWidth, panelBodyHeight, ETextBoxPositionAlign_LEFT, EDirection_RIGHT, !fill, !fill, Edges(), standardPanelColor, "tile to attack description");
	miniGameMenuPage->addBox(new ShapeBox(MiniGameCharacterShapeBoxPreset(EShapeBoxClass_CIRCLE,							whenToShow),								TextBoxPositionInfo(ETextBoxPositionAlign_CENTER,	ETextBoxTextAlign_CENTER, bullet,	  bullet,					Edges(0, 0,  10, 5)),														white),			attackTilesBlock);
	miniGameMenuPage->addBox(new TextBox(MiniGamePlayerBoxPreset(ECharacterStatBoxValueToDisplay_CHARACTER_CUR_ATTACK_NAME, whenToShow), ETextBoxFunction_NO_FUNCTION,	TextBoxPositionInfo(ETextBoxPositionAlign_LEFT,		ETextBoxTextAlign_CENTER, panelWidth, panelBodyHeight,			Edges(0, 10, 0,  0)), font, TextBoxSizeInfo(optionText,		optionText),	white),	false,	attackTilesBlock);
	characterOptionsDetailsBlock->mpSubElems.push_back(attackTilesBlock);

	// SELECT CHARACTER TO ATTACK
	whenToShow = { EMiniGameState_PLAYER_WAIT_FOR_ATTACK_CHARACTER_INPUT };
	miniGameMenuPage->addBox(new TextBox(MiniGameBoxPreset("Choose a Character", whenToShow), ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_CENTER, panelWidth, panelHeadingHeight / 2, Edges(0, 5, 0, 0)), font, TextBoxSizeInfo(headingText, headingText), darkPink), false, characterOptionsHeadingBlock);
	characterSelectionPanelBlockNum = (int)characterOptionsDetailsBlock->mpSubElems.size();
	createMiniGameCharacterSelectionMenu(menuManager, screen, worldData);


	// LOG
	BlockAlignElementsVertically* debugStatLine = new BlockAlignElementsVertically(Hitbox(CoordsX1Y1WidthHeight(20, screen.mGameLevelChunkHeight - 15, screen.mGameLevelChunkWidth, 50)), ETextBoxPositionAlign_LEFT, EDirection_RIGHT, EDirection_UP, !fill, !fill, Edges(), highlightPanelColor, "log line");
	miniGameMenuPage->addBox(new TextBox(MiniGameStatBoxPreset(EGameStatBoxValueToDisplay_MINI_GAME_DEBUG_LINE), ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_CENTER, screen.mGameLevelChunkWidth - 15, 50, Edges()), font, TextBoxSizeInfo(detailText , detailText), darkPink), false, debugStatLine);
	miniGameMenuPage->mpBlocks.push_back(debugStatLine);

	// undo
	BlockAlignElementsVertically* undoBlock = new BlockAlignElementsVertically(Hitbox(CoordsX1Y1WidthHeight(1165, 417, 200, 50)), ETextBoxPositionAlign_CENTER, EDirection_RIGHT, EDirection_UP_AND_DOWN, !fill, !fill, Edges(), clear, "undo block");
	miniGameMenuPage->addBox(new TextBox(StandardTextBoxPreset("UNDO"), ETextBoxFunction_MINI_GAME_UNDO_BOX, TextBoxPositionInfo(ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, 200, 50, Edges()), font, TextBoxSizeInfo(optionText, optionText + 5, 2), optionBox), true, undoBlock);
	miniGameMenuPage->mpBlocks.push_back(undoBlock);


	miniGameMenuPage = nullptr;
	debugHUDBlock = nullptr;
	characterOptionsBlock = nullptr;
	characterOptionsHeadingBlock = nullptr;
	characterOptionsDetailsBlock = nullptr;
}

void createMiniGameCharacterStatsMenu(MenuManager& menuManager, const ScreenObject& screen, MiniGameWorldData& worldData)
{
	MenuPage* miniGameMenuPage = menuManager.mpMenuPages[EMenuPageType_MINI_GAME_MENU];

	const char* font = StyleManager::fontName;
	const TextBoxColorInfo colors = TextBoxColorInfo(StyleManager::pink);
	const SDL_Color& panelColor				= StyleManager::sunYellow;
	const SDL_Color& healthColor			= StyleManager::lightYellow;
	const SDL_Color& healthTextColor		= StyleManager::gold;
	const SDL_Color& healthBackgroundColor	= StyleManager::pink;


	const bool fill = true;

	int maxWidth = 1000;
	int maxHeight = 300;
	int curX = 20;
	int curY = 20;
	const int iconWidth  = 90;
	const int iconHeight = 90;
	const int spacing = 10;
	const int padding = 5;

	const int headingSize = 20;
	const int detailSize = 15;


	BlockAlignElementsVertically* statBlock = new BlockAlignElementsVertically(Hitbox(CoordsX1Y1WidthHeight(curX, curY, maxWidth, maxHeight)), ETextBoxPositionAlign_LEFT, EDirection_RIGHT, EDirection_DOWN, !fill, !fill, Edges(), panelColor);
	BlockAlignElementsHorizontally* levelLine			= new BlockAlignElementsHorizontally(maxWidth, maxHeight, ETextBoxPositionAlign_LEFT, EDirection_RIGHT, !fill, !fill, Edges(0, padding, 0, 0),	panelColor);
	miniGameMenuPage->addBox(new TextBox(MiniGameStatBoxPreset(EGameStatBoxValueToDisplay_CUR_LEVEL_NUMBER), ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_LEFT, maxWidth, maxHeight / 4, Edges(2, 0, 0, 0)), font, TextBoxSizeInfo(headingSize), colors), false, levelLine);
	BlockAlignElementsHorizontally* characterStatsLine	= new BlockAlignElementsHorizontally(maxWidth, maxHeight, ETextBoxPositionAlign_LEFT, EDirection_RIGHT, !fill, !fill, Edges(),					panelColor);
		
	if (worldData.getStage()->mGrid.isSetUp())
	{
		CombatManager& combatManager = worldData.getStage()->mCombatManager;
		for (int count = 0; count < combatManager.getAllCharacters().size(); count++)
		{
			CombatCharacter* pCurChar = combatManager.getFromAllCharacters(count);
			maxWidth = 200;
			int curPaddingLeft = (count == 0) ? padding : 0;

			BlockAlignElementsHorizontally* curCharBlock = new BlockAlignElementsHorizontally(	maxWidth, maxHeight, ETextBoxPositionAlign_LEFT, EDirection_RIGHT, !fill, !fill, Edges(0, 0, curPaddingLeft, padding + spacing));

			BlockAlignElementsVertically*	curIconBlock = new BlockAlignElementsVertically(	maxWidth, maxHeight, ETextBoxPositionAlign_CENTER, EDirection_DOWN, !fill, !fill, Edges());
			miniGameMenuPage->addBox(new ImageBox(MiniGameCharacterImageBoxPreset(count, true), ImageBoxPositionInfo(0, ETextBoxPositionAlign_CENTER, iconWidth, iconHeight, Edges()), pCurChar->mIconFileName), curIconBlock);

			BlockAlignElementsVertically* curInfoBlock = new BlockAlignElementsVertically(		maxWidth, maxHeight, ETextBoxPositionAlign_LEFT, EDirection_DOWN, !fill, !fill, Edges());

			BlockAlignElementsVertically* curNameBlock = new BlockAlignElementsVertically(		maxWidth, maxHeight, ETextBoxPositionAlign_LEFT, EDirection_DOWN, !fill, !fill, Edges());
			miniGameMenuPage->addBox(new TextBox(MiniGameCharacterBoxPreset(count, true, ECharacterStatBoxValueToDisplay_CHARACTER_NAME),	ETextBoxFunction_NO_FUNCTION,	TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_LEFT, maxWidth,		maxHeight / 4, Edges(padding,	8,			0, 0)), font, TextBoxSizeInfo(headingSize), colors), false, curNameBlock);

			BlockAlignElementsHorizontally* curTextBlock	= new BlockAlignElementsHorizontally(maxWidth, maxHeight, ETextBoxPositionAlign_LEFT, EDirection_RIGHT, !fill, !fill, Edges());
			BlockAlignElementsVertically*	curTitlesBlock	= new BlockAlignElementsVertically(	 maxWidth, maxHeight, ETextBoxPositionAlign_LEFT, EDirection_DOWN,  !fill, !fill, Edges(0, 0, 0, 5));
			miniGameMenuPage->addBox(new TextBox(MiniGameCharacterBoxPreset(count, true, "HP: "),											ETextBoxFunction_NO_FUNCTION,	TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_LEFT, maxWidth,		maxHeight / 4, Edges(0,			3,			0, 0)), font, TextBoxSizeInfo(detailSize),  colors), false, curTitlesBlock);
			miniGameMenuPage->addBox(new TextBox(MiniGameCharacterBoxPreset(count, true, "DEF: "),											ETextBoxFunction_NO_FUNCTION,	TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_LEFT, maxWidth,		maxHeight / 4, Edges(0,			3,			0, 0)), font, TextBoxSizeInfo(detailSize),  colors), false, curTitlesBlock);
			miniGameMenuPage->addBox(new TextBox(MiniGameCharacterBoxPreset(count, true, "STUNS: "),										ETextBoxFunction_NO_FUNCTION,	TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_LEFT, maxWidth,		maxHeight / 4, Edges(0,			padding,	0, 0)), font, TextBoxSizeInfo(detailSize),  colors), false, curTitlesBlock);

			BlockAlignElementsVertically*	curNumsBlock = new BlockAlignElementsVertically(	maxWidth, maxHeight, ETextBoxPositionAlign_LEFT, EDirection_DOWN,  !fill, !fill, Edges());
			miniGameMenuPage->addBox(new HealthBox(HealthBoxPreset(			count,		 ECharacterStatBoxValueToDisplay_CHARACTER_HEALTH),									TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_LEFT, maxWidth - 50,	maxHeight / 4, Edges(0,			3,			0, 0)), font, detailSize, healthColor, healthBackgroundColor, healthTextColor), curNumsBlock);
			miniGameMenuPage->addBox(new HealthBox(HealthBoxPreset(			count,		 ECharacterStatBoxValueToDisplay_CHARACTER_DEFENSE),								TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_LEFT, maxWidth - 50,	maxHeight / 4, Edges(0,			3,			0, 0)), font, detailSize, healthColor, healthBackgroundColor, healthTextColor), curNumsBlock);
			miniGameMenuPage->addBox(new TextBox(MiniGameCharacterBoxPreset(count, true, ECharacterStatBoxValueToDisplay_CHARACTER_STUN),	ETextBoxFunction_NO_FUNCTION,	TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_LEFT, maxWidth - 50,	maxHeight / 4, Edges(0,			padding,	0, 0)), font, TextBoxSizeInfo(detailSize), colors), false, curNumsBlock);
			curTextBlock->mpSubElems.push_back(curTitlesBlock);
			curTextBlock->mpSubElems.push_back(curNumsBlock);

			curInfoBlock->mpSubElems.push_back(curNameBlock);
			curInfoBlock->mpSubElems.push_back(curTextBlock);

			curCharBlock->mpSubElems.push_back(curIconBlock);
			curCharBlock->mpSubElems.push_back(curInfoBlock);
			characterStatsLine->mpSubElems.push_back(curCharBlock);
			curCharBlock = nullptr;
			curInfoBlock = nullptr;
			curNameBlock = nullptr;
			curTextBlock = nullptr;
			curTitlesBlock = nullptr;
			curNumsBlock = nullptr;
			pCurChar = nullptr;
		}
	}
	
	statBlock->mpSubElems.push_back(levelLine);
	statBlock->mpSubElems.push_back(characterStatsLine);
	levelLine = nullptr;
	characterStatsLine = nullptr;
	

	if (miniGameMenuPage->mpBlocks.size() > characterStatsBlockNum && miniGameMenuPage->mpBlocks[characterStatsBlockNum])
	{
		//replace the character stat block
		statBlock->setTexture(screen.mpRenderer);
		statBlock->setMaxSize();
		miniGameMenuPage->deleteBlock(miniGameMenuPage->mpBlocks[characterStatsBlockNum]);
		miniGameMenuPage->mpBlocks[characterStatsBlockNum] = statBlock;
	}
	else
	{
		//first time through, place the block
		miniGameMenuPage->mpBlocks.push_back(statBlock);
	}
	
	statBlock = nullptr;
}

void createMiniGameCharacterSelectionMenu(MenuManager& menuManager, const ScreenObject& screen, MiniGameWorldData& worldData)
{
	MenuPage* miniGameMenuPage = menuManager.mpMenuPages[EMenuPageType_MINI_GAME_MENU];
	const char* font = StyleManager::fontName;

	const TextBoxColorInfo optionBox	= TextBoxColorInfo(StyleManager::white, StyleManager::sunYellow, StyleManager::pink, StyleManager::pink, StyleManager::white, StyleManager::sunYellow);
	const SDL_Color& clear = StyleManager::clear;
	const SDL_Color& white = StyleManager::white;

	const int& panelWidth			= StyleManager::panelWidth;
	const int& panelHeight			= StyleManager::panelHeight;
	const int& panelBodyHeight		= StyleManager::panelBodyHeight;

	const int& optionText	= StyleManager::optionText;
	const int& bullet		= StyleManager::bullet;

	const bool fill = true;

	BlockAlignElementsVertically* characterOptionsBlock = (BlockAlignElementsVertically*)miniGameMenuPage->mpBlocks[characterOptionsBlockNum];
	BlockAlignElementsVertically* characterOptionsDetailsBlock = (BlockAlignElementsVertically*)characterOptionsBlock->mpSubElems[characterOptionsDetailsBlockNum];

	BlockAlignElementsVertically* pCharacterSelectionPanel = new BlockAlignElementsVertically(panelWidth, panelBodyHeight, ETextBoxPositionAlign_LEFT, EDirection_DOWN, !fill, !fill, Edges(12, 0, 0, 0), clear, "character selection panel block");
	std::vector <EMiniGameState> whenToShow = { EMiniGameState_PLAYER_WAIT_FOR_ATTACK_CHARACTER_INPUT };

	if (worldData.getStage()->mGrid.isSetUp())
	{
		CombatManager& combatManager = worldData.getStage()->mCombatManager;
		for (int countCharacter = 0; countCharacter < combatManager.getAllCharacters().size(); countCharacter++)
		{
			EMiniGameCombatCharacterType characterType = combatManager.getFromAllCharacters(countCharacter)->mType;
			BlockAlignElementsHorizontally* pCurCharacterBlock = new BlockAlignElementsHorizontally(	panelWidth, panelBodyHeight, ETextBoxPositionAlign_LEFT,	EDirection_RIGHT, !fill, !fill, Edges(),  clear, (combatManager.getFromAllCharacters(countCharacter)->mName + " selection block"));

			BlockAlignElementsVertically* pShapeBlock			= new BlockAlignElementsVertically(		panelWidth, panelBodyHeight, ETextBoxPositionAlign_CENTER,	EDirection_DOWN,  !fill, !fill, Edges(9, 0, 0, 0));
			miniGameMenuPage->addBox(new ShapeBox(MiniGameCharacterShapeBoxPreset(EShapeBoxClass_CIRCLE, countCharacter, characterType, true, whenToShow), TextBoxPositionInfo(ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, bullet, bullet, Edges(0, 0, 10, 10)), white), pShapeBlock);

			BlockAlignElementsVertically* pCharacterTextBlock	= new BlockAlignElementsVertically(		panelWidth, panelBodyHeight, ETextBoxPositionAlign_LEFT,	EDirection_DOWN,  !fill, !fill, Edges());
			miniGameMenuPage->addBox(new TextBox(MiniGameCharacterBoxPreset(countCharacter, characterType, true, ECharacterStatBoxValueToDisplay_CHARACTER_NAME, whenToShow), ETextBoxFunction_ATTACK_CHARACTER_BOX, TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_CENTER, panelWidth, panelBodyHeight / 4, Edges()), font, TextBoxSizeInfo(optionText, optionText + 5, 2), optionBox), true, pCharacterTextBlock);

			pCurCharacterBlock->mpSubElems.push_back(pShapeBlock);
			pCurCharacterBlock->mpSubElems.push_back(pCharacterTextBlock);
			pCharacterSelectionPanel->mpSubElems.push_back(pCurCharacterBlock);

			pShapeBlock = nullptr;
			pCharacterTextBlock = nullptr;
			pCurCharacterBlock = nullptr;
		}
	}

	if ((int)characterOptionsDetailsBlock->mpSubElems.size() > characterSelectionPanelBlockNum && characterOptionsDetailsBlock->mpSubElems[characterSelectionPanelBlockNum])
	{
		// replace the attack panel block
		pCharacterSelectionPanel->setTexture(screen.mpRenderer);
		pCharacterSelectionPanel->setMaxSize();
		miniGameMenuPage->deleteBlock((UIBlock*)characterOptionsDetailsBlock->mpSubElems[characterSelectionPanelBlockNum]);
		characterOptionsDetailsBlock->mpSubElems[characterSelectionPanelBlockNum] = pCharacterSelectionPanel;
	}
	else
	{
		// first time through, place the block
		characterOptionsDetailsBlock->mpSubElems.push_back(pCharacterSelectionPanel);
	}

	pCharacterSelectionPanel = nullptr;
	characterOptionsBlock = nullptr;
	characterOptionsDetailsBlock = nullptr;
}

void createMiniGameCharacterAttackPanel(MenuManager& menuManager, const ScreenObject& screen, MiniGameWorldData& worldData) 
{
	MenuPage* miniGameMenuPage = menuManager.mpMenuPages[EMenuPageType_MINI_GAME_MENU];
	const char* font = StyleManager::fontName;

	const TextBoxColorInfo	black		= TextBoxColorInfo(StyleManager::black);
	const TextBoxColorInfo	optionBox	= TextBoxColorInfo(StyleManager::white, StyleManager::sunYellow, StyleManager::pink, StyleManager::pink, StyleManager::white, StyleManager::sunYellow);
	const SDL_Color&		white		= StyleManager::white;
	const SDL_Color&		clear		= StyleManager::clear;

	const int& panelWidth			= StyleManager::panelWidth;
	const int& panelHeight			= StyleManager::panelHeight;
	const int& panelHeadingHeight	= StyleManager::panelHeadingHeight;
	const int& panelBodyHeight		= StyleManager::panelBodyHeight;

	const int& optionText			= StyleManager::optionText;
	const int& detailText			= StyleManager::detailText;
	const int& bullet				= StyleManager::bullet;

	const bool fill = true;

	BlockAlignElementsVertically* characterOptionsBlock = (BlockAlignElementsVertically*)miniGameMenuPage->mpBlocks[characterOptionsBlockNum];
	BlockAlignElementsVertically* characterOptionsDetailsBlock = (BlockAlignElementsVertically*)characterOptionsBlock->mpSubElems[characterOptionsDetailsBlockNum];

	BlockAlignElementsVertically* pAttackPanel = new BlockAlignElementsVertically(panelWidth, panelBodyHeight, ETextBoxPositionAlign_LEFT, EDirection_DOWN, !fill, !fill, Edges(), clear, "main attack panel block");
	std::vector <EMiniGameState> whenToShow = { EMiniGameState_PLAYER_WAIT_FOR_ATTACK_OPTION_INPUT };
	if (worldData.mCurLevelNumber != -1)
	{
		CombatManager& combatManager = worldData.getStage()->mCombatManager;
		for (int countCharacter = 0; countCharacter < (int)combatManager.getAllCharacters().size(); countCharacter++)
		{
			CombatCharacter& character = *combatManager.getFromAllCharacters(countCharacter);
			if (character.mType != EMiniGameCombatCharacterType_PLAYER)
			{
				continue;
			}

			//CHARACTER
			//CHOOSE ATTACK
			//OPTION NAME, DIAGRAM
			//OPTION TYPE
			//OPTION DAMAGE
			//OPTION SPECIAL EFFECTS / NOTES
			BlockAlignElementsGrid* pCharacterAttackBlock = new BlockAlignElementsGrid(panelWidth, panelBodyHeight, ETextBoxPositionAlign_LEFT, false, 3, !fill, !fill, Edges(10, 0, 0, 0), clear, (character.mName + " Attack panel block"));
			const std::vector<Attack>& attacks = character.mCombatMovementManager.getAttacks();
			for (int countAttack = 0; countAttack < (int)attacks.size(); countAttack++)
			{
				const Attack& curAttack = attacks[countAttack];
				EMiniGameCombatAttackCategoryType category = curAttack.mCategory;
				BlockAlignElementsVertically* pCharacterAttackShapeBlock = new BlockAlignElementsVertically(panelWidth, panelBodyHeight, ETextBoxPositionAlign_CENTER,  EDirection_DOWN, !fill, !fill, Edges(16, 0, 0, 0), clear, "bullet");
				miniGameMenuPage->addBox(new ShapeBox(MiniGamePlayerAttackShapeBoxPreset(EShapeBoxClass_CIRCLE,			  countCharacter, false,																						countAttack, category, whenToShow),										TextBoxPositionInfo(ETextBoxPositionAlign_CENTER,	ETextBoxTextAlign_CENTER, bullet,	bullet,					Edges(0,  0, 10, 10)),															white),				pCharacterAttackShapeBlock);
				
				BlockAlignElementsVertically* pCharacterAttackTextBlock	 = new BlockAlignElementsVertically(panelWidth, panelBodyHeight, ETextBoxPositionAlign_LEFT,	EDirection_DOWN, !fill, !fill, Edges(3, 10, 0, 0), clear, "attack details");
				miniGameMenuPage->addBox(										new TextBox(MiniGamePlayerAttackBoxPreset(countCharacter, false,	ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_NAME,						countAttack, category, whenToShow), ETextBoxFunction_ATTACK_STYLE_BOX,	TextBoxPositionInfo(ETextBoxPositionAlign_LEFT,		ETextBoxTextAlign_LEFT,  panelWidth, panelBodyHeight / 4,	Edges(0,  3, 0,  0)), font, TextBoxSizeInfo(optionText,  optionText + 5, 2),	optionBox), true,	pCharacterAttackTextBlock);
				miniGameMenuPage->addBox(										new TextBox(MiniGamePlayerAttackBoxPreset(countCharacter, false,	ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_TYPE,						countAttack, category, whenToShow), ETextBoxFunction_NO_FUNCTION,		TextBoxPositionInfo(ETextBoxPositionAlign_LEFT,		ETextBoxTextAlign_LEFT,  panelWidth, panelBodyHeight / 4,	Edges(0,  3, 0,  0)), font, TextBoxSizeInfo(detailText,  detailText),			black),		false,	pCharacterAttackTextBlock);
				if (curAttack.mDamagePercent != 0) { miniGameMenuPage->addBox(	new TextBox(MiniGamePlayerAttackBoxPreset(countCharacter, false,	ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_DAMAGE,						countAttack, category, whenToShow), ETextBoxFunction_NO_FUNCTION,		TextBoxPositionInfo(ETextBoxPositionAlign_LEFT,		ETextBoxTextAlign_LEFT,  panelWidth, panelBodyHeight / 4,	Edges(0,  3, 0,  0)), font, TextBoxSizeInfo(detailText,  detailText),			black),		false,	pCharacterAttackTextBlock); }
				miniGameMenuPage->addBox(										new TextBox(MiniGamePlayerAttackBoxPreset(countCharacter, false,	ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_SPECIAL_EFFECTS_AND_NOTES,	countAttack, category, whenToShow), ETextBoxFunction_NO_FUNCTION,		TextBoxPositionInfo(ETextBoxPositionAlign_LEFT,		ETextBoxTextAlign_LEFT,  panelWidth, panelBodyHeight/ 4,	Edges(0,  0, 0,  0)), font, TextBoxSizeInfo(detailText,  detailText),			black),		false,	pCharacterAttackTextBlock);
				
				pCharacterAttackBlock->mpSubElems.push_back(pCharacterAttackShapeBlock);
				pCharacterAttackBlock->mpSubElems.push_back(pCharacterAttackTextBlock);

				createBlockDiagram(curAttack, countCharacter, countAttack, miniGameMenuPage, pCharacterAttackBlock, whenToShow, menuManager);

				pCharacterAttackTextBlock = nullptr;
				pCharacterAttackShapeBlock = nullptr;
			}
			pAttackPanel->mpSubElems.push_back(pCharacterAttackBlock);
			pCharacterAttackBlock = nullptr;
		}
	}

	if ((int)characterOptionsDetailsBlock->mpSubElems.size() > attackPanelBlockNum && characterOptionsDetailsBlock->mpSubElems[attackPanelBlockNum])
	{
		//replace the attack panel block
		pAttackPanel->setTexture(screen.mpRenderer);
		pAttackPanel->setMaxSize();
		miniGameMenuPage->deleteBlock((UIBlock*)characterOptionsDetailsBlock->mpSubElems[attackPanelBlockNum]);
		characterOptionsDetailsBlock->mpSubElems[attackPanelBlockNum] = pAttackPanel;
	}
	else
	{
		//first time through, place the block
		characterOptionsDetailsBlock->mpSubElems.push_back(pAttackPanel);
	}

	pAttackPanel = nullptr;
	characterOptionsBlock = nullptr;
	characterOptionsDetailsBlock = nullptr;
}

void createBlockDiagram(const Attack& attack, const int characterIndex, const int attackNum, MenuPage* pPage, UIBlock* pBlock, const std::vector<EMiniGameState>& whenToShow, MenuManager& menuManager)
{
	int blockSize = 15;
	int spacing = 5;
	SDL_Color blockColor = StyleManager::white;
	SDL_Color clear = StyleManager::clear;
	bool fill = true;

	int out = attack.mOut;
	int num = attack.mNum;

	int leftBlockMargin = 20;

	
	if (attack.mType == EMiniGameCombatMoveAttackTypes_WHOLE_GRID)
	{
		num = 4;
		out = 0;
		blockSize /= 3;
		spacing /= 2;
		leftBlockMargin = 5;
	}

	int maxWidth = (2 * num + 1) * (blockSize + spacing);
	int maxHeight = maxWidth;
	
	int maxBlockHeight = pBlock->mMaxHeight;
	int outSpacing = out * (blockSize + spacing) + spacing;

	const EMiniGameCombatAttackCategoryType& category = attack.mCategory;

	BlockAlignElementsVertically* pDiagramBlock = new BlockAlignElementsVertically(200, maxBlockHeight, ETextBoxPositionAlign_LEFT, EDirection_DOWN, !fill, !fill, Edges(0, 0, leftBlockMargin, 0), clear, "block diagram");
	BlockAlignElementsHorizontally* pCurRow = nullptr;
	switch(attack.mType)
	{ 
	case EMiniGameCombatMoveAttackTypes_SQUARE:
	case EMiniGameCombatMoveAttackTypes_WHOLE_GRID:
		for (int i = 0; i < (2 * (num - out) + 1); i++)
		{
			pCurRow = new BlockAlignElementsHorizontally(maxWidth, maxHeight, ETextBoxPositionAlign_CENTER, EDirection_RIGHT, !fill, !fill, Edges(), clear, ("block diagram row " + std::to_string(i)));
			for (int j = 0; j < (2 * (num - out) + 1); j++)
			{
				if (i == num && j == num)
				{
					// middle dot
					pPage->addBox(new ShapeBox(MiniGamePlayerAttackShapeBoxPreset(EShapeBoxClass_CIRCLE, characterIndex, false, attackNum, category, whenToShow), TextBoxPositionInfo(ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, blockSize, blockSize, Edges(outSpacing,	outSpacing, outSpacing, outSpacing)),	blockColor), pCurRow);
				}
				else 
				{
					// block 
					pPage->addBox(new ShapeBox(MiniGamePlayerAttackShapeBoxPreset(EShapeBoxClass_RECT,	 characterIndex, false, attackNum, category, whenToShow), TextBoxPositionInfo(ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, blockSize, blockSize, Edges(spacing,		spacing,	spacing,	spacing)),		blockColor), pCurRow);
				}
			}
			pDiagramBlock->mpSubElems.push_back(pCurRow);
		}
		break;
	case EMiniGameCombatMoveAttackTypes_CROSS:
		for (int i = 0; i < (2 * (num - out) + 1); i++)
		{
			pCurRow = new BlockAlignElementsHorizontally(maxWidth, maxHeight, ETextBoxPositionAlign_CENTER, EDirection_RIGHT, !fill, !fill, Edges(), clear, ("block diagram row " + std::to_string(i)));
			if (i == num)
			{
				for (int j = 0; j < (2 * (num - out) + 1); j++)
				{
					if (j == num) 
					{
						// middle dot
						pPage->addBox(new ShapeBox(MiniGamePlayerAttackShapeBoxPreset(EShapeBoxClass_CIRCLE,	characterIndex, false, attackNum, category, whenToShow), TextBoxPositionInfo(ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, blockSize, blockSize, Edges(outSpacing,	outSpacing, outSpacing,			outSpacing)),		blockColor), pCurRow);
					}
					else 
					{
						// left / right blocks
						pPage->addBox(new ShapeBox(MiniGamePlayerAttackShapeBoxPreset(EShapeBoxClass_RECT,		characterIndex, false, attackNum, category, whenToShow), TextBoxPositionInfo(ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, blockSize, blockSize, Edges(spacing,		spacing,	spacing,			spacing)),			blockColor), pCurRow);
					}
					
				}
			}
			else
			{
						// top / bottom blocks
						int curSpacingHoriz = (blockSize + 2 * spacing) * num + spacing;
						pPage->addBox(new ShapeBox(MiniGamePlayerAttackShapeBoxPreset(EShapeBoxClass_RECT,		characterIndex, false, attackNum, category, whenToShow), TextBoxPositionInfo(ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, blockSize, blockSize, Edges(spacing,		spacing,	curSpacingHoriz,	curSpacingHoriz)),	blockColor), pCurRow);
			}
			pDiagramBlock->mpSubElems.push_back(pCurRow);
		}
		break;
	case EMiniGameCombatMoveAttackTypes_CHECKERBOARD:
		for (int i = 0; i < (2 * (num - out) + 1); i++)
		{
			pCurRow = new BlockAlignElementsHorizontally(maxWidth, maxHeight, ETextBoxPositionAlign_CENTER, EDirection_RIGHT, !fill, !fill, Edges(), clear, ("block diagram row " + std::to_string(i)));
			for (int j = 0; j < (2 * (num - out) + 1); j++)
			{
				if (i == num && j == num)
				{
					// middle dot
					pPage->addBox(new ShapeBox(MiniGamePlayerAttackShapeBoxPreset(EShapeBoxClass_CIRCLE,	characterIndex, false, attackNum, category, whenToShow), TextBoxPositionInfo(ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, blockSize, blockSize, Edges(outSpacing,	outSpacing, outSpacing, outSpacing)),	blockColor), pCurRow);
				}
				else if (i % 2 == j % 2)
				{
					// invisible block 
					pPage->addBox(new ShapeBox(MiniGamePlayerAttackShapeBoxPreset(EShapeBoxClass_RECT,		characterIndex, false, attackNum, category, whenToShow), TextBoxPositionInfo(ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, blockSize, blockSize, Edges(spacing,		spacing,	spacing,	spacing)),		clear),		 pCurRow);
				}
				else
				{
					pPage->addBox(new ShapeBox(MiniGamePlayerAttackShapeBoxPreset(EShapeBoxClass_RECT,		characterIndex, false, attackNum, category, whenToShow), TextBoxPositionInfo(ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, blockSize, blockSize, Edges(spacing,		spacing,	spacing,	spacing)),		blockColor), pCurRow);
				}
			}
			pDiagramBlock->mpSubElems.push_back(pCurRow);
		}
		break;
	case EMiniGameCombatMoveAttackTypes_ONE_CHARACTER:
	case EMiniGameCombatMoveAttackTypes_ONE_ENEMY:
	case EMiniGameCombatMoveAttackTypes_ONE_PLAYER:
		maxWidth = blockSize;
		maxHeight = blockSize;
		pCurRow = new BlockAlignElementsHorizontally(maxWidth, maxHeight, ETextBoxPositionAlign_CENTER, EDirection_RIGHT, !fill, !fill, Edges(), clear, ("block diagram row 0"));
		// middle dot
		pPage->addBox(new ShapeBox(MiniGamePlayerAttackShapeBoxPreset(EShapeBoxClass_CIRCLE, characterIndex, false, attackNum, category, whenToShow), TextBoxPositionInfo(ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, blockSize, blockSize, Edges(outSpacing, outSpacing, outSpacing, outSpacing)), blockColor), pCurRow);
		pDiagramBlock->mpSubElems.push_back(pCurRow);
		break;
	default:
		SDL_assert(false);
	}
	if ((int)pDiagramBlock->mpSubElems.size() > 0)
	{
		pBlock->mpSubElems.push_back(pDiagramBlock);
	}
	else
	{
		delete pDiagramBlock;
	}
	pDiagramBlock = nullptr;
	pCurRow = nullptr;
}

void setUpFontSizeChart(FontSizeChart& fontSizeChart, SDL_Renderer* pRenderer)  { fontSizeChart.createFontChart(StyleManager::fontName, pRenderer); }
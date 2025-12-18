#include "lotusAdventureMenus.h"

void createMenus(MenuManager& menuManager, ScreenObject& screen, MiniGameWorldData& worldData, StyleManager& styleManager, SettingsManager& settingsManager)
{
	SDL_Renderer* pRenderer = screen.mpRenderer;

	const char* fontFileName = styleManager.fontName;

	SDL_Color pink = styleManager.pink;
	SDL_Color white = styleManager.white;
	SDL_Color teal = styleManager.teal;
	SDL_Color hintBlue = styleManager.hintBlue;
	SDL_Color clear = styleManager.clear;

	int levelChunkWidth = screen.mGameLevelChunkWidth;
	int levelChunkHeight = screen.mGameLevelChunkHeight;

	bool fill = true;

	//MAIN MENU PAGE
	MenuPage* mainMenuPage = new MenuPage();

	int maxWidth = levelChunkWidth;
	BlockAlignElementsHorizontally* blockH = new BlockAlignElementsHorizontally(Hitbox(CoordsX1Y1WidthHeight(0, 0, maxWidth, 300)), ETextBoxPositionAlign_CENTER, EDirection_LEFT_AND_RIGHT, EDirection_DOWN, !fill, !fill, Edges(200, 0, 0, 0), clear, "head block");
	BlockAlignElementsVertically* blockV = new BlockAlignElementsVertically(maxWidth, 300, ETextBoxPositionAlign_CENTER, EDirection_DOWN, !fill, !fill, Edges(0, 300, 0, 0));
	mainMenuPage->addBox(new ImageBox(DontAutoShowImageBoxPreset(ETextBoxID_TAKE_DAMAGE_SCREEN), ImageBoxPositionInfo(0, ETextBoxPositionAlign_LEFT, 1600, 200, Edges()), "Menu/TitleScreen.bmp"), blockV);
	mainMenuPage->addBox(new TextBox(StandardTextBoxPreset("PLAY GAME"),		ETextBoxFunction_PLAY_GAME_BOX,		 TextBoxPositionInfo(ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, maxWidth, 140, Edges(0, 10, 0, 0)), fontFileName, TextBoxSizeInfo(60, 80), TextBoxColorInfo(hintBlue, white, teal, teal)), true, blockV);
	mainMenuPage->addBox(new TextBox(StandardTextBoxPreset("PLAY MINI GAME"),	ETextBoxFunction_PLAY_MINI_GAME_BOX, TextBoxPositionInfo(ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, maxWidth, 140, Edges(0, 0,  0, 0)), fontFileName, TextBoxSizeInfo(60, 80), TextBoxColorInfo(hintBlue, white, teal, teal)), true, blockV);
	blockH->mpSubBlocks.push_back(blockV);
	
	BlockAlignElementsHorizontally* blockModel = new BlockAlignElementsHorizontally(Hitbox(CoordsX1Y1WidthHeight(0, 900, 600, 600)), ETextBoxPositionAlign_LEFT, EDirection_RIGHT, EDirection_UP, !fill, !fill, Edges(0, 0, 0, 0), clear);
	mainMenuPage->addBox(new ImageBox(DontAutoShowImageBoxPreset(ETextBoxID_TAKE_DAMAGE_SCREEN), ImageBoxPositionInfo(0, ETextBoxPositionAlign_LEFT, 600, 600, Edges()), "Menu/TitleScreenModel.bmp"), blockModel);
	
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
	createMainGameMenu(menuManager, screen, styleManager);


	//MINI GAME MENU PAGE
	createMiniGameMenu(menuManager, screen, worldData, styleManager, settingsManager);

	//SETTINGS
	MenuPage* settingsPage = new MenuPage();

	settingsPage->mpAllDisplayOnlyTextBoxes.push_back(new TextBox(StandardTextBoxPreset("SETTINGS"),  ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(Vect2(levelChunkWidth / 2, 200), ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, 1600, 200), fontFileName, TextBoxSizeInfo(60, 80), TextBoxColorInfo(white, teal)));
	settingsPage->mpAllDisplayOnlyTextBoxes.push_back(new TextBox(StandardTextBoxPreset("Text Size"), ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(Vect2(levelChunkWidth / 2, 200), ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, 1600, 200), fontFileName, TextBoxSizeInfo(60, 80), TextBoxColorInfo(white, teal)));
	menuManager.mpMenuPages.push_back(settingsPage);

	menuManager.mpCurMenuPage = menuManager.mpMenuPages[EMenuPageType_MAIN_GAME_MENU];

	menuManager.setAllTextBoxTextures();
	menuManager.setUpBlocks();
}

void createMainGameMenu(MenuManager& menuManager, ScreenObject& screen, StyleManager& styleManager)
{
	SDL_Color hintBlue = styleManager.hintBlue;
	SDL_Color clear = styleManager.clear;
	const char* fontFileName = styleManager.fontName;

	int levelChunkWidth = screen.mGameLevelChunkWidth;
	int levelChunkHeight = screen.mGameLevelChunkHeight;

	bool fill = true;

	//MAIN GAME MENU PAGE
	MenuPage* mainGameMenuPage = new MenuPage();

	//Take Damage screen
	BlockAlignElementsVertically* takeDamageBlock = new BlockAlignElementsVertically(Hitbox(CoordsX1Y1WidthHeight(0, 0, levelChunkWidth, levelChunkHeight)), ETextBoxPositionAlign_LEFT, EDirection_RIGHT, EDirection_DOWN, fill, fill, Edges());
	mainGameMenuPage->addBox(new ImageBox(DontAutoShowImageBoxPreset(ETextBoxID_TAKE_DAMAGE_SCREEN), ImageBoxPositionInfo(Vect2(0, 0), 0, ETextBoxPositionAlign_LEFT, 1600, 900), "UIElements/takeDamageScreen.bmp"), takeDamageBlock);
	mainGameMenuPage->mpBlocks.push_back(takeDamageBlock);

	//UPPER HUD
	int maxWidth = 400;
	int maxHeight = 100;
	BlockAlignElementsVertically*	upperHUDBlock	= new BlockAlignElementsVertically(Hitbox(CoordsX1Y1WidthHeight(0, 0, maxWidth, 450)), ETextBoxPositionAlign_LEFT, EDirection_RIGHT, EDirection_DOWN, !fill, !fill, Edges());
	BlockAlignElementsHorizontally* uLine1			= new BlockAlignElementsHorizontally(maxWidth, maxHeight, ETextBoxPositionAlign_CENTER, EDirection_RIGHT, !fill, !fill, Edges(10, 0, 0, 0));
	mainGameMenuPage->addBox(new ImageBox(ImageBoxPreset(), ImageBoxPositionInfo(0, ETextBoxPositionAlign_LEFT, 80, 80, Edges(0, 0, 0, 0)), "UIElements/HUDUI.bmp"), uLine1);
	mainGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EGameStatBoxValueToDisplay_CUR_LEVEL_NUMBER), ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_CENTER, maxWidth, maxHeight, Edges(0, 0, 0, 0)), fontFileName, TextBoxSizeInfo(16, 16), TextBoxColorInfo(hintBlue)), false, uLine1);

	BlockAlignElementsHorizontally* uLine2 = new BlockAlignElementsHorizontally(maxWidth, maxHeight, ETextBoxPositionAlign_CENTER, EDirection_RIGHT, !fill, !fill, Edges(10, 0, 5, 0));
	BlockAlignElementsVertically*	uLine3 = new BlockAlignElementsVertically(	maxWidth, maxHeight, ETextBoxPositionAlign_CENTER, EDirection_DOWN,  !fill, !fill, Edges(0,  0, 0, 0));
	mainGameMenuPage->addBox(new ImageBox(ImageBoxPreset(), ImageBoxPositionInfo(0, ETextBoxPositionAlign_CENTER, 20, 20, Edges(0, 0, 0, 0)), "UIElements/keyUI.bmp"), uLine3);
	mainGameMenuPage->addBox(new ImageBox(ImageBoxPreset(), ImageBoxPositionInfo(0, ETextBoxPositionAlign_CENTER, 20, 20, Edges(3, 0, 0, 0)), "UIElements/targetUI.bmp"), uLine3);
	mainGameMenuPage->addBox(new ImageBox(ImageBoxPreset(), ImageBoxPositionInfo(0, ETextBoxPositionAlign_CENTER, 20, 20, Edges(3, 0, 0, 0)), "UIElements/collectibleUI.bmp"), uLine3);
	mainGameMenuPage->addBox(new ImageBox(ImageBoxPreset(), ImageBoxPositionInfo(0, ETextBoxPositionAlign_CENTER, 20, 20, Edges(3, 0, 0, 0)), "UIElements/enemiesUI.bmp"), uLine3);

	BlockAlignElementsVertically* uLine4 = new BlockAlignElementsVertically(maxWidth, maxHeight, ETextBoxPositionAlign_LEFT, EDirection_DOWN, !fill, !fill, Edges(0, 0, 8, 0));
	mainGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EGameStatBoxValueToDisplay_CUR_KEYS),			ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_CENTER, maxWidth, maxHeight, Edges(3, 0, 0, 0)), fontFileName, TextBoxSizeInfo(12, 12), TextBoxColorInfo(hintBlue)), false, uLine4);
	mainGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EGameStatBoxValueToDisplay_CUR_TARGETS),			ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_CENTER, maxWidth, maxHeight, Edges(5, 0, 0, 0)), fontFileName, TextBoxSizeInfo(12, 12), TextBoxColorInfo(hintBlue)), false, uLine4);
	mainGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EGameStatBoxValueToDisplay_CUR_COLLECTIBLES),	ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_CENTER, maxWidth, maxHeight, Edges(5, 0, 0, 0)), fontFileName, TextBoxSizeInfo(12, 12), TextBoxColorInfo(hintBlue)), false, uLine4);
	mainGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EGameStatBoxValueToDisplay_CUR_ENEMIES_LEFT),	ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_CENTER, maxWidth, maxHeight, Edges(9, 0, 0, 0)), fontFileName, TextBoxSizeInfo(12, 12), TextBoxColorInfo(hintBlue)), false, uLine4);

	upperHUDBlock->mpSubBlocks.push_back(uLine1);
	upperHUDBlock->mpSubBlocks.push_back(uLine2);
	uLine2->mpSubBlocks.push_back(uLine3);
	uLine2->mpSubBlocks.push_back(uLine4);

	mainGameMenuPage->mpBlocks.push_back(upperHUDBlock);

	//debug HUD
	maxWidth = 200;
	maxHeight = 100;
	BlockAlignElementsVertically* debugHUDBlock = new BlockAlignElementsVertically(Hitbox(CoordsX1Y1WidthHeight(levelChunkWidth, 0, maxWidth, maxHeight)), ETextBoxPositionAlign_LEFT, EDirection_LEFT, EDirection_DOWN, !fill, !fill, Edges());
	mainGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EGameStatBoxValueToDisplay_CUR_KEYBOARD),		ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_CENTER, maxWidth, maxHeight, Edges(3, 0, 0, 0)), fontFileName, TextBoxSizeInfo(12, 12), TextBoxColorInfo(hintBlue)), false, debugHUDBlock);

	mainGameMenuPage->mpBlocks.push_back(debugHUDBlock);

	//LOWER LEFT HUD
	BlockAlignElementsVertically*	lowerLeftHUDBlock	= new BlockAlignElementsVertically(Hitbox(CoordsX1Y1WidthHeight(0, 900, maxWidth, 450)),	ETextBoxPositionAlign_LEFT,		EDirection_RIGHT, EDirection_UP, !fill, !fill, Edges(),				clear, "head block");
	BlockAlignElementsHorizontally* llLine1				= new BlockAlignElementsHorizontally(maxWidth, maxHeight,									ETextBoxPositionAlign_CENTER,	EDirection_RIGHT,				 !fill, !fill, Edges(0, 10, 0, 0),	clear, "slash block");
	mainGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EGameStatBoxValueToDisplay_SLASH_KEY), ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_CENTER, 100, 60, Edges(0, 0, 4, 0)), fontFileName, TextBoxSizeInfo(15, 15), TextBoxColorInfo(hintBlue)), false, llLine1);
	mainGameMenuPage->addBox(new ImageBox(DontAutoShowImageBoxPreset(ETextBoxID_SLASH_UI), ImageBoxPositionInfo(0, ETextBoxPositionAlign_CENTER, 40, 25, Edges(0, 0, 20, 0)), "UIElements/slashUI.bmp"), llLine1);

	BlockAlignElementsHorizontally* llLine2 = new BlockAlignElementsHorizontally(maxWidth, maxHeight, ETextBoxPositionAlign_LEFT, EDirection_RIGHT, !fill, !fill, Edges(0, 2, 0, 0), clear, "double jump block");
	mainGameMenuPage->addBox(new ImageBox(DontAutoShowImageBoxPreset(ETextBoxID_DOUBLE_JUMP_UI), ImageBoxPositionInfo(0, ETextBoxPositionAlign_CENTER, 40, 30, Edges(0, 0, 38, 0)), "UIElements/doubleJumpUI.bmp"), llLine2);

	BlockAlignElementsHorizontally* llLine3 = new BlockAlignElementsHorizontally(	maxWidth, maxHeight, ETextBoxPositionAlign_CENTER, EDirection_RIGHT,	!fill, !fill, Edges(0, 10, 0, 0), clear, "projectile block");
	BlockAlignElementsVertically* llLine4 = new BlockAlignElementsVertically(		maxWidth, maxHeight, ETextBoxPositionAlign_CENTER, EDirection_DOWN,		!fill, !fill, Edges(20, 0, 0, 0), clear, "projectile keys block");
	mainGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EGameStatBoxValueToDisplay_PROJECTILE_HORIZONTAL_KEY),	ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_CENTER, 100, 60, Edges(0,  0, 3, 0)), fontFileName, TextBoxSizeInfo(15, 15), TextBoxColorInfo(hintBlue)), false, llLine4);
	mainGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EGameStatBoxValueToDisplay_PROJECTILE_VERTICAL_KEY),		ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_CENTER, 100, 60, Edges(10, 0, 3, 0)), fontFileName, TextBoxSizeInfo(15, 15), TextBoxColorInfo(hintBlue)), false, llLine4);

	BlockAlignElementsHorizontally* llLine5 = new BlockAlignElementsHorizontally(maxWidth, maxHeight, ETextBoxPositionAlign_LEFT, EDirection_RIGHT, fill, fill, Edges(0, 0, 0, 0), clear, "projectile icons block");
	mainGameMenuPage->addBox(new ImageBox(DontAutoShowImageBoxPreset(ETextBoxID_PROJECTILE_UI), ImageBoxPositionInfo(30, ETextBoxPositionAlign_CENTER, 80, 90, Edges(0, 0, 30, 0)), "UIElements/swordUI.bmp"), llLine5);
	mainGameMenuPage->addBox(new ImageBox(DontAutoShowImageBoxPreset(ETextBoxID_PROJECTILE_UI), ImageBoxPositionInfo(30, ETextBoxPositionAlign_CENTER, 80, 90, Edges(0, 0, 22, 0)), "UIElements/swordUI.bmp"), llLine5);
	mainGameMenuPage->addBox(new ImageBox(DontAutoShowImageBoxPreset(ETextBoxID_PROJECTILE_UI), ImageBoxPositionInfo(30, ETextBoxPositionAlign_CENTER, 80, 90, Edges(0, 0, 22, 0)), "UIElements/swordUI.bmp"), llLine5);
	mainGameMenuPage->addBox(new ImageBox(DontAutoShowImageBoxPreset(ETextBoxID_PROJECTILE_UI), ImageBoxPositionInfo(30, ETextBoxPositionAlign_CENTER, 80, 90, Edges(0, 0, 22, 0)), "UIElements/swordUI.bmp"), llLine5);
	mainGameMenuPage->addBox(new ImageBox(DontAutoShowImageBoxPreset(ETextBoxID_PROJECTILE_UI), ImageBoxPositionInfo(30, ETextBoxPositionAlign_CENTER, 80, 90, Edges(0, 0, 22, 0)), "UIElements/swordUI.bmp"), llLine5);
	mainGameMenuPage->addBox(new ImageBox(DontAutoShowImageBoxPreset(ETextBoxID_PROJECTILE_UI), ImageBoxPositionInfo(30, ETextBoxPositionAlign_CENTER, 80, 90, Edges(0, 0, 22, 0)), "UIElements/swordUI.bmp"), llLine5);
	mainGameMenuPage->addBox(new ImageBox(DontAutoShowImageBoxPreset(ETextBoxID_PROJECTILE_UI), ImageBoxPositionInfo(30, ETextBoxPositionAlign_CENTER, 80, 90, Edges(0, 0, 22, 0)), "UIElements/swordUI.bmp"), llLine5);

	llLine3->mpSubBlocks.push_back(llLine4);
	llLine3->mpSubBlocks.push_back(llLine5);

	lowerLeftHUDBlock->mpSubBlocks.push_back(llLine1);
	lowerLeftHUDBlock->mpSubBlocks.push_back(llLine2);
	lowerLeftHUDBlock->mpSubBlocks.push_back(llLine3);

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

	lrLine1->mpSubBlocks.push_back(lrLine2);
	lrLine1->mpSubBlocks.push_back(lrLine3);
	lrLine1->mpSubBlocks.push_back(lrLine4);

	//resetLevel
	BlockAlignElementsVertically* lrLine5 = new BlockAlignElementsVertically(maxWidth, maxHeight, ETextBoxPositionAlign_CENTER, EDirection_DOWN, !fill, !fill, Edges(20, 0, 0, 10), clear, "Reset Level Block");
	mainGameMenuPage->addBox(new ImageBox(ImageBoxPreset(), ImageBoxPositionInfo(0, ETextBoxPositionAlign_CENTER, 20, 20, Edges(2, 0, 0, 0)), "UIElements/resetLevelUI.bmp"), lrLine5);
	mainGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EGameStatBoxValueToDisplay_LEVEL_RESET_KEY), ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, 100, 60, Edges(0, 0, 0, 0)), fontFileName, TextBoxSizeInfo(12, 12), TextBoxColorInfo(hintBlue)), false, lrLine5);

	//resetCheckpoint
	BlockAlignElementsVertically* lrLine6 = new BlockAlignElementsVertically(maxWidth, maxHeight, ETextBoxPositionAlign_CENTER, EDirection_DOWN, !fill, !fill, Edges(20, 0, 0, 10), clear, "Reset Checkpoint Block");
	mainGameMenuPage->addBox(new ImageBox(ImageBoxPreset(), ImageBoxPositionInfo(0, ETextBoxPositionAlign_CENTER, 20, 20, Edges(2, 0, 0, 0)), "UIElements/resetCheckpointUI.bmp"), lrLine6);
	mainGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EGameStatBoxValueToDisplay_CHECKPOINT_RESET_KEY), ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, 100, 60, Edges(0, 0, 0, 0)), fontFileName, TextBoxSizeInfo(12, 12), TextBoxColorInfo(hintBlue)), false, lrLine6);


	lowerRightHUDBlock->mpSubBlocks.push_back(lrLine1);
	lowerRightHUDBlock->mpSubBlocks.push_back(lrLine5);
	lowerRightHUDBlock->mpSubBlocks.push_back(lrLine6);
	mainGameMenuPage->mpBlocks.push_back(lowerRightHUDBlock);

	menuManager.mpMenuPages.push_back(mainGameMenuPage);
}

void createMiniGameMenu(MenuManager& menuManager, ScreenObject& screen,  MiniGameWorldData& worldData, StyleManager& styleManager, SettingsManager& settingsManager)
{
	const char* font = styleManager.fontName;

	MenuPage* miniGameMenuPage = new MenuPage();
	menuManager.mpMenuPages.push_back(miniGameMenuPage);

	createMiniGameCharacterStatsMenu(menuManager, screen, worldData, styleManager, settingsManager); //block 0

	

	TextBoxColorInfo black		= TextBoxColorInfo(styleManager.black);
	TextBoxColorInfo darkPink	= TextBoxColorInfo(styleManager.pink);
	TextBoxColorInfo white		= TextBoxColorInfo(styleManager.white);
	TextBoxColorInfo colors		= TextBoxColorInfo(styleManager.pink,	styleManager.white,		styleManager.sunYellow, styleManager.sunYellow);
	TextBoxColorInfo optionBox	= TextBoxColorInfo(styleManager.white,	styleManager.sunYellow, styleManager.pink,		styleManager.pink,		styleManager.white, styleManager.sunYellow);
	SDL_Color highlightPanelColor	= styleManager.sunYellow;
	SDL_Color standardPanelColor	= styleManager.lightPink;

	bool fill = true;
	
	int maxWidth = 400;
	int maxHeight = 300;
	int curX = 1300;
	int curY = 10;
	BlockAlignElementsVertically* upperRightBlock = new BlockAlignElementsVertically(Hitbox(CoordsX1Y1WidthHeight(curX, curY, maxWidth, maxHeight)), ETextBoxPositionAlign_CENTER, EDirection_LEFT_AND_RIGHT, EDirection_DOWN, !fill, !fill, Edges(), styleManager.pink);

	miniGameMenuPage->addBox(new TextBox(StandardTextBoxPreset("PLAY GAME"), ETextBoxFunction_PLAY_GAME_BOX,		TextBoxPositionInfo(ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, maxWidth, 60, Edges(15, 10, 15, 15)), font, TextBoxSizeInfo(15, 25, 2), optionBox), true, upperRightBlock);
	miniGameMenuPage->addBox(new TextBox(StandardTextBoxPreset("MAIN MENU"), ETextBoxFunction_GO_TO_MAIN_MENU_BOX,	TextBoxPositionInfo(ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, maxWidth, 60, Edges(0,  15, 15, 15)), font, TextBoxSizeInfo(15, 25, 2), optionBox), true, upperRightBlock);

	//debug HUD
	maxWidth = 200;
	maxHeight = 100;
	BlockAlignElementsVertically* debugHUDBlock = new BlockAlignElementsVertically(Hitbox(CoordsX1Y1WidthHeight(1200, 0, maxWidth, maxHeight)), ETextBoxPositionAlign_LEFT, EDirection_RIGHT, EDirection_DOWN, !fill, !fill, Edges());
	miniGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EGameStatBoxValueToDisplay_CUR_KEYBOARD), ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_CENTER, maxWidth, maxHeight, Edges(3, 0, 0, 0)), font, TextBoxSizeInfo(12, 12), white), false, debugHUDBlock);


	std::vector <EMiniGameState> whenToShow;
	curX = 700;
	curY = 375;
	maxWidth = 700;
	maxHeight = 450;
	int maxLines = 6; // for maxHeight

	BlockAlignElementsVertically* characterOptionsBlock			= new BlockAlignElementsVertically(Hitbox(CoordsX1Y1WidthHeight(curX, curY, maxWidth, maxHeight)),	ETextBoxPositionAlign_LEFT,		EDirection_RIGHT, EDirection_DOWN,	fill, fill,	Edges(),	standardPanelColor);
	BlockAlignElementsVertically* characterOptionsHeadingBlock	= new BlockAlignElementsVertically(maxWidth, (maxHeight / 3),										ETextBoxPositionAlign_CENTER,	EDirection_DOWN,					fill, fill,	Edges(),	highlightPanelColor);
	BlockAlignElementsVertically* characterOptionsDetailsBlock	= new BlockAlignElementsVertically(maxWidth, ((2 * maxHeight) / 3),									ETextBoxPositionAlign_LEFT,		EDirection_DOWN,					fill, fill,	Edges(),	standardPanelColor);
	characterOptionsBlock->mpSubBlocks.push_back(characterOptionsHeadingBlock);
	characterOptionsBlock->mpSubBlocks.push_back(characterOptionsDetailsBlock);

	whenToShow = { EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT, EMiniGameState_PLAYER_WAIT_FOR_ACTION_INPUT, EMiniGameState_PLAYER_WAIT_FOR_ATTACK_INPUT, EMiniGameState_PLAYER_WAIT_FOR_ATTACK_SUB_INPUT, EMiniGameState_PLAYER_TAKE_ACTION_ATTACK };
	miniGameMenuPage->addBox(new TextBox(MiniGamePlayerBoxPreset(ECharacterStatBoxValueToDisplay_CHARACTER_NAME, whenToShow), ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, maxWidth, maxHeight / maxLines, Edges(50, 10, 0, 0)), font, TextBoxSizeInfo(20, 20), black), false, characterOptionsHeadingBlock);

	int headingText = 35;
	int optionText	= 30;
	int detailText	= 20;
	int bullet		= 20;

	//CHARACTER
	//SELECT TILE TO MOVE TO
	whenToShow = { EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT };

	miniGameMenuPage->addBox(new TextBox(MiniGameBoxPreset("Select a Tile to Move To", whenToShow), ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, maxWidth, maxHeight / maxLines, Edges(0, 10, 0, 0)), font, TextBoxSizeInfo(headingText, headingText), darkPink),	false, characterOptionsHeadingBlock);
	
	BlockAlignElementsHorizontally* moveRowBlock = new BlockAlignElementsHorizontally(maxWidth, ((2 * maxHeight) / 3), ETextBoxPositionAlign_CENTER, EDirection_RIGHT, !fill, !fill, Edges(10, 0, 0, 0), standardPanelColor);
	miniGameMenuPage->addBox(new ShapeBox(MiniGameShapeBoxPreset(EShapeBoxClass_CIRCLE,									whenToShow),								TextBoxPositionInfo(ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, bullet,		bullet,					Edges(0, 0,  10, 5)),														styleManager.white),			moveRowBlock);
	miniGameMenuPage->addBox(new TextBox(MiniGameBoxPreset("Move Style:",												whenToShow), ETextBoxFunction_NO_FUNCTION,	TextBoxPositionInfo(ETextBoxPositionAlign_LEFT,	  ETextBoxTextAlign_CENTER, maxWidth,	maxHeight / maxLines,	Edges(0, 0,  0,  0)), font, TextBoxSizeInfo(optionText,	optionText),		white),					false,	moveRowBlock);
	miniGameMenuPage->addBox(new TextBox(MiniGamePlayerBoxPreset(ECharacterStatBoxValueToDisplay_CHARACTER_MOVE_TYPE,	whenToShow), ETextBoxFunction_NO_FUNCTION,	TextBoxPositionInfo(ETextBoxPositionAlign_LEFT,	  ETextBoxTextAlign_CENTER, maxWidth,	maxHeight / maxLines,	Edges(0, 0,  0,  5)), font, TextBoxSizeInfo(optionText,	optionText),		white),					false,	moveRowBlock);
	characterOptionsDetailsBlock->mpSubBlocks.push_back(moveRowBlock);

	////CHARACTER
	////CHOOSE ACTION
	whenToShow = { EMiniGameState_PLAYER_WAIT_FOR_ACTION_INPUT };
	miniGameMenuPage->addBox(new TextBox(MiniGameBoxPreset("Select an action",  whenToShow), ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_CENTER, maxWidth, maxHeight / maxLines, Edges(0, 10, 0, 0)), font, TextBoxSizeInfo(headingText, headingText),	darkPink),	false, characterOptionsHeadingBlock);
	
	BlockAlignElementsVertically* actionsBlock	= new BlockAlignElementsVertically(maxWidth, ((2 * maxHeight) / 3), ETextBoxPositionAlign_LEFT, EDirection_DOWN, !fill, !fill, Edges(),	standardPanelColor);
	BlockAlignElementsHorizontally* attackOptionRowBlock = new BlockAlignElementsHorizontally(maxWidth, ((2 * maxHeight) / 3), ETextBoxPositionAlign_CENTER, EDirection_RIGHT, !fill, !fill, Edges(0, 10, 0, 0));
	miniGameMenuPage->addBox(new ShapeBox(MiniGameShapeBoxPreset(EShapeBoxClass_CIRCLE,	whenToShow),														TextBoxPositionInfo(ETextBoxPositionAlign_CENTER,	ETextBoxTextAlign_CENTER,	bullet,		bullet,					Edges(0, 0,  10, 10)),																styleManager.white),	attackOptionRowBlock);
	miniGameMenuPage->addBox(new TextBox(MiniGameBoxPreset("ATTACK",					whenToShow), ETextBoxFunction_ATTACK_CUR_COMBAT_CHARACTER_BOX,		TextBoxPositionInfo(ETextBoxPositionAlign_LEFT,		ETextBoxTextAlign_CENTER,	maxWidth,	maxHeight / maxLines,	Edges()),				font, TextBoxSizeInfo(optionText,	optionText + 5, 2),		optionBox),	true,		attackOptionRowBlock);
	BlockAlignElementsHorizontally* defendOptionRowBlock = new BlockAlignElementsHorizontally(maxWidth, ((2 * maxHeight) / 3), ETextBoxPositionAlign_CENTER, EDirection_RIGHT, !fill, !fill, Edges(0, 10, 0, 0));
	miniGameMenuPage->addBox(new ShapeBox(MiniGameShapeBoxPreset(EShapeBoxClass_CIRCLE,	whenToShow),														TextBoxPositionInfo(ETextBoxPositionAlign_CENTER,	ETextBoxTextAlign_CENTER,	bullet,		bullet,					Edges(0, 0,  10, 10)),																styleManager.white),	defendOptionRowBlock);
	miniGameMenuPage->addBox(new TextBox(MiniGameBoxPreset("DEFEND",					whenToShow), ETextBoxFunction_DEFEND_CUR_COMBAT_CHARACTER_BOX,		TextBoxPositionInfo(ETextBoxPositionAlign_LEFT,		ETextBoxTextAlign_CENTER,	maxWidth,	maxHeight / maxLines,	Edges()),				font, TextBoxSizeInfo(optionText,	optionText + 5, 2),		optionBox),	true,		defendOptionRowBlock);
	BlockAlignElementsHorizontally* passOptionRowBlock   = new BlockAlignElementsHorizontally(maxWidth, ((2 * maxHeight) / 3), ETextBoxPositionAlign_CENTER, EDirection_RIGHT, !fill, !fill, Edges());
	miniGameMenuPage->addBox(new ShapeBox(MiniGameShapeBoxPreset(EShapeBoxClass_CIRCLE,	whenToShow),														TextBoxPositionInfo(ETextBoxPositionAlign_CENTER,	ETextBoxTextAlign_CENTER,	bullet,		bullet,					Edges(0, 0,  10, 10)),																styleManager.white),	passOptionRowBlock);
	miniGameMenuPage->addBox(new TextBox(MiniGameBoxPreset("PASS TURN",					whenToShow), ETextBoxFunction_PASS_CUR_COMBAT_CHARACTER_TURN_BOX,	TextBoxPositionInfo(ETextBoxPositionAlign_LEFT,		ETextBoxTextAlign_CENTER,	maxWidth,	maxHeight / maxLines,	Edges()),				font, TextBoxSizeInfo(optionText,	optionText + 5, 2),		optionBox),	true,		passOptionRowBlock);
	actionsBlock->mpSubBlocks.push_back(attackOptionRowBlock);
	actionsBlock->mpSubBlocks.push_back(defendOptionRowBlock);
	actionsBlock->mpSubBlocks.push_back(passOptionRowBlock);
	characterOptionsDetailsBlock->mpSubBlocks.push_back(actionsBlock);

	////CHARACTER
	////CHOOSE ATTACK DIRECTION
	whenToShow = { EMiniGameState_PLAYER_WAIT_FOR_ATTACK_SUB_INPUT };

	miniGameMenuPage->addBox(new TextBox(MiniGameBoxPreset("Choose Attack Direction",										whenToShow), ETextBoxFunction_NO_FUNCTION,					TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_CENTER, maxWidth, maxHeight / maxLines, Edges(0, 10, 0, 0)), font, TextBoxSizeInfo(headingText,	headingText),	darkPink),	false, characterOptionsHeadingBlock);
	miniGameMenuPage->addBox(new TextBox(MiniGameBoxPreset("All tiles in the direction will be attacked",					whenToShow), ETextBoxFunction_NO_FUNCTION,					TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_CENTER, maxWidth, maxHeight / maxLines, Edges(0, 10, 0, 0)), font, TextBoxSizeInfo(detailText, detailText),		black),		false, characterOptionsHeadingBlock);

	BlockAlignElementsVertically* attackDirectionBlock = new BlockAlignElementsVertically(maxWidth, ((2 * maxHeight) / 3), ETextBoxPositionAlign_LEFT, EDirection_DOWN, !fill, !fill, Edges(), standardPanelColor);
	BlockAlignElementsVertically* attackDetailsBlock = new BlockAlignElementsVertically(maxWidth, ((2 * maxHeight) / 3), ETextBoxPositionAlign_LEFT, EDirection_DOWN, !fill, !fill, Edges());
	miniGameMenuPage->addBox(new TextBox(MiniGamePlayerBoxPreset(ECharacterStatBoxValueToDisplay_CHARACTER_CUR_ATTACK_NAME, whenToShow), ETextBoxFunction_NO_FUNCTION,					TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_CENTER, maxWidth, maxHeight / maxLines, Edges(5, 5, 0, 0)), font, TextBoxSizeInfo(detailText,	detailText),		black),		false, attackDetailsBlock);
	BlockAlignElementsHorizontally* leftOptionRowBlock = new BlockAlignElementsHorizontally(maxWidth, ((2 * maxHeight) / 3), ETextBoxPositionAlign_CENTER, EDirection_RIGHT, !fill, !fill, Edges());
	miniGameMenuPage->addBox(new ShapeBox(MiniGameShapeBoxPreset(EShapeBoxClass_CIRCLE,	whenToShow),												TextBoxPositionInfo(ETextBoxPositionAlign_CENTER,	ETextBoxTextAlign_CENTER, bullet,		bullet,					Edges(0, 0,  10, 5)),														styleManager.white),	leftOptionRowBlock);
	miniGameMenuPage->addBox(new TextBox(MiniGameBoxPreset("LEFT",						whenToShow), ETextBoxFunction_ATTACK_DIRECTION_LEFT_BOX,	TextBoxPositionInfo(ETextBoxPositionAlign_LEFT,		ETextBoxTextAlign_CENTER, maxWidth,		maxHeight / maxLines,	Edges(0, 10, 0,  0)), font, TextBoxSizeInfo(optionText,	optionText, 2),		optionBox), true,		leftOptionRowBlock);
	BlockAlignElementsHorizontally* rightOptionRowBlock = new BlockAlignElementsHorizontally(maxWidth, ((2 * maxHeight) / 3), ETextBoxPositionAlign_CENTER, EDirection_RIGHT, !fill, !fill, Edges());
	miniGameMenuPage->addBox(new ShapeBox(MiniGameShapeBoxPreset(EShapeBoxClass_CIRCLE,	whenToShow),												TextBoxPositionInfo(ETextBoxPositionAlign_CENTER,	ETextBoxTextAlign_CENTER, bullet,		bullet,					Edges(0, 0,  10, 5)),														styleManager.white),	rightOptionRowBlock);
	miniGameMenuPage->addBox(new TextBox(MiniGameBoxPreset("RIGHT",						whenToShow), ETextBoxFunction_ATTACK_DIRECTION_RIGHT_BOX,	TextBoxPositionInfo(ETextBoxPositionAlign_LEFT,		ETextBoxTextAlign_CENTER, maxWidth,		maxHeight / maxLines,	Edges(0, 10, 0,  0)), font, TextBoxSizeInfo(optionText,	optionText, 2),		optionBox), true,		rightOptionRowBlock);
	BlockAlignElementsHorizontally* upOptionRowBlock = new BlockAlignElementsHorizontally(maxWidth, ((2 * maxHeight) / 3), ETextBoxPositionAlign_CENTER, EDirection_RIGHT, !fill, !fill, Edges());
	miniGameMenuPage->addBox(new ShapeBox(MiniGameShapeBoxPreset(EShapeBoxClass_CIRCLE,	whenToShow),												TextBoxPositionInfo(ETextBoxPositionAlign_CENTER,	ETextBoxTextAlign_CENTER, bullet,		bullet,					Edges(0, 0,  10, 5)),														styleManager.white),	upOptionRowBlock);
	miniGameMenuPage->addBox(new TextBox(MiniGameBoxPreset("UP",						whenToShow), ETextBoxFunction_ATTACK_DIRECTION_UP_BOX,		TextBoxPositionInfo(ETextBoxPositionAlign_LEFT,		ETextBoxTextAlign_CENTER, maxWidth,		maxHeight / maxLines,	Edges(0, 10, 0,  0)), font, TextBoxSizeInfo(optionText,	optionText, 2),		optionBox), true,		upOptionRowBlock);
	BlockAlignElementsHorizontally* downOptionRowBlock = new BlockAlignElementsHorizontally(maxWidth, ((2 * maxHeight) / 3), ETextBoxPositionAlign_CENTER, EDirection_RIGHT, !fill, !fill, Edges());
	miniGameMenuPage->addBox(new ShapeBox(MiniGameShapeBoxPreset(EShapeBoxClass_CIRCLE,	whenToShow),												TextBoxPositionInfo(ETextBoxPositionAlign_CENTER,	ETextBoxTextAlign_CENTER, bullet,		bullet,					Edges(0, 0,  10, 5)),														styleManager.white),	downOptionRowBlock);
	miniGameMenuPage->addBox(new TextBox(MiniGameBoxPreset("DOWN",						whenToShow), ETextBoxFunction_ATTACK_DIRECTION_DOWN_BOX,	TextBoxPositionInfo(ETextBoxPositionAlign_LEFT,		ETextBoxTextAlign_CENTER, maxWidth,		maxHeight / maxLines,	Edges(0, 0,  0,  0)), font, TextBoxSizeInfo(optionText,	optionText, 2),		optionBox), true,		downOptionRowBlock);
	attackDirectionBlock->mpSubBlocks.push_back(attackDetailsBlock);
	attackDirectionBlock->mpSubBlocks.push_back(leftOptionRowBlock);
	attackDirectionBlock->mpSubBlocks.push_back(rightOptionRowBlock);
	attackDirectionBlock->mpSubBlocks.push_back(upOptionRowBlock);
	attackDirectionBlock->mpSubBlocks.push_back(downOptionRowBlock);
	characterOptionsDetailsBlock->mpSubBlocks.push_back(attackDirectionBlock);

	////CHARACTER
	////SELECT TILE TO ATTACK
	whenToShow = { EMiniGameState_PLAYER_TAKE_ACTION_ATTACK };
	miniGameMenuPage->addBox(new TextBox(MiniGameBoxPreset("Select a tile to Attack",										whenToShow), ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_CENTER, maxWidth, maxHeight / maxLines, Edges(0, 10, 0, 0)), font, TextBoxSizeInfo(headingText,	headingText),	darkPink),	false, characterOptionsHeadingBlock);
	BlockAlignElementsHorizontally* attackTilesBlock = new BlockAlignElementsHorizontally(maxWidth, ((2 * maxHeight) / 3), ETextBoxPositionAlign_LEFT, EDirection_RIGHT, !fill, !fill, Edges(), standardPanelColor);
	miniGameMenuPage->addBox(new ShapeBox(MiniGameShapeBoxPreset(EShapeBoxClass_CIRCLE,										whenToShow),								TextBoxPositionInfo(ETextBoxPositionAlign_CENTER,	ETextBoxTextAlign_CENTER, bullet,		bullet,					Edges(0, 0,  10, 5)),															styleManager.white),	attackTilesBlock);
	miniGameMenuPage->addBox(new TextBox(MiniGamePlayerBoxPreset(ECharacterStatBoxValueToDisplay_CHARACTER_CUR_ATTACK_NAME, whenToShow), ETextBoxFunction_NO_FUNCTION,	TextBoxPositionInfo(ETextBoxPositionAlign_LEFT,		ETextBoxTextAlign_CENTER, maxWidth,		maxHeight / maxLines,	Edges(0, 10, 0,  0)), font, TextBoxSizeInfo(optionText,		optionText),		white),		false,		attackTilesBlock);
	characterOptionsDetailsBlock->mpSubBlocks.push_back(attackTilesBlock);

	curX = 20;
	curY = screen.mGameLevelChunkHeight - 15;
	BlockAlignElementsVertically* debugStatLine = new BlockAlignElementsVertically(Hitbox(CoordsX1Y1WidthHeight(curX, curY, maxWidth, maxHeight)), ETextBoxPositionAlign_LEFT, EDirection_RIGHT, EDirection_UP, !fill, !fill, Edges(), highlightPanelColor);
	miniGameMenuPage->addBox(new TextBox(MiniGameStatBoxPreset(EGameStatBoxValueToDisplay_MINI_GAME_DEBUG_LINE), ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_CENTER, maxWidth, maxHeight / maxLines, Edges()), font, TextBoxSizeInfo(detailText , detailText), darkPink), false, debugStatLine);


	miniGameMenuPage->mpBlocks.push_back(upperRightBlock); //block 1
	miniGameMenuPage->mpBlocks.push_back(debugHUDBlock); //block 2
	miniGameMenuPage->mpBlocks.push_back(characterOptionsBlock); //block 3
	createMiniGameCharacterAttackPanel(menuManager, screen, worldData, styleManager, settingsManager); //block 4
	miniGameMenuPage->mpBlocks.push_back(debugStatLine); //block 5

	miniGameMenuPage = nullptr;
	upperRightBlock = nullptr;
	debugHUDBlock = nullptr;
	characterOptionsBlock = nullptr;
	characterOptionsHeadingBlock = nullptr;
	characterOptionsDetailsBlock = nullptr;

	
}

void createMiniGameCharacterStatsMenu(MenuManager& menuManager, ScreenObject& screen, MiniGameWorldData& worldData, StyleManager& styleManager, SettingsManager& settingsManager)
{
	MenuPage* miniGameMenuPage = menuManager.mpMenuPages[EMenuPageType_MINI_GAME_MENU];

	const char* font = styleManager.fontName;
	TextBoxColorInfo colors = TextBoxColorInfo(styleManager.pink);
	SDL_Color panelColor = styleManager.sunYellow;

	bool fill = true;

	int maxWidth = 1000;
	int maxHeight = 300;
	int curX = 20;
	int curY = 20;
	int spacing = 25;
	int padding = 10;
	BlockAlignElementsVertically* statBlock = new BlockAlignElementsVertically(Hitbox(CoordsX1Y1WidthHeight(curX, curY, maxWidth, maxHeight)), ETextBoxPositionAlign_LEFT, EDirection_RIGHT, EDirection_DOWN, !fill, !fill, Edges(), panelColor);
	if (worldData.mCurLevelNumber != -1)
	{
		BlockAlignElementsHorizontally* levelLine = new BlockAlignElementsHorizontally(maxWidth, maxHeight, ETextBoxPositionAlign_LEFT, EDirection_RIGHT, !fill, !fill, Edges(0, padding, 0, 0), panelColor);
		miniGameMenuPage->addBox(new TextBox(MiniGameStatBoxPreset(EGameStatBoxValueToDisplay_CUR_LEVEL_NUMBER), ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_LEFT, maxWidth, maxHeight / 4, Edges(2, 0, 0, 0)), font, TextBoxSizeInfo(20, 20), colors), false, levelLine);
		BlockAlignElementsHorizontally* characterStatsLine = new BlockAlignElementsHorizontally(Hitbox(CoordsX1Y1WidthHeight(curX, curY, maxWidth, maxHeight)), ETextBoxPositionAlign_LEFT, EDirection_RIGHT, EDirection_DOWN, !fill, !fill, Edges(), panelColor);
		
		CombatManager& combatManager = worldData.getStage()->mCombatManager;
		for (int count = 0; count < combatManager.mpAllCombatCharacters.size(); count++)
		{
			CombatCharacter* pCurChar = combatManager.mpAllCombatCharacters[count];
			maxWidth = 200;
			int curPaddingLeft = (count == 0) ? padding : 0;

			BlockAlignElementsHorizontally* curCharBlock = new BlockAlignElementsHorizontally(maxWidth, maxHeight, ETextBoxPositionAlign_LEFT, EDirection_RIGHT, !fill, !fill, Edges(0, 0, curPaddingLeft, padding + spacing));

			BlockAlignElementsVertically* curIconBlock = new BlockAlignElementsVertically(maxWidth, maxHeight, ETextBoxPositionAlign_LEFT, EDirection_DOWN, !fill, !fill, Edges());
			miniGameMenuPage->addBox(new ImageBox(ImageBoxPreset(), ImageBoxPositionInfo(0, ETextBoxPositionAlign_LEFT, 100, 100, Edges(0, 8, 0, 0)), pCurChar->mIconFileName), curIconBlock);

			
			BlockAlignElementsVertically* curTitlesBlock = new BlockAlignElementsVertically(maxWidth, maxHeight, ETextBoxPositionAlign_LEFT, EDirection_DOWN, !fill, !fill, Edges(0, 0, 0, 5));
			miniGameMenuPage->addBox(new TextBox(MiniGameCharacterBoxPreset(count, true, ECharacterStatBoxValueToDisplay_CHARACTER_NAME),		ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_LEFT, maxWidth, maxHeight / 4, Edges(padding,		8,			0, 0)), font, TextBoxSizeInfo(20, 20), colors), false, curTitlesBlock);
			miniGameMenuPage->addBox(new TextBox(StandardTextBoxPreset("HP: "),																	ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_LEFT, maxWidth, maxHeight / 4, Edges(0,				3,			0, 0)), font, TextBoxSizeInfo(15, 15), colors), false, curTitlesBlock);
			miniGameMenuPage->addBox(new TextBox(StandardTextBoxPreset("DEF: "),																ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_LEFT, maxWidth, maxHeight / 4, Edges(0,				3,			0, 0)), font, TextBoxSizeInfo(15, 15), colors), false, curTitlesBlock);
			miniGameMenuPage->addBox(new TextBox(StandardTextBoxPreset("STUNS: "),																ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_LEFT, maxWidth, maxHeight / 4, Edges(0,				padding,	0, 0)), font, TextBoxSizeInfo(15, 15), colors), false, curTitlesBlock);

			BlockAlignElementsVertically* curNumsBlock = new BlockAlignElementsVertically(maxWidth, maxHeight, ETextBoxPositionAlign_LEFT, EDirection_DOWN, !fill, !fill, Edges(0, 0, 0, 0));
			miniGameMenuPage->addBox(new HealthBox(HealthBoxPreset(count), TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_LEFT, maxWidth - 50, maxHeight / 4, Edges(padding + 32, 3, 0, 0)), font, 15, styleManager.lightYellow, styleManager.pink, styleManager.pink), curNumsBlock);
			miniGameMenuPage->addBox(new TextBox(MiniGameCharacterBoxPreset(count, true, ECharacterStatBoxValueToDisplay_CHARACTER_DEFENSE),	ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_LEFT, maxWidth, maxHeight / 4, Edges(0,				3,			0, 0)), font, TextBoxSizeInfo(15, 15), colors), false, curNumsBlock);
			miniGameMenuPage->addBox(new TextBox(MiniGameCharacterBoxPreset(count, true, ECharacterStatBoxValueToDisplay_CHARACTER_STUN),		ETextBoxFunction_NO_FUNCTION, TextBoxPositionInfo(ETextBoxPositionAlign_LEFT, ETextBoxTextAlign_LEFT, maxWidth, maxHeight / 4, Edges(0,				padding,	0, 0)), font, TextBoxSizeInfo(15, 15), colors), false, curNumsBlock);

			curCharBlock->mpSubBlocks.push_back(curIconBlock);
			curCharBlock->mpSubBlocks.push_back(curTitlesBlock);
			curCharBlock->mpSubBlocks.push_back(curNumsBlock);
			characterStatsLine->mpSubBlocks.push_back(curCharBlock);
			curCharBlock = nullptr;
			pCurChar = nullptr;
		}
		statBlock->mpSubBlocks.push_back(levelLine);
		statBlock->mpSubBlocks.push_back(characterStatsLine);
		levelLine = nullptr;
		characterStatsLine = nullptr;
	}
	

	int blockNum = 0;
	if (miniGameMenuPage->mpBlocks.size() > blockNum && miniGameMenuPage->mpBlocks[blockNum])
	{
		//replace the character stat block
		statBlock->setAllTextures(screen.mpRenderer);
		miniGameMenuPage->deleteBlock(miniGameMenuPage->mpBlocks[blockNum]);
		miniGameMenuPage->mpBlocks[blockNum] = statBlock;
	}
	else
	{
		//first time through, place the block
		miniGameMenuPage->mpBlocks.push_back(statBlock);
	}
	
	statBlock = nullptr;
}

void createMiniGameCharacterAttackPanel(MenuManager& menuManager, ScreenObject& screen, MiniGameWorldData& worldData, StyleManager& styleManager, SettingsManager& settingsManager) 
{
	MenuPage* miniGameMenuPage = menuManager.mpMenuPages[EMenuPageType_MINI_GAME_MENU];
	const char* font = styleManager.fontName;

	TextBoxColorInfo black = TextBoxColorInfo(styleManager.black);
	TextBoxColorInfo darkPink = TextBoxColorInfo(styleManager.pink);
	TextBoxColorInfo white = TextBoxColorInfo(styleManager.white);
	TextBoxColorInfo colors = TextBoxColorInfo(styleManager.pink, styleManager.white, styleManager.sunYellow, styleManager.sunYellow);
	TextBoxColorInfo optionBox = TextBoxColorInfo(styleManager.white, styleManager.sunYellow, styleManager.pink, styleManager.pink, styleManager.white, styleManager.sunYellow);
	SDL_Color highlightPanelColor = styleManager.sunYellow;
	SDL_Color standardPanelColor = styleManager.lightPink;

	bool fill = true;

	int curX = 700;
	int curY = 375;
	int maxWidth = 700;
	int maxHeight = 450;
	int maxLines = 6; // for maxHeight

	// TODO pass in the character options blocks parameters to this method, so that they match
	// since you can't render a background on these panels without overrendering all internal backgrounds
	BlockAlignElementsVertically* characterOptionsBlock			= new BlockAlignElementsVertically(Hitbox(CoordsX1Y1WidthHeight(curX, curY, maxWidth, maxHeight)),	ETextBoxPositionAlign_LEFT,		EDirection_RIGHT, EDirection_DOWN,	fill, fill,	Edges(),	styleManager.clear, "Attack panel - character options block");
	BlockAlignElementsVertically* characterOptionsHeadingBlock	= new BlockAlignElementsVertically(maxWidth, (maxHeight / 3),										ETextBoxPositionAlign_CENTER,	EDirection_DOWN,					fill, fill,	Edges(),	styleManager.clear);
	BlockAlignElementsVertically* characterOptionsDetailsBlock	= new BlockAlignElementsVertically(maxWidth, ((2 * maxHeight) / 3),									ETextBoxPositionAlign_LEFT,		EDirection_DOWN,					fill, fill,	Edges(),	styleManager.clear);

	int headingText = 35;
	int optionText = 30;
	int detailText = 20;
	int bullet = 20;

	maxHeight = ((2 * maxHeight) / 3);

	std::vector <EMiniGameState> whenToShow;
	whenToShow = { EMiniGameState_PLAYER_WAIT_FOR_ATTACK_INPUT };

	if (worldData.mCurLevelNumber != -1)
	{
		CombatManager& combatManager = worldData.getStage()->mCombatManager;
		for (int countCharacter = 0; countCharacter < combatManager.mpAllCombatCharacters.size(); countCharacter++)
		{
			if (combatManager.mpAllCombatCharacters[countCharacter]->mType != EMiniGameCombatCharacterType_PLAYER)
			{
				continue;
			}

			//CHARACTER
			//CHOOSE ATTACK
			//OPTION NAME
			//OPTION TYPE
			//OPTION DAMAGE
			//OPTION SPECIAL EFFECTS / NOTES
			miniGameMenuPage->addBox(new TextBox(MiniGameCharacterBoxPreset(countCharacter, false, ECharacterStatBoxValueToDisplay_CHARACTER_NAME,	whenToShow), ETextBoxFunction_NO_FUNCTION,		TextBoxPositionInfo(ETextBoxPositionAlign_CENTER,	ETextBoxTextAlign_CENTER, maxWidth, maxHeight / maxLines, Edges(50, 10, 0, 0)), font, TextBoxSizeInfo(20,		   20),				black),		false, characterOptionsHeadingBlock);
			miniGameMenuPage->addBox(new TextBox(MiniGameCharacterBoxPreset(countCharacter, false, "Choose Attack",									whenToShow), ETextBoxFunction_NO_FUNCTION,		TextBoxPositionInfo(ETextBoxPositionAlign_LEFT,		ETextBoxTextAlign_CENTER, maxWidth, maxHeight / maxLines, Edges(0,  10, 0, 0)), font, TextBoxSizeInfo(headingText, headingText),	darkPink),	false, characterOptionsHeadingBlock);
			BlockAlignElementsGrid* attackPanelBlock = new BlockAlignElementsGrid(maxWidth, maxHeight / 2, ETextBoxPositionAlign_CENTER, false, 3, fill, fill, Edges(0, 0, 0, 0), styleManager.clear, "Attack panel block");
			for (int countAttack = 0; countAttack < combatManager.mpAllCombatCharacters[countCharacter]->mCombatMovementManager.getAttacks().size(); countAttack++)
			{
				BlockAlignElementsVertically* pShapeBlock		= new BlockAlignElementsVertically(	maxWidth, maxHeight, ETextBoxPositionAlign_LEFT, EDirection_DOWN, !fill, !fill, Edges(12, 0, 0, 0));
				miniGameMenuPage->addBox(new ShapeBox(MiniGameShapeBoxPreset(EShapeBoxClass_CIRCLE,	countCharacter, false,	ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_NAME,										whenToShow),										TextBoxPositionInfo(ETextBoxPositionAlign_CENTER,	ETextBoxTextAlign_CENTER, bullet,	bullet,					Edges(0, 0, 10, 10)),															styleManager.white),	pShapeBlock);
				
				BlockAlignElementsVertically* pAttackTextBlock	= new BlockAlignElementsVertically(maxWidth, maxHeight, ETextBoxPositionAlign_LEFT,	EDirection_DOWN, !fill, !fill, Edges(0, 10, 0, 0));
				miniGameMenuPage->addBox(new TextBox(MiniGameCharacterBoxPreset(					countCharacter, false,	ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_NAME,						countAttack,	whenToShow), ETextBoxFunction_ATTACK_STYLE_BOX,		TextBoxPositionInfo(ETextBoxPositionAlign_LEFT,		ETextBoxTextAlign_CENTER, maxWidth, maxHeight / maxLines,	Edges(0, 10, 0, 0)),  font, TextBoxSizeInfo(optionText,  optionText + 5, 2),	optionBox), true,		pAttackTextBlock);
				miniGameMenuPage->addBox(new TextBox(MiniGameCharacterBoxPreset(					countCharacter, false,	ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_TYPE,						countAttack,	whenToShow), ETextBoxFunction_NO_FUNCTION,			TextBoxPositionInfo(ETextBoxPositionAlign_LEFT,		ETextBoxTextAlign_CENTER, maxWidth, maxHeight / maxLines,	Edges(0,  10, 0, 0)), font, TextBoxSizeInfo(detailText,  detailText),			black),		false,		pAttackTextBlock);
				miniGameMenuPage->addBox(new TextBox(MiniGameCharacterBoxPreset(					countCharacter, false,	ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_DAMAGE,						countAttack,	whenToShow), ETextBoxFunction_NO_FUNCTION,			TextBoxPositionInfo(ETextBoxPositionAlign_LEFT,		ETextBoxTextAlign_CENTER, maxWidth, maxHeight / maxLines,	Edges(0,  10, 0, 0)), font, TextBoxSizeInfo(detailText,  detailText),			black),		false,		pAttackTextBlock);
				miniGameMenuPage->addBox(new TextBox(MiniGameCharacterBoxPreset(					countCharacter, false,	ECharacterStatBoxValueToDisplay_CHARACTER_ATTACK_OPTION_SPECIAL_EFFECTS_AND_NOTES,	countAttack,	whenToShow), ETextBoxFunction_NO_FUNCTION,			TextBoxPositionInfo(ETextBoxPositionAlign_LEFT,		ETextBoxTextAlign_CENTER, maxWidth, maxHeight / maxLines,	Edges(0,  0,  0, 0)), font, TextBoxSizeInfo(detailText,  detailText),			black),		false,		pAttackTextBlock);
				
				attackPanelBlock->mpSubBlocks.push_back(pShapeBlock);
				attackPanelBlock->mpSubBlocks.push_back(pAttackTextBlock);

				//block diagram
				Attack curAttack = combatManager.mpAllCombatCharacters[countCharacter]->mCombatMovementManager.getAttacks()[countAttack];
				createBlockDiagram(curAttack.mType, countCharacter, miniGameMenuPage, attackPanelBlock, whenToShow, menuManager, styleManager);

				pShapeBlock		 = nullptr;
				pAttackTextBlock = nullptr;
			}
			characterOptionsDetailsBlock->mpSubBlocks.push_back(attackPanelBlock);
			attackPanelBlock = nullptr;
			
		}

		characterOptionsBlock->mpSubBlocks.push_back(characterOptionsHeadingBlock);
		characterOptionsBlock->mpSubBlocks.push_back(characterOptionsDetailsBlock);
	}

	int blockNum = 4;
	if (miniGameMenuPage->mpBlocks.size() > blockNum && miniGameMenuPage->mpBlocks[blockNum])
	{
		//replace the character stat block
		characterOptionsBlock->setAllTextures(screen.mpRenderer);
		miniGameMenuPage->deleteBlock(miniGameMenuPage->mpBlocks[blockNum]);
		miniGameMenuPage->mpBlocks[blockNum] = characterOptionsBlock;
	}
	else
	{
		//first time through, place the block
		miniGameMenuPage->mpBlocks.push_back(characterOptionsBlock);
	}

	characterOptionsBlock = nullptr;
	characterOptionsHeadingBlock = nullptr;
	characterOptionsDetailsBlock = nullptr;
}

void createBlockDiagram(EMiniGameCombatMoveAttackTypes diagramType, int characterIndex, MenuPage* pPage, UIBlock* pBlock, std::vector<EMiniGameState> whenToShow, MenuManager& menuManager, StyleManager& styleManager) 
{
	int blockSize = 15;
	int spacing = 5;
	SDL_Color blockColor = styleManager.white;
	bool fill = true;

	int out = 0;
	int num = 0;
	bool square = false;
	bool cross = false;
	bool checkered = false;

	switch (diagramType)
	{
	case EMiniGameCombatMoveAttackTypes_SQUARE1:
		num = 1;
		square = true;
		break;		
	case EMiniGameCombatMoveAttackTypes_SQUARE2:
		num = 2;
		square = true;
		break;
	case EMiniGameCombatMoveAttackTypes_SQUARE2_1UNIT_OUT:
		num = 3;
		out = 1;
		square = true;
		break;
	case EMiniGameCombatMoveAttackTypes_SQUARE3_2UNITS_OUT:
		num = 5;
		out = 2;
		square = true;
		break;
	case EMiniGameCombatMoveAttackTypes_CROSS1:
		num = 1;
		cross = true;
		break;
	case EMiniGameCombatMoveAttackTypes_CROSS1_1UNIT_OUT:
		num = 2;
		cross = true;
		break;
	case EMiniGameCombatMoveAttackTypes_CROSS2:
		num = 2;
		cross = true;
		break;
	case EMiniGameCombatMoveAttackTypes_CROSS2_1UNIT_OUT:
		num = 3;
		cross = true;
		break;
	case EMiniGameCombatMoveAttackTypes_CROSS3_1UNIT_OUT:
		num = 4;
		cross = true;
		break;
	case EMiniGameCombatMoveAttackTypes_CROSS4:
		num = 4;
		cross = true;
		break;
	case EMiniGameCombatMoveAttackTypes_CHECKERBOARD2UNITS:
		num = 2;
		checkered = true;
		break;
	default:
		return;
	}

	int maxWidth = (2 * num + 1) * (blockSize + spacing);
	int maxHeight = maxWidth;
	int leftBlockMargin = 20;
	int maxBlockHeight = pBlock->mMaxHeight;
	//int rowYMargin = (maxBlockHeight - maxHeight) / 2;
	int outSpacing = out * (blockSize + spacing) + spacing;


	BlockAlignElementsVertically* pDiagramBlock = new BlockAlignElementsVertically(200, maxBlockHeight, ETextBoxPositionAlign_LEFT, EDirection_DOWN, fill, fill, Edges(0, 0, leftBlockMargin, 0), styleManager.clear, "block diagram");

	if (square)
	{
		for (int i = 0; i < (2 * (num - out) + 1); i++)
		{
			BlockAlignElementsHorizontally* pCurRow = new BlockAlignElementsHorizontally(maxWidth, maxHeight, ETextBoxPositionAlign_CENTER, EDirection_RIGHT, !fill, !fill, Edges(), styleManager.clear, "block diagram row " + i);
			for (int j = 0; j < (2 * (num - out) + 1); j++)
			{
				if (i == num && j == num)
				{
					//middle dot
					pPage->addBox(new ShapeBox(MiniGameShapeBoxPreset(EShapeBoxClass_CIRCLE, characterIndex, false, whenToShow), TextBoxPositionInfo(ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, blockSize, blockSize, Edges(outSpacing, outSpacing, outSpacing, outSpacing)), blockColor), pCurRow);
				}
				else 
				{
					//block 
					pPage->addBox(new ShapeBox(MiniGameShapeBoxPreset(EShapeBoxClass_RECT, characterIndex, false, whenToShow), TextBoxPositionInfo(ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, blockSize, blockSize, Edges(spacing, spacing, spacing, spacing)), blockColor), pCurRow);
				}


			}
			pDiagramBlock->mpSubBlocks.push_back(pCurRow);
		}
	}
	else if (cross)
	{
		for (int i = 0; i < (2 * (num - out) + 1); i++)
		{
			
			BlockAlignElementsHorizontally* pCurRow = new BlockAlignElementsHorizontally(maxWidth, maxHeight, ETextBoxPositionAlign_CENTER, EDirection_RIGHT, !fill, !fill, Edges(), styleManager.clear, ("block diagram row " + std::to_string(i)));
			if (i == num)
			{
				for (int j = 0; j < (2 * (num - out) + 1); j++)
				{
					if (j == num) 
					{
						//middle dot
						pPage->addBox(new ShapeBox(MiniGameShapeBoxPreset(EShapeBoxClass_CIRCLE, characterIndex, false, whenToShow), TextBoxPositionInfo(ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, blockSize, blockSize, Edges(outSpacing, outSpacing, outSpacing, outSpacing)), blockColor), pCurRow);

					}
					else 
					{
						//left / right blocks
						pPage->addBox(new ShapeBox(MiniGameShapeBoxPreset(EShapeBoxClass_RECT, characterIndex, false, whenToShow), TextBoxPositionInfo(ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, blockSize, blockSize, Edges(spacing, spacing, spacing, spacing)), blockColor), pCurRow);
					}
					
				}
			}
			else
			{
				//top / bottom blocks
				int curSpacingHoriz = (blockSize + 2 * spacing) * num + spacing;
				pPage->addBox(new ShapeBox(MiniGameShapeBoxPreset(EShapeBoxClass_RECT, characterIndex, false, whenToShow), TextBoxPositionInfo(ETextBoxPositionAlign_CENTER, ETextBoxTextAlign_CENTER, blockSize, blockSize, Edges(spacing, spacing, curSpacingHoriz, curSpacingHoriz)), blockColor), pCurRow);
			}
			pDiagramBlock->mpSubBlocks.push_back(pCurRow);
		}
	}
	
	pBlock->mpSubBlocks.push_back(pDiagramBlock);
}

void setUpFontSizeChart(FontSizeChart& fontSizeChart, StyleManager& styleManager, SDL_Renderer* pRenderer)  { fontSizeChart.createFontChart(styleManager.fontName, pRenderer); }
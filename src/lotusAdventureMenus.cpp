#include "lotusAdventureMenus.h"

int characterStatsBlockNum;
int characterOptionsBlockNum;
int characterOptionsDetailsBlockNum;
int attackPanelBlockNum;
int characterSelectionPanelBlockNum;

void createMenus(MenuManager& menuManager, const ScreenObject& screen, MiniGameWorldData& worldData)
{
	// MAIN MENU PAGE
	createMainMenu(menuManager, screen);

	// MAIN GAME MENU PAGE
	createMainGameMenu(menuManager, screen);


	// MINI GAME MENU PAGE
	createMiniGameMenu(menuManager, screen, worldData);

	menuManager.mpCurMenuPage = menuManager.mpMenuPages[EMenuPageType_MAIN_GAME_MENU];

	menuManager.setAllTextBoxTextures();
	menuManager.setUpBlocks();
}

void createMainMenu(MenuManager& menuManager, const ScreenObject& screen)
{
	const char* fontFileName = StyleManager::fontName;
	SDL_Color white		= StyleManager::white;
	SDL_Color teal		= StyleManager::teal;
	SDL_Color hintBlue	= StyleManager::hintBlue;
	SDL_Color clear		= StyleManager::clear;

	const int levelChunkWidth = screen.mGameLevelChunkWidth;
	const int levelChunkHeight = screen.mGameLevelChunkHeight;
	const bool fill = true;

	//MAIN MENU PAGE
	MenuPage* mainMenuPage = new MenuPage();

	mainMenuPage->addBox(new ImageBox(ImageBoxPreset(), UIPositionInfo(0, EUIPositionAlign_LEFT, EUIPositionAlign_BOTTOM, Hitbox(CoordsX1Y1WidthHeight(0, levelChunkHeight - 600, 600, 600)), Edges()), "Menu/TitleScreenModel.bmp"));


	UIBlock* titleText = new UIBlock(Hitbox(CoordsX1Y1WidthHeight(0, 0, levelChunkWidth, 300)), EUIPositionAlign_CENTER, EUIPositionAlign_TOP, EDirection_RIGHT, EDirection_DOWN, false, 1, fill, !fill, Edges(100, 0, 0, 0), 0, clear, "title text block");
	mainMenuPage->addBox(new ImageBox(ImageBoxPreset(),																	UIPositionInfo(0,	EUIPositionAlign_CENTER, EUIPositionAlign_TOP, levelChunkWidth, 200, Edges()),				"Menu/TitleScreen.bmp"), titleText);
	mainMenuPage->addBox(new TextBox(StandardTextBoxPreset("PLAY GAME"),		ETextBoxFunction_PLAY_GAME_BOX,			UIPositionInfo(		EUIPositionAlign_CENTER, EUIPositionAlign_TOP, levelChunkWidth, 140, Edges()),				fontFileName, TextBoxSizeInfo(60, 80), TextBoxColorInfo(hintBlue, white, teal, teal, clear, clear)), true, titleText);
	mainMenuPage->addBox(new TextBox(StandardTextBoxPreset("PLAY MINI GAME"),	ETextBoxFunction_PLAY_MINI_GAME_BOX,	UIPositionInfo(		EUIPositionAlign_CENTER, EUIPositionAlign_TOP, levelChunkWidth, 140, Edges(8, 0, 0, 0)),	fontFileName, TextBoxSizeInfo(60, 80), TextBoxColorInfo(hintBlue, white, teal, teal, clear, clear)), true, titleText);
	mainMenuPage->mpElems.push_back(titleText);
	titleText = nullptr;

	//debug HUD
	int maxWidth = 200;
	int maxHeight = 100;
	mainMenuPage->addBox(new TextBox(GameStatBoxPreset(EUIBoxValueToDisplay_CUR_KEYBOARD), ETextBoxFunction_NO_FUNCTION, UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_TOP, Hitbox(CoordsX1Y1WidthHeight(levelChunkWidth - maxWidth, 0, maxWidth, maxHeight)), Edges(3, 0, 0, 0)), fontFileName, TextBoxSizeInfo(12, 12), TextBoxColorInfo(hintBlue)), false);

	menuManager.mpMenuPages.push_back(mainMenuPage);
	mainMenuPage = nullptr;
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
	mainGameMenuPage->addBox(new ImageBox(DontAutoShowImageBoxPreset(EUIBoxType_TAKE_DAMAGE_SCREEN), UIPositionInfo(0, EUIPositionAlign_LEFT, EUIPositionAlign_TOP, Hitbox(CoordsX1Y1WidthHeight(0, 0, levelChunkWidth, levelChunkHeight)), Edges()), "UIElements/takeDamageScreen.bmp"));

	//UPPER HUD
	int maxWidth = 400;
	int maxHeight = 100;
	UIBlock* upperHUDBlock	= new UIBlock(Hitbox(CoordsX1Y1WidthHeight(0, 0, maxWidth, 450)),	EUIPositionAlign_LEFT, EUIPositionAlign_TOP, EDirection_RIGHT, EDirection_DOWN, true, 2, !fill, !fill, Edges(10, 0, 10, 0),	0,	clear, "main game upper hud block");
	UIBlock* uLine1			= new UIBlock(maxWidth, maxHeight,									EUIPositionAlign_LEFT, EUIPositionAlign_TOP, EDirection_RIGHT, EDirection_DOWN, true, 1, !fill, !fill, Edges(),				0,	clear, "main game upper hud block line 1");
	mainGameMenuPage->addBox(new ImageBox(ImageBoxPreset(), UIPositionInfo(0, EUIPositionAlign_LEFT, EUIPositionAlign_CENTER, 80, 80, Edges()), "UIElements/HUDUI.bmp"), uLine1);
	mainGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EUIBoxValueToDisplay_CUR_LEVEL_NUMBER), ETextBoxFunction_NO_FUNCTION, UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_CENTER, maxWidth, maxHeight, Edges()), fontFileName, TextBoxSizeInfo(16), TextBoxColorInfo(hintBlue)), false, uLine1);

	UIBlock* uLine2			= new UIBlock(maxWidth, maxHeight, EUIPositionAlign_LEFT, EUIPositionAlign_TOP, EDirection_RIGHT, EDirection_DOWN, true, 6, !fill, !fill, Edges(6, 0, 1, 0),		3,	clear, "main game stats");
	mainGameMenuPage->addBox(new ImageBox(ImageBoxPreset(), UIPositionInfo(0,																			EUIPositionAlign_CENTER, EUIPositionAlign_CENTER,	20,			20,			Edges()), "UIElements/keyUI.bmp"),													uLine2);
	mainGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EUIBoxValueToDisplay_CUR_KEYS),				ETextBoxFunction_NO_FUNCTION, UIPositionInfo(	EUIPositionAlign_CENTER, EUIPositionAlign_CENTER,	maxWidth,	maxHeight,	Edges()), fontFileName, TextBoxSizeInfo(12), TextBoxColorInfo(hintBlue)), false,	uLine2);
	mainGameMenuPage->addBox(new ImageBox(ImageBoxPreset(), UIPositionInfo(0,																			EUIPositionAlign_CENTER, EUIPositionAlign_CENTER,	20,			20,			Edges()), "UIElements/targetUI.bmp"),												uLine2);
	mainGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EUIBoxValueToDisplay_CUR_TARGETS),			ETextBoxFunction_NO_FUNCTION, UIPositionInfo(	EUIPositionAlign_CENTER, EUIPositionAlign_CENTER,	maxWidth,	maxHeight,	Edges()), fontFileName, TextBoxSizeInfo(12), TextBoxColorInfo(hintBlue)), false,	uLine2);
	mainGameMenuPage->addBox(new ImageBox(ImageBoxPreset(), UIPositionInfo(0,																			EUIPositionAlign_CENTER, EUIPositionAlign_CENTER,	20,			20,			Edges()), "UIElements/collectibleUI.bmp"),											uLine2);
	mainGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EUIBoxValueToDisplay_CUR_COLLECTIBLES),		ETextBoxFunction_NO_FUNCTION, UIPositionInfo(	EUIPositionAlign_CENTER, EUIPositionAlign_CENTER,	maxWidth,	maxHeight,	Edges()), fontFileName, TextBoxSizeInfo(12), TextBoxColorInfo(hintBlue)), false,	uLine2);
	mainGameMenuPage->addBox(new ImageBox(ImageBoxPreset(), UIPositionInfo(0,																			EUIPositionAlign_CENTER, EUIPositionAlign_CENTER,	20,			20,			Edges()), "UIElements/enemiesUI.bmp"),												uLine2);
	mainGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EUIBoxValueToDisplay_CUR_ENEMIES_LEFT),		ETextBoxFunction_NO_FUNCTION, UIPositionInfo(	EUIPositionAlign_CENTER, EUIPositionAlign_CENTER,	maxWidth,	maxHeight,	Edges()), fontFileName, TextBoxSizeInfo(12), TextBoxColorInfo(hintBlue)), false,	uLine2);

	upperHUDBlock->mpSubElems.push_back(uLine1);
	upperHUDBlock->mpSubElems.push_back(uLine2);
	mainGameMenuPage->mpElems.push_back(upperHUDBlock);
	uLine1 = nullptr;
	uLine2 = nullptr;
	upperHUDBlock = nullptr;

	//debug HUD
	maxWidth = 200;
	maxHeight = 100;
	mainGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EUIBoxValueToDisplay_CUR_KEYBOARD), ETextBoxFunction_NO_FUNCTION, UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_TOP, Hitbox(CoordsX1Y1WidthHeight(levelChunkWidth - maxWidth, 0, maxWidth, maxHeight)), Edges(10, 0, 0, 0)), fontFileName, TextBoxSizeInfo(12), TextBoxColorInfo(hintBlue)), false);

	int leftTextSize = 15;
	//LOWER LEFT HUD
	UIBlock* lowerLeftHUDBlock = new UIBlock(Hitbox(CoordsX1Y1WidthHeight(0, levelChunkHeight, maxWidth, 450)), EUIPositionAlign_LEFT, EUIPositionAlign_TOP, EDirection_RIGHT, EDirection_UP, false, 2, !fill, !fill, Edges(0, 10, 4, 0), 10, clear, "lower left block");
	// slash
	mainGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EUIBoxValueToDisplay_SLASH_KEY), ETextBoxFunction_NO_FUNCTION,	UIPositionInfo(		EUIPositionAlign_CENTER,	EUIPositionAlign_CENTER,	100, 60, Edges()), fontFileName, TextBoxSizeInfo(leftTextSize), TextBoxColorInfo(hintBlue)), false, lowerLeftHUDBlock);
	mainGameMenuPage->addBox(new ImageBox(DontAutoShowImageBoxPreset(EUIBoxType_SLASH_UI),									UIPositionInfo(0,	EUIPositionAlign_LEFT,		EUIPositionAlign_CENTER,	40, 25,  Edges(0, 0, 10, 0)), "UIElements/slashUI.bmp"),		lowerLeftHUDBlock);

	// double jump
	mainGameMenuPage->addBox(new ShapeBox(ShapeBoxPreset(EShapeBoxClass_RECT),												UIPositionInfo(		EUIPositionAlign_LEFT,		EUIPositionAlign_TOP,		10, 10,  Edges(0, 0, 0,  0)), clear),							lowerLeftHUDBlock);
	mainGameMenuPage->addBox(new ImageBox(DontAutoShowImageBoxPreset(EUIBoxType_DOUBLE_JUMP_UI),							UIPositionInfo(0,	EUIPositionAlign_LEFT,		EUIPositionAlign_CENTER,	40, 30,  Edges(5, 0, 10,  0)),  "UIElements/doubleJumpUI.bmp"),	lowerLeftHUDBlock);
	
	// projectiles
	UIBlock* projectileKeys = new UIBlock(maxWidth, maxHeight, EUIPositionAlign_CENTER, EUIPositionAlign_CENTER, EDirection_RIGHT, EDirection_DOWN, true, 2, !fill, !fill, Edges(0, 0, 0, 0), 10, clear, "projectile keys block");
	mainGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EUIBoxValueToDisplay_PROJECTILE_HORIZONTAL_KEY),		ETextBoxFunction_NO_FUNCTION, UIPositionInfo(EUIPositionAlign_CENTER, EUIPositionAlign_TOP, 100, 60, Edges()), fontFileName, TextBoxSizeInfo(leftTextSize), TextBoxColorInfo(hintBlue)), false, projectileKeys);
	mainGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EUIBoxValueToDisplay_PROJECTILE_VERTICAL_KEY),		ETextBoxFunction_NO_FUNCTION, UIPositionInfo(EUIPositionAlign_CENTER, EUIPositionAlign_TOP, 100, 60, Edges()), fontFileName, TextBoxSizeInfo(leftTextSize), TextBoxColorInfo(hintBlue)), false, projectileKeys);
	lowerLeftHUDBlock->mpSubElems.push_back(projectileKeys);
	projectileKeys = nullptr;

	UIBlock* projectileIcons = new UIBlock(maxWidth, maxHeight, EUIPositionAlign_LEFT, EUIPositionAlign_CENTER, EDirection_RIGHT, EDirection_DOWN, true, 1, !fill, !fill, Edges(0, 0, 20, 0), 22, clear, "projectile icons block");
	mainGameMenuPage->addBox(new ImageBox(DontAutoShowImageBoxPreset(EUIBoxType_PROJECTILE_UI), UIPositionInfo(30, EUIPositionAlign_CENTER, EUIPositionAlign_CENTER, 80, 90, Edges()), "UIElements/swordUI.bmp"), projectileIcons);
	mainGameMenuPage->addBox(new ImageBox(DontAutoShowImageBoxPreset(EUIBoxType_PROJECTILE_UI), UIPositionInfo(30, EUIPositionAlign_CENTER, EUIPositionAlign_CENTER, 80, 90, Edges()), "UIElements/swordUI.bmp"), projectileIcons);
	mainGameMenuPage->addBox(new ImageBox(DontAutoShowImageBoxPreset(EUIBoxType_PROJECTILE_UI), UIPositionInfo(30, EUIPositionAlign_CENTER, EUIPositionAlign_CENTER, 80, 90, Edges()), "UIElements/swordUI.bmp"), projectileIcons);
	mainGameMenuPage->addBox(new ImageBox(DontAutoShowImageBoxPreset(EUIBoxType_PROJECTILE_UI), UIPositionInfo(30, EUIPositionAlign_CENTER, EUIPositionAlign_CENTER, 80, 90, Edges()), "UIElements/swordUI.bmp"), projectileIcons);
	mainGameMenuPage->addBox(new ImageBox(DontAutoShowImageBoxPreset(EUIBoxType_PROJECTILE_UI), UIPositionInfo(30, EUIPositionAlign_CENTER, EUIPositionAlign_CENTER, 80, 90, Edges()), "UIElements/swordUI.bmp"), projectileIcons);
	mainGameMenuPage->addBox(new ImageBox(DontAutoShowImageBoxPreset(EUIBoxType_PROJECTILE_UI), UIPositionInfo(30, EUIPositionAlign_CENTER, EUIPositionAlign_CENTER, 80, 90, Edges()), "UIElements/swordUI.bmp"), projectileIcons);
	mainGameMenuPage->addBox(new ImageBox(DontAutoShowImageBoxPreset(EUIBoxType_PROJECTILE_UI), UIPositionInfo(30, EUIPositionAlign_CENTER, EUIPositionAlign_CENTER, 80, 90, Edges()), "UIElements/swordUI.bmp"), projectileIcons);
	lowerLeftHUDBlock->mpSubElems.push_back(projectileIcons);
	projectileIcons = nullptr;

	mainGameMenuPage->mpElems.push_back(lowerLeftHUDBlock);
	lowerLeftHUDBlock = nullptr;

	//LOWER RIGHT HUD
	UIBlock* lowerRightHUDBlock = new UIBlock(Hitbox(CoordsX1Y1WidthHeight(levelChunkWidth, levelChunkHeight, maxWidth, 450)), EUIPositionAlign_LEFT, EUIPositionAlign_TOP, EDirection_LEFT, EDirection_UP, true, 2, !fill, !fill, Edges(0, 10, 0, 10), 5, clear, "lower right block");

	int rightTextSize = 12;
	int iconSize = 20;
	
	//Movement arrow keys icons
	UIBlock* movementArrowIcons = new UIBlock(80, 80, EUIPositionAlign_CENTER, EUIPositionAlign_TOP, EDirection_RIGHT, EDirection_DOWN, true, 1, !fill, !fill, Edges(), 2, clear, "movement arrow icons");
	mainGameMenuPage->addBox(new ImageBox(ImageBoxPreset(), UIPositionInfo(0, EUIPositionAlign_LEFT, EUIPositionAlign_BOTTOM, iconSize, iconSize, Edges(0, 0,  0, 0)),	"UIElements/leftUI.bmp"),		movementArrowIcons);
	mainGameMenuPage->addBox(new ImageBox(ImageBoxPreset(), UIPositionInfo(0, EUIPositionAlign_LEFT, EUIPositionAlign_BOTTOM, iconSize, iconSize, Edges(0, 22, 0, 0)),	"UIElements/upUI.bmp"),			movementArrowIcons);
	mainGameMenuPage->addBox(new ImageBox(ImageBoxPreset(), UIPositionInfo(0, EUIPositionAlign_LEFT, EUIPositionAlign_BOTTOM, iconSize, iconSize, Edges(0, 0,  0, 0)),	"UIElements/rightUI.bmp"),		movementArrowIcons);
	lowerRightHUDBlock->mpSubElems.push_back(movementArrowIcons);
	movementArrowIcons = nullptr;

	// reset level icon
	mainGameMenuPage->addBox(new ImageBox(ImageBoxPreset(), UIPositionInfo(0, EUIPositionAlign_CENTER, EUIPositionAlign_BOTTOM, iconSize, iconSize, Edges()),				"UIElements/resetLevelUI.bmp"), lowerRightHUDBlock);
	
	// reset checkpoint icon
	mainGameMenuPage->addBox(new ImageBox(ImageBoxPreset(), UIPositionInfo(0, EUIPositionAlign_CENTER, EUIPositionAlign_BOTTOM, iconSize, iconSize, Edges()),				"UIElements/resetCheckpointUI.bmp"), lowerRightHUDBlock);

	
	// movement arrow keys text
	UIBlock* movementArrowTextGrid = new UIBlock(maxWidth, maxHeight, EUIPositionAlign_CENTER, EUIPositionAlign_TOP, EDirection_RIGHT, EDirection_DOWN, false, 1, !fill, !fill, Edges(3, 0, 0, 0), 0, clear, "Movement Keys Block");
	mainGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EUIBoxValueToDisplay_MOVEMENT_LEFT_KEY),		ETextBoxFunction_NO_FUNCTION, UIPositionInfo(EUIPositionAlign_CENTER, EUIPositionAlign_TOP, maxWidth, 60, Edges()),			fontFileName, TextBoxSizeInfo(rightTextSize), TextBoxColorInfo(hintBlue)), false, movementArrowTextGrid);
	mainGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EUIBoxValueToDisplay_MOVEMENT_UP_KEY),		ETextBoxFunction_NO_FUNCTION, UIPositionInfo(EUIPositionAlign_CENTER, EUIPositionAlign_TOP, maxWidth, 60, Edges()),			fontFileName, TextBoxSizeInfo(rightTextSize), TextBoxColorInfo(hintBlue)), false, movementArrowTextGrid);
	mainGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EUIBoxValueToDisplay_MOVEMENT_RIGHT_KEY),	ETextBoxFunction_NO_FUNCTION, UIPositionInfo(EUIPositionAlign_CENTER, EUIPositionAlign_TOP, maxWidth, 60, Edges()),			fontFileName, TextBoxSizeInfo(rightTextSize), TextBoxColorInfo(hintBlue)), false, movementArrowTextGrid);
	lowerRightHUDBlock->mpSubElems.push_back(movementArrowTextGrid);
	movementArrowTextGrid = nullptr;

	// reset level text
	mainGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EUIBoxValueToDisplay_LEVEL_RESET_KEY),		ETextBoxFunction_NO_FUNCTION, UIPositionInfo(EUIPositionAlign_CENTER, EUIPositionAlign_TOP, 100, 60, Edges(3, 0, 0, 0)), fontFileName, TextBoxSizeInfo(rightTextSize), TextBoxColorInfo(hintBlue)), false, lowerRightHUDBlock);

	//reset checkpoint text
	mainGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EUIBoxValueToDisplay_CHECKPOINT_RESET_KEY),	ETextBoxFunction_NO_FUNCTION, UIPositionInfo(EUIPositionAlign_CENTER, EUIPositionAlign_TOP, 100, 60, Edges(3, 0, 0, 0)), fontFileName, TextBoxSizeInfo(rightTextSize), TextBoxColorInfo(hintBlue)), false, lowerRightHUDBlock);

	mainGameMenuPage->mpElems.push_back(lowerRightHUDBlock);
	lowerRightHUDBlock = nullptr;

	menuManager.mpMenuPages.push_back(mainGameMenuPage);
	mainGameMenuPage = nullptr;
}

void createMiniGameMenu(MenuManager& menuManager, const ScreenObject& screen,  MiniGameWorldData& worldData)
{
	const char* font = StyleManager::fontName;

	MenuPage* miniGameMenuPage = new MenuPage();
	menuManager.mpMenuPages.push_back(miniGameMenuPage);

	characterStatsBlockNum = (int)miniGameMenuPage->mpElems.size();
	createMiniGameCharacterStatsMenu(menuManager, screen, worldData);

	const TextBoxColorInfo black			= TextBoxColorInfo(StyleManager::black);
	const TextBoxColorInfo darkPink			= TextBoxColorInfo(StyleManager::pink);
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
	miniGameMenuPage->addBox(new TextBox(GameStatBoxPreset(EUIBoxValueToDisplay_CUR_KEYBOARD), ETextBoxFunction_NO_FUNCTION, UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_TOP, Hitbox(CoordsX1Y1WidthHeight(1200, 0, 200, 50)), Edges(3, 0, 0, 0)), font, TextBoxSizeInfo(12), white), false);

	const int& panelWidth			= StyleManager::panelWidth;
	const int& panelHeight			= StyleManager::panelHeight;
	const int& panelHeadingHeight	= StyleManager::panelHeadingHeight;
	const int& panelBodyHeight		= StyleManager::panelBodyHeight;

	std::vector <EMiniGameState> whenToShow;
	UIBlock* characterOptionsBlock			= new UIBlock(Hitbox(CoordsX1Y1WidthHeight(700, 375, panelWidth, panelHeight)),		 EUIPositionAlign_LEFT,		EUIPositionAlign_TOP, EDirection_RIGHT, EDirection_DOWN, false, 1, fill, fill,	Edges(), 0, standardPanelColor,  "character panel box");
	UIBlock* characterOptionsHeadingBlock	= new UIBlock(										 panelWidth, panelHeadingHeight, EUIPositionAlign_CENTER,	EUIPositionAlign_TOP, EDirection_RIGHT, EDirection_DOWN, false, 1, fill, fill,	Edges(), 0,	highlightPanelColor, "character panel heading box");
	UIBlock* characterOptionsDetailsBlock	= new UIBlock(										 panelWidth, panelBodyHeight,	 EUIPositionAlign_LEFT,		EUIPositionAlign_TOP, EDirection_RIGHT, EDirection_DOWN, false, 1, fill, fill,	Edges(), 0,	standardPanelColor,  "character panel details box");
	characterOptionsBlock->mpSubElems.push_back(characterOptionsHeadingBlock);
	characterOptionsBlock->mpSubElems.push_back(characterOptionsDetailsBlock);
	miniGameMenuPage->mpElems.push_back(characterOptionsBlock);
	characterOptionsBlockNum = (int) miniGameMenuPage->mpElems.size() - 1;
	characterOptionsDetailsBlockNum = (int) characterOptionsBlock->mpSubElems.size() - 1;

	whenToShow = { EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT, EMiniGameState_PLAYER_WAIT_FOR_ACTION_INPUT, EMiniGameState_PLAYER_WAIT_FOR_ATTACK_OPTION_INPUT, EMiniGameState_PLAYER_WAIT_FOR_ATTACK_DIRECTION_INPUT, EMiniGameState_PLAYER_WAIT_FOR_ATTACK_TILE_INPUT, EMiniGameState_PLAYER_WAIT_FOR_ATTACK_CHARACTER_INPUT };
	miniGameMenuPage->addBox(new TextBox(MiniGamePlayerBoxPreset(EUIBoxValueToDisplay_CHARACTER_NAME, whenToShow), ETextBoxFunction_NO_FUNCTION, UIPositionInfo(EUIPositionAlign_CENTER, EUIPositionAlign_TOP, panelWidth, panelHeadingHeight / 2, Edges(18, 8, 0, 0)), font, TextBoxSizeInfo(20), black), false, characterOptionsHeadingBlock);

	

	// SELECT TILE TO MOVE TO
	whenToShow = { EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT };
	miniGameMenuPage->addBox(new TextBox(MiniGameBoxPreset("Select a Tile to Move To",	whenToShow), ETextBoxFunction_NO_FUNCTION, UIPositionInfo(EUIPositionAlign_CENTER, EUIPositionAlign_TOP, panelWidth, panelHeadingHeight / 2, Edges(0, 10, 0, 0)), font, TextBoxSizeInfo(headingText), darkPink), false, characterOptionsHeadingBlock);
	
	UIBlock* moveRowBlock = new UIBlock(panelWidth, panelBodyHeight, EUIPositionAlign_LEFT, EUIPositionAlign_TOP, EDirection_RIGHT, EDirection_DOWN, true, 1, !fill, !fill, Edges(10, 0, 0, 0), 0, standardPanelColor, "movement block");
	miniGameMenuPage->addBox(new ShapeBox(MiniGameCharacterShapeBoxPreset(EShapeBoxClass_CIRCLE,						whenToShow),								UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_CENTER,	bullet,		bullet,				Edges(0, 0,  10, 5)),										white),			moveRowBlock);
	miniGameMenuPage->addBox(new TextBox(MiniGameBoxPreset("Move Style:",												whenToShow), ETextBoxFunction_NO_FUNCTION,	UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_CENTER,	panelWidth,	panelBodyHeight,	Edges(0, 0,  0,  0)), font, TextBoxSizeInfo(optionText),	white),	false,	moveRowBlock);
	miniGameMenuPage->addBox(new TextBox(MiniGamePlayerBoxPreset(EUIBoxValueToDisplay_CHARACTER_MOVE_TYPE,	whenToShow), ETextBoxFunction_NO_FUNCTION,	UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_CENTER,	panelWidth,	panelBodyHeight,	Edges(0, 0,  0,  5)), font, TextBoxSizeInfo(optionText),	white),	false,	moveRowBlock);
	characterOptionsDetailsBlock->mpSubElems.push_back(moveRowBlock);
	moveRowBlock = nullptr;

	// CHOOSE ACTION
	whenToShow = { EMiniGameState_PLAYER_WAIT_FOR_ACTION_INPUT };
	miniGameMenuPage->addBox(new TextBox(MiniGameBoxPreset("Select an Action",			whenToShow), ETextBoxFunction_NO_FUNCTION, UIPositionInfo(EUIPositionAlign_CENTER, EUIPositionAlign_TOP, panelWidth, panelHeadingHeight / 2, Edges(0, 10, 0, 0)), font, TextBoxSizeInfo(headingText), darkPink),	false, characterOptionsHeadingBlock);
	
	UIBlock* actionsBlock = new UIBlock(panelWidth, panelBodyHeight, EUIPositionAlign_LEFT, EUIPositionAlign_TOP, EDirection_RIGHT, EDirection_DOWN, false, 2, !fill, !fill, Edges(10, 0, 10, 0), 7, clear, "actions block");
	miniGameMenuPage->addBox(new ShapeBox(MiniGameCharacterShapeBoxPreset(EShapeBoxClass_CIRCLE,	whenToShow),														UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_CENTER, bullet,		bullet,			 Edges()), white), actionsBlock);
	miniGameMenuPage->addBox(new TextBox(MiniGameBoxPreset("ATTACK",								whenToShow), ETextBoxFunction_ATTACK_CUR_COMBAT_CHARACTER_BOX,		UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_CENTER, panelWidth,	panelBodyHeight, Edges()), font, TextBoxSizeInfo(optionText, optionText + 5, 2), optionBox), true, actionsBlock);
	miniGameMenuPage->addBox(new ShapeBox(MiniGameCharacterShapeBoxPreset(EShapeBoxClass_CIRCLE,	whenToShow),														UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_CENTER, bullet,		bullet,			 Edges()), white), actionsBlock);
	miniGameMenuPage->addBox(new TextBox(MiniGameBoxPreset("SUPPORT",								whenToShow), ETextBoxFunction_SUPPORT_CUR_COMBAT_CHARACTER_BOX,		UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_CENTER, panelWidth,	panelBodyHeight, Edges()), font, TextBoxSizeInfo(optionText, optionText + 5, 2), optionBox), true, actionsBlock);
	miniGameMenuPage->addBox(new ShapeBox(MiniGameCharacterShapeBoxPreset(EShapeBoxClass_CIRCLE,	whenToShow),														UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_CENTER, bullet,		bullet,			 Edges()), white), actionsBlock);
	miniGameMenuPage->addBox(new TextBox(MiniGameBoxPreset("DEFEND",								whenToShow), ETextBoxFunction_DEFEND_CUR_COMBAT_CHARACTER_BOX,		UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_CENTER, panelWidth,	panelBodyHeight, Edges()), font, TextBoxSizeInfo(optionText, optionText + 5, 2), optionBox), true, actionsBlock);
	miniGameMenuPage->addBox(new ShapeBox(MiniGameCharacterShapeBoxPreset(EShapeBoxClass_CIRCLE,	whenToShow),														UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_CENTER, bullet,		bullet,			 Edges()), white), actionsBlock);
	miniGameMenuPage->addBox(new TextBox(MiniGameBoxPreset("HEAL",									whenToShow), ETextBoxFunction_HEAL_CUR_COMBAT_CHARACTER_BOX,		UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_CENTER, panelWidth,	panelBodyHeight, Edges()), font, TextBoxSizeInfo(optionText, optionText + 5, 2), optionBox), true, actionsBlock);
	miniGameMenuPage->addBox(new ShapeBox(MiniGameCharacterShapeBoxPreset(EShapeBoxClass_CIRCLE,	whenToShow),														UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_CENTER, bullet,		bullet,			 Edges()), white), actionsBlock);
	miniGameMenuPage->addBox(new TextBox(MiniGameBoxPreset("PASS TURN",								whenToShow), ETextBoxFunction_PASS_CUR_COMBAT_CHARACTER_TURN_BOX,	UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_CENTER, panelWidth,	panelBodyHeight, Edges()), font, TextBoxSizeInfo(optionText, optionText + 5, 2), optionBox), true, actionsBlock);
	characterOptionsDetailsBlock->mpSubElems.push_back(actionsBlock);
	actionsBlock = nullptr;

	// CHOOSE ATTACK
	whenToShow = { EMiniGameState_PLAYER_WAIT_FOR_ATTACK_OPTION_INPUT };
	miniGameMenuPage->addBox(new TextBox(MiniGameBoxPreset("Choose an Attack",			whenToShow), ETextBoxFunction_NO_FUNCTION, UIPositionInfo(EUIPositionAlign_CENTER, EUIPositionAlign_TOP, panelWidth, panelHeadingHeight / 2, Edges(0, 10, 0, 0)), font, TextBoxSizeInfo(headingText), darkPink), false, characterOptionsHeadingBlock);
	attackPanelBlockNum = (int)characterOptionsDetailsBlock->mpSubElems.size();
	createMiniGameCharacterAttackPanel(menuManager, screen, worldData); 

	// CHOOSE ATTACK DIRECTION
	whenToShow = { EMiniGameState_PLAYER_WAIT_FOR_ATTACK_DIRECTION_INPUT };
	miniGameMenuPage->addBox(new TextBox(MiniGameBoxPreset("Choose Attack Direction",						whenToShow), ETextBoxFunction_NO_FUNCTION, UIPositionInfo(EUIPositionAlign_CENTER, EUIPositionAlign_TOP, panelWidth, panelHeadingHeight / 4,	Edges()),			font, TextBoxSizeInfo(headingText - 7),	darkPink),	false, characterOptionsHeadingBlock);
	miniGameMenuPage->addBox(new TextBox(MiniGameBoxPreset("All tiles in the direction will be attacked",	whenToShow), ETextBoxFunction_NO_FUNCTION, UIPositionInfo(EUIPositionAlign_CENTER, EUIPositionAlign_TOP, panelWidth, panelHeadingHeight / 4,	Edges(0, 8, 0, 0)), font, TextBoxSizeInfo(detailText - 7),	black),		false, characterOptionsHeadingBlock);

	UIBlock* attackDirectionBlock	= new UIBlock(panelWidth, panelBodyHeight, EUIPositionAlign_LEFT, EUIPositionAlign_TOP, EDirection_RIGHT, EDirection_DOWN, false, 1, !fill, !fill, Edges(10, 0, 10, 0),	0, clear, "attack direction");
	UIBlock* attackNameBlock		= new UIBlock(panelWidth, panelBodyHeight, EUIPositionAlign_LEFT, EUIPositionAlign_TOP, EDirection_RIGHT, EDirection_DOWN, true,  1, !fill, !fill, Edges(),				0, clear, "attack name");
	miniGameMenuPage->addBox(new TextBox(MiniGamePlayerBoxPreset(EUIBoxValueToDisplay_CHARACTER_CUR_ATTACK_NAME, whenToShow), ETextBoxFunction_NO_FUNCTION,		UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_CENTER,	panelWidth,	panelBodyHeight / 5,	Edges()), font, TextBoxSizeInfo(optionText),						white),		false,	attackNameBlock);
	UIBlock* directionOptionsBlock	= new UIBlock(panelWidth, panelBodyHeight, EUIPositionAlign_LEFT, EUIPositionAlign_TOP, EDirection_RIGHT, EDirection_DOWN, false, 2, !fill, !fill, Edges(),				7, clear, "directions options");
	miniGameMenuPage->addBox(new ShapeBox(MiniGameCharacterShapeBoxPreset(EShapeBoxClass_CIRCLE,	whenToShow),															UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_CENTER,	bullet,		bullet,					Edges()),															white),				directionOptionsBlock);
	miniGameMenuPage->addBox(new TextBox(MiniGameDirectionBoxPreset("LEFT",							whenToShow, EDirection_LEFT),	ETextBoxFunction_ATTACK_DIRECTION_BOX,	UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_CENTER,	panelWidth,	panelBodyHeight / 5,	Edges()), font, TextBoxSizeInfo(optionText,	optionText + 5, 2),		optionBox), true,	directionOptionsBlock);
	miniGameMenuPage->addBox(new ShapeBox(MiniGameCharacterShapeBoxPreset(EShapeBoxClass_CIRCLE,	whenToShow),															UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_CENTER,	bullet,		bullet,					Edges()),															white),				directionOptionsBlock);
	miniGameMenuPage->addBox(new TextBox(MiniGameDirectionBoxPreset("RIGHT",						whenToShow, EDirection_RIGHT),	ETextBoxFunction_ATTACK_DIRECTION_BOX,	UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_CENTER,	panelWidth,	panelBodyHeight / 5,	Edges()), font, TextBoxSizeInfo(optionText,	optionText + 5, 2),		optionBox), true,	directionOptionsBlock);
	miniGameMenuPage->addBox(new ShapeBox(MiniGameCharacterShapeBoxPreset(EShapeBoxClass_CIRCLE,	whenToShow),															UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_CENTER,	bullet,		bullet,					Edges()),															white),				directionOptionsBlock);
	miniGameMenuPage->addBox(new TextBox(MiniGameDirectionBoxPreset("UP",							whenToShow, EDirection_UP),		ETextBoxFunction_ATTACK_DIRECTION_BOX,	UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_CENTER,	panelWidth,	panelBodyHeight / 5,	Edges()), font, TextBoxSizeInfo(optionText,	optionText + 5, 2),		optionBox), true,	directionOptionsBlock);
	miniGameMenuPage->addBox(new ShapeBox(MiniGameCharacterShapeBoxPreset(EShapeBoxClass_CIRCLE,	whenToShow),															UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_CENTER,	bullet,		bullet,					Edges()),															white),				directionOptionsBlock);
	miniGameMenuPage->addBox(new TextBox(MiniGameDirectionBoxPreset("DOWN",							whenToShow, EDirection_DOWN),	ETextBoxFunction_ATTACK_DIRECTION_BOX,	UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_CENTER,	panelWidth,	panelBodyHeight / 5,	Edges()), font, TextBoxSizeInfo(optionText,	optionText + 5, 2),		optionBox), true,	directionOptionsBlock);
	attackDirectionBlock->mpSubElems.push_back(attackNameBlock);
	attackDirectionBlock->mpSubElems.push_back(directionOptionsBlock);
	characterOptionsDetailsBlock->mpSubElems.push_back(attackDirectionBlock);
	attackNameBlock = nullptr;
	directionOptionsBlock = nullptr;
	attackDirectionBlock = nullptr;

	// SELECT TILE TO ATTACK
	whenToShow = { EMiniGameState_PLAYER_WAIT_FOR_ATTACK_TILE_INPUT };
	miniGameMenuPage->addBox(new TextBox(MiniGameBoxPreset("Select a Tile to Attack",	whenToShow), ETextBoxFunction_NO_FUNCTION, UIPositionInfo(EUIPositionAlign_CENTER, EUIPositionAlign_TOP, panelWidth, panelHeadingHeight / 2,	Edges(0, 10, 0, 0)),	font, TextBoxSizeInfo(headingText),	darkPink), false, characterOptionsHeadingBlock);
	
	UIBlock* attackTilesBlock = new UIBlock(panelWidth, panelBodyHeight, EUIPositionAlign_LEFT, EUIPositionAlign_TOP, EDirection_RIGHT, EDirection_DOWN, true, 1, !fill, !fill, Edges(10, 0, 10, 0), 7, standardPanelColor, "tile to attack description");
	miniGameMenuPage->addBox(new ShapeBox(MiniGameCharacterShapeBoxPreset(EShapeBoxClass_CIRCLE,							whenToShow),								UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_CENTER,	bullet,		bullet,					Edges()),														white),			attackTilesBlock);
	miniGameMenuPage->addBox(new TextBox(MiniGamePlayerBoxPreset(EUIBoxValueToDisplay_CHARACTER_CUR_ATTACK_NAME, whenToShow), ETextBoxFunction_NO_FUNCTION,  UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_CENTER,	panelWidth, panelBodyHeight,		Edges()),	font, TextBoxSizeInfo(optionText,	optionText),	white),	false,	attackTilesBlock);
	characterOptionsDetailsBlock->mpSubElems.push_back(attackTilesBlock);
	attackTilesBlock = nullptr;

	// SELECT CHARACTER TO ATTACK
	whenToShow = { EMiniGameState_PLAYER_WAIT_FOR_ATTACK_CHARACTER_INPUT };
	miniGameMenuPage->addBox(new TextBox(MiniGameBoxPreset("Choose a Character",		whenToShow), ETextBoxFunction_NO_FUNCTION, UIPositionInfo(EUIPositionAlign_CENTER, EUIPositionAlign_TOP, panelWidth, panelHeadingHeight / 2, Edges(0, 10, 0, 0)),	font, TextBoxSizeInfo(headingText), darkPink), false, characterOptionsHeadingBlock);
	characterSelectionPanelBlockNum = (int)characterOptionsDetailsBlock->mpSubElems.size();
	createMiniGameCharacterSelectionMenu(menuManager, screen, worldData);

	// in between states, show the box
	whenToShow = { EMiniGameState_BUFFER, EMiniGameState_CHARACTER_STUNNED, EMiniGameState_ENEMY_MOVE_CHARACTER, EMiniGameState_ENEMY_TAKE_ACTION, EMiniGameState_PLAYER_COMPLETE_ACTION_ATTACK };
	miniGameMenuPage->addBox(new TextBox(MiniGameBoxPreset(" ", whenToShow), ETextBoxFunction_NO_FUNCTION, UIPositionInfo(EUIPositionAlign_CENTER,	EUIPositionAlign_TOP,		panelWidth, panelHeadingHeight / 2, Edges(0, 10, 0, 0)), font, TextBoxSizeInfo(headingText),	darkPink),	false, characterOptionsHeadingBlock);
	miniGameMenuPage->addBox(new TextBox(MiniGameBoxPreset(" ", whenToShow), ETextBoxFunction_NO_FUNCTION, UIPositionInfo(EUIPositionAlign_LEFT,	EUIPositionAlign_CENTER,	panelWidth, panelBodyHeight,		Edges(10, 0, 0, 0)), font, TextBoxSizeInfo(optionText),		white),		false, characterOptionsDetailsBlock);

	// LOG
	UIBlock* debugStatLine = new UIBlock(Hitbox(CoordsX1Y1WidthHeight(0, screen.mGameLevelChunkHeight, screen.mGameLevelChunkWidth, 50)), EUIPositionAlign_LEFT, EUIPositionAlign_TOP, EDirection_RIGHT, EDirection_UP, true, 1, !fill, !fill, Edges(0, 15, 20, 0), 0, highlightPanelColor, "log line");
	miniGameMenuPage->addBox(new TextBox(MiniGameStatBoxPreset(EUIBoxValueToDisplay_MINI_GAME_DEBUG_LINE), ETextBoxFunction_NO_FUNCTION, UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_TOP, screen.mGameLevelChunkWidth - 15, 50, Edges()), font, TextBoxSizeInfo(detailText , detailText), darkPink), false, debugStatLine);
	miniGameMenuPage->mpElems.push_back(debugStatLine);
	debugStatLine = nullptr;

	// undo
	miniGameMenuPage->addBox(new TextBox(StandardTextBoxPreset("UNDO"), ETextBoxFunction_MINI_GAME_UNDO_BOX, UIPositionInfo(EUIPositionAlign_CENTER, EUIPositionAlign_CENTER, Hitbox(CoordsX1Y1WidthHeight(1250, 417, 150, 50)), Edges()), font, TextBoxSizeInfo(optionText, optionText + 5, 2), optionBox), true);

	miniGameMenuPage = nullptr;
	characterOptionsBlock = nullptr;
	characterOptionsHeadingBlock = nullptr;
	characterOptionsDetailsBlock = nullptr;
}

void createMiniGameCharacterStatsMenu(MenuManager& menuManager, const ScreenObject& screen, MiniGameWorldData& worldData)
{
	MenuPage* miniGameMenuPage = menuManager.mpMenuPages[EMenuPageType_MINI_GAME_MENU];
	const int levelChunkWidth = screen.mGameLevelChunkWidth;
	const int levelChunkHeight = screen.mGameLevelChunkHeight;

	const char* font = StyleManager::fontName;
	const TextBoxColorInfo colors = TextBoxColorInfo(StyleManager::pink);
	const SDL_Color& panelColor				= StyleManager::sunYellow;
	const SDL_Color& healthColor			= StyleManager::lightYellow;
	const SDL_Color& healthTextColor		= StyleManager::gold;
	const SDL_Color& healthBackgroundColor	= StyleManager::pink;
	const SDL_Color& clear					= StyleManager::clear;


	const bool fill = true;

	int maxWidth = levelChunkWidth;
	int maxHeight = 300;
	int curX = 20;
	int curY = 20;
	const int iconWidth  = 85;
	const int iconHeight = 85;
	const int spacing = 10;
	const int padding = 5;

	const int headingSize = 20;
	const int detailSize = 15;

	// level
	// round num
	// character icon, hp, defense, stuns
	UIBlock* statBlock	= new UIBlock(Hitbox(CoordsX1Y1WidthHeight(curX, curY, maxWidth, maxHeight)), EUIPositionAlign_LEFT, EUIPositionAlign_TOP, EDirection_RIGHT, EDirection_DOWN, true, 2, !fill, !fill, Edges(), 2, panelColor, "head stats block");
	UIBlock* levelStats = new UIBlock(maxWidth, maxHeight, EUIPositionAlign_LEFT, EUIPositionAlign_TOP, EDirection_RIGHT, EDirection_DOWN, false, 2, !fill, !fill, Edges(), 2, panelColor, "level stats");
	miniGameMenuPage->addBox(new TextBox(MiniGameStatBoxPreset(EUIBoxValueToDisplay_CUR_LEVEL_NUMBER), ETextBoxFunction_NO_FUNCTION, UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_TOP, maxWidth, maxHeight / 4, Edges()), font, TextBoxSizeInfo(headingSize), colors), false, levelStats);
	miniGameMenuPage->addBox(new TextBox(MiniGameStatBoxPreset(EUIBoxValueToDisplay_COMBAT_ROUND_NUM), ETextBoxFunction_NO_FUNCTION, UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_TOP, maxWidth, maxHeight / 4, Edges()), font, TextBoxSizeInfo(headingSize), colors), false, levelStats);
	statBlock->mpSubElems.push_back(levelStats);
	UIBlock* characterStatsLine	= new UIBlock(maxWidth, maxHeight, EUIPositionAlign_LEFT, EUIPositionAlign_TOP, EDirection_RIGHT, EDirection_DOWN, true, 1, !fill, !fill, Edges(),	0, panelColor, "character stats line");
	
	if (worldData.getStage()->mGrid.isSetUp())
	{
		CombatManager& combatManager = worldData.getStage()->mCombatManager;
		for (int count = 0; count < combatManager.getAllCharacters().size(); count++)
		{
			CombatCharacter* pCurChar = combatManager.getFromAllCharacters(count);
			maxWidth = 140;
			maxHeight = 50;
			int curPaddingLeft = (count == 0) ? padding : 0;

			miniGameMenuPage->addBox(new ImageBox(MiniGameCharacterImageBoxPreset(count, true), UIPositionInfo(0, EUIPositionAlign_CENTER, EUIPositionAlign_CENTER, iconWidth, iconHeight, Edges()), pCurChar->mIconFileName), characterStatsLine);

			UIBlock* curCharacterInfoBlock	= new UIBlock(maxWidth, maxHeight, EUIPositionAlign_LEFT, EUIPositionAlign_TOP, EDirection_RIGHT, EDirection_DOWN, true,  2, !fill, !fill, Edges(0, 0, 0, 20),	0, clear, "character stat info block");
			miniGameMenuPage->addBox(new TextBox(MiniGameCharacterBoxPreset(count, true, EUIBoxValueToDisplay_CHARACTER_NAME),	ETextBoxFunction_NO_FUNCTION,	UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_TOP, 200, maxHeight, Edges(padding,	8, 0, 0)), font, TextBoxSizeInfo(headingSize), colors), false, curCharacterInfoBlock);

			UIBlock* curTextBarBlock		= new UIBlock(maxWidth, maxHeight, EUIPositionAlign_LEFT, EUIPositionAlign_TOP, EDirection_RIGHT, EDirection_DOWN, false, 2, !fill, !fill, Edges(),				0, clear, "character stat text/bar block");
			miniGameMenuPage->addBox(new TextBox(MiniGameCharacterBoxPreset(count, true, "HP: "),													ETextBoxFunction_NO_FUNCTION,	UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_TOP, maxWidth,	maxHeight, Edges(0,	3,			0, 0)), font, TextBoxSizeInfo(detailSize),  colors), false, curTextBarBlock);
			miniGameMenuPage->addBox(new HealthBox(HealthBoxPreset(			count,		 EUIBoxValueToDisplay_CHARACTER_HEALTH),													UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_TOP, maxWidth,	maxHeight, Edges(0,	3,			0, 0)), font, detailSize, healthColor, healthBackgroundColor, healthTextColor), curTextBarBlock);
			miniGameMenuPage->addBox(new TextBox(MiniGameCharacterBoxPreset(count, true, "DEF: "),													ETextBoxFunction_NO_FUNCTION,	UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_TOP, maxWidth,	maxHeight, Edges(0,	3,			0, 0)), font, TextBoxSizeInfo(detailSize),  colors), false, curTextBarBlock);
			miniGameMenuPage->addBox(new HealthBox(HealthBoxPreset(			count,		 EUIBoxValueToDisplay_CHARACTER_DEFENSE),													UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_TOP, maxWidth,	maxHeight, Edges(0,	3,			0, 0)), font, detailSize, healthColor, healthBackgroundColor, healthTextColor), curTextBarBlock);
			miniGameMenuPage->addBox(new TextBox(MiniGameCharacterBoxPreset(count, true, "POISON: "),												ETextBoxFunction_NO_FUNCTION,	UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_TOP, maxWidth,	maxHeight, Edges(0,	3,			0, 0)), font, TextBoxSizeInfo(detailSize),  colors), false, curTextBarBlock);
			miniGameMenuPage->addBox(new TextBox(MiniGameCharacterBoxPreset(count, true, EUIBoxValueToDisplay_CHARACTER_HEALTH_MODIFIER_AMOUNT),	ETextBoxFunction_NO_FUNCTION,	UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_TOP, maxWidth,	maxHeight, Edges(0,	3,			0, 0)), font, TextBoxSizeInfo(detailSize),	colors), false, curTextBarBlock);
			miniGameMenuPage->addBox(new TextBox(MiniGameCharacterBoxPreset(count, true, "STUNS: "),												ETextBoxFunction_NO_FUNCTION,	UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_TOP, maxWidth,	maxHeight, Edges(0,	padding,	0, 0)), font, TextBoxSizeInfo(detailSize),  colors), false, curTextBarBlock);
			miniGameMenuPage->addBox(new TextBox(MiniGameCharacterBoxPreset(count, true, EUIBoxValueToDisplay_CHARACTER_STUN),						ETextBoxFunction_NO_FUNCTION,	UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_TOP, maxWidth,	maxHeight, Edges(0,	padding,	0, 0)), font, TextBoxSizeInfo(detailSize),	colors), false, curTextBarBlock);
			curCharacterInfoBlock->mpSubElems.push_back(curTextBarBlock);

			characterStatsLine->mpSubElems.push_back(curCharacterInfoBlock);
			curCharacterInfoBlock = nullptr;
			curTextBarBlock = nullptr;
			pCurChar = nullptr;
		}
	}
	
	statBlock->mpSubElems.push_back(characterStatsLine);
	characterStatsLine = nullptr;
	
	if (miniGameMenuPage->mpElems.size() > characterStatsBlockNum && miniGameMenuPage->mpElems[characterStatsBlockNum])
	{
		//replace the character stat block
		statBlock->setTexture(screen.mpRenderer);
		statBlock->setMaxSize();
		miniGameMenuPage->deleteBlock((UIBlock*)miniGameMenuPage->mpElems[characterStatsBlockNum]);
		miniGameMenuPage->mpElems[characterStatsBlockNum] = statBlock;
	}
	else
	{
		//first time through, place the block
		miniGameMenuPage->mpElems.push_back(statBlock);
	}
	
	statBlock = nullptr;
	miniGameMenuPage = nullptr;
}

void createMiniGameCharacterSelectionMenu(MenuManager& menuManager, const ScreenObject& screen, MiniGameWorldData& worldData)
{
	MenuPage* miniGameMenuPage = menuManager.mpMenuPages[EMenuPageType_MINI_GAME_MENU];
	const char* font = StyleManager::fontName;

	const TextBoxColorInfo optionBox	= TextBoxColorInfo(StyleManager::white, StyleManager::sunYellow, StyleManager::pink, StyleManager::pink, StyleManager::white, StyleManager::sunYellow);
	const SDL_Color& clear = StyleManager::clear;
	const SDL_Color& white = StyleManager::white;

	const int& panelWidth			= StyleManager::panelWidth;
	const int& panelBodyHeight		= StyleManager::panelBodyHeight;

	const int& optionText	= StyleManager::optionText;
	const int& bullet		= StyleManager::bullet;

	const bool fill = true;

	UIBlock* characterOptionsBlock			= (UIBlock*)miniGameMenuPage->mpElems[characterOptionsBlockNum];
	UIBlock* characterOptionsDetailsBlock	= (UIBlock*)characterOptionsBlock->mpSubElems[characterOptionsDetailsBlockNum];

	UIBlock* pCharacterSelectionPanel		= new UIBlock(panelWidth, panelBodyHeight, EUIPositionAlign_LEFT, EUIPositionAlign_TOP, EDirection_RIGHT, EDirection_DOWN, false, 2, !fill, !fill, Edges(10, 0, 10, 0), 7, clear, "character selection panel block");
	std::vector <EMiniGameState> whenToShow = { EMiniGameState_PLAYER_WAIT_FOR_ATTACK_CHARACTER_INPUT };

	if (worldData.getStage()->mGrid.isSetUp())
	{
		CombatManager& combatManager = worldData.getStage()->mCombatManager;
		for (int countCharacter = 0; countCharacter < combatManager.getAllCharacters().size(); countCharacter++)
		{
			ECombatCharacterType characterType = combatManager.getFromAllCharacters(countCharacter)->mType;
			miniGameMenuPage->addBox(new ShapeBox(MiniGameCharacterShapeBoxPreset(EShapeBoxClass_CIRCLE,	countCharacter, characterType, true,										whenToShow),										UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_CENTER, bullet,		bullet,					Edges()), white), pCharacterSelectionPanel);
			miniGameMenuPage->addBox(new TextBox(MiniGameCharacterBoxPreset(								countCharacter, characterType, true, EUIBoxValueToDisplay_CHARACTER_NAME,	whenToShow), ETextBoxFunction_ATTACK_CHARACTER_BOX, UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_CENTER, panelWidth,	panelBodyHeight / 4,	Edges()), font, TextBoxSizeInfo(optionText, optionText + 5, 2), optionBox), true, pCharacterSelectionPanel);
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
	miniGameMenuPage = nullptr;
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
	const int& panelBodyHeight		= StyleManager::panelBodyHeight;

	const int& optionText			= StyleManager::optionText;
	const int& detailText			= StyleManager::detailText;
	const int& bullet				= StyleManager::bullet;

	const bool fill = true;

	UIBlock* characterOptionsBlock			= (UIBlock*)miniGameMenuPage->mpElems[characterOptionsBlockNum];
	UIBlock* characterOptionsDetailsBlock	= (UIBlock*)characterOptionsBlock->mpSubElems[characterOptionsDetailsBlockNum];

	UIBlock* pAttackBlock = new UIBlock(panelWidth, panelBodyHeight, EUIPositionAlign_LEFT, EUIPositionAlign_TOP, EDirection_RIGHT, EDirection_DOWN, false, 2, !fill, !fill, Edges(10, 0, 10, 0), 0, clear, "Attack block");
	std::vector <EMiniGameState> whenToShow = { EMiniGameState_PLAYER_WAIT_FOR_ATTACK_OPTION_INPUT };
	if (worldData.mCurLevelNumber != -1)
	{
		CombatManager& combatManager = worldData.getStage()->mCombatManager;
		for (int countCharacter = 0; countCharacter < (int)combatManager.getAllCharacters().size(); countCharacter++)
		{
			CombatCharacter& character = *combatManager.getFromAllCharacters(countCharacter);
			if (character.mType != ECombatCharacterType_PLAYER)
			{
				continue;
			}

			//CHARACTER
			//CHOOSE ATTACK
			//OPTION NAME, DIAGRAM
			//TARGET TYPE
			//DAMAGE
			//SPECIAL EFFECTS / NOTES
			const std::vector<Attack>& attacks = character.mCombatMovementManager.getAttacks();
			for (int countAttack = 0; countAttack < (int)attacks.size(); countAttack++)
			{
				const Attack& curAttack = attacks[countAttack];
				ECombatAttackCategoryType category = curAttack.mCategory;

				// bullet, name
				// spacer, details
				UIBlock* bulletAndAttackDetails	= new UIBlock(panelWidth, panelBodyHeight, EUIPositionAlign_LEFT, EUIPositionAlign_TOP, EDirection_RIGHT, EDirection_DOWN, false, 2, !fill, !fill, Edges(0, 9, 0, 0), 0, clear, "Attack bullet and details block");
				miniGameMenuPage->addBox(new ShapeBox(MiniGamePlayerAttackShapeBoxPreset(EShapeBoxClass_CIRCLE,	countCharacter, false,														countAttack, category, whenToShow),										UIPositionInfo(EUIPositionAlign_LEFT,	EUIPositionAlign_CENTER,	bullet,		bullet,					Edges(0, 0, 0, 7)), white),															bulletAndAttackDetails);
				miniGameMenuPage->addBox(new TextBox(MiniGamePlayerAttackBoxPreset(								countCharacter, false, EUIBoxValueToDisplay_CHARACTER_ATTACK_OPTION_NAME,	countAttack, category, whenToShow), ETextBoxFunction_ATTACK_STYLE_BOX,	UIPositionInfo(EUIPositionAlign_LEFT,	EUIPositionAlign_CENTER,	panelWidth, panelBodyHeight / 4,	Edges()), font, TextBoxSizeInfo(optionText,  optionText + 5, 2), optionBox), true,	bulletAndAttackDetails);
				miniGameMenuPage->addBox(new ShapeBox(MiniGamePlayerAttackShapeBoxPreset(EShapeBoxClass_CIRCLE, countCharacter, false,														countAttack, category, whenToShow),										UIPositionInfo(EUIPositionAlign_LEFT,	EUIPositionAlign_TOP,		bullet,		bullet,					Edges()), clear),																	bulletAndAttackDetails);


				UIBlock* pAttackDetailBlock		= new UIBlock(panelWidth, panelBodyHeight, EUIPositionAlign_LEFT, EUIPositionAlign_TOP, EDirection_RIGHT, EDirection_DOWN, false, 1, !fill, !fill, Edges(5, 0, 0, 0), 0, clear, "attack details");
				if (curAttack.mDamagePercent != 0)  { miniGameMenuPage->addBox(	new TextBox(MiniGamePlayerAttackBoxPreset(countCharacter, false, EUIBoxValueToDisplay_CHARACTER_ATTACK_OPTION_DAMAGE,						countAttack, category, whenToShow), ETextBoxFunction_NO_FUNCTION, UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_TOP,	panelWidth, panelBodyHeight / 4,	Edges()), font, TextBoxSizeInfo(detailText), black), false, pAttackDetailBlock); }
				if (curAttack.mCooldownAmount != 0) { miniGameMenuPage->addBox(	new TextBox(MiniGamePlayerAttackBoxPreset(countCharacter, false, EUIBoxValueToDisplay_CHARACTER_ATTACK_OPTION_CUR_COOLDOWN,					countAttack, category, whenToShow), ETextBoxFunction_NO_FUNCTION, UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_TOP,	panelWidth, panelBodyHeight / 4,	Edges()), font, TextBoxSizeInfo(detailText), black), false, pAttackDetailBlock); }
				miniGameMenuPage->addBox(										new TextBox(MiniGamePlayerAttackBoxPreset(countCharacter, false, EUIBoxValueToDisplay_CHARACTER_ATTACK_OPTION_SPECIAL_EFFECTS_AND_NOTES,	countAttack, category, whenToShow), ETextBoxFunction_NO_FUNCTION, UIPositionInfo(EUIPositionAlign_LEFT, EUIPositionAlign_TOP,	panelWidth, panelBodyHeight / 4,	Edges()), font, TextBoxSizeInfo(detailText), black), false, pAttackDetailBlock);
				bulletAndAttackDetails->mpSubElems.push_back(pAttackDetailBlock);

				pAttackBlock->mpSubElems.push_back(bulletAndAttackDetails);
				createBlockDiagram(curAttack, countCharacter, countAttack, miniGameMenuPage, pAttackBlock, whenToShow, menuManager);

				pAttackDetailBlock = nullptr;
				bulletAndAttackDetails = nullptr;
			}
		}
	}

	if ((int)characterOptionsDetailsBlock->mpSubElems.size() > attackPanelBlockNum && characterOptionsDetailsBlock->mpSubElems[attackPanelBlockNum])
	{
		//replace the attack panel block
		pAttackBlock->setTexture(screen.mpRenderer);
		pAttackBlock->setMaxSize();
		miniGameMenuPage->deleteBlock((UIBlock*)characterOptionsDetailsBlock->mpSubElems[attackPanelBlockNum]);
		characterOptionsDetailsBlock->mpSubElems[attackPanelBlockNum] = pAttackBlock;
	}
	else
	{
		//first time through, place the block
		characterOptionsDetailsBlock->mpSubElems.push_back(pAttackBlock);
	}

	pAttackBlock = nullptr;
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

	
	if (attack.mType == ECombatActionGridPattern_WHOLE_GRID)
	{
		num = 4;
		out = 0;
		blockSize /= 3;
		spacing /= 2;
		leftBlockMargin = 5;
	}

	int maxWidth = (2 * num + 1) * (blockSize + spacing);
	int maxHeight = maxWidth;
	
	int maxBlockHeight = StyleManager::panelBodyHeight;
	int outSpacing = out * (blockSize + spacing) + spacing;

	const ECombatAttackCategoryType& category = attack.mCategory;

	int size = (2 * (num - out) + 1);
	UIBlock* pDiagramBlock = new UIBlock(200, maxBlockHeight, EUIPositionAlign_LEFT, EUIPositionAlign_TOP, EDirection_RIGHT, EDirection_DOWN, true, size, !fill, !fill, Edges(0, 9, leftBlockMargin, 0), 0, clear, "block diagram");
	switch(attack.mType)
	{ 
	case ECombatActionGridPattern_SQUARE:
	case ECombatActionGridPattern_WHOLE_GRID:
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				if (i == num && j == num)
				{
					// middle dot
					pPage->addBox(new ShapeBox(MiniGamePlayerAttackShapeBoxPreset(EShapeBoxClass_CIRCLE, characterIndex, false, attackNum, category, whenToShow), UIPositionInfo(EUIPositionAlign_CENTER, EUIPositionAlign_CENTER, blockSize, blockSize, Edges(outSpacing,	outSpacing, outSpacing, outSpacing)),	blockColor), pDiagramBlock);
				}
				else 
				{
					// block 
					pPage->addBox(new ShapeBox(MiniGamePlayerAttackShapeBoxPreset(EShapeBoxClass_RECT,	 characterIndex, false, attackNum, category, whenToShow), UIPositionInfo(EUIPositionAlign_CENTER, EUIPositionAlign_CENTER, blockSize, blockSize, Edges(spacing,		spacing,	spacing,	spacing)),		blockColor), pDiagramBlock);
				}
			}
		}
		break;
	case ECombatActionGridPattern_CROSS:
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				if (i == num && j == num)
				{
					// middle dot
					pPage->addBox(new ShapeBox(MiniGamePlayerAttackShapeBoxPreset(EShapeBoxClass_CIRCLE, characterIndex, false, attackNum, category, whenToShow), UIPositionInfo(EUIPositionAlign_CENTER, EUIPositionAlign_CENTER, blockSize, blockSize, Edges(outSpacing, outSpacing, outSpacing, outSpacing)), blockColor), pDiagramBlock);
				}
				else if (i == num || j == num)
				{
					// valid block 
					pPage->addBox(new ShapeBox(MiniGamePlayerAttackShapeBoxPreset(EShapeBoxClass_RECT,   characterIndex, false, attackNum, category, whenToShow), UIPositionInfo(EUIPositionAlign_CENTER, EUIPositionAlign_CENTER, blockSize, blockSize, Edges(spacing, spacing, spacing, spacing)), blockColor), pDiagramBlock);
				}
				else
				{
					// spacer block
					pPage->addBox(new ShapeBox(MiniGamePlayerAttackShapeBoxPreset(EShapeBoxClass_RECT,   characterIndex, false, attackNum, category, whenToShow), UIPositionInfo(EUIPositionAlign_CENTER, EUIPositionAlign_CENTER, blockSize, blockSize, Edges(spacing, spacing, spacing, spacing)), clear), pDiagramBlock);

				}
			}
		}
		break;
	case ECombatActionGridPattern_CHECKERBOARD:
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				if (i == num && j == num)
				{
					// middle dot
					pPage->addBox(new ShapeBox(MiniGamePlayerAttackShapeBoxPreset(EShapeBoxClass_CIRCLE,	characterIndex, false, attackNum, category, whenToShow), UIPositionInfo(EUIPositionAlign_CENTER, EUIPositionAlign_CENTER, blockSize, blockSize, Edges(outSpacing,	outSpacing, outSpacing, outSpacing)),	blockColor), pDiagramBlock);
				}
				else if (i % 2 == j % 2)
				{
					// invisible block 
					pPage->addBox(new ShapeBox(MiniGamePlayerAttackShapeBoxPreset(EShapeBoxClass_RECT,		characterIndex, false, attackNum, category, whenToShow), UIPositionInfo(EUIPositionAlign_CENTER, EUIPositionAlign_CENTER, blockSize, blockSize, Edges(spacing,		spacing,	spacing,	spacing)),		clear),		 pDiagramBlock);
				}
				else
				{
					pPage->addBox(new ShapeBox(MiniGamePlayerAttackShapeBoxPreset(EShapeBoxClass_RECT,		characterIndex, false, attackNum, category, whenToShow), UIPositionInfo(EUIPositionAlign_CENTER, EUIPositionAlign_CENTER, blockSize, blockSize, Edges(spacing,		spacing,	spacing,	spacing)),		blockColor), pDiagramBlock);
				}
			}
		}
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
}

void setUpFontSizeChart(FontSizeChart& fontSizeChart, SDL_Renderer* pRenderer)  { fontSizeChart.createFontChart(StyleManager::fontName, pRenderer); }
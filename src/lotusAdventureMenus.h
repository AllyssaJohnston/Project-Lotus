#pragma once
#include "miniGameWorldDataHelper.h"
#include "menuHelper.h"
#include "textBoxPresets.h"
#include "textBoxHelper.h"
#include "UIBlockHelper.h"

void createMenus(MenuManager& menuManager, ScreenObject& screen, MiniGameWorldData& worldData, const StyleManager& styleManager);

void createMainGameMenu(MenuManager& menuManager, ScreenObject& screen, const StyleManager& styleManager);

void createMiniGameMenu(MenuManager& menuManager, ScreenObject& screen, MiniGameWorldData& worldData, const StyleManager& styleManager);

void createMiniGameCharacterStatsMenu(MenuManager& menuManager, ScreenObject& screen, MiniGameWorldData& worldData, const StyleManager& styleManager);

void createMiniGameCharacterSelectionMenu(MenuManager& menuManager, ScreenObject& screen, MiniGameWorldData& worldData, const StyleManager& styleManager);

void createMiniGameCharacterAttackPanel(MenuManager& menuManager, ScreenObject& screen, MiniGameWorldData& worldData, const StyleManager& styleManager);

void createBlockDiagram(const EMiniGameCombatMoveAttackTypes diagramType, const int characterIndex, MenuPage* pPage, UIBlock* pBlock, const std::vector<EMiniGameState>& whenToShow, MenuManager& menuManager, const StyleManager& styleManager);

void setUpFontSizeChart(FontSizeChart& fontSizeChart, const StyleManager& styleManager, SDL_Renderer* pRenderer);
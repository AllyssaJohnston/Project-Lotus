#pragma once
#include "miniGameWorldDataHelper.h"
#include "menuHelper.h"
#include "textBoxPresets.h"
#include "textBoxHelper.h"
#include "UIBlockHelper.h"

void createMenus(MenuManager& menuManager, ScreenObject& screen, MiniGameWorldData& worldData, StyleManager& styleManager, SettingsManager& settingsManager);

void createMainGameMenu(MenuManager& menuManager, ScreenObject& screen, StyleManager& styleManager);

void createMiniGameMenu(MenuManager& menuManager, ScreenObject& screen, MiniGameWorldData& worldData, StyleManager& styleManager, SettingsManager& settingsManager);

void createMiniGameCharacterStatsMenu(MenuManager& menuManager, ScreenObject& screen, MiniGameWorldData& worldData, StyleManager& styleManager, SettingsManager& settingsManager);

void createMiniGameCharacterAttackPanel(MenuManager& menuManager, ScreenObject& screen, MiniGameWorldData& worldData, StyleManager& styleManager, SettingsManager& settingsManager);

void createBlockDiagram(EMiniGameCombatMoveAttackTypes diagramType, int characterIndex, MenuPage* pPage, UIBlock* pBlock, std::vector<EMiniGameState> whenToShow, MenuManager& menuManager, StyleManager& styleManager);

void setUpFontSizeChart(FontSizeChart& fontSizeChart, StyleManager& styleManager, SDL_Renderer* pRenderer);
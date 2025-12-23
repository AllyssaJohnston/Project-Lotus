#pragma once
#include "miniGameWorldData.h"
#include "menuManager.h"
#include "uiBoxPresets.h"

void createMenus(MenuManager& menuManager, const ScreenObject& screen, MiniGameWorldData& worldData);

void createMainGameMenu(MenuManager& menuManager, const ScreenObject& screen);

void createMiniGameMenu(MenuManager& menuManager, const ScreenObject& screen, MiniGameWorldData& worldData);

void createMiniGameCharacterStatsMenu(MenuManager& menuManager, const ScreenObject& screen, MiniGameWorldData& worldData);

void createMiniGameCharacterSelectionMenu(MenuManager& menuManager, const ScreenObject& screen, MiniGameWorldData& worldData);

void createMiniGameCharacterAttackPanel(MenuManager& menuManager, const ScreenObject& screen, MiniGameWorldData& worldData);

void createBlockDiagram(const Attack& attack, const int characterIndex, const int attackNum, MenuPage* pPage, UIBlock* pBlock, const std::vector<EMiniGameState>& whenToShow, MenuManager& menuManager);

void setUpFontSizeChart(FontSizeChart& fontSizeChart, SDL_Renderer* pRenderer);
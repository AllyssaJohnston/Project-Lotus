#include "settingsHelper.h"

SettingsManager::SettingsManager()
{
	leftKeys.push_back(EKeyboardInput_LEFT);
	leftKeys.push_back(EKeyboardInput_A);

	rightKeys.push_back(EKeyboardInput_RIGHT);
	rightKeys.push_back(EKeyboardInput_D);

	upKeys.push_back(EKeyboardInput_UP);
	upKeys.push_back(EKeyboardInput_W);
	upKeys.push_back(EKeyboardInput_SPACE_BAR);
}
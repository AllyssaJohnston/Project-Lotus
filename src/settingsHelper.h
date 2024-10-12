#pragma once
#include <vector>
#include "helperClass.h"

class SettingsManager
{
public:
	std::vector <EKeyboardInput> leftKeys;
	std::vector <EKeyboardInput> upKeys;
	std::vector <EKeyboardInput> rightKeys;

	EKeyboardInput resetLevel      = EKeyboardInput_R;
	EKeyboardInput resetCheckpoint = EKeyboardInput_T;

	EKeyboardInput shootProjectileHorizontal = EKeyboardInput_J;
	EKeyboardInput shootProjectileVertical   = EKeyboardInput_K;

	EKeyboardInput slash                     = EKeyboardInput_L;

	int								mLastFrameTextIncrease		= 0;         
	int								mTextIncrease				= 0;
	float							mTextBoxChangeWidthFactor	= 0;

	//DEBUGGING Tools
	bool							mSingleSteppingMode		= false;
	bool							mFrameStepInputRequest	= false;

	SettingsManager();
	
};
#include "helperClass.h"

bool isWaitForInput(EMiniGameState state) { return state == EMiniGameState_PLAYER_WAIT_FOR_MOVE_INPUT || state == EMiniGameState_PLAYER_WAIT_FOR_ACTION_INPUT 
		|| state == EMiniGameState_PLAYER_WAIT_FOR_ATTACK_OPTION_INPUT || state == EMiniGameState_PLAYER_WAIT_FOR_ATTACK_DIRECTION_INPUT || state == EMiniGameState_PLAYER_WAIT_FOR_ATTACK_TILE_INPUT 
		|| state == EMiniGameState_PLAYER_WAIT_FOR_ATTACK_CHARACTER_INPUT; }


std::vector<std::string> tokenize(const std::string input, const std::string delimiter)
{
	std::vector<std::string> tokens;
	int start = 0;
	int end   = 0;
	while (start < (int)input.size())
	{
		int end = (int)input.find(delimiter, start);
		std::string token = "";
		if (end == -1)
		{
			token = input.substr(start);
			start = (int)input.size();
		}
		else
		{
			token = input.substr(start, end - start);
			start = end + 1;
		}
		tokens.push_back(token);
	}
	return tokens;
}

std::vector<std::string> tokenizeByStringLength(const std::string input, int lineLength)
{
	std::vector<std::string> tokens = tokenize(input, " ");
	std::string curLine = "";
	std::vector<std::string> lines;
	for (int i = 0; i < tokens.size(); i++)
	{
		if (0 == i and (tokens[0].length() > lineLength))
		{
			SDL_assert(false);  // max width is too small
		}
		else if (curLine.length() + tokens[i].length() + 1 < lineLength)
		{
			curLine += " " + tokens[i];
		}
		else
		{
			lines.push_back(curLine);
			curLine = tokens[i];
		}
	}
	lines.push_back(curLine);

	return lines;
}

std::string createStringFromKeyboardList(const std::vector <EKeyboardInput>& list)
{
	std::string message;
	for (int count = 0; count < list.size() - 1; count++)
	{
		message += keyboardToStringMap.at(list[count]) + " / ";
	}
	message += keyboardToStringMap.at(list[list.size() - 1]);
	return message;
}

float degreesToImageRotationDegrees(int startingImageRotation, int degrees)
{
	int updatedDegrees = startingImageRotation - degrees;
	if (updatedDegrees < 0)
	{
		updatedDegrees += 360;
	}
	return (float)updatedDegrees;
}


std::string directionToString(EDirection dir)
{
	switch (dir)
	{
	case EDirection_LEFT:
		return "left";
	case EDirection_LEFT_AND_RIGHT:
		return "left and right";
	case EDirection_RIGHT:
		return "right";
	case EDirection_UP:
		return "up";
	case EDirection_UP_AND_DOWN:
		return "up and down";
	case EDirection_DOWN:
		return "down";
	case EDirection_NONE:
		return "none";
	}
	return "invalid";
}

std::string floatDecimalToString(float num)
{
	int numPercent = int(num * 100.0f);
	return std::to_string(numPercent / 100) + "." + std::to_string(numPercent % 100);
}





KeyData::KeyData(int key, int repeat) : mKey(key), mRepeat(repeat) { ; }

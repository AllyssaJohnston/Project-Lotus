#include "helperClass.h"

std::vector<std::string> tokenize(std::string input, std::string delimiter)
{
	std::vector<std::string> tokens;
	int start = 0;
	int end   = 0;
	while (start < input.size())
	{
		int end = input.find(delimiter, start);
		std::string token = "";
		if (end == -1)
		{
			token = input.substr(start);
			start = input.size();
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

std::vector<std::string> tokenizeByStringLength(std::string input, int lineLength)
{
	std::vector<std::string> tokens = tokenize(input, " ");
	std::string curLine = "";
	std::vector<std::string> lines;
	for (int i = 0; i < tokens.size(); i++)
	{
		if (0 == i and (tokens[0].length() > lineLength))
		{
			SDL_assert(false);  //max width is too small
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

std::string createStringFromKeyboardList(std::vector <EKeyboardInput> list)
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

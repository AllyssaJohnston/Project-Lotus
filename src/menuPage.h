#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_ttf.h>
#include <string>
#include <vector>
#include "uiBox.h"
#include "UIBlock.h"
#include "helpers.h"
#include "menuHelpers.h"


class MenuPage
{
private:
	TextBox* mpCurSelectedTextBox = nullptr;
	TextBox* mpLastFrameCurTextBox = nullptr;
	TextBox* mpCurTextBox = nullptr;
public:
	std::vector <TextBox*>		mpAllSelectableTextBoxes;
	std::vector <TextBox*>		mpAllDisplayOnlyTextBoxes;
	std::vector <ImageBox*>		mpImageBoxes;
	std::vector <ShapeBox*>		mpShapeBoxes;
	std::vector <HealthBox*>	mpHealthBoxes;
	std::vector <UIElement*>	mpElems;

	~MenuPage();

	void preTick();

	void addBox(TextBox* pTextBox, bool selectable, UIBlock* pBlock);
	void addBox(TextBox* pTextBox, bool selectable);
	void addBox(ImageBox* pTextBox, UIBlock* pBlock);
	void addBox(ImageBox* pTextBox);
	void addBox(ShapeBox* pTextBox, UIBlock* pBlock);
	void addBox(ShapeBox* pTextBox);
	void addBox(HealthBox* pTextBox, UIBlock* pBlock);
	void addBox(HealthBox* pTextBox);

	void useInput(const std::vector <int>& eventVect);

	void setCurTextBoxIfValid(int count);

	void setCurTextBox(TextBox* pTextBox);

	void setCurSelectedTextBox(TextBox* pTextBox);

	TextBox* getCurTextBox() const;

	TextBox* getCurSelectedTextBox() const;

	bool curTextBoxChange();

	int getCurTextBoxIndex() const;

	std::vector<SDL_Texture*> getCurTextBoxTextures() const;

	std::vector <TextBox*> getAllTextBoxes() const;

	std::vector <TextBox*> getCurTextBoxes() const;

	std::vector <UIElement*> getAllElems() const;

	void setDefaultSelectedBox();

	void adjustBlocks();

	void deleteBlock(UIBlock* pBlock);
};
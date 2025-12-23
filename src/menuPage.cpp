#include "menuPage.h"

MenuPage::~MenuPage()
{
	mpCurSelectedTextBox = nullptr;
	mpLastFrameCurTextBox = nullptr;
	mpCurTextBox = nullptr;
	for (TextBox* box : mpAllSelectableTextBoxes)
	{
		delete box;
	}
	for (TextBox* box : mpAllDisplayOnlyTextBoxes)
	{
		delete box;
	}
	for (ImageBox* box : mpImageBoxes)
	{
		delete box;
	}
	for (UIBlock* box : mpBlocks)
	{
		delete box;
	}
}

void MenuPage::preTick()
{
	mpLastFrameCurTextBox = mpCurTextBox;
}


void MenuPage::addBox(TextBox* pTextBox, bool selectable, UIBlock* pBlock)
{
	pBlock->mpAllBoxes.push_back(pTextBox);
	if (selectable)
	{
		mpAllSelectableTextBoxes.push_back(pTextBox);
	}
	else
	{
		mpAllDisplayOnlyTextBoxes.push_back(pTextBox);
	}
}

void MenuPage::addBox(ImageBox* pBox, UIBlock* pBlock)
{
	pBlock->mpAllBoxes.push_back(pBox);
	mpImageBoxes.push_back(pBox);
}

void MenuPage::addBox(ShapeBox* pBox, UIBlock* pBlock)
{
	pBlock->mpAllBoxes.push_back(pBox);
	mpShapeBoxes.push_back(pBox);
}

void MenuPage::addBox(HealthBox* pBox, UIBlock* pBlock)
{
	pBlock->mpAllBoxes.push_back(pBox);
	mpHealthBoxes.push_back(pBox);
}


void MenuPage::useInput(const std::vector <int>& eventVect)
{
	for (int curEventEnum : eventVect)
	{
		switch (curEventEnum)
		{
		case EKeyboardInput_UP:

			setCurTextBoxIfValid(getCurTextBoxIndex() - 1);
			break;

		case EKeyboardInput_DOWN:

			setCurTextBoxIfValid(getCurTextBoxIndex() + 1);
			break;
		}
	}
}


void MenuPage::setCurTextBoxIfValid(int count)
{
	if (count >= 0 and count < mpAllSelectableTextBoxes.size() and mpAllSelectableTextBoxes[count]->mShow)
	{
		setCurTextBox(mpAllSelectableTextBoxes[count]);
	}
}

void MenuPage::setCurTextBox(TextBox* pTextBox)
{
	if (mpCurTextBox != nullptr)
	{
		mpCurTextBox->changeIsHighlighted(false);
	}
	mpCurTextBox = pTextBox;
	if (mpCurTextBox != nullptr)
	{
		mpCurTextBox->changeIsHighlighted(true);
	}
}

void MenuPage::setCurSelectedTextBox(TextBox* pTextBox) { mpCurSelectedTextBox = pTextBox; }


bool MenuPage::curTextBoxChange() { return mpLastFrameCurTextBox != mpCurTextBox; }


TextBox* MenuPage::getCurTextBox() const { return mpCurTextBox; }

TextBox* MenuPage::getCurSelectedTextBox() const { return mpCurSelectedTextBox; }

int MenuPage::getCurTextBoxIndex() const
{
	for (int count = 0; count < mpAllSelectableTextBoxes.size(); count++)
	{
		if (mpAllSelectableTextBoxes[count] == mpCurTextBox)
		{
			return count;
		}
	}
	return -1;
}

std::vector<SDL_Texture*> MenuPage::getCurTextBoxTextures() const { return mpCurTextBox->getTextBoxTexture(); }

std::vector <TextBox*> MenuPage::getAllTextBoxes() const
{
	std::vector <TextBox*> allTextBoxes;
	for (TextBox* pTextBox : mpAllSelectableTextBoxes)
	{
		allTextBoxes.push_back(pTextBox);
	}
	for (TextBox* pTextBox : mpAllDisplayOnlyTextBoxes)
	{
		allTextBoxes.push_back(pTextBox);
	}
	return allTextBoxes;
}

std::vector <TextBox*> MenuPage::getCurTextBoxes() const
{
	std::vector <TextBox*> allCurTextBoxes;
	for (TextBox* pCurTextBox : mpAllSelectableTextBoxes)
	{
		if (pCurTextBox->mShow)
		{
			allCurTextBoxes.push_back(pCurTextBox);
		}
	}

	for (TextBox* pCurTextBox : mpAllDisplayOnlyTextBoxes)
	{
		if (pCurTextBox->mShow)
		{
			allCurTextBoxes.push_back(pCurTextBox);
		}
	}

	return allCurTextBoxes;
}

std::vector <UIBlock*> MenuPage::getAllBlocks() const
{
	std::vector <UIBlock*> list;
	for (UIBlock* pBlock : mpBlocks)
	{
		pBlock->getAllBlocksInternal(list);
	}
	return list;
}


void MenuPage::updateAllUIBoxesShowState(MiniGameStateManagerData& data, const CombatManager& combatManager)
{
	// update show state for all textboxes
	for (TextBox* pCurTextBox : mpAllSelectableTextBoxes)
	{
		pCurTextBox->mShow = shouldShowUIBox(pCurTextBox->mData, data, combatManager);
	}

	for (TextBox* pCurTextBox : mpAllDisplayOnlyTextBoxes)
	{
		pCurTextBox->mShow = shouldShowUIBox(pCurTextBox->mData, data, combatManager);
	}

	for (ImageBox* pImageBox : mpImageBoxes)
	{
		pImageBox->mShow = shouldShowUIBox(pImageBox->mData, data, combatManager);
	}

	for (ShapeBox* pShapeBox : mpShapeBoxes)
	{
		pShapeBox->mShow = shouldShowUIBox(pShapeBox->mData, data, combatManager);
	}

	for (HealthBox* pHealthBox : mpHealthBoxes)
	{
		pHealthBox->mShow = shouldShowUIBox(pHealthBox->mData, data, combatManager);
	}
}

void MenuPage::setDefaultSelectedBox()
{
	for (TextBox* pCurTextBox : mpAllSelectableTextBoxes)
	{
		if (pCurTextBox->mShow)
		{
			setCurTextBox(pCurTextBox);
			return;
		}
	}
}


void MenuPage::adjustBlocks()
{
	for (UIBlock* pBlock : mpBlocks)
	{
		pBlock->updateBlocks();
	}
}

void MenuPage::deleteBlock(UIBlock* pBlock)
{
	std::vector<UIBox*> allBoxes = pBlock->getAllBoxes();
	for (int i = (int)allBoxes.size() - 1; i > -1; i--)
	{
		UIBox* pBox = allBoxes[i];
		switch (pBox->mClassType)
		{
		case EUIBoxClass_TEXTBOX:
			mpAllDisplayOnlyTextBoxes.erase(std::remove(mpAllDisplayOnlyTextBoxes.begin(), mpAllDisplayOnlyTextBoxes.end(), pBox), mpAllDisplayOnlyTextBoxes.end());
			mpAllSelectableTextBoxes.erase(std::remove(mpAllSelectableTextBoxes.begin(), mpAllSelectableTextBoxes.end(), pBox), mpAllSelectableTextBoxes.end());
			break;
		case EUIBoxClass_IMAGEBOX:
			mpImageBoxes.erase(std::remove(mpImageBoxes.begin(), mpImageBoxes.end(), pBox), mpImageBoxes.end());
			break;
		case EUIBoxClass_SHAPEBOX:
			mpShapeBoxes.erase(std::remove(mpShapeBoxes.begin(), mpShapeBoxes.end(), pBox), mpShapeBoxes.end());
			break;
		case EUIBoxClass_HEALTHBOX:
			mpHealthBoxes.erase(std::remove(mpHealthBoxes.begin(), mpHealthBoxes.end(), pBox), mpHealthBoxes.end());
			break;
		}

		delete pBox;
	}
	delete pBlock;
}



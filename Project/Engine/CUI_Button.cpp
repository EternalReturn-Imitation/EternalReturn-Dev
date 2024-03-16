#include "pch.h"
#include "CUI_Button.h"

CUI_Button::CUI_Button()
	: CUIComponent(COMPONENT_TYPE::UI_BUTTON)
{
}

CUI_Button::CUI_Button(const CUI_Button& _Other)
	: CUIComponent(COMPONENT_TYPE::UI_BUTTON)
{
}

CUI_Button::~CUI_Button()
{
}

void CUI_Button::begin()
{
}

void CUI_Button::tick()
{
	CUIComponent::tick();
}

void CUI_Button::finaltick()
{
	CUIComponent::finaltick();
}

void CUI_Button::SaveToLevelFile(FILE* _File)
{
}

void CUI_Button::LoadFromLevelFile(FILE* _FILE)
{
}


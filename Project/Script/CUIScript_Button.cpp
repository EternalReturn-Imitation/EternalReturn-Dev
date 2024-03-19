#include "pch.h"
#include "CUIScript_Button.h"

CUIScript_Button::CUIScript_Button()
	: CScript((UINT)SCRIPT_TYPE::UISCRIPT_BUTTON)
{
}

CUIScript_Button::CUIScript_Button(const CUIScript_Button& _origin)
	: CScript((UINT)SCRIPT_TYPE::UISCRIPT_BUTTON)
{
}

CUIScript_Button::~CUIScript_Button()
{
}

void CUIScript_Button::init()
{
}

void CUIScript_Button::begin()
{
	GetOwner()->UI_Button()->SetUIScript(static_cast<CScript*>(this));
	GetOwner()->UI_Button()->SetUIFunc_CsrClick(static_cast<CUI_DELEGATE>(&CUIScript_Button::Calculate));
}

void CUIScript_Button::tick()
{
}

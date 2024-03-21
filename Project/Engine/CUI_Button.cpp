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
	m_pOriginTex = GetOwner()->MeshRender()->GetMaterial(0)->GetTexParam(TEX_PARAM::TEX_0);
	SaveResRef(m_pOriginTex.Get(), _File);
	int a = 0;
}

void CUI_Button::LoadFromLevelFile(FILE* _FILE)
{
	LoadResRef(m_pOriginTex, _FILE);
	int i = 0;
}


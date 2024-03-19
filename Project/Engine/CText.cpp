#include "pch.h"
#include "CText.h"

#include "CLevelMgr.h"
#include "CLevel.h"

#include "CFontMgr.h"
#include "CTransform.h"
#include "CEngine.h"

CText::CText()
	: CComponent(COMPONENT_TYPE::TEXTCOMP)
	, m_vFontPos{}
	, m_vOffsetPos{}
	, m_fFontSize(100.f)
	, m_FontColor(FONT_RGBA(255, 255, 255, 255))
	, m_Flags(0)
{
	m_Font = L"³Ø½¼Lv2°íµñ";
	m_Flags = FW1_CENTER | FW1_VCENTER;

}

CText::CText(const CText& _Origin)
	: CComponent(COMPONENT_TYPE::TEXTCOMP)
	, m_vFontPos{_Origin.m_vFontPos}
	, m_vOffsetPos{_Origin.m_vOffsetPos}
	, m_Font(_Origin.m_Font)
	, m_fFontSize(_Origin.m_fFontSize)
	, m_FontColor(_Origin.m_FontColor)
	, m_Flags(_Origin.m_Flags)
{
}

CText::~CText()
{
}

void CText::begin()
{
}

void CText::tick()
{
}

void CText::TextInit(const wchar_t* _Font, Vec2 _OffsetPos, float _FontSize, UINT _FontColor, UINT _Flags)
{
	m_vOffsetPos = _OffsetPos;
	m_Font = _Font;
	m_fFontSize = _FontSize;
	m_FontColor = _FontColor;
	m_Flags = _Flags;
}

void CText::finaltick()
{
	m_vFontPos = Transform()->GetRelativePos();

	m_vFontPos.y *= -1.f;

	RECT rect = {};

	GetClientRect(CEngine::GetInst()->GetMainWnd(), &rect);
	float width = (float)(rect.right - rect.left);
	float height = (float)(rect.bottom - rect.top);

	m_vFontPos.x += width / 2.f + m_vOffsetPos.x;
	m_vFontPos.y += height / 2.f - m_vOffsetPos.y;
}

void CText::render()
{
	LEVEL_STATE state = CLevelMgr::GetInst()->GetCurLevel()->GetState();
	if (state == LEVEL_STATE::PLAY  && m_str.size() != 0)
	{
		CFontMgr::GetInst()->DrawFont(m_str.c_str(), m_Font.c_str(), m_vFontPos.x, m_vFontPos.y, m_fFontSize, m_FontColor, m_Flags);
	}
}

void CText::SaveToLevelFile(FILE* _File)
{
	SaveWString(m_str, _File);
	fwrite(&m_vFontPos, sizeof(Vec2), 1, _File);
	fwrite(&m_vOffsetPos, sizeof(Vec2), 1, _File);
	fwrite(&m_Font, sizeof(UINT), 1, _File);
	fwrite(&m_fFontSize, sizeof(float), 1, _File);
	fwrite(&m_FontColor, sizeof(UINT), 1, _File);
	fwrite(&m_Flags, sizeof(UINT), 1, _File);
}

void CText::LoadFromLevelFile(FILE* _FILE)
{
	LoadWString(m_str, _FILE);
	fread(&m_vFontPos, sizeof(Vec2), 1, _FILE);
	fread(&m_vOffsetPos, sizeof(Vec2), 1, _FILE);
	fread(&m_Font, sizeof(UINT), 1, _FILE);
	fread(&m_fFontSize, sizeof(float), 1, _FILE);
	fread(&m_FontColor, sizeof(UINT), 1, _FILE);
	fread(&m_Flags, sizeof(UINT), 1, _FILE);
}
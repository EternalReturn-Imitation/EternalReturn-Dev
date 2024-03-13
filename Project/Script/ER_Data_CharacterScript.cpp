#include "pch.h"
#include "ER_Data_CharacterScript.h"

ER_Data_CharacterScript::ER_Data_CharacterScript()
	: CScript((UINT)SCRIPT_TYPE::ER_DATA_CHARACTERSCRIPT)
{
}

ER_Data_CharacterScript::~ER_Data_CharacterScript()
{
}

void ER_Data_CharacterScript::init()
{
	// [텍스쳐 로딩]
	if (0 < m_strKey.length())
	{
		wstring PortraitTexKey = L"Char_Portrait_";		// portrait
		wstring FullTexKey = L"Char_Full_";				// Full
		wstring MapTexKey = L"Char_Map_";				// map

		PortraitTexKey += m_strKey + L".png";
		FullTexKey += m_strKey + L".png";
		MapTexKey += m_strKey + L".png";

		m_PortraitTex = CResMgr::GetInst()->FindRes<CTexture>(PortraitTexKey);
		m_FullTax = CResMgr::GetInst()->FindRes<CTexture>(FullTexKey);
		m_MapTex = CResMgr::GetInst()->FindRes<CTexture>(MapTexKey);
	}
}

void ER_Data_CharacterScript::begin()
{
}

void ER_Data_CharacterScript::tick()
{
}

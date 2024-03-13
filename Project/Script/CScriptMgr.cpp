#include "pch.h"
#include "CScriptMgr.h"

#include "CCameraMoveScript.h"
#include "CFindPathScript.h"
#include "CMissileScript.h"
#include "CMonsterScript.h"
#include "CPlanetScript.h"
#include "CPlayerScript.h"
#include "ER_Data_CharacterScript.h"
#include "ER_Data_ItemScript.h"
#include "ER_PlayerScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CFindPathScript");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CMonsterScript");
	_vec.push_back(L"CPlanetScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"ER_Data_CharacterScript");
	_vec.push_back(L"ER_Data_ItemScript");
	_vec.push_back(L"ER_PlayerScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CFindPathScript" == _strScriptName)
		return new CFindPathScript;
	if (L"CMissileScript" == _strScriptName)
		return new CMissileScript;
	if (L"CMonsterScript" == _strScriptName)
		return new CMonsterScript;
	if (L"CPlanetScript" == _strScriptName)
		return new CPlanetScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"ER_Data_CharacterScript" == _strScriptName)
		return new ER_Data_CharacterScript;
	if (L"ER_Data_ItemScript" == _strScriptName)
		return new ER_Data_ItemScript;
	if (L"ER_PlayerScript" == _strScriptName)
		return new ER_PlayerScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CCameraMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::FINDPATHSCRIPT:
		return new CFindPathScript;
		break;
	case (UINT)SCRIPT_TYPE::MISSILESCRIPT:
		return new CMissileScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERSCRIPT:
		return new CMonsterScript;
		break;
	case (UINT)SCRIPT_TYPE::PLANETSCRIPT:
		return new CPlanetScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::ER_DATA_CHARACTERSCRIPT:
		return new ER_Data_CharacterScript;
		break;
	case (UINT)SCRIPT_TYPE::ER_DATA_ITEMSCRIPT:
		return new ER_Data_ItemScript;
		break;
	case (UINT)SCRIPT_TYPE::ER_PLAYERSCRIPT:
		return new ER_PlayerScript;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return L"CCameraMoveScript";
		break;

	case SCRIPT_TYPE::FINDPATHSCRIPT:
		return L"CFindPathScript";
		break;

	case SCRIPT_TYPE::MISSILESCRIPT:
		return L"CMissileScript";
		break;

	case SCRIPT_TYPE::MONSTERSCRIPT:
		return L"CMonsterScript";
		break;

	case SCRIPT_TYPE::PLANETSCRIPT:
		return L"CPlanetScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::ER_DATA_CHARACTERSCRIPT:
		return L"ER_Data_CharacterScript";
		break;

	case SCRIPT_TYPE::ER_DATA_ITEMSCRIPT:
		return L"ER_Data_ItemScript";
		break;

	case SCRIPT_TYPE::ER_PLAYERSCRIPT:
		return L"ER_PlayerScript";
		break;

	}
	return nullptr;
}
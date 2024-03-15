#include "pch.h"
#include "CScriptMgr.h"

#include "CCameraMoveScript.h"
#include "CFindPathScript.h"
#include "CMissileScript.h"
#include "CMonsterScript.h"
#include "CPlanetScript.h"
#include "CPlayerScript.h"
#include "ER_ActionScript_Aya.h"
#include "ER_ActionScript_Hyunwoo.h"
#include "ER_ActionScript_Jackie.h"
#include "ER_ActionScript_Rio.h"
#include "ER_ActionScript_Yuki.h"
#include "ER_CamControllerScript.h"
#include "ER_DataScript_Character.h"
#include "ER_DataScript_Item.h"
#include "ER_PlayerScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CFindPathScript");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CMonsterScript");
	_vec.push_back(L"CPlanetScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"ER_ActionScript_Aya");
	_vec.push_back(L"ER_ActionScript_Hyunwoo");
	_vec.push_back(L"ER_ActionScript_Jackie");
	_vec.push_back(L"ER_ActionScript_Rio");
	_vec.push_back(L"ER_ActionScript_Yuki");
	_vec.push_back(L"ER_CamControllerScript");
	_vec.push_back(L"ER_DataScript_Character");
	_vec.push_back(L"ER_DataScript_Item");
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
	if (L"ER_ActionScript_Aya" == _strScriptName)
		return new ER_ActionScript_Aya;
	if (L"ER_ActionScript_Hyunwoo" == _strScriptName)
		return new ER_ActionScript_Hyunwoo;
	if (L"ER_ActionScript_Jackie" == _strScriptName)
		return new ER_ActionScript_Jackie;
	if (L"ER_ActionScript_Rio" == _strScriptName)
		return new ER_ActionScript_Rio;
	if (L"ER_ActionScript_Yuki" == _strScriptName)
		return new ER_ActionScript_Yuki;
	if (L"ER_CamControllerScript" == _strScriptName)
		return new ER_CamControllerScript;
	if (L"ER_DataScript_Character" == _strScriptName)
		return new ER_DataScript_Character;
	if (L"ER_DataScript_Item" == _strScriptName)
		return new ER_DataScript_Item;
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
	case (UINT)SCRIPT_TYPE::ER_ACTIONSCRIPT_AYA:
		return new ER_ActionScript_Aya;
		break;
	case (UINT)SCRIPT_TYPE::ER_ACTIONSCRIPT_HYUNWOO:
		return new ER_ActionScript_Hyunwoo;
		break;
	case (UINT)SCRIPT_TYPE::ER_ACTIONSCRIPT_JACKIE:
		return new ER_ActionScript_Jackie;
		break;
	case (UINT)SCRIPT_TYPE::ER_ACTIONSCRIPT_RIO:
		return new ER_ActionScript_Rio;
		break;
	case (UINT)SCRIPT_TYPE::ER_ACTIONSCRIPT_YUKI:
		return new ER_ActionScript_Yuki;
		break;
	case (UINT)SCRIPT_TYPE::ER_CAMCONTROLLERSCRIPT:
		return new ER_CamControllerScript;
		break;
	case (UINT)SCRIPT_TYPE::ER_DATASCRIPT_CHARACTER:
		return new ER_DataScript_Character;
		break;
	case (UINT)SCRIPT_TYPE::ER_DATASCRIPT_ITEM:
		return new ER_DataScript_Item;
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

	case SCRIPT_TYPE::ER_ACTIONSCRIPT_AYA:
		return L"ER_ActionScript_Aya";
		break;

	case SCRIPT_TYPE::ER_ACTIONSCRIPT_HYUNWOO:
		return L"ER_ActionScript_Hyunwoo";
		break;

	case SCRIPT_TYPE::ER_ACTIONSCRIPT_JACKIE:
		return L"ER_ActionScript_Jackie";
		break;

	case SCRIPT_TYPE::ER_ACTIONSCRIPT_RIO:
		return L"ER_ActionScript_Rio";
		break;

	case SCRIPT_TYPE::ER_ACTIONSCRIPT_YUKI:
		return L"ER_ActionScript_Yuki";
		break;

	case SCRIPT_TYPE::ER_CAMCONTROLLERSCRIPT:
		return L"ER_CamControllerScript";
		break;

	case SCRIPT_TYPE::ER_DATASCRIPT_CHARACTER:
		return L"ER_DataScript_Character";
		break;

	case SCRIPT_TYPE::ER_DATASCRIPT_ITEM:
		return L"ER_DataScript_Item";
		break;

	case SCRIPT_TYPE::ER_PLAYERSCRIPT:
		return L"ER_PlayerScript";
		break;

	}
	return nullptr;
}
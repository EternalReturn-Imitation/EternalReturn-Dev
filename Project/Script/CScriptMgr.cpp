#include "pch.h"
#include "CScriptMgr.h"

#include "CEditCamControlScript.h"
#include "CFollowMainCamScript.h"
#include "CMissileScript.h"
#include "CMonsterScript.h"
#include "CPlanetScript.h"
#include "ER_ActionScript_Aya.h"
#include "ER_ActionScript_Hyunwoo.h"
#include "ER_ActionScript_Jackie.h"
#include "ER_ActionScript_Rio.h"
#include "ER_ActionScript_Yuki.h"
#include "ER_AyaQEffect.h"
#include "ER_CamControllerScript.h"
#include "ER_Cursor.h"
#include "ER_DataScript_Character.h"
#include "ER_DataScript_Item.h"
#include "ER_DataScript_ItemBox.h"
#include "ER_DataScript_LandMeshBase.h"
#include "ER_EffectScript.h"
#include "ER_PlayerScript.h"
#include "ER_PlayerScript_Range.h"
#include "ER_ProjectileScript.h"
#include "ER_UIScript_Gauge.h"
#include "ER_UIScript_ItemBox.h"
#include "ER_UIScript_ItemSlot.h"
#include "ER_UIScript_SkillSlot.h"
#include "ER_UIScript_SkillUpBtn.h"
#include "ER_UIScript_StatusBarGauge.h"
#include "ER_UIScript_TrackingStatusBar.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CEditCamControlScript");
	_vec.push_back(L"CFollowMainCamScript");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CMonsterScript");
	_vec.push_back(L"CPlanetScript");
	_vec.push_back(L"ER_ActionScript_Aya");
	_vec.push_back(L"ER_ActionScript_Hyunwoo");
	_vec.push_back(L"ER_ActionScript_Jackie");
	_vec.push_back(L"ER_ActionScript_Rio");
	_vec.push_back(L"ER_ActionScript_Yuki");
	_vec.push_back(L"ER_AyaQEffect");
	_vec.push_back(L"ER_CamControllerScript");
	_vec.push_back(L"ER_Cursor");
	_vec.push_back(L"ER_DataScript_Character");
	_vec.push_back(L"ER_DataScript_Item");
	_vec.push_back(L"ER_DataScript_ItemBox");
	_vec.push_back(L"ER_DataScript_LandMeshBase");
	_vec.push_back(L"ER_EffectScript");
	_vec.push_back(L"ER_PlayerScript");
	_vec.push_back(L"ER_PlayerScript_Range");
	_vec.push_back(L"ER_ProjectileScript");
	_vec.push_back(L"ER_UIScript_Gauge");
	_vec.push_back(L"ER_UIScript_ItemBox");
	_vec.push_back(L"ER_UIScript_ItemSlot");
	_vec.push_back(L"ER_UIScript_SkillSlot");
	_vec.push_back(L"ER_UIScript_SkillUpBtn");
	_vec.push_back(L"ER_UIScript_StatusBarGauge");
	_vec.push_back(L"ER_UIScript_TrackingStatusBar");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CEditCamControlScript" == _strScriptName)
		return new CEditCamControlScript;
	if (L"CFollowMainCamScript" == _strScriptName)
		return new CFollowMainCamScript;
	if (L"CMissileScript" == _strScriptName)
		return new CMissileScript;
	if (L"CMonsterScript" == _strScriptName)
		return new CMonsterScript;
	if (L"CPlanetScript" == _strScriptName)
		return new CPlanetScript;
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
	if (L"ER_AyaQEffect" == _strScriptName)
		return new ER_AyaQEffect;
	if (L"ER_CamControllerScript" == _strScriptName)
		return new ER_CamControllerScript;
	if (L"ER_Cursor" == _strScriptName)
		return new ER_Cursor;
	if (L"ER_DataScript_Character" == _strScriptName)
		return new ER_DataScript_Character;
	if (L"ER_DataScript_Item" == _strScriptName)
		return new ER_DataScript_Item;
	if (L"ER_DataScript_ItemBox" == _strScriptName)
		return new ER_DataScript_ItemBox;
	if (L"ER_DataScript_LandMeshBase" == _strScriptName)
		return new ER_DataScript_LandMeshBase;
	if (L"ER_EffectScript" == _strScriptName)
		return new ER_EffectScript;
	if (L"ER_PlayerScript" == _strScriptName)
		return new ER_PlayerScript;
	if (L"ER_PlayerScript_Range" == _strScriptName)
		return new ER_PlayerScript_Range;
	if (L"ER_ProjectileScript" == _strScriptName)
		return new ER_ProjectileScript;
	if (L"ER_UIScript_Gauge" == _strScriptName)
		return new ER_UIScript_Gauge;
	if (L"ER_UIScript_ItemBox" == _strScriptName)
		return new ER_UIScript_ItemBox;
	if (L"ER_UIScript_ItemSlot" == _strScriptName)
		return new ER_UIScript_ItemSlot;
	if (L"ER_UIScript_SkillSlot" == _strScriptName)
		return new ER_UIScript_SkillSlot;
	if (L"ER_UIScript_SkillUpBtn" == _strScriptName)
		return new ER_UIScript_SkillUpBtn;
	if (L"ER_UIScript_StatusBarGauge" == _strScriptName)
		return new ER_UIScript_StatusBarGauge;
	if (L"ER_UIScript_TrackingStatusBar" == _strScriptName)
		return new ER_UIScript_TrackingStatusBar;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::EDITCAMCONTROLSCRIPT:
		return new CEditCamControlScript;
		break;
	case (UINT)SCRIPT_TYPE::FOLLOWMAINCAMSCRIPT:
		return new CFollowMainCamScript;
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
	case (UINT)SCRIPT_TYPE::ER_AYAQEFFECT:
		return new ER_AyaQEffect;
		break;
	case (UINT)SCRIPT_TYPE::ER_CAMCONTROLLERSCRIPT:
		return new ER_CamControllerScript;
		break;
	case (UINT)SCRIPT_TYPE::ER_CURSOR:
		return new ER_Cursor;
		break;
	case (UINT)SCRIPT_TYPE::ER_DATASCRIPT_CHARACTER:
		return new ER_DataScript_Character;
		break;
	case (UINT)SCRIPT_TYPE::ER_DATASCRIPT_ITEM:
		return new ER_DataScript_Item;
		break;
	case (UINT)SCRIPT_TYPE::ER_DATASCRIPT_ITEMBOX:
		return new ER_DataScript_ItemBox;
		break;
	case (UINT)SCRIPT_TYPE::ER_DATASCRIPT_LANDMESHBASE:
		return new ER_DataScript_LandMeshBase;
		break;
	case (UINT)SCRIPT_TYPE::ER_EFFECTSCRIPT:
		return new ER_EffectScript;
		break;
	case (UINT)SCRIPT_TYPE::ER_PLAYERSCRIPT:
		return new ER_PlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::ER_PLAYERSCRIPT_RANGE:
		return new ER_PlayerScript_Range;
		break;
	case (UINT)SCRIPT_TYPE::ER_PROJECTILESCRIPT:
		return new ER_ProjectileScript;
		break;
	case (UINT)SCRIPT_TYPE::ER_UISCRIPT_GAUGE:
		return new ER_UIScript_Gauge;
		break;
	case (UINT)SCRIPT_TYPE::ER_UISCRIPT_ITEMBOX:
		return new ER_UIScript_ItemBox;
		break;
	case (UINT)SCRIPT_TYPE::ER_UISCRIPT_ITEMSLOT:
		return new ER_UIScript_ItemSlot;
		break;
	case (UINT)SCRIPT_TYPE::ER_UISCRIPT_SKILLSLOT:
		return new ER_UIScript_SkillSlot;
		break;
	case (UINT)SCRIPT_TYPE::ER_UISCRIPT_SKILLUPBTN:
		return new ER_UIScript_SkillUpBtn;
		break;
	case (UINT)SCRIPT_TYPE::ER_UISCRIPT_STATUSBARGAUGE:
		return new ER_UIScript_StatusBarGauge;
		break;
	case (UINT)SCRIPT_TYPE::ER_UISCRIPT_TRACKINGSTATUSBAR:
		return new ER_UIScript_TrackingStatusBar;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::EDITCAMCONTROLSCRIPT:
		return L"CEditCamControlScript";
		break;

	case SCRIPT_TYPE::FOLLOWMAINCAMSCRIPT:
		return L"CFollowMainCamScript";
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

	case SCRIPT_TYPE::ER_AYAQEFFECT:
		return L"ER_AyaQEffect";
		break;

	case SCRIPT_TYPE::ER_CAMCONTROLLERSCRIPT:
		return L"ER_CamControllerScript";
		break;

	case SCRIPT_TYPE::ER_CURSOR:
		return L"ER_Cursor";
		break;

	case SCRIPT_TYPE::ER_DATASCRIPT_CHARACTER:
		return L"ER_DataScript_Character";
		break;

	case SCRIPT_TYPE::ER_DATASCRIPT_ITEM:
		return L"ER_DataScript_Item";
		break;

	case SCRIPT_TYPE::ER_DATASCRIPT_ITEMBOX:
		return L"ER_DataScript_ItemBox";
		break;

	case SCRIPT_TYPE::ER_DATASCRIPT_LANDMESHBASE:
		return L"ER_DataScript_LandMeshBase";
		break;

	case SCRIPT_TYPE::ER_EFFECTSCRIPT:
		return L"ER_EffectScript";
		break;

	case SCRIPT_TYPE::ER_PLAYERSCRIPT:
		return L"ER_PlayerScript";
		break;

	case SCRIPT_TYPE::ER_PLAYERSCRIPT_RANGE:
		return L"ER_PlayerScript_Range";
		break;

	case SCRIPT_TYPE::ER_PROJECTILESCRIPT:
		return L"ER_ProjectileScript";
		break;

	case SCRIPT_TYPE::ER_UISCRIPT_GAUGE:
		return L"ER_UIScript_Gauge";
		break;

	case SCRIPT_TYPE::ER_UISCRIPT_ITEMBOX:
		return L"ER_UIScript_ItemBox";
		break;

	case SCRIPT_TYPE::ER_UISCRIPT_ITEMSLOT:
		return L"ER_UIScript_ItemSlot";
		break;

	case SCRIPT_TYPE::ER_UISCRIPT_SKILLSLOT:
		return L"ER_UIScript_SkillSlot";
		break;

	case SCRIPT_TYPE::ER_UISCRIPT_SKILLUPBTN:
		return L"ER_UIScript_SkillUpBtn";
		break;

	case SCRIPT_TYPE::ER_UISCRIPT_STATUSBARGAUGE:
		return L"ER_UIScript_StatusBarGauge";
		break;

	case SCRIPT_TYPE::ER_UISCRIPT_TRACKINGSTATUSBAR:
		return L"ER_UIScript_TrackingStatusBar";
		break;

	}
	return nullptr;
}
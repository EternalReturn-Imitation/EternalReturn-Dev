#pragma once

#include <vector>
#include <string>

enum SCRIPT_TYPE
{
	EDITCAMCONTROLSCRIPT,
	FOLLOWMAINCAMSCRIPT,
	MISSILESCRIPT,
	MONSTERSCRIPT,
	PLANETSCRIPT,
	UISCRIPT_BUTTON,
	ER_ACTIONSCRIPT_AYA,
	ER_ACTIONSCRIPT_HYUNWOO,
	ER_ACTIONSCRIPT_JACKIE,
	ER_ACTIONSCRIPT_RIO,
	ER_ACTIONSCRIPT_YUKI,
	ER_CAMCONTROLLERSCRIPT,
	ER_DATASCRIPT_CHARACTER,
	ER_DATASCRIPT_ITEM,
	ER_DATASCRIPT_ITEMBOX,
	ER_DATASCRIPT_LANDMESHBASE,
	ER_PLAYERSCRIPT,
	END,
};

using namespace std;

class CScript;

class CScriptMgr
{
public:
	static void GetScriptInfo(vector<wstring>& _vec);
	static CScript * GetScript(const wstring& _strScriptName);
	static CScript * GetScript(UINT _iScriptType);
	static const wchar_t * GetScriptName(CScript * _pScript);
};

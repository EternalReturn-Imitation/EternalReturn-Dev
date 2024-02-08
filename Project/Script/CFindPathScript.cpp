#include "pch.h"
#include "CFindPathScript.h"

#include <Engine/CNaviMap.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CGameObject.h>

CFindPathScript::CFindPathScript()
	:CScript((UINT)SCRIPT_TYPE::FINDPATHSCRIPT)
{
}

CFindPathScript::~CFindPathScript()
{
}

void CFindPathScript::begin()
{
}

void CFindPathScript::tick()
{
}

void CFindPathScript::SaveToLevelFile(FILE* _File)
{
}

void CFindPathScript::LoadFromLevelFile(FILE* _FILE)
{
}

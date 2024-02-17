#include "pch.h"
#include "CFindPath.h"

#include "CNaviMap.h"
#include "CLevelMgr.h"
#include "CGameObject.h"
#include "CKeyMgr.h"
#include "CPathFindMgr.h"
#include "CTransform.h"

CFindPath::CFindPath()
	:CComponent(COMPONENT_TYPE::FINDPATH)
	, LaycastResultTrigger(false)
{
}

CFindPath::~CFindPath()
{
}

void CFindPath::begin()
{
}

void CFindPath::tick()
{
}

void CFindPath::finaltick()
{
	if (KEY_PRESSED(KEY::LBTN))
	{
		LaycastResultTrigger = true;

		tNaviResult naviResult = CPathFindMgr::GetInst()->GetNaviResult();
		int a = 0;
		Vec3 curPos = GetOwner()->Transform()->GetRelativePos();
		GetOwner()->Transform()->SetRelativePos(Vec3(naviResult.resultPos.x,naviResult.resultPos.y,naviResult.resultPos.z));
	}

	//여기에 naviResult변수를 만들면 한 틱 느리게 값을 받음.
	if (LaycastResultTrigger) {

		LaycastResultTrigger = false;
	}
}

void CFindPath::SaveToLevelFile(FILE* _File)
{
}

void CFindPath::LoadFromLevelFile(FILE* _FILE)
{
}

void CFindPath::SaveToDB(int _gameObjectID, COMPONENT_TYPE _componentType)
{
}

void CFindPath::LoadFromDB(int _gameObjectID)
{
}

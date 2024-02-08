#include "pch.h"
#include "CFindPath.h"

#include "CNaviMap.h"
#include "CLevelMgr.h"
#include "CGameObject.h"
#include "CKeyMgr.h"
#include "CPathFindMgr.h"

CFindPath::CFindPath()
	:CComponent(COMPONENT_TYPE::FINDPATH)
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
		CNaviMap* naviMap = (CNaviMap*)(CPathFindMgr::GetInst()->GetNaviMapObject());
		if (naviMap == nullptr)
			assert(false);
		tNaviResult naviResult = naviMap->GetRayResultPos();
		int a = 0;
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

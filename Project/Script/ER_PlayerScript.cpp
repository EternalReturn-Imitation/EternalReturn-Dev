#include "pch.h"
#include "ER_PlayerScript.h"

#include <Engine\CPathFindMgr.h>
#include <Engine\CRenderMgr.h>

ER_PlayerScript::ER_PlayerScript()
	: CScript((UINT)SCRIPT_TYPE::ER_PLAYERSCRIPT)
{
}

ER_PlayerScript::~ER_PlayerScript()
{
}

void ER_PlayerScript::init()
{
}

void ER_PlayerScript::begin()
{
}

void ER_PlayerScript::tick()
{
	if (KEY_TAP(KEY::RBTN))
	{
		// cursor On UI
		if(false)
		{ }
		// cursor On Enemy
		else if(false)
		{ }
		// cursor On Box
		else if(false)
		{ }
		// cursor On Land (else)
		else
		{
			MoveToPoint();
		}
	}
}

void ER_PlayerScript::MoveToPoint()
{
	// 비교용 Map Collider 받기
	CGameObject* Map = CPathFindMgr::GetInst()->GetMapCollider();

	// 비교용 Main Camera의 Ray 받기
	CCamera* mainCam = CRenderMgr::GetInst()->GetMainCam();
	tRay ray = mainCam->GetRay();

	// 교차점 위치 얻기
	IntersectResult result = mainCam->IsCollidingBtwRayRect(ray, Map);
	Vec3 TargetPos = result.vCrossPoint;


	CFindPath* findpathcomp = GetOwner()->FindPath();
	
	if (findpathcomp)
	{
		DEBUG_LOG_COMMAND(ToString(GetOwner()->GetName()).c_str(), "FindPath", u8"이동명령");
		findpathcomp->FindPath(TargetPos);
	}
}
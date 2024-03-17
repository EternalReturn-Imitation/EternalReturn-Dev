#include "pch.h"
#include "ER_PlayerScript.h"

#include <Engine\CPathFindMgr.h>
#include <Engine\CRenderMgr.h>
#include <Engine\CCollisionMgr.h>

#include <Engine\CFindPath.h>

#include "ER_CamControllerScript.h"
#include "ER_ActionScript_Character.h"

ER_PlayerScript::ER_PlayerScript()
	: CScript((UINT)SCRIPT_TYPE::ER_PLAYERSCRIPT)
	, m_pActionScript(nullptr)
{
}

ER_PlayerScript::ER_PlayerScript(const ER_PlayerScript& _origin)
	: m_pActionScript(nullptr)
	, CScript((UINT)SCRIPT_TYPE::ER_PLAYERSCRIPT)
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
		CGameObject* pMainCam = CRenderMgr::GetInst()->GetMainCam()->GetOwner();
		pMainCam->GetScript<ER_CamControllerScript>()->SetTarget(GetOwner());

		m_pActionScript = GetOwner()->GetScript<ER_ActionScript_Character>();
}

void ER_PlayerScript::tick()
{
	// [ Mouse ]

	CGameObject* pFocusObj = GetFocusObj();	// 타겟 오브젝트
	Vec3 vTargetPoint = GetFocusPoint();	// 타겟 지점
	Vec3 vFocusDir = GetFocusDir(vTargetPoint);	// 타겟 방향

	if (KEY_TAP(KEY::LBTN))
	{
		Vec3 vPos = GetOwner()->Transform()->GetRelativePos();

		Vec3 testtp = vTargetPoint;
		Vec3 testvp = vPos;

		testtp.y = 0.f;
		testvp.y = 0.f;

		float dist = Vec3::Distance(testtp, testvp);

		Vec3 TargetPos = GetOwner()->FindPath()->findMaxClearDistance(vFocusDir, 0, dist);
		
		if (TargetPos != Vec3(0.f, 0.f, 0.f))
		{
			vPos.x += TargetPos.x;
			vPos.z += TargetPos.z;
			GetOwner()->Transform()->SetRelativePos(vPos);
		}
	}

	if (KEY_TAP(KEY::RBTN))
	{
		DEBUG_LOG_INPUT(ToString(GetOwner()->GetName()).c_str(), "TAP", "RBTN");

		// 현재 충돌중인 오브젝트
		// Get Cur Collision CGameObject Type 값

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
			m_pActionScript->Move(nullptr, vTargetPoint);
		}
	}
	else if (KEY_PRESSED(KEY::RBTN))
	{
		// 이동 유지
		m_pActionScript->Move(nullptr, vTargetPoint);
	}
	
	if (KEY_TAP(KEY::Q))
	{
		
	}
	if (KEY_TAP(KEY::W))
	{
	}
	if (KEY_TAP(KEY::E))
	{
	}
	if (KEY_TAP(KEY::R))
	{
	}
	
	if (KEY_TAP(KEY::A))
	{
	}

	// 정지
	if (KEY_TAP(KEY::S))
	{
		// 목적 경로 초기화
		GetOwner()->FindPath()->ClearPath();
	}
	
	if (KEY_TAP(KEY::X))
	{
		m_pActionScript->Rest();
	}
	
	if (KEY_TAP(KEY::TAB))
	{
	}



	// Camera Pixed this Obj
	if (KEY_TAP(KEY::Y))
	{
		CGameObject* pMainCam = CRenderMgr::GetInst()->GetMainCam()->GetOwner();
		pMainCam->GetScript<ER_CamControllerScript>()->CameraFixToggle();
	}
}

Vec3 ER_PlayerScript::GetFocusPoint()
{
	// 비교용 Map Collider 받기
	CGameObject* Map = CPathFindMgr::GetInst()->GetMapCollider();

	// 비교용 Main Camera의 Ray 받기
	CCamera* mainCam = CRenderMgr::GetInst()->GetMainCam();
	tRay ray = mainCam->GetRay();

	// 교차점 위치 얻기
	IntersectResult result = CCollisionMgr::GetInst()->IsCollidingBtwRayRect(ray, Map);
	Vec3 TargetPos = result.vCrossPoint;

	return TargetPos;
}

CGameObject* ER_PlayerScript::GetFocusObj()
{
	return nullptr;
}

Vec3 ER_PlayerScript::GetFocusDir(Vec3 _Point)
{
	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 Dir = (_Point - vPos).Normalize();
	
	return Dir;
}

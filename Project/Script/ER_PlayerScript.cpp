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

	tFSMData data = {};

	// [ Mouse Control ]
	// 이동
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
			data.v4Data = vTargetPoint;
			m_pActionScript->Move(data);
		}
	}
	else if (KEY_PRESSED(KEY::RBTN))
	{
		// 이동 유지
		data.v4Data = vTargetPoint;
		m_pActionScript->Move(data);
	}
	
	if (KEY_TAP(KEY::A))
	{
		// 공격
		// 공격 상태가 아닌경우 공격명령대기상태

		// 공격명령 상태라면
	}


	if (KEY_TAP(KEY::Q))
	{
		m_pActionScript->Skill_Q(data);
	}
	if (KEY_TAP(KEY::W))
	{
		m_pActionScript->Skill_W(data);
	}
	if (KEY_TAP(KEY::E))
	{
		data.v4Data = vTargetPoint;
		m_pActionScript->Skill_E(data);
	}
	if (KEY_TAP(KEY::R))
	{
		m_pActionScript->Skill_R(data);
	}
	

	// 정지
	if (KEY_TAP(KEY::S))
	{
		// 목적 경로 초기화
		GetOwner()->FindPath()->ClearPath();
	}
	
	// 휴식
	if (KEY_TAP(KEY::X))
	{
		m_pActionScript->Rest(data);
	}

	// UI

	if (KEY_TAP(KEY::ESC))
	{
		// 게임메뉴
		// 종료, 설정 등
	}
	
	if (KEY_TAP(KEY::TAB))
	{
		// 상태 UI 띄우기
	}



	// Camera Pixed this Obj
	if (KEY_TAP(KEY::Y))
	{
		CGameObject* pMainCam = CRenderMgr::GetInst()->GetMainCam()->GetOwner();
		pMainCam->GetScript<ER_CamControllerScript>()->CameraFixToggle();
	}

	if (KEY_PRESSED(KEY::SPACE))
	{
		CGameObject* pMainCam = CRenderMgr::GetInst()->GetMainCam()->GetOwner();
		pMainCam->GetScript<ER_CamControllerScript>()->FollowPlayerCamera();
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

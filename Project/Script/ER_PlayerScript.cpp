#include "pch.h"
#include "ER_PlayerScript.h"

#include <Engine\CPathFindMgr.h>
#include <Engine\CRenderMgr.h>

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
			m_pActionScript->Move(pFocusObj, vTargetPoint);
		}
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
	IntersectResult result = mainCam->IsCollidingBtwRayRect(ray, Map);
	Vec3 TargetPos = result.vCrossPoint;

	return TargetPos;
}

CGameObject* ER_PlayerScript::GetFocusObj()
{
	return nullptr;
}

Vec3 ER_PlayerScript::GetFocusDir(Vec3 _Point)
{
	// Cal Front Dir
	float yRad = atan2(-DirectX::XMVectorGetX(_Point),
		sqrt(DirectX::XMVectorGetY(_Point) *
			DirectX::XMVectorGetY(_Point) +
			DirectX::XMVectorGetZ(_Point) *
			DirectX::XMVectorGetZ(_Point)));

	// 위를 향하는 경우 radian 구해주기
	if (_Point.z > 0.0f)
		yRad = (DirectX::XM_PI - yRad);

	return Vec3(0.f, yRad, 0.f);
}

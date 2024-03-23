#include "pch.h"
#include "ER_PlayerScript.h"

#include <Engine\CPathFindMgr.h>
#include <Engine\CRenderMgr.h>
#include <Engine\CCollisionMgr.h>
#include <Engine\CUIMgr.h>

#include "ER_BattleSystem.h"
#include "ER_DataScript_Character.h"

#include <Engine\CFindPath.h>

#include "ER_CamControllerScript.h"
#include "ER_ActionScript_Character.h"

#include "CUIScript_Button.h"

#define LAYER_ITEMBOX 5
#define LAYER_MONSTER 11
#define LAYER_CHARACTER 12

ER_PlayerScript::ER_PlayerScript()
	: CScript((UINT)SCRIPT_TYPE::ER_PLAYERSCRIPT)
	, m_Character(nullptr)
	, m_pActionScript(nullptr)
	, m_pRangeObject(nullptr)
	, m_AttackCsr(false)
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
	m_Character = GetOwner()->GetScript <ER_DataScript_Character>();
}

void ER_PlayerScript::tick()
{
	// 캐릭터가 사망상태면 리턴
	if (m_Character->IsDeadState())
		return;
	
	// [ Mouse ]
	std::pair<CGameObject*,int> pTargetObj = GetFocusObj();	// 타겟 오브젝트
	Vec3 vCsrPoint = GetFocusPoint();	// 타겟 지점

	tFSMData data = {};

	// v4Data : 마우스 위치, 마우스 방향 활용가능
	// lParam : 현재 마우스위치에 충돌중인 오브젝트

	data.v4Data = vCsrPoint;
	data.lParam = (DWORD_PTR)pTargetObj.first;

	// [ Mouse Control ]
	// 마우스가 UI위에 있다면 입력을 받지 않는다.
	// 이동
	if (KEY_TAP(KEY::RBTN))
	{
		DEBUG_LOG_INPUT(ToString(GetOwner()->GetName()).c_str(), "TAP", "RBTN");

		// 현재 충돌중인 오브젝트
		// Get Cur Collision CGameObject Type 값

		// 공격대기상태라면 해제

		// cursor On UI
		CGameObject* FocusedUI = CUIMgr::GetInst()->GetFocusedUI();
		CGameObject* FocusedUIs = CUIMgr::GetInst()->GetFocusedPriorityUI();
		if (FocusedUI)
		{
			// UI는 UIMgr로 확인하기때문에 UIMgr에서 Focused UI가 있는지 판단.
			// UI는 UI에서 판단하기때문에 동작하지 않고 넘어간다.

			// 플레이어스크립트에서는 별도 동작 하지 않음
			int a = 0;
		}
		else if (pTargetObj.second == LAYER_ITEMBOX)
		{
			//몬스터인경우(Layer 이름 : ItemBox)
			m_pActionScript->Farming(data);
		}
		else if (pTargetObj.second == LAYER_MONSTER)
		{
			//몬스터인경우(Layer 이름 : Monster)
			int i = 0;
		}
		else if (pTargetObj.second == LAYER_CHARACTER)
		{
			m_pActionScript->Attack(data);

			//적 캐릭터인 경우(Layer 이름 : Character)
		}
		// cursor On Land (else)
		else
		{
			m_pActionScript->Move(data);
			
		}
		m_AttackCsr = false;
	}
	
	
	if (KEY_TAP(KEY::A))
	{
		m_AttackCsr = true;
		// 타겟오브젝트가 업없는경우  마우스 누르고있기로 이동 가능
		
		// 공격
		// 공격 상태가 아닌경우 공격대기상태설정
		// 공격대기상태는 마우스커서변경만 영향
	}


	if (m_AttackCsr && KEY_TAP(KEY::LBTN))
	{
		// KEY_TAP 앞에 공격명령상태 판단 참거짓 값 추가하여 (true && KEY_TAP(KEY::LBTN)) 으로 조건 생성
		// 공격대기상태 해제
		// pTargetObj가 Enemy인 경우 Attack 명령 실행
		if (pTargetObj.second == LAYER_CHARACTER)
		{
			m_pActionScript->Attack(data);
		}
		m_AttackCsr = false;
	}


	// v4Data : 마우스 위치, 마우스 방향 활용가능
	// lParam : 현재 마우스위치에 충돌중인 오브젝트
	
	if (KEY_TAP(KEY::Q))
	{
		m_pActionScript->Skill_Q(data);
		m_AttackCsr = false;
	}
	if (KEY_TAP(KEY::W))
	{
		m_pActionScript->Skill_W(data);
		m_AttackCsr = false;
	}
	if (KEY_TAP(KEY::E))
	{
		m_pActionScript->Skill_E(data);
		m_AttackCsr = false;
	}
	if (KEY_TAP(KEY::R))
	{
		m_pActionScript->Skill_R(data);
		m_AttackCsr = false;
	}
	

	// 정지
	if (KEY_TAP(KEY::S))
	{
		// 목적 경로 초기화
		GetOwner()->FindPath()->ClearPath();
		m_AttackCsr = false;
	}
	
	// 휴식
	if (KEY_TAP(KEY::X))
	{
		m_pActionScript->Rest(data);
		m_AttackCsr = false;
	}

	// 제작(CRAFT)
	if (KEY_TAP(KEY::APOSTROPHE)) {
		m_pActionScript->Craft(data);
		m_AttackCsr = false;
	}

	// UI
	if (KEY_TAP(KEY::ESC))
	{
		// 게임메뉴
		// 종료, 설정 등
		m_AttackCsr = false;
	}
	
	if (KEY_TAP(KEY::TAB))
	{
		// 상태 UI 띄우기
	}



	// 카메라 시야 캐릭터로 고정
	if (KEY_TAP(KEY::Y))
	{
		CGameObject* pMainCam = CRenderMgr::GetInst()->GetMainCam()->GetOwner();
		pMainCam->GetScript<ER_CamControllerScript>()->CameraFixToggle();
	}

	// SPACE 바 누르는동안 메인카메라가 캐릭터를 추적한다.
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

std::pair<CGameObject*,int> ER_PlayerScript::GetFocusObj()
{
	// 마우스 레이와 겹쳐진 오브젝트 판단
	vector<CGameObject*> rayColObjs = CCollisionMgr::GetInst()->GetCurRayColObjs();
	
	if (rayColObjs.size() == 1) 
		return make_pair(rayColObjs[0], rayColObjs[0]->GetLayerIndex());
	else if (rayColObjs.size() > 1)
		return make_pair(rayColObjs[0], rayColObjs[0]->GetLayerIndex());
	else
		return make_pair(nullptr, -1);
}

Vec3 ER_PlayerScript::GetFocusDir(Vec3 _Point)
{
	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 Dir = (_Point - vPos).Normalize();
	
	return Dir;
}
#include "pch.h"
#include "CUIMgr.h"

#include "CKeyMgr.h"
#include "CLevelMgr.h"

#include "CTransform.h"
#include "CUIComponent.h"

#include "CLevel.h"
#include "CLayer.h"


CUIMgr::CUIMgr()
    : m_pFocusedUI(nullptr)
    , m_pPriorityUI(nullptr)
{
}

CUIMgr::~CUIMgr()
{
}

void CUIMgr::init()
{
}

void CUIMgr::tick()
{
	bool bCsrTap = KEY_TAP(KEY::LBTN);
	bool bCsrReleased = KEY_RELEASE(KEY::LBTN);

	// 현재 레벨 접근
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();

	
	// UI Layer 가져오기
	const vector<CGameObject*>& vecUI = pCurLevel->GetLayer(31)->GetParentObject();

	// 실행중인 UI중 가장 앞에있는(z값이 낮은) UI를 포커스 UI로 한다.
	m_pFocusedUI = nullptr;

	// Focused UI
	for (int i = (int)vecUI.size() - 1; 0 <= i; --i)
	{
		CGameObject* pOwner = vecUI[i];
		CUIComponent* pUI = pOwner->GetUIComponent();

		// UI Componenet가 없다
		if (!pUI)
			continue;
		
		if (pUI->IsCsrOn() && !(pOwner->IsDead()))
		{
			if (m_pFocusedUI == nullptr)
			{
				m_pFocusedUI = pOwner;
				continue;
			}

			if (m_pFocusedUI->GetUIComponent()->IsCsrOn())
			{
				float PrevFocusZ = m_pFocusedUI->Transform()->GetWorldPos().z;
				float NewFocusZ = pOwner->Transform()->GetWorldPos().z;

				if (PrevFocusZ > NewFocusZ)
					m_pFocusedUI = pOwner;
			}
			else
			{
				m_pFocusedUI = pOwner;
			}
		}
	}

	for (int i = (int)vecUI.size() - 1; 0 <= i; --i)
	{
		CGameObject* pOwner = vecUI[i];

		if (pOwner->GetName() == L"UI_ItemBox_BackGround" && KEY_TAP(KEY::RBTN))
			int a = 0;

		m_pPriorityUI = GetPriorityUI(pOwner); // 현재 UI에 우선순위 UI가 있는지 확인한다.


		if (nullptr == m_pPriorityUI)	// 우선순위 UI가 없는 경우 다음 UI 검사를 진행
			continue;

		if (m_pFocusedUI != pOwner)		// 우선순위 UI가 포커싱된 UI가 아닌경우
			continue;

		CUIComponent* pPriorityUI = m_pPriorityUI->GetUIComponent();

		// 마우스온 이벤트 호출
		pPriorityUI->CsrOn();

		// 이번 update에 Lbtn이 눌렸다.
		if (bCsrTap)
		{
			// LbtnDoww 이벤트 호출
			pPriorityUI->CsrTap();
			break;

			// 렌더 순서는 z값에 의해 결정되어 여기서 관여하지 않는다.
		}

		// 이번 update에 Lbtn이 떼졌다.
		else if (bCsrReleased)
		{
			// 이전에 눌린적이 있다.
			if (pPriorityUI->IsCsrPressed())
			{
				// 클릭 이벤트 호출
				pPriorityUI->CsrClick();
				pPriorityUI->CsrRelease();
			}
		}
	}
}

CGameObject* CUIMgr::GetPriorityUI(CGameObject* _pParentUI)
{
	bool bCsrReleased = KEY_RELEASE(KEY::LBTN);

	CUIComponent* pPriorityUI = nullptr;

	CGameObject* pParentUI = _pParentUI;
	
	if (!IsUIObject(_pParentUI))
		return nullptr;

	static queue<CGameObject*> _queue;
	_queue.push(pParentUI);				// 인자로 들어온 부모UI를 가장 먼저 넣는다.

	while (!_queue.empty()) // queue에 객체가 남지 않을때까지 반복
	{
		CGameObject* pUIObj = _queue.front(); // Queue의 가장 앞에 있는 UI를 가져온다.
		CUIComponent* pUI = pUIObj->GetUIComponent();

		_queue.pop(); // 가져온 UI를 Queue에서 지운다.

		const std::vector<CGameObject*>& ChildUI = pUIObj->GetChild(); // 가져온 UI의 자식UI들을 가져온다.

		for (size_t i = 0; i < ChildUI.size(); ++i)
		{
			// UI Component를 보유중이라면 Queue에 집에 넣는다.
			if(IsUIObject(ChildUI[i]))
				_queue.push(ChildUI[i]);
		}

		// UI의 상태 확인
		if (pUI->IsCsrOn())
		{
			// 우선순위 UI로 지정됐지만, 더높은 우선순위를 가진 다른 UI가 나온 경우.
			// 기존 UI의 CsrPressed 상태를 갱신한다.
			if (bCsrReleased && nullptr != pPriorityUI && pPriorityUI->IsCsrPressed())
			{
				pPriorityUI->CsrRelease();
			}

			// 우선순위 UI로 지정
			pPriorityUI = pUI;
		}

		// 우선순위UI로 지정 안된 UI는 마우스 해제조건이 발생하면 LbtnUp 상태로 놓는다.
		else if (bCsrReleased && pUI->IsCsrPressed())
		{
			pUI->CsrRelease();
		}
	}

	if (nullptr == pPriorityUI)
		return nullptr;
	else
		return pPriorityUI->GetOwner();
}

bool CUIMgr::IsUIObject(CGameObject* _obj)
{
	return nullptr != _obj->GetUIComponent();
}

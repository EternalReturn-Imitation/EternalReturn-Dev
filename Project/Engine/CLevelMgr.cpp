#include "pch.h"
#include "CLevelMgr.h"
#include "CPathFindMgr.h"

#include "CCollisionMgr.h"
#include "CGameObject.h"
#include "CCamera.h"

#include "CLevel.h"
#include "CLayer.h"

CLevelMgr::CLevelMgr()
	: m_pCurLevel(nullptr)
{
}

CLevelMgr::~CLevelMgr()
{
	if (nullptr != m_pCurLevel)
		delete m_pCurLevel;
}

void CLevelMgr::init()
{
	m_pCurLevel = new CLevel;
	m_pCurLevel->ChangeState(LEVEL_STATE::PLAY);
}

void CLevelMgr::tick()
{
	m_pCurLevel->clear();

	if (LEVEL_STATE::PLAY == m_pCurLevel->GetState())
	{
		m_pCurLevel->tick();		
	}

	m_pCurLevel->finaltick();
}

void CLevelMgr::LevenEnter()
{
	CGameObject* MapCollider = m_pCurLevel->FindObjectByName(L"MapCollider");
	CPathFindMgr::GetInst()->SetMapCollider(MapCollider);
	
	// Camear 다시 세팅
	vector<CGameObject*> vecCam = m_pCurLevel->GetLayer(30)->GetParentObject();

	for (auto Cam : vecCam)
	{
		int i = Cam->Camera()->GetCamIdx();
		Cam->Camera()->SetCameraIndex(i);
	}

}


CGameObject* CLevelMgr::FindObjectByName(const wstring& _strName)
{
	return m_pCurLevel->FindObjectByName(_strName);	
}

void CLevelMgr::FindObjectByName(const wstring& _strName, vector<CGameObject*>& _vec)
{
	m_pCurLevel->FindObjectByName(_strName, _vec);
}

void CLevelMgr::ChangeLevel(CLevel* _NextLevel)
{
	if (nullptr != m_pCurLevel)
	{
		delete m_pCurLevel;
		m_pCurLevel = nullptr;
	}

	m_pCurLevel = _NextLevel;
}

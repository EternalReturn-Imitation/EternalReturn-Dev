#include "pch.h"
#include "CEngine.h"

#include "CDevice.h"
#include "CPathMgr.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CResMgr.h"
#include "CLevelMgr.h"
#include "CCollisionMgr.h"
#include "CRenderMgr.h"
#include "CEventMgr.h"
#include "CFontMgr.h"

CEngine::CEngine()
	: m_hWnd(nullptr)
{
}

CEngine::~CEngine()
{

}

int CEngine::init(HWND _hWnd, UINT _iWidth, UINT _iHeight)
{
	// 메인 윈도우 핸들
	m_hWnd = _hWnd;
	m_vResolution = Vec2((float)_iWidth, (float)_iHeight);

	// 해상도에 맞는 작업영역 크기 조정
	RECT rt = { 0, 0, (int)_iWidth, (int)_iHeight};
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(m_hWnd, nullptr, 10, 10, rt.right - rt.left, rt.bottom - rt.top, 0);
	ShowWindow(m_hWnd, true);


	// Device 초기화
	if (FAILED(CDevice::GetInst()->init(m_hWnd, _iWidth, _iHeight)))
	{
		MessageBox(nullptr, L"Device 초기화 실패", L"에러", MB_OK);
		return E_FAIL;
	}


	// Manager 초기화
	CPathMgr::GetInst()->init();

	CKeyMgr::GetInst()->init();

	CTimeMgr::GetInst()->init();

	CResMgr::GetInst()->init();

	CRenderMgr::GetInst()->init();

	CFontMgr::GetInst()->init();

	CLevelMgr::GetInst()->init();		
	


	return S_OK;
}

void CEngine::progress()
{
	// 엔진 매니저 및 레벨, 오브젝트 논리구조 실행
	tick();

	// 카메라를 지정, 카메라가 바라보는 시점으로 화면을 윈도우에 그림
	render();

	// Event 처리, tick 에서 바로 처리가 불가능한것들을 모아서 지연처리
	CEventMgr::GetInst()->tick();
}

void CEngine::tick()
{
	// Manager Tick
	CResMgr::GetInst()->tick();
	CTimeMgr::GetInst()->tick(); // DT(DeltaTime), FPS 구하기
	CKeyMgr::GetInst()->tick();	

	// FMOD Update
	CSound::g_pFMOD->update();

	// Level Update
	// Level 안에 존재하는 모든 GameObject 들이 Tick 을 호출받음
	CLevelMgr::GetInst()->tick();

	// Level 내에 GameObject 들의 변경점에 의해서 발생한 충돌을 체크한다.
	CCollisionMgr::GetInst()->tick();
}

void CEngine::render()
{	
	CRenderMgr::GetInst()->render();	

	// FPS, DT 출력
	CTimeMgr::GetInst()->render();
}

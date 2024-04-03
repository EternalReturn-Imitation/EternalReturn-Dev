#include "pch.h"
#include "ER_UIScript_TrackingStatusBar.h"

#include "ER_UIScript_Gauge.h"
#include "ER_DataScript_Character.h"
#include <Engine\CRenderMgr.h>
#include <Engine\CDevice.h>

ER_UIScript_TrackingStatusBar::ER_UIScript_TrackingStatusBar()
	: CScript((UINT)SCRIPT_TYPE::ER_UISCRIPT_TRACKINGSTATUSBAR)
	, Board(nullptr)
	, HPGuage(nullptr)
	, SPGuage(nullptr)
	, m_CharaterData(nullptr)
	, m_iLevel(0)
{
}

ER_UIScript_TrackingStatusBar::~ER_UIScript_TrackingStatusBar()
{
}

void ER_UIScript_TrackingStatusBar::init(CGameObject* _CharacterObj)
{
	ER_DataScript_Character* _data = _CharacterObj->GetScript<ER_DataScript_Character>();

	m_CharaterData = _data->GetStatus();

	Board = onew(CGameObject);
	AddComponents(Board, _TRANSFORM | _MESHRENDER | _TEXT);
	Board->SetName(L"UI_TrackingStatusBar_Board");

	// MeshRender Setting
	Board->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	Board->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"2DUISTD_Mtrl"), 0);
	Board->MeshRender()->GetDynamicMaterial(0);
	Board->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"TrackingStatusBar.png"));
	Board->LoadAllPrefabFromObjName();

	

	HPGuage = onew(CGameObject);
	ER_UIScript_Gauge* HPGuageScript = onew(ER_UIScript_Gauge);
	HPGuage->AddComponent(HPGuageScript);
	HPGuageScript->SetUI(0);
	HPGuageScript->RegistCharacetr(_data);

	SPGuage = onew(CGameObject);
	ER_UIScript_Gauge* SPGuageScript = onew(ER_UIScript_Gauge);
	SPGuage->AddComponent(SPGuageScript);
	SPGuageScript->SetUI(1);
	SPGuageScript->RegistCharacetr(_data);

	Board->AddChild(HPGuage);
	Board->AddChild(SPGuage);

	Board->Text()->SetText(1, _data->GetCharacterKorName().c_str());
	SpawnGameObject(Board, L"InGameUI");
}

void ER_UIScript_TrackingStatusBar::begin()
{
	Board->Text()->SetReference(0, 1, (DWORD_PTR)&m_CharaterData->iLevel);
}

void ER_UIScript_TrackingStatusBar::tick()
{
	TrackingCharacterPos();
}

void ER_UIScript_TrackingStatusBar::TrackingCharacterPos()
{
	Vec3 WorldPos = GetOwner()->Transform()->GetWorldPos();
	XMVECTOR pos = XMLoadFloat3(&WorldPos);

	// 뷰 매트릭스와 프로젝션 매트릭스 로드
	XMMATRIX view = CRenderMgr::GetInst()->GetMainCam()->GetViewMat();
	XMMATRIX proj = CRenderMgr::GetInst()->GetMainCam()->GetProjMat();

	Vec2 vResol = CDevice::GetInst()->GetRenderResolution();

	// 스크린 좌표로 변환
	XMVECTOR screenPos = XMVector3Project(pos, 0.0f, 0.0f, vResol.x, vResol.y, 0.0f, 1.0f, proj, view, XMMatrixIdentity());

	Vec3 FinalPos;
	XMStoreFloat3(&FinalPos, screenPos);

	FinalPos.x -= (vResol.x / 2.f);
	FinalPos.y -= (vResol.y / 2.f);
	FinalPos.y = -FinalPos.y;
	FinalPos.z = 0.f;

	Board->Transform()->SetRelativePos(Vec3(FinalPos.x, FinalPos.y + 135.f, -1.2f));
}


#include "pch.h"
#include "ER_UIScript_ItemBox.h"

#include <Engine\CResMgr.h>
#include <Engine\CRenderMgr.h>
#include <Engine\CTexture.h>
#include <Engine\CDevice.h>
#include <Engine\define.h>

#include "ER_UIScript_ItemSlot.h"
#include "ER_DataScript_ItemBox.h"

ER_UIScript_ItemBox::ER_UIScript_ItemBox()
	: CScript((UINT)SCRIPT_TYPE::ER_UISCRIPT_ITEMBOX)
	, m_pSlotUI{}
{
}

ER_UIScript_ItemBox::~ER_UIScript_ItemBox()
{
}

Vec3 ER_UIScript_ItemBox::GetUIPos(CGameObject* _Box)
{
	Vec3 WorldPos = _Box->Transform()->GetWorldPos();
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

	return FinalPos;
}

void ER_UIScript_ItemBox::init()
{
	GetOwner()->SetName(L"UI_ItemBox");
	AddComponents(GetOwner(), _TRANSFORM | _MESHRENDER | _UICOMPONENT);

	MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"2DUISTD_Mtrl"), 0);
	MeshRender()->GetDynamicMaterial(0);

	MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"ItemBoxUI.png"));

	for (int i = 0; i < (UINT)ITEMBOXSLOT::END; ++i)
	{
		wstring name = L"UI_ItemBox_Slot_" + std::to_wstring(i);
		m_pSlotUI[i] = onew(CGameObject);
		m_pSlotUI[i]->SetName(name);
		ER_UIScript_ItemSlot* ItemSlotScript = onew(ER_UIScript_ItemSlot);
		m_pSlotUI[i]->AddComponent(ItemSlotScript);
		ItemSlotScript->init();
		GetOwner()->AddChild(m_pSlotUI[i]);
	}
	GetOwner()->LoadAllPrefabFromObjName();

}

void ER_UIScript_ItemBox::begin()
{
}

void ER_UIScript_ItemBox::tick()
{
}

void ER_UIScript_ItemBox::RegistItemBoxData(ER_DataScript_ItemBox* _data)
{
	// _data->GetItemList();
	for (int i = 0; i < (UINT)ITEMBOXSLOT::END; ++i)
	{
		m_pSlotUI[i]->GetScript<ER_UIScript_ItemSlot>()->RegistSlotAdress(&_data->GetItemList()[i], ER_UIScript_ItemSlot::eSlotType::COMMON);
	}
	
	Vec3 vPos = GetUIPos(_data->GetOwner());

	Transform()->SetRelativePos(vPos.x + 100, vPos.y - 50.f, -1.3f);

}


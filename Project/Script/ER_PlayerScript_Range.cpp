#include "pch.h"
#include "ER_PlayerScript.h"
#include "ER_PlayerScript_Range.h"
#include "ER_DataScript_Character.h"

ER_PlayerScript_Range::ER_PlayerScript_Range()
	: CScript((UINT)SCRIPT_TYPE::ER_PLAYERSCRIPT_RANGE)
	, m_CharacterData(nullptr)
	, m_pOwner(nullptr)
	, m_bIsDecal(true)
{
}

ER_PlayerScript_Range::~ER_PlayerScript_Range()
{
}


void ER_PlayerScript_Range::init()
{
	CGameObject* Owner = GetOwner();
	AddComponents(Owner, _TRANSFORM);
	
	// 2D Circle
	if (m_bIsDecal)
	{
		AddComponents(Owner, _DECAL);
		Owner->Decal()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
		Owner->Decal()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DecalMtrl"), 0);
		Owner->Decal()->GetMaterial(0)->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"CircleRange.png"));
	}
	else
	{
		AddComponents(Owner, _MESHRENDER);

		Vec3 vRot = {};
		vRot.x = XMConvertToRadians(90);
		Owner->Transform()->SetRelativeRot(vRot);

		Owner->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CircleMesh"));
		Owner->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"), 0);
		Owner->MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"CircleRange2.png"));
	}



	Owner->SetEnable(true);
}

void ER_PlayerScript_Range::begin()
{
	
}

void ER_PlayerScript_Range::tick()
{
	float frange = m_CharacterData->GetStatus()->fAtakRange;
	Vec3 vPos = m_pOwner->Transform()->GetRelativePos();
	
	Vec3 vScale = {};
	if (m_bIsDecal)
	{
		vPos.y -= 0.5f;
		vScale = { frange * 2, 1.5f, frange * 2 };
	}
	else
		vScale = { frange * 2, frange * 2 ,1.f };

	Transform()->SetRelativePos(vPos);
	GetOwner()->Transform()->SetRelativeScale(vScale);

}

void ER_PlayerScript_Range::SetOwnerObj(CGameObject* _owner)
{
	m_pOwner = _owner;
	m_pOwner->GetScript<ER_PlayerScript>()->SetRangeObject(GetOwner());
	m_CharacterData = m_pOwner->GetScript<ER_DataScript_Character>();
	
	wstring name = m_CharacterData->GetCharacterName() + L"Ragne";
	GetOwner()->SetName(name);
}

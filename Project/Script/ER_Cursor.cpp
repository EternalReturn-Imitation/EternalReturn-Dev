#include "pch.h"
#include "ER_Cursor.h"

#include <Engine\CKeyMgr.h>

ER_Cursor::ER_Cursor()
	: CScript((UINT)SCRIPT_TYPE::ER_CURSOR)
	, m_CsrState(0)
	, m_pDragItemTex(nullptr)
{
}

ER_Cursor::~ER_Cursor()
{
}
void ER_Cursor::init()
{
	GetOwner()->SetName(L"Cursor");

	AddComponents(GetOwner(), _TRANSFORM | _MESHRENDER);
	MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"2DUIIndicator_Mtrl"), 0);
	MeshRender()->GetDynamicMaterial(0);

	MeshRender()->GetMaterial(0)->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Cursor_01.png"));			// 기본
	MeshRender()->GetMaterial(0)->SetTexParam(TEX_1, CResMgr::GetInst()->FindRes<CTexture>(L"Cursor_05.png"));			// 공격
	MeshRender()->GetMaterial(0)->SetTexParam(TEX_2, CResMgr::GetInst()->FindRes<CTexture>(L"Cursor_09.png"));			// 아이템
	MeshRender()->GetMaterial(0)->SetTexParam(TEX_3, CResMgr::GetInst()->FindRes<CTexture>(L"Ico_Ping_Target.png"));	// 타겟

	Transform()->SetRelativeScale(Vec3(35.f, 35.f, 1.f));
	Transform()->SetOffsetTrigger(true);

	m_pDragItemTex = onew(CGameObject);
	m_pDragItemTex->SetName(L"Cursor_ItemDragTex");
	AddComponents(m_pDragItemTex, _TRANSFORM | _MESHRENDER);
	m_pDragItemTex->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	m_pDragItemTex->Transform()->SetRelativePos(Vec3(-0.5, 0.5f, 1.1f));
	m_pDragItemTex->Transform()->SetRelativeScale(Vec3(1.88f, 1.1f, 1.f));

	GetOwner()->AddChild(m_pDragItemTex);
}

void ER_Cursor::tick()
{
	Vec2 v2Pos= CKeyMgr::GetInst()->GetUnProjectPos();
	v2Pos.x += 17.5;
	v2Pos.y -= 17.5;

	Vec3 vPos(v2Pos.x, v2Pos.y, -5.f);

	Transform()->SetRelativePos(vPos);

	if (m_CsrState != m_PrevState)
	{
		MeshRender()->GetMaterial(0)->SetScalarParam(INT_0, &m_CsrState);
		m_PrevState = m_CsrState;
	}
}

void ER_Cursor::SetState(int state)
{
	m_CsrState = state;
}

void ER_Cursor::CsrOn()
{
}

void ER_Cursor::CsrTap()
{
}

void ER_Cursor::CsrRelease()
{
}

void ER_Cursor::CsrClick()
{
}

void ER_Cursor::CsrBeginOn()
{
}

void ER_Cursor::CsrAway()
{
}

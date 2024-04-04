#include "pch.h"
#include "HyunwooREffect.h"
#include <Engine/CAnim2D.h>

HyunwooREffect::HyunwooREffect()
	: m_pRRange(nullptr)
	, m_pRAttack(nullptr)
	, m_fUVTime(0.49f)
{
}

HyunwooREffect::~HyunwooREffect()
{
}

void HyunwooREffect::begin()
{
}

void HyunwooREffect::tick()
{
	if (m_fUVTime > 0.5f && m_fUVTime<1.5f && m_pRAttack != nullptr) {
		m_fUVTime += DT;
		Vec3 scale = m_pRAttack->Transform()->GetRelativeScale();
		scale.y += 2.5 * DT;
		m_pRAttack->Transform()->SetRelativeScale(scale);
		Vec3 pos = m_pRAttack->Transform()->GetRelativePos();
		pos.z -= 1.25f * DT;
		m_pRAttack->Transform()->SetRelativePos(pos);
	}
}

void HyunwooREffect::SpawnEffect(Vec3 _pos, Vec3 _dir, float _scale)
{
	CGameObject* dummyParent01 = new CGameObject();
	AddComponents(dummyParent01, _TRANSFORM);

	//_dir.y += Deg2Rad(180.f);'
	//_dir.y = 0.f;
	dummyParent01->Transform()->SetRelativeRot(_dir);

	SpawnGameObject(dummyParent01, _pos, L"Effect");

	CGameObject* tdExample01 = new CGameObject();
	tdExample01->SetName(L"HyunwooREffect");
	AddComponents(tdExample01, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	tdExample01->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	tdExample01->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);
	tdExample01->MeshRender()->GetDynamicMaterial(0);

	tdExample01->Transform()->SetRelativeRot(Vec3(Deg2Rad(90.f), 0.f, 0.f));
	tdExample01->Transform()->SetRelativeScale(Vec3(4.0f, 5.0f, 1.0f));
	Vec3 pos = tdExample01->Transform()->GetRelativePos();
	pos.z -= 4.0f;
	tdExample01->Transform()->SetRelativePos(pos);

	Ptr<CTexture> animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"FX_BI_Jan_Skill01_01_Range_add02.png");
	tdExample01->Animator2D()->CreateAnimation(L"FX_BI_Jan_Skill01_01_Range_add02", animAtlas, Vec2(0.f, 0.f), Vec2(151.f, 204.f), Vec2(151.f, 204.f), 1, 1);
	tdExample01->Animator2D()->Play(L"FX_BI_Jan_Skill01_01_Range_add02", true);	

	dummyParent01->AddChild(tdExample01);

	m_pRRange = dummyParent01;

	CGameObject* tdExample02 = new CGameObject();
	tdExample02->SetName(L"HyunwooREffect02");
	AddComponents(tdExample02, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	tdExample02->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	tdExample02->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);
	tdExample02->MeshRender()->GetDynamicMaterial(0);

	tdExample02->Transform()->SetRelativeRot(Vec3(Deg2Rad(90.f), 0.f, 0.f));
	tdExample02->Transform()->SetRelativeScale(Vec3(4.0f, 2.5f, 1.0f));
	pos = tdExample02->Transform()->GetRelativePos();
	pos.z -= 2.75f;
	pos.y += 0.01f;
	tdExample02->Transform()->SetRelativePos(pos);

	animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"FX_BI_Jan_Skill01_01_Range_add03.png");
	tdExample02->Animator2D()->CreateAnimation(L"FX_BI_Jan_Skill01_01_Range_add03", animAtlas, Vec2(0.f, 0.f), Vec2(151.f, 204.f), Vec2(151.f, 204.f), 1, 1);
	tdExample02->Animator2D()->Play(L"FX_BI_Jan_Skill01_01_Range_add03", true);

	dummyParent01->AddChild(tdExample02);

	m_pRAttack = tdExample02;

	m_fUVTime = 0.51f;
}

void HyunwooREffect::DeleteREffect()
{
	if (m_pRRange != nullptr) {
		DestroyObject(m_pRRange);
		m_fUVTime = 0.49f;
		m_pRRange = nullptr;
		m_pRAttack = nullptr;
	}	
}

void HyunwooREffect::SaveToLevelFile(FILE* _File)
{
}

void HyunwooREffect::LoadFromLevelFile(FILE* _FILE)
{
}

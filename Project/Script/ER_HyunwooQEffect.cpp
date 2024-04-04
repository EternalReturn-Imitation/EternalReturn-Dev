#include "pch.h"
#include "ER_HyunwooQEffect.h"

void ER_HyunwooQEffect::begin()
{
}

void ER_HyunwooQEffect::tick()
{
}

void ER_HyunwooQEffect::SpawnEffect(Vec3 _pos, Vec3 _dir, float _scale)
{
	CGameObject* dummyParent01 = new CGameObject();
	AddComponents(dummyParent01, _TRANSFORM);

	_dir.y += Deg2Rad(180.f);
	dummyParent01->Transform()->SetRelativeRot(_dir);

	SpawnGameObject(dummyParent01, _pos, L"Effect");

	CGameObject* tdExample01 = new CGameObject();
	tdExample01->SetName(L"HyunwooQEffect01");
	AddComponents(tdExample01, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	tdExample01->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	tdExample01->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);
	tdExample01->MeshRender()->GetDynamicMaterial(0);

	tdExample01->Transform()->SetRelativeRot(Vec3(Deg2Rad(90.f), -Deg2Rad(45.f), 0.f));
	tdExample01->Transform()->SetRelativeScale(Vec3(5.0f, 5.0f, 2.0f));
	Vec3 pos = tdExample01->Transform()->GetRelativePos();
	pos.z += 2.5f;
	tdExample01->Transform()->SetRelativePos(pos);

	Ptr<CTexture> animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"FX_BI_GroundBomb_04.png");
	tdExample01->Animator2D()->CreateAnimation(L"FX_BI_GroundBomb_04", animAtlas, Vec2(0.f, 0.f), Vec2(256.f, 256.f), Vec2(256.f, 256.f), 1, 1);
	tdExample01->Animator2D()->Play(L"FX_BI_GroundBomb_04", true);

	dummyParent01->AddChild(tdExample01);

	std::thread t1(&ER_EffectScript::SpawnAnimationEffect, this, tdExample01, 1.0f, dummyParent01);
	t1.detach();
}

void ER_HyunwooQEffect::SaveToLevelFile(FILE* _File)
{
}

void ER_HyunwooQEffect::LoadFromLevelFile(FILE* _FILE)
{
}

ER_HyunwooQEffect::ER_HyunwooQEffect()
{
}

ER_HyunwooQEffect::~ER_HyunwooQEffect()
{
}

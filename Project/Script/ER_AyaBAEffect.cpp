#include "pch.h"
#include "ER_AyaBAEffect.h"

void ER_AyaBAEffect::begin()
{
}

void ER_AyaBAEffect::tick()
{
}

void ER_AyaBAEffect::SpawnEffect(Vec3 _pos, Vec3 _dir, float _scale)
{
	CGameObject* dummyParent = new CGameObject();
	AddComponents(dummyParent, _TRANSFORM);

	dummyParent->Transform()->SetRelativeRot(_dir);

	SpawnGameObject(dummyParent, _pos, L"Effect");

	CGameObject* tdExample = new CGameObject();
	AddComponents(tdExample, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	tdExample->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	tdExample->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);
	tdExample->MeshRender()->GetDynamicMaterial(0);

	tdExample->Transform()->SetRelativeRot(Vec3(Deg2Rad(90.f), Deg2Rad(-90.f), 0.f));

	Ptr<CTexture> animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"FX_BI_Shoot_012_Work.png");
	tdExample->Animator2D()->CreateAnimation(L"FX_BI_Shoot_012_Work", animAtlas, Vec2(0.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 128.f), 4, 1);
	tdExample->Animator2D()->Play(L"FX_BI_Shoot_012_Work", true);

	dummyParent->AddChild(tdExample);

	std::thread t(&ER_EffectScript::SpawnAnimationEffect, this, tdExample, 0.2f, dummyParent);
	t.detach();
}

void ER_AyaBAEffect::SaveToLevelFile(FILE* _File)
{
}

void ER_AyaBAEffect::LoadFromLevelFile(FILE* _FILE)
{
}

ER_AyaBAEffect::ER_AyaBAEffect()
{
}

ER_AyaBAEffect::~ER_AyaBAEffect()
{
}

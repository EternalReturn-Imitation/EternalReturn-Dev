#include "pch.h"
#include "ER_YukiREffect.h"

void ER_YukiREffect::begin()
{
}

void ER_YukiREffect::tick()
{
}

void ER_YukiREffect::SpawnEffect(Vec3 _pos, Vec3 _dir, float _scale)
{
}

void ER_YukiREffect::AreaSpawn(Vec3 _pos, Vec3 _dir, float _scale)
{
	CGameObject* dummyParent01 = new CGameObject();
	AddComponents(dummyParent01, _TRANSFORM);

	_dir.y += Deg2Rad(180.f);
	dummyParent01->Transform()->SetRelativeRot(_dir);

	SpawnGameObject(dummyParent01, _pos, L"Effect");

	CGameObject* tdExample01 = new CGameObject();
	tdExample01->SetName(L"YukiREffect");
	AddComponents(tdExample01, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	tdExample01->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	tdExample01->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);
	tdExample01->MeshRender()->GetDynamicMaterial(0);

	tdExample01->Transform()->SetRelativeRot(Vec3(Deg2Rad(90.f), 0.f, 0.f));
	tdExample01->Transform()->SetRelativeScale(Vec3(12.0f, 6.0f, 6.0f));
	Vec3 pos = tdExample01->Transform()->GetRelativePos();
	pos.z += 2.5f;
	tdExample01->Transform()->SetRelativePos(pos);

	Ptr<CTexture> animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"FX_BI_Yuki_02_Red.png");
	tdExample01->Animator2D()->CreateAnimation(L"FX_BI_Yuki_02_Red", animAtlas, Vec2(0.f, 0.f), Vec2(512.f, 256.f), Vec2(512.f, 256.f), 1, 1);
	tdExample01->Animator2D()->Play(L"FX_BI_Yuki_02_Red", true);

	dummyParent01->AddChild(tdExample01);

	std::thread t1(&ER_EffectScript::SpawnAnimationEffect, this, tdExample01, 1.0f, dummyParent01);
	t1.detach();
}

void ER_YukiREffect::DistortionSpawn(Vec3 _pos, Vec3 _dir, float _scale)
{
	CGameObject* dummyParent01 = new CGameObject();
	AddComponents(dummyParent01, _TRANSFORM);

	dummyParent01->Transform()->SetRelativeRot(_dir);

	SpawnGameObject(dummyParent01, _pos, L"Effect");

	CGameObject* tdExample = new CGameObject();
	tdExample->SetName(L"Distortion");
	AddComponents(tdExample, _TRANSFORM | _MESHRENDER);
	tdExample->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	tdExample->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DistortionMtrl"), 0);

	tdExample->Transform()->SetRelativeRot(Vec3(Deg2Rad(90.f), 0.f, 0.f));
	tdExample->Transform()->SetRelativeScale(12.0f, 6.f, 1.f);
	tdExample->Transform()->SetRelativePos(Vec3(0.f, 0.f, -3.f));

	dummyParent01->AddChild(tdExample);

	std::thread t1(&ER_EffectScript::SpawnAnimationEffect, this, tdExample, 1.0f, dummyParent01);
	t1.detach();
}

void ER_YukiREffect::SaveToLevelFile(FILE* _File)
{
}

void ER_YukiREffect::LoadFromLevelFile(FILE* _FILE)
{
}

ER_YukiREffect::ER_YukiREffect()
{
}

ER_YukiREffect::~ER_YukiREffect()
{
}

#include "pch.h"
#include "ER_YukiEEffect.h"

ER_YukiEEffect::ER_YukiEEffect()
{
}

ER_YukiEEffect::~ER_YukiEEffect()
{
}

void ER_YukiEEffect::begin()
{
}

void ER_YukiEEffect::tick()
{
}

void ER_YukiEEffect::SpawnEffect(Vec3 _pos, Vec3 _dir, float _scale)
{
	_pos.y += 1.f;

	CGameObject* dummyParent01 = new CGameObject();
	AddComponents(dummyParent01, _TRANSFORM);
	
	dummyParent01->Transform()->SetRelativeRot(_dir);

	SpawnGameObject(dummyParent01, _pos, L"Effect");

	CGameObject* tdExample01 = new CGameObject();
	tdExample01->SetName(L"YukiEEffectfirst");
	AddComponents(tdExample01, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	tdExample01->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	tdExample01->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);
	tdExample01->MeshRender()->GetDynamicMaterial(0);
	tdExample01->Transform()->SetRelativeRot(Vec3(Deg2Rad(180.f), -Deg2Rad(90.f), 0.f));
	tdExample01->Transform()->SetRelativeScale(Vec3(4.0f, 4.0f, 4.0f));

	Ptr<CTexture> animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"FX_BI_Sword_01.png");
	tdExample01->Animator2D()->CreateAnimation(L"FX_BI_Sword_01", animAtlas, Vec2(0.f, 0.f), Vec2(256.f, 256.f), Vec2(256.f, 256.f), 1, 1);
	tdExample01->Animator2D()->Play(L"FX_BI_Sword_01", true);

	dummyParent01->AddChild(tdExample01);


	CGameObject* dummyParent02 = new CGameObject();
	AddComponents(dummyParent02, _TRANSFORM);
	
	dummyParent02->Transform()->SetRelativeRot(_dir);
	
	SpawnGameObject(dummyParent02, _pos, L"Effect");
	
	CGameObject* tdExample02 = new CGameObject();
	tdExample02->SetName(L"YukiEEffectfirst");
	AddComponents(tdExample02, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	tdExample02->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	tdExample02->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);
	tdExample02->MeshRender()->GetDynamicMaterial(0);
	tdExample02->Transform()->SetRelativeRot(Vec3(Deg2Rad(180.f), -Deg2Rad(90.f), Deg2Rad(70.f)));
	tdExample02->Transform()->SetRelativeScale(Vec3(4.0f, 4.0f, 4.0f));
	
	animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"FX_BI_Sword_01_Violet01.png");
	tdExample02->Animator2D()->CreateAnimation(L"FX_BI_Sword_01_Violet01", animAtlas, Vec2(0.f, 0.f), Vec2(256.f, 256.f), Vec2(256.f, 256.f), 1, 1);
	tdExample02->Animator2D()->Play(L"FX_BI_Sword_01_Violet01", true);
	
	dummyParent02->AddChild(tdExample02);

	std::thread t1(&ER_EffectScript::SpawnAnimationEffect, this, tdExample01, 0.2f, dummyParent01);
	t1.detach();

	std::thread t2(&ER_EffectScript::SpawnAnimationEffect, this, tdExample02, 1.0f, dummyParent02);
	t2.detach();
}

void ER_YukiEEffect::SaveToLevelFile(FILE* _File)
{
}

void ER_YukiEEffect::LoadFromLevelFile(FILE* _FILE)
{
}

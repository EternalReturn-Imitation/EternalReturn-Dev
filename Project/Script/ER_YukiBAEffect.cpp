#include "pch.h"
#include "ER_YukiBAEffect.h"

void ER_YukiBAEffect::begin()
{
}

void ER_YukiBAEffect::tick()
{
}

void ER_YukiBAEffect::SpawnEffect(Vec3 _pos, Vec3 _dir, float _scale)
{	
}

void ER_YukiBAEffect::SpawnFirstEffect(Vec3 _pos, Vec3 _dir, float _scale)
{
	CGameObject* dummyParent = new CGameObject();
	AddComponents(dummyParent, _TRANSFORM);

	_dir.y += Deg2Rad(30.f);
	dummyParent->Transform()->SetRelativeRot(_dir);

	SpawnGameObject(dummyParent, _pos, L"Effect");

	CGameObject* tdExample = new CGameObject();
	AddComponents(tdExample, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	tdExample->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	tdExample->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);

	tdExample->Transform()->SetRelativeRot(Vec3(Deg2Rad(70.f), 0.f, 0.f));
	tdExample->Transform()->SetRelativeScale(Vec3(4.0f, 4.0f, 4.0f));

	Ptr<CTexture> animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"FX_BI_Sword_02AB.png");
	tdExample->Animator2D()->CreateAnimation(L"FX_BI_Sword_02AB", animAtlas, Vec2(0.f, 0.f), Vec2(256.f, 256.f), Vec2(256.f, 256.f), 1, 1);
	tdExample->Animator2D()->Play(L"FX_BI_Sword_02AB", true);

	dummyParent->AddChild(tdExample);

	std::thread t(&ER_EffectScript::SpawnAnimationEffect, this, tdExample, 0.2f, dummyParent);
	t.detach();
}

void ER_YukiBAEffect::SpawnSecondEffect(Vec3 _pos, Vec3 _dir, float _scale)
{
	CGameObject* dummyParent = new CGameObject();
	AddComponents(dummyParent, _TRANSFORM);

	_dir.y -= Deg2Rad(90.f);
	dummyParent->Transform()->SetRelativeRot(_dir);

	SpawnGameObject(dummyParent, _pos, L"Effect");

	CGameObject* tdExample = new CGameObject();
	AddComponents(tdExample, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	tdExample->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	tdExample->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);

	tdExample->Transform()->SetRelativeRot(Vec3(Deg2Rad(70.f), 0.f, 0.f));
	tdExample->Transform()->SetRelativeScale(Vec3(4.0f, 4.0f, 4.0f));

	Ptr<CTexture> animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"FX_BI_Sword_05.png");
	tdExample->Animator2D()->CreateAnimation(L"FX_BI_Sword_05", animAtlas, Vec2(0.f, 0.f), Vec2(256.f, 256.f), Vec2(256.f, 256.f), 1, 1);
	tdExample->Animator2D()->Play(L"FX_BI_Sword_05", true);

	dummyParent->AddChild(tdExample);

	std::thread t(&ER_EffectScript::SpawnAnimationEffect, this, tdExample, 0.2f, dummyParent);
	t.detach();
}

void ER_YukiBAEffect::SaveToLevelFile(FILE* _File)
{
}

void ER_YukiBAEffect::LoadFromLevelFile(FILE* _FILE)
{
}

ER_YukiBAEffect::ER_YukiBAEffect()
{
}

ER_YukiBAEffect::~ER_YukiBAEffect()
{
}

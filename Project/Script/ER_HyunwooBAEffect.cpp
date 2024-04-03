#include "pch.h"
#include "ER_HyunwooBAEffect.h"

void ER_HyunwooBAEffect::begin()
{
}

void ER_HyunwooBAEffect::tick()
{
}

void ER_HyunwooBAEffect::SpawnEffect(Vec3 _pos, Vec3 _dir, float _scale)
{
}

void ER_HyunwooBAEffect::SpawnFirstEffect(Vec3 _pos, Vec3 _dir, float _scale)
{
	_pos.y += 1.5f;

	CGameObject* dummyParent = new CGameObject();
	AddComponents(dummyParent, _TRANSFORM);

	_dir.y += Deg2Rad(30.f);
	dummyParent->Transform()->SetRelativeRot(_dir);

	SpawnGameObject(dummyParent, _pos, L"Effect");

	CGameObject* tdExample = new CGameObject();
	AddComponents(tdExample, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	tdExample->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	tdExample->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);

	Vec3 pos = tdExample->Transform()->GetRelativePos();
	pos.x += 0.6f;
	tdExample->Transform()->SetRelativePos(pos);
	tdExample->Transform()->SetRelativeRot(Vec3(Deg2Rad(90.f), 0.f, 0.f));
	tdExample->Transform()->SetRelativeScale(Vec3(0.5f, 3.0f, 0.5f));

	Ptr<CTexture> animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"FX_Wind02.png");
	tdExample->Animator2D()->CreateAnimation(L"FX_Wind02", animAtlas, Vec2(51.f, 0.f), Vec2(51.f, 256.f), Vec2(51.f, 256.f), 1, 12);
	tdExample->Animator2D()->Play(L"FX_Wind02", true);

	dummyParent->AddChild(tdExample);

	std::thread t(&ER_EffectScript::SpawnAnimationEffect, this, tdExample, 0.2f, dummyParent);
	t.detach();
}

void ER_HyunwooBAEffect::SpawnSecondEffect(Vec3 _pos, Vec3 _dir, float _scale)
{
	_pos.y += 1.5f;

	CGameObject* dummyParent = new CGameObject();
	AddComponents(dummyParent, _TRANSFORM);

	_dir.y -= Deg2Rad(30.f);
	dummyParent->Transform()->SetRelativeRot(_dir);

	SpawnGameObject(dummyParent, _pos, L"Effect");

	CGameObject* tdExample = new CGameObject();
	AddComponents(tdExample, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	tdExample->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	tdExample->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);

	Vec3 pos = tdExample->Transform()->GetRelativePos();
	pos.x -= 0.5f;
	pos.z += 0.2f;
	tdExample->Transform()->SetRelativePos(pos);
	tdExample->Transform()->SetRelativeRot(Vec3(Deg2Rad(90.f), 0.f, 0.f));
	tdExample->Transform()->SetRelativeScale(Vec3(0.5f, 3.0f, 0.5f));

	Ptr<CTexture> animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"FX_Wind02.png");
	tdExample->Animator2D()->CreateAnimation(L"FX_Wind02", animAtlas, Vec2(204.f, 0.f), Vec2(51.f, 256.f), Vec2(51.f, 256.f), 1, 12);
	tdExample->Animator2D()->Play(L"FX_Wind02", true);

	dummyParent->AddChild(tdExample);

	std::thread t(&ER_EffectScript::SpawnAnimationEffect, this, tdExample, 0.2f, dummyParent);
	t.detach();
}

void ER_HyunwooBAEffect::SaveToLevelFile(FILE* _File)
{
}

void ER_HyunwooBAEffect::LoadFromLevelFile(FILE* _FILE)
{
}

ER_HyunwooBAEffect::ER_HyunwooBAEffect()
{
}

ER_HyunwooBAEffect::~ER_HyunwooBAEffect()
{
}

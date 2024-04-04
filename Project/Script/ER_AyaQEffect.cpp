#include "pch.h"
#include "ER_AyaQEffect.h"

void ER_AyaQEffect::begin()
{
}

void ER_AyaQEffect::tick()
{
}

void ER_AyaQEffect::SpawnEffect(Vec3 _pos, Vec3 _dir, float _scale)
{
	CGameObject* dummyParent = new CGameObject();
	AddComponents(dummyParent, _TRANSFORM);

	dummyParent->Transform()->SetRelativeRot(_dir);

	SpawnGameObject(dummyParent, _pos, L"Effect");
#pragma region effect
	CGameObject* tdExample = onew(CGameObject);
	tdExample->SetName(L"QEffectAnim");
	AddComponents(tdExample, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	tdExample->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	tdExample->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);
	tdExample->MeshRender()->GetDynamicMaterial(0);
	Ptr<CTexture> animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"rShot.png");
	tdExample->Animator2D()->CreateAnimation(L"rShot", animAtlas, Vec2(0.f, 0.f), Vec2(256.f, 256.f), Vec2(256.f, 256.f), 1, 15);
	tdExample->Animator2D()->Play(L"rShot", true);

	dummyParent->AddChild(tdExample);
#pragma endregion

	std::thread t(&ER_EffectScript::EffectRotation, this, tdExample, Vec3(3.0f, 3.0f, 3.0f), Vec3(3.0f, 3.0f, 3.0f), true, 0.2f, dummyParent);
	t.detach();
}

void ER_AyaQEffect::SaveToLevelFile(FILE* _File)
{
}

void ER_AyaQEffect::LoadFromLevelFile(FILE* _FILE)
{
}

ER_AyaQEffect::ER_AyaQEffect()
{
}

ER_AyaQEffect::~ER_AyaQEffect()
{
}

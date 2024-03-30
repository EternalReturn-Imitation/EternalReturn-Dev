#include "pch.h"
#include "ER_RioBAEffect.h"

#include "ER_ActionScript_Rio.h"
#include <thread>

ER_RioBAEffect::ER_RioBAEffect()
{
}

ER_RioBAEffect::~ER_RioBAEffect()
{
}

void ER_RioBAEffect::begin()
{	
}

void ER_RioBAEffect::tick()
{
}

void ER_RioBAEffect::SpawnEffect()
{
	CGameObject* tdExample = new CGameObject();
	AddComponents(tdExample, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	tdExample->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	tdExample->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);

	tdExample->Transform()->SetRelativePos(Vec3(0.08f, 1.31f, -0.76f));

	Ptr<CTexture> animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"wWind.png");
	tdExample->Animator2D()->CreateAnimation(L"wWind", animAtlas, Vec2(0.f, 0.f), Vec2(100.f, 100.f), Vec2(100.f, 100.f), 1, 15);
	tdExample->Animator2D()->Play(L"wWind", true);

	m_aEffect[0] = tdExample;

	SpawnGameObjectToParent(tdExample, GetOwner());

	if (GetOwner()->GetScript<ER_ActionScript_Rio>()->GetBowType()) {
		std::thread t(&ER_EffectScript::EffectRotation, this, tdExample, Vec3(1.0f, 1.0f, 1.0f), Vec3(3.0f, 3.0f, 3.0f), true, 0.2f);
		t.detach();
	}
	else {
		std::thread t(&ER_EffectScript::EffectRotation, this, tdExample, Vec3(1.0f, 1.0f, 1.0f), Vec3(1.0f, 1.0f, 1.0f), true, 0.2f);
		t.detach();
	}	
}

void ER_RioBAEffect::SaveToLevelFile(FILE* _File)
{
}

void ER_RioBAEffect::LoadFromLevelFile(FILE* _FILE)
{
}
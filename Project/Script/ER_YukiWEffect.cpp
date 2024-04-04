#include "pch.h"
#include "ER_YukiWEffect.h"

#include <Engine\CAnim2D.h>

ER_YukiWEffect::ER_YukiWEffect()
	:count(0)
	, m_pSakuraAnimation(nullptr)
{
}

ER_YukiWEffect::~ER_YukiWEffect()
{
}

void ER_YukiWEffect::begin()
{
}

void ER_YukiWEffect::tick()
{
	wstring yukiAnimTex[5] = { L"FX_BI_Yuki_01SE02",L"FX_BI_Yuki_01SE03" ,L"FX_BI_Yuki_01SE04" ,L"FX_BI_Yuki_01SE05" ,L"FX_BI_Yuki_01SE06" };

	if (m_pSakuraAnimation != nullptr && m_pSakuraAnimation->Animator2D()->GetAnim()->IsFinish()) {
		if (count >= 5) {
			DestroyObject(m_pSakuraAnimation);
			m_pSakuraAnimation = nullptr;
			count = 0;
		}
		else {
			m_pSakuraAnimation->Animator2D()->Play(yukiAnimTex[count], false);
			++count;
		}		
	}
}

void ER_YukiWEffect::SpawnEffect(Vec3 _pos, Vec3 _dir, float _scale)
{
	CGameObject* tdExample = new CGameObject();
	tdExample->SetName(L"YukiWEffect");
	AddComponents(tdExample, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	tdExample->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	tdExample->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);
	tdExample->MeshRender()->GetDynamicMaterial(0);

	tdExample->Transform()->SetRelativeRot(Vec3(Deg2Rad(90.f), 0.f, 0.f));
	tdExample->Transform()->SetRelativeScale(Vec3(2.0f, 2.0f, 2.0f));

	Ptr<CTexture> animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"FX_BI_Yuki_01SE.png");
	tdExample->Animator2D()->CreateAnimation(L"FX_BI_Yuki_01SE01", animAtlas, Vec2(0.f, 0.f), Vec2(85.3f, 85.3f), Vec2(85.3f, 85.3f), 6, 24);
	tdExample->Animator2D()->CreateAnimation(L"FX_BI_Yuki_01SE02", animAtlas, Vec2(0.f, 85.3f), Vec2(85.3f, 85.3f), Vec2(85.3f, 85.3f), 6, 24);
	tdExample->Animator2D()->CreateAnimation(L"FX_BI_Yuki_01SE03", animAtlas, Vec2(0.f, 170.6f), Vec2(85.3f, 85.3f), Vec2(85.3f, 85.3f), 6, 24);
	tdExample->Animator2D()->CreateAnimation(L"FX_BI_Yuki_01SE04", animAtlas, Vec2(0.f, 255.9f), Vec2(85.3f, 85.3f), Vec2(85.3f, 85.3f), 6, 24);
	tdExample->Animator2D()->CreateAnimation(L"FX_BI_Yuki_01SE05", animAtlas, Vec2(0.f, 341.2f), Vec2(85.3f, 85.3f), Vec2(85.3f, 85.3f), 6, 24);
	tdExample->Animator2D()->CreateAnimation(L"FX_BI_Yuki_01SE06", animAtlas, Vec2(0.f, 426.5f), Vec2(85.3f, 85.3f), Vec2(85.3f, 85.3f), 6, 24);
	tdExample->Animator2D()->Play(L"FX_BI_Yuki_01SE01", false);

	m_pSakuraAnimation = tdExample;

	SpawnGameObject(tdExample, _pos, L"Effect");
}

void ER_YukiWEffect::SpawnSakuraEffect(CGameObject* _targetObj, float _endTime, CGameObject* _dummpyParent)
{
}

void ER_YukiWEffect::SaveToLevelFile(FILE* _File)
{
}

void ER_YukiWEffect::LoadFromLevelFile(FILE* _FILE)
{
}



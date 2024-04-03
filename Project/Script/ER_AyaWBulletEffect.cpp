#include "pch.h"
#include "ER_AyaWBulletEffect.h"

#include "ER_EffectSystem.h"

#include <random>

void ER_AyaWBulletEffect::begin()
{
	SpawnEffect();
}

void ER_AyaWBulletEffect::tick()
{
	m_fTime += DT;

	if (m_fTime > 0.05f) {
		std::random_device rd; // 시드 생성기
		std::mt19937 gen(rd()); // 메르센 트위스터 엔진 사용

		std::uniform_real_distribution<> dis(1.0, 2.0);

		// 랜덤 실수 생성
		float randomValue = (float)dis(gen);

		CGameObject* dummyParent = new CGameObject();
		AddComponents(dummyParent, _TRANSFORM);

		dummyParent->Transform()->SetRelativeRot(m_vEffectDir);

		SpawnGameObject(dummyParent, GetOwner()->Transform()->GetRelativePos(), L"Effect");
#pragma region effect
		CGameObject* tdExample = onew(CGameObject);
		AddComponents(tdExample, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
		tdExample->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		tdExample->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);

		Ptr<CTexture> animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"rShot.png");
		tdExample->Animator2D()->CreateAnimation(L"rShot", animAtlas, Vec2(0.f, 0.f), Vec2(256.f, 256.f), Vec2(256.f, 256.f), 1, 15);
		tdExample->Animator2D()->Play(L"rShot", true);

		dummyParent->AddChild(tdExample);
#pragma endregion
		std::thread t(&ER_EffectScript::EffectRotation, this, tdExample, Vec3(randomValue, randomValue, randomValue), Vec3(randomValue, randomValue, randomValue), true, 0.1f, dummyParent);
		t.detach();

		m_fTime -= 0.05f;
	}

	//파티클 이동
	if (m_pParticle != nullptr)
		m_pParticle->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());
}

void ER_AyaWBulletEffect::SpawnEffect(Vec3 _pos, Vec3 _dir, float _scale)
{
#pragma region effect
	CGameObject* tdExample = onew(CGameObject);
	AddComponents(tdExample, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	tdExample->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	tdExample->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);

	Vec3 rot = Vec3(XMConvertToRadians(180.f), 0.f, XMConvertToRadians(90.f));
	tdExample->Transform()->SetRelativeRot(rot);
	tdExample->Transform()->SetRelativeScale(Vec3(8.0f, 8.0f, 8.0f));

	Ptr<CTexture> animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"FX_BI_TX_AyaShootFire.png");
	tdExample->Animator2D()->CreateAnimation(L"FX_BI_TX_AyaShootFire", animAtlas, Vec2(0.f, 0.f), Vec2(256.f, 128.f), Vec2(256.f, 128.f), 1, 15);
	tdExample->Animator2D()->Play(L"FX_BI_TX_AyaShootFire", true);

	SpawnGameObjectToParent(tdExample, GetOwner());
#pragma endregion
#pragma region particle
	CGameObject* testParticle = onew(CGameObject);
	AddComponents(testParticle, _TRANSFORM | _PARTICLESYSTEM);
	CParticleSystem* Particle = testParticle->ParticleSystem();

	tParticleModule particle_data = Particle->GetParticleInfo();      // 파티클데이터 얻기

	particle_data.ModuleCheck[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN] = true;
	particle_data.ModuleCheck[(UINT)PARTICLE_MODULE::SCALE_CHANGE] = true;
	particle_data.ModuleCheck[(UINT)PARTICLE_MODULE::COLOR_CHANGE] = true;
	particle_data.ModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = true;
	particle_data.ModuleCheck[(UINT)PARTICLE_MODULE::DRAG] = true;
	particle_data.ModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE] = false;
	particle_data.ModuleCheck[(UINT)PARTICLE_MODULE::RENDER] = false;

	particle_data.StartScale = 1.f;
	particle_data.EndScale = 1.f;

	particle_data.vSpawnScaleMin = Vec3(0.3f, 0.3f, 0.3f);
	particle_data.vSpawnScaleMax = Vec3(0.3f, 0.3f, 0.3f);
	particle_data.vBoxShapeScale = Vec3(0.001f, 0.001f, 0.001f);

	particle_data.MinLifeTime = 0.5f;
	particle_data.MaxLifeTime = 0.5f;

	particle_data.vStartColor = Vec3(1.f, 1.f, 1.f);
	particle_data.vEndColor = Vec3(0.6f, 0.6f, 0.6f);


	particle_data.AddVelocityType = 0;
	particle_data.vVelocityDir = Vec3(0.f, 0.f, 0.f);
	particle_data.Speed = 2.5f;

	particle_data.SpawnRate =50;

	particle_data.StartDrag = 0.1f;
	particle_data.EndDrag = 0.f;

	Particle->SetMaxParticleCount(100);
	Particle->SetParticleTexture(CResMgr::GetInst()->FindRes<CTexture>(L"Core.png"));
	Particle->SetParticleInfo(particle_data);   // 파티클 데이터 세팅

	m_pParticle = testParticle;

	SpawnGameObject(testParticle, L"Effect");
#pragma endregion
}

void ER_AyaWBulletEffect::SaveToLevelFile(FILE* _File)
{
}

void ER_AyaWBulletEffect::LoadFromLevelFile(FILE* _FILE)
{
}

ER_AyaWBulletEffect::ER_AyaWBulletEffect()
	:m_fTime(0.f)
{
}

ER_AyaWBulletEffect::~ER_AyaWBulletEffect()
{
	ER_EffectSystem::GetInst()->AddDeleteParticles(m_pParticle);
}

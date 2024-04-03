#include "pch.h"
#include "ER_BulletEffectScript.h"

#include "ER_EffectSystem.h"

void ER_BulletEffectScript::begin()
{
	SpawnEffect();
}

void ER_BulletEffectScript::tick()
{
	if (m_pParticle != nullptr)
		m_pParticle->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());
}

void ER_BulletEffectScript::SpawnEffect(Vec3 _pos, Vec3 _dir, float _scale)
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

	particle_data.MinLifeTime = 0.2f;
	particle_data.MaxLifeTime = 0.2f;

	particle_data.vStartColor = Vec3(1.f, 1.f, 1.f);
	particle_data.vEndColor = Vec3(0.6f, 0.6f, 0.6f);

	particle_data.AddVelocityType = 0;
	particle_data.vVelocityDir = Vec3(0.f, 0.f, 0.f);
	particle_data.Speed = 2.5f;

	particle_data.SpawnRate = 50;

	particle_data.StartDrag = 0.1f;
	particle_data.EndDrag = 0.f;

	Particle->SetMaxParticleCount(100);
	Particle->SetParticleTexture(CResMgr::GetInst()->FindRes<CTexture>(L"Core.png"));
	Particle->SetParticleInfo(particle_data);   // 파티클 데이터 세팅
	//Particle->SetParticleSpawnNum(1);

	m_pParticle = testParticle;

	SpawnGameObject(testParticle, GetOwner()->Transform()->GetRelativePos(), 15);
#pragma endregion
}

void ER_BulletEffectScript::SaveToLevelFile(FILE* _File)
{
}

void ER_BulletEffectScript::LoadFromLevelFile(FILE* _FILE)
{
}

ER_BulletEffectScript::ER_BulletEffectScript()
	:m_pParticle(nullptr)
{
}

ER_BulletEffectScript::~ER_BulletEffectScript()
{
	ER_EffectSystem::GetInst()->AddDeleteParticles(m_pParticle);
}

#include "pch.h"
#include "ER_AyaQEffect.h"

ER_AyaQEffect::ER_AyaQEffect()
{
}

ER_AyaQEffect::~ER_AyaQEffect()
{
}

void ER_AyaQEffect::begin()
{
	CGameObject* testParticle = new CGameObject;
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

	particle_data.StartScale = 0.8f;
	particle_data.EndScale = 0.001f;

	particle_data.vSpawnScaleMin = Vec3(1.f, 1.f, 1.f);
	particle_data.vSpawnScaleMax = Vec3(1.f, 1.f, 1.f);
	particle_data.vBoxShapeScale = Vec3(0.001f, 0.001f, 0.001f);

	particle_data.MinLifeTime = 3.f;
	particle_data.MaxLifeTime = 3.f;

	particle_data.vStartColor = Vec3(1.f, 1.f, 1.f);
	particle_data.vEndColor = Vec3(0.6f, 0.6f, 0.6f);


	particle_data.AddVelocityType = 0;
	particle_data.vVelocityDir = Vec3(0.f, 1.f, 0.f);
	particle_data.Speed = 2.5f;

	particle_data.SpawnRate = 25;

	particle_data.EndDrag = -1.f;

	//Particle->SetSpawnRate(1);
	Particle->SetMaxParticleCount(25);
	Particle->SetParticleTexture(CResMgr::GetInst()->FindRes<CTexture>(L"Smoke.png"));
	Particle->SetParticleInfo(particle_data);   // 파티클 데이터 세팅

	GetOwner()->AddChild(testParticle);
}

void ER_AyaQEffect::tick()
{
}

void ER_AyaQEffect::SaveToLevelFile(FILE* _File)
{
}

void ER_AyaQEffect::LoadFromLevelFile(FILE* _FILE)
{
}


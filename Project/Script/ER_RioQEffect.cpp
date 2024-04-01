#include "pch.h"
#include "ER_RioQEffect.h"

ER_RioQEffect::ER_RioQEffect()
{
}

ER_RioQEffect::~ER_RioQEffect()
{
}

void ER_RioQEffect::begin()
{
	
}

void ER_RioQEffect::tick()
{
}

void ER_RioQEffect::SpawnEffect(Vec3 _pos, Vec3 _dir, float _scale)
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

	particle_data.StartScale = 0.4f;
	particle_data.EndScale = 0.001f;

	particle_data.vSpawnScaleMin = Vec3(0.3f, 0.3f, 0.3f);
	particle_data.vSpawnScaleMax = Vec3(0.3f, 0.3f, 0.3f);
	particle_data.vBoxShapeScale = Vec3(0.001f, 0.001f, 0.001f);

	particle_data.MinLifeTime = 3.f;
	particle_data.MaxLifeTime = 3.f;

	particle_data.vStartColor = Vec3(1.f, 1.f, 1.f);
	particle_data.vEndColor = Vec3(0.6f, 0.6f, 0.6f);


	particle_data.AddVelocityType = 0;
	particle_data.vVelocityDir = Vec3(0.f, 1.f, 0.f);
	particle_data.Speed = 2.5f;

	particle_data.SpawnRate = 10;

	particle_data.EndDrag = -1.f;

	//Particle->SetMaxParticleCount(10);
	Particle->SetParticleTexture(CResMgr::GetInst()->FindRes<CTexture>(L"Star4_Orange.png"));
	Particle->SetParticleInfo(particle_data);   // 파티클 데이터 세팅
	Particle->SetParticleSpawnNum(1);
	Particle->SetDestoryTrigger(true);

	Particle->Transform()->SetRelativePos(Vec3(0.6f,1.8f,0.38f));

	SpawnGameObjectToParent(testParticle, GetOwner());

	//Vec3 ownerPos = GetOwner()->Transform()->GetRelativePos();
	//Particle->Transform()->SetRelativePos(Vec3(ownerPos.x+0.6f, ownerPos.y+1.8f, ownerPos.z+0.38f));
	//SpawnGameObject(testParticle, L"Effect");
}

void ER_RioQEffect::SaveToLevelFile(FILE* _File)
{
}

void ER_RioQEffect::LoadFromLevelFile(FILE* _FILE)
{
}



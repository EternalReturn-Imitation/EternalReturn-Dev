#include "pch.h"
#include "ER_ArrowEffectScript.h"

void ER_ArrowEffectScript::begin()
{
	SpawnEffect();
}

void ER_ArrowEffectScript::tick()
{
}

void ER_ArrowEffectScript::SpawnEffect(Vec3 _pos, Vec3 _dir)
{
#pragma region effect
	CGameObject* tdExample = new CGameObject();
	AddComponents(tdExample, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	tdExample->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	tdExample->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);

	Vec3 rot = Vec3(XMConvertToRadians(180.f), 0.f, XMConvertToRadians(90.f));
	tdExample->Transform()->SetRelativeRot(rot);

	Ptr<CTexture> animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"FX_BI_HitGlow_04_Orange.png");
	tdExample->Animator2D()->CreateAnimation(L"FX_BI_HitGlow_04_Orange", animAtlas, Vec2(0.f, 0.f), Vec2(100.f, 100.f), Vec2(100.f, 100.f), 1, 15);
	tdExample->Animator2D()->Play(L"FX_BI_HitGlow_04_Orange", true);

	SpawnGameObjectToParent(tdExample, GetOwner());
#pragma endregion

#pragma region particle
	CGameObject* testParticle = new CGameObject;
	AddComponents(testParticle, _TRANSFORM | _PARTICLESYSTEM);
	CParticleSystem* Particle = testParticle->ParticleSystem();

	tParticleModule particle_data = Particle->GetParticleInfo();      // 파티클데이터 얻기

	particle_data.ModuleCheck[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN] = true;
	particle_data.ModuleCheck[(UINT)PARTICLE_MODULE::SCALE_CHANGE] = true;
	particle_data.ModuleCheck[(UINT)PARTICLE_MODULE::COLOR_CHANGE] = true;
	particle_data.ModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = false;
	particle_data.ModuleCheck[(UINT)PARTICLE_MODULE::DRAG] = false;
	particle_data.ModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE] = false;
	particle_data.ModuleCheck[(UINT)PARTICLE_MODULE::RENDER] = false;

	particle_data.StartScale = 5.f;
	particle_data.EndScale = 5.f;

	particle_data.vSpawnScaleMin = Vec3(0.3f, 0.3f, 0.3f);
	particle_data.vSpawnScaleMax = Vec3(0.3f, 0.3f, 0.3f);
	particle_data.vBoxShapeScale = Vec3(0.001f, 0.001f, 0.001f);

	particle_data.MinLifeTime = 2.f;
	particle_data.MaxLifeTime = 2.f;

	particle_data.vStartColor = Vec3(1.f, 1.f, 1.f);
	particle_data.vEndColor = Vec3(0.6f, 0.6f, 0.6f);


	particle_data.AddVelocityType = 2;
	particle_data.vVelocityDir = Vec3(1.f, 0.f, 1.f);
	particle_data.Speed = 2.5f;

	particle_data.SpawnRate = 40.f;

	particle_data.EndDrag = -1.f;

	particle_data.fNoiseTerm = 2.f;
	particle_data.fNoiseForce = 2.f;

	float angle = Rad2Deg(GetOwner()->Transform()->GetRelativeRot().y);

	float offset = 45.f;
	angle -= offset;
	
	if (angle < 0) {
		angle = 360.f + angle;
	}


	particle_data.vRot = Vec4(0.f,0.f, angle ,0.f);

	//Particle->SetMaxParticleCount(10);
	Particle->SetParticleTexture(CResMgr::GetInst()->FindRes<CTexture>(L"FX_BI_TX_RioShootFire_Orange.png"));
	Particle->SetParticleInfo(particle_data);   // 파티클 데이터 세팅
	Particle->SetParticleSpawnNum(100);

	rot = Vec3(XMConvertToRadians(180.f), 0.f, XMConvertToRadians(90.f));
	testParticle->Transform()->SetRelativeRot(rot);

	SpawnGameObjectToParent(testParticle, GetOwner());
#pragma endregion
}

void ER_ArrowEffectScript::SaveToLevelFile(FILE* _File)
{
}

void ER_ArrowEffectScript::LoadFromLevelFile(FILE* _FILE)
{
}

ER_ArrowEffectScript::ER_ArrowEffectScript()
{
}

ER_ArrowEffectScript::~ER_ArrowEffectScript()
{
}

#include "pch.h"
#include "ER_YukiQEffect.h"

#include "ER_EffectSystem.h"

void ER_YukiQEffect::begin()
{
}

void ER_YukiQEffect::tick()
{
}

void ER_YukiQEffect::SpawnEffect(Vec3 _pos, Vec3 _dir, float _scale)
{
	CGameObject* dummyParent = new CGameObject();
	AddComponents(dummyParent, _TRANSFORM);

	dummyParent->Transform()->SetRelativeRot(_dir);
	
	SpawnGameObject(dummyParent, _pos, L"Effect");

	CGameObject* testParticle = new CGameObject;
	AddComponents(testParticle, _TRANSFORM | _PARTICLESYSTEM);

	Vec3 pos = testParticle->Transform()->GetRelativePos();
	pos.y += 1.3f;
	pos.z -= 0.4f;
	testParticle->Transform()->SetRelativePos(pos);
	
	CParticleSystem* Particle = testParticle->ParticleSystem();

	tParticleModule particle_data = Particle->GetParticleInfo();      // 파티클데이터 얻기

	particle_data.ModuleCheck[(UINT)PARTICLE_MODULE::PARTICLE_SPAWN] = true;
	particle_data.ModuleCheck[(UINT)PARTICLE_MODULE::SCALE_CHANGE] = true;
	particle_data.ModuleCheck[(UINT)PARTICLE_MODULE::COLOR_CHANGE] = true;
	particle_data.ModuleCheck[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = true;
	particle_data.ModuleCheck[(UINT)PARTICLE_MODULE::DRAG] = true;
	particle_data.ModuleCheck[(UINT)PARTICLE_MODULE::NOISE_FORCE] = false;
	particle_data.ModuleCheck[(UINT)PARTICLE_MODULE::RENDER] = false;

	particle_data.StartScale = 0.5f;
	particle_data.EndScale = 0.5f;

	particle_data.vSpawnScaleMin = Vec3(0.3f, 0.3f, 0.3f);
	particle_data.vSpawnScaleMax = Vec3(0.3f, 0.3f, 0.3f);
	particle_data.vBoxShapeScale = Vec3(0.001f, 0.001f, 0.001f);

	particle_data.MinLifeTime = 1.f;
	particle_data.MaxLifeTime = 1.f;

	particle_data.vStartColor = Vec3(1.f, 1.f, 1.f);
	particle_data.vEndColor = Vec3(0.6f, 0.6f, 0.6f);


	particle_data.AddVelocityType = 1;
	particle_data.vVelocityDir = Vec3(0.f, 0.f, 0.f);
	particle_data.Speed = 0.3f;

	particle_data.SpawnRate = 20;

	particle_data.StartDrag = 2.f;
	particle_data.EndDrag = 10.f;

	particle_data.fNoiseTerm = 2.f;
	particle_data.fNoiseForce = 2.f;

	//Particle->SetMaxParticleCount(10);
	Particle->SetParticleTexture(CResMgr::GetInst()->FindRes<CTexture>(L"Fx_UI_Tierlight_OW.png"));
	Particle->SetParticleInfo(particle_data);   // 파티클 데이터 세팅
	Particle->SetParticleSpawnNum(100);

	dummyParent->AddChild(testParticle);

	std::thread t(&ER_YukiQEffect::SpawnParticleEffect, this, testParticle, 1.0f, dummyParent);
	t.detach();
}

void ER_YukiQEffect::SpawnParticleEffect(CGameObject* _targetObj, float _endTime, CGameObject* _dummpyParent)
{
	auto startTime = std::chrono::high_resolution_clock::now();
	int restTime = 10;

	float exeCount = _endTime * 1000.f / restTime; //실행되는 횟수

	while (true) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);

		//시간이 지나면 종료
		if (elapsedTime.count() >= _endTime * 1000.f)
			break;

		_dummpyParent->Transform()->SetRelativePos(GetOwner()->Transform()->GetRelativePos());

		std::this_thread::sleep_for(std::chrono::milliseconds(restTime));
	}

	ER_EffectSystem::GetInst()->AddDeleteParticles(_targetObj, _dummpyParent);
}

void ER_YukiQEffect::SpawnAttackEffect(Vec3 _pos, Vec3 _dir, float _scale)
{
	CGameObject* dummyParent = new CGameObject();
	AddComponents(dummyParent, _TRANSFORM);

	_dir.y += Deg2Rad(0.f);
	dummyParent->Transform()->SetRelativeRot(_dir);

	SpawnGameObject(dummyParent, _pos, L"Effect");

	CGameObject* tdExample = new CGameObject();
	AddComponents(tdExample, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	tdExample->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	tdExample->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);

	tdExample->Transform()->SetRelativeRot(Vec3(Deg2Rad(90.f), 0.f, 0.f));
	tdExample->Transform()->SetRelativeScale(Vec3(4.0f, 4.0f, 4.0f));

	Ptr<CTexture> animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"FX_BI_Sword_02AB_Biolet.png");
	tdExample->Animator2D()->CreateAnimation(L"FX_BI_Sword_02AB_Biolet", animAtlas, Vec2(0.f, 0.f), Vec2(256.f, 256.f), Vec2(256.f, 256.f), 1, 1);
	tdExample->Animator2D()->Play(L"FX_BI_Sword_02AB_Biolet", true);

	dummyParent->AddChild(tdExample);

	std::thread t(&ER_EffectScript::SpawnAnimationEffect, this, tdExample, 0.2f, dummyParent);
	t.detach();
}

void ER_YukiQEffect::SaveToLevelFile(FILE* _File)
{
}

void ER_YukiQEffect::LoadFromLevelFile(FILE* _FILE)
{
}

ER_YukiQEffect::ER_YukiQEffect()
{
}

ER_YukiQEffect::~ER_YukiQEffect()
{
}

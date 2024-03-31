#include "pch.h"
#include "ER_EffectSystem.h"

#include <thread>
#include <chrono>

#include "ER_ActionScript_Rio.h"

ER_EffectSystem::ER_EffectSystem()
{
}

ER_EffectSystem::~ER_EffectSystem()
{
}

void ER_EffectSystem::init()
{
}

void ER_EffectSystem::SpawnHitEffect(CGameObject* _attacker, CGameObject* _hitter, Vec3 _pos, Vec3 _dir)
{
	Vec3 hitEffectPos = _hitter->Transform()->GetWorldPos();
	hitEffectPos.y += 1.5f;

	Vec3 effectMoveDir = _attacker->Transform()->GetWorldPos() - _hitter->Transform()->GetWorldPos();
	effectMoveDir.Normalize();

	hitEffectPos = hitEffectPos + effectMoveDir * 0.5f;

	Vec3 hitEffectDir = _attacker->Transform()->GetRelativeRot();
	float yRot = Rad2Deg(hitEffectDir.y);
	
	yRot += 180.f;
	if (yRot > 270.f) {
		yRot = yRot - 270.f;
		yRot = yRot - 90.f;
	}

	hitEffectDir.y = Deg2Rad(yRot);

	if (_attacker->GetScript<ER_ActionScript_Rio>() != nullptr) {
		thread t(&ER_EffectSystem::SpawnRioHitEffect, this, hitEffectPos, hitEffectDir);
		t.detach();
	}
}

void ER_EffectSystem::SpawnRioHitEffect(Vec3 _pos, Vec3 _dir)
{
	//히트 이펙트1(바람) 스폰
	CGameObject* windHitEffect = new CGameObject();
	AddComponents(windHitEffect, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	windHitEffect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	windHitEffect->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);

	windHitEffect->Transform()->SetRelativePos(Vec3(_pos.x,_pos.y,_pos.z));
	windHitEffect->Transform()->SetRelativeRot(_dir);


	Ptr<CTexture> animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"wWind.png");
	windHitEffect->Animator2D()->CreateAnimation(L"wWind", animAtlas, Vec2(0.f, 0.f), Vec2(100.f, 100.f), Vec2(100.f, 100.f), 1, 15);
	windHitEffect->Animator2D()->Play(L"wWind", true);

	SpawnGameObject(windHitEffect, L"Effect");

	auto startTime = std::chrono::high_resolution_clock::now();
	int restTime = 10;

	//0.4f는 실행된든 시간
	float endTime = 0.25f;
	float exeCount = endTime * 1000.f / restTime; //실행되는 횟수

	//1.0f는 시작 크기, 3.0f는 끝날때 크기
	float startScale = 1.0f;
	float endScale = 3.0f;
	float scaleIncreasing = (endScale - startScale) / exeCount;

	while (true) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);

		//시간이 지나면 종료
		if (elapsedTime.count() >= endTime * 1000.f)
			break;

		//크기 변경
		if (endScale >= startScale) {
			startScale += scaleIncreasing;
		}
		windHitEffect->Transform()->SetRelativeScale(Vec3(startScale, startScale, startScale));

		std::this_thread::sleep_for(std::chrono::milliseconds(restTime));
	}

	DestroyObject(windHitEffect);
}

#include "pch.h"
#include "ER_EffectSystem.h"

#include <thread>
#include <chrono>

#include "ER_ActionScript_Rio.h"
#include "ER_ActionScript_Aya.h"
#include "ER_ActionScript_Yuki.h"
#include "ER_ActionScript_Hyunwoo.h"

ER_EffectSystem::ER_EffectSystem()
	:m_iYukiCount(0)
{
}

ER_EffectSystem::~ER_EffectSystem()
{
}

void ER_EffectSystem::init()
{
}

void ER_EffectSystem::progress()
{
	for (int i = 0; i < m_vParticleLifeTime.size(); ++i) {
		m_vParticleLifeTime[i] -= DT;
		if (m_vParticleLifeTime[i] < 0.f) {
			DestroyObject(m_vDeleteParticles[i]);
			m_vDeleteParticles[i] = nullptr;
			m_vDeleteParticles.erase(m_vDeleteParticles.begin() + i);
			m_vParticleLifeTime.erase(m_vParticleLifeTime.begin() + i);
		}
	}
}

void ER_EffectSystem::SpawnHitEffect(CGameObject* _attacker, CGameObject* _hitter, CGameObject* _projectile, Vec3 _pos, Vec3 _dir)
{
	Vec3 hitEffectPos;	//��Ʈ�� ����Ʈ ���� ��ġ
	Vec3 effectMoveDir;	//����Ʈ ��ġ ����
	Vec3 hitEffectDir;	//����Ʈ�� �ٶ󺸴� ����
	float yRot;			//����Ʈ�� ���� ����(y����)

	//����ü�� ���� ��
	if (_projectile != nullptr) {
		hitEffectPos = _hitter->Transform()->GetWorldPos();
		hitEffectPos.y += 1.5f;

		effectMoveDir = _projectile->Transform()->GetRelativePos() - _hitter->Transform()->GetRelativePos();
		effectMoveDir.y = 0.f;
		effectMoveDir.Normalize();

		hitEffectPos = hitEffectPos + (effectMoveDir * 0.5f);

		hitEffectDir = _projectile->Transform()->GetRelativeRot();
		yRot = Rad2Deg(hitEffectDir.y);

		yRot += 180.f;
		if (yRot > 270.f) {
			yRot = yRot - 270.f;
			yRot = yRot - 90.f;
		}

		hitEffectDir.y = Deg2Rad(yRot);
	}
	//����ü�� ���� ��
	else {

	}
	

	if (_attacker->GetScript<ER_ActionScript_Rio>() != nullptr) {
		thread t(&ER_EffectSystem::SpawnRioHitEffect, this, hitEffectPos, hitEffectDir, effectMoveDir);
		t.detach();
	}
	else if (_attacker->GetScript<ER_ActionScript_Aya>() != nullptr) {
		Vec3 hitPosByAya = _hitter->Transform()->GetWorldPos();
		hitPosByAya.y += 1.5f;
		thread t(&ER_EffectSystem::SpawnAyaHitEffect, this, hitPosByAya);
		t.detach();
	}
	else if (_attacker->GetScript<ER_ActionScript_Yuki>() != nullptr) {
		Vec3 hitPosByYuki = _hitter->Transform()->GetWorldPos();
		hitPosByYuki.y += 1.8f;
		hitPosByYuki.z -= 0.2f;
		hitPosByYuki.x += 0.2f;
		thread t(&ER_EffectSystem::SpawnYukiHitEffect, this, hitPosByYuki);
		t.detach();
	}
	else if (_attacker->GetScript<ER_ActionScript_Hyunwoo>() != nullptr) {
		Vec3 hitPosByYuki = _hitter->Transform()->GetWorldPos();
		hitPosByYuki.y += 1.8f;
		hitPosByYuki.z -= 0.2f;
		hitPosByYuki.x += 0.2f;
		thread t(&ER_EffectSystem::SpawnHyunwooHitEffect, this, hitPosByYuki);
		t.detach();
	}
}

void ER_EffectSystem::SpawnSkillHitEffect(CGameObject* _attacker, CGameObject* _hitter, const tSkill_Info* _skillInfo, CGameObject* _projectile,  Vec3 _pos, Vec3 _dir)
{
	Vec3 hitEffectPos;	//��Ʈ�� ����Ʈ ���� ��ġ
	Vec3 effectMoveDir;	//����Ʈ ��ġ ����
	Vec3 hitEffectDir;	//����Ʈ�� �ٶ󺸴� ����
	float yRot;			//����Ʈ�� ���� ����(y����)

	//����ü�� ���� ��
	if (_projectile != nullptr) {
		hitEffectPos = _hitter->Transform()->GetWorldPos();
		hitEffectPos.y += 1.5f;

		effectMoveDir = _projectile->Transform()->GetRelativePos() - _hitter->Transform()->GetRelativePos();
		effectMoveDir.y = 0.f;
		effectMoveDir.Normalize();

		hitEffectPos = hitEffectPos + (effectMoveDir * 0.5f);

		hitEffectDir = _projectile->Transform()->GetRelativeRot();
		yRot = Rad2Deg(hitEffectDir.y);

		yRot += 180.f;
		if (yRot > 270.f) {
			yRot = yRot - 270.f;
			yRot = yRot - 90.f;
		}

		hitEffectDir.y = Deg2Rad(yRot);
	}
	//����ü�� ���� ��
	else {

	}


	//���� ��ų
	if (_skillInfo->strName == L"Rio_W" || _skillInfo->strName == L"Rio_W2") {
		thread t(&ER_EffectSystem::SpawnRioHitEffect, this, hitEffectPos, hitEffectDir, effectMoveDir);
		t.detach();
		thread t1(&ER_EffectSystem::SpawnRioWHitEffect, this, hitEffectPos, hitEffectDir, effectMoveDir, _hitter);
		t1.detach();
	}
	else if (_skillInfo->strName == L"Rio_R") {
		thread t(&ER_EffectSystem::SpawnRioRHitEffect, this, hitEffectPos, hitEffectDir, effectMoveDir);
		t.detach();
	}
	//�ƾ� ��ų
	else if (_skillInfo->strName != L"Aya_R" && _skillInfo->strName.substr(0, 4) == L"Aya_") {
		Vec3 hitPosByAya = _hitter->Transform()->GetWorldPos();
		hitPosByAya.y += 1.5f;
		thread t(&ER_EffectSystem::SpawnAyaHitEffect, this, hitPosByAya);
		t.detach();
	}
	//��Ű ��ų
	else if (_skillInfo->strName == L"Yuki_Q") {
		Vec3 hitPosByYuki = _hitter->Transform()->GetWorldPos();
		hitPosByYuki.y += 1.8f;
		thread t1(&ER_EffectSystem::SpawnYukiQHitEffect, this, hitPosByYuki);
		t1.detach();

		hitPosByYuki = _hitter->Transform()->GetWorldPos();
		hitPosByYuki.y += 1.8f;
		hitPosByYuki.z -= 0.2f;
		hitPosByYuki.x += 0.2f;
		thread t2(&ER_EffectSystem::SpawnYukiHitEffect, this, hitPosByYuki);
		t2.detach();
	}
	else if (_skillInfo->strName == L"Yuki_E") {
		Vec3 hitPosByYuki = _hitter->Transform()->GetWorldPos();
		hitPosByYuki.y += 1.8f;
		hitPosByYuki.z -= 0.2f;
		hitPosByYuki.x += 0.2f;
		thread t2(&ER_EffectSystem::SpawnYukiHitEffect, this, hitPosByYuki);
		t2.detach();
	}
	else if (_skillInfo->strName == L"Yuki_R1") {
		++m_iYukiCount;
		Vec3 hitPosByYuki = _hitter->Transform()->GetWorldPos();
		hitPosByYuki.y += 2.5f;
		thread t2(&ER_EffectSystem::SpawnYukiR1HitEffect, this, hitPosByYuki, _hitter);
		t2.detach();

		hitPosByYuki = _hitter->Transform()->GetWorldPos();
		hitPosByYuki.y += 1.8f;
		hitPosByYuki.z -= 0.2f;
		hitPosByYuki.x += 0.2f;
		thread t1(&ER_EffectSystem::SpawnYukiHitEffect, this, hitPosByYuki);
		t1.detach();
	}
	else if (_skillInfo->strName == L"Yuki_R2") {
		m_iYukiCount = 0;
		Vec3 hitPosByYuki = _hitter->Transform()->GetWorldPos();
		hitPosByYuki.y += 1.8f;
		hitPosByYuki.z -= 0.2f;
		hitPosByYuki.x += 0.2f;
		thread t1(&ER_EffectSystem::SpawnYukiHitEffect, this, hitPosByYuki);
		t1.detach();

		thread t2(&ER_EffectSystem::SpawnYukiR2HitEffect, this, hitPosByYuki);
		t2.detach();
	}
	else if (_skillInfo->strName == L"Hyunwoo_Q") {
		Vec3 hitPosByHyunwoo = _hitter->Transform()->GetWorldPos();
		thread t1(&ER_EffectSystem::SpawnHyunwooQHitEffect, this, hitPosByHyunwoo, _hitter);
		t1.detach();
	}
	else if (_skillInfo->strName == L"Hyunwoo_E") {
		Vec3 hitPosByHyunwoo = _hitter->Transform()->GetWorldPos();
		hitPosByHyunwoo.y += 1.8f;
		hitPosByHyunwoo.z -= 0.2f;
		hitPosByHyunwoo.x += 0.2f;
		thread t1(&ER_EffectSystem::SpawnHyunwooEHitEffect, this, hitPosByHyunwoo, _attacker->Transform()->GetRelativeRot(), _hitter);
		t1.detach();
	}
}

void ER_EffectSystem::SpawnRioHitEffect(Vec3 _pos, Vec3 _dir, Vec3 _effectMoveDir)
{
	//���� �θ� ����Ʈ01 ����
	CGameObject* dummyParent01 = onew(CGameObject);
	AddComponents(dummyParent01, _TRANSFORM);

	dummyParent01->Transform()->SetRelativeRot(Vec3(0.f, _dir.y, 0.f));

	SpawnGameObject(dummyParent01, _pos, L"Effect");

	//���� �θ� ����Ʈ01 ����
	CGameObject* dummyParent02 = onew(CGameObject);
	AddComponents(dummyParent02, _TRANSFORM);

	dummyParent02->Transform()->SetRelativeRot(Vec3(0.f, _dir.y, 0.f));

	SpawnGameObject(dummyParent02, _pos, L"Effect");

	//��Ʈ ����Ʈ1(�ٶ�) ����
	CGameObject* windHitEffect = onew(CGameObject);
	windHitEffect->SetName(L"WindHitEffect");
	AddComponents(windHitEffect, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	windHitEffect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	windHitEffect->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);
	windHitEffect->MeshRender()->GetDynamicMaterial(0);

	Ptr<CTexture> animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"wWind.png");
	windHitEffect->Animator2D()->CreateAnimation(L"wWind", animAtlas, Vec2(0.f, 0.f), Vec2(256.f, 256.f), Vec2(256.f, 256.f), 1, 15);
	windHitEffect->Animator2D()->Play(L"wWind", true);

	dummyParent01->AddChild(windHitEffect);
	
	//��Ʈ �̺�Ʈ2(Ÿ��) ����
	CGameObject* attackHitEffect = onew(CGameObject);
	attackHitEffect->SetName(L"attackHitEffect01");
	AddComponents(attackHitEffect, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	attackHitEffect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	attackHitEffect->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);
	attackHitEffect->MeshRender()->GetDynamicMaterial(0);

	animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"rioHit2_Orange.png");
	attackHitEffect->Animator2D()->CreateAnimation(L"rioHit2_Orange", animAtlas, Vec2(0.f, 0.f), Vec2(1024.f, 1024.f), Vec2(1024.f, 1024.f), 1, 15);
	attackHitEffect->Animator2D()->Play(L"rioHit2_Orange", true);

	dummyParent02->AddChild(attackHitEffect);

	auto startTime = std::chrono::high_resolution_clock::now();
	int restTime = 10;

	//0.4f�� ����ȵ� �ð�
	float endTime = 0.25f;
	float exeCount = endTime * 1000.f / restTime; //����Ǵ� Ƚ��

	//1.0f�� ���� ũ��, 3.0f�� ������ ũ��
	float startScale = 1.0f;
	float endScale = 3.0f;
	float scaleIncreasing = (endScale - startScale) / exeCount;

	//startMovePos�� ���� ũ��, 1.2f�� ���� �� ũ��
	float startMovePos = 0.f;
	float endMovePos = 0.5f;
	float moveIncreasing = (endMovePos - startMovePos) / exeCount;

	//�ִϸ��̼��� ���İ��� ������� �ð�
	float alphaTime = 0.1f;
	if (endTime < 0.2f)
		alphaTime = 0.05f;

	bool alphaTrigger = true;
	while (true) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);

		//�ð��� ������ ����
		if (elapsedTime.count() >= endTime * 1000.f)
			break;

		//ũ�� ����
		if (endScale >= startScale) {
			startScale += scaleIncreasing;
		}
		windHitEffect->Transform()->SetRelativeScale(Vec3(startScale/1.f, startScale/1.f, startScale/1.f));
		attackHitEffect->Transform()->SetRelativeScale(Vec3(startScale/2.f, startScale / 2.f, startScale/2.f));

		//ȸ�� ����
		Vec3 rot = windHitEffect->Transform()->GetRelativeRot();
		rot.z += exeCount / 500.f;
		windHitEffect->Transform()->SetRelativeRot(rot);

		//�����̱�
		Vec3 pos = dummyParent01->Transform()->GetRelativePos();
		_effectMoveDir.y = 0.f;
		_effectMoveDir.Normalize();
		Vec3 resultPos = pos + (_effectMoveDir * moveIncreasing);
		dummyParent01->Transform()->SetRelativePos(resultPos);

		if ((endTime * 1000.f) - elapsedTime.count() <= alphaTime * 1000.f && alphaTrigger) {
			if(windHitEffect->Animator2D())
				windHitEffect->Animator2D()->SetAlphaEraseTime(alphaTime);
			if (attackHitEffect->Animator2D())
				attackHitEffect->Animator2D()->SetAlphaEraseTime(alphaTime);

			alphaTrigger = false;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(restTime));
	}

	DestroyObject(dummyParent01);
	DestroyObject(dummyParent02);
}

void ER_EffectSystem::SpawnAyaHitEffect(Vec3 _pos)
{
	//���� �θ� ����Ʈ01 ����
	CGameObject* dummyParent01 = onew(CGameObject);
	AddComponents(dummyParent01, _TRANSFORM);

	dummyParent01->Transform()->SetRelativeRot(Vec3(Deg2Rad(90.f), 0.f, 0.f));

	SpawnGameObject(dummyParent01, _pos, L"Effect");

	//��Ʈ ����Ʈ1(��) ����
	CGameObject* gunHitEffect = onew(CGameObject);
	gunHitEffect->SetName(L"gunHitEffect01");
	AddComponents(gunHitEffect, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	gunHitEffect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	gunHitEffect->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);
	gunHitEffect->MeshRender()->GetDynamicMaterial(0);

	Ptr<CTexture> animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"Fx_ShootGlowSE_04.png");
	gunHitEffect->Animator2D()->CreateAnimation(L"Fx_ShootGlowSE_04", animAtlas, Vec2(0.f, 0.f), Vec2(64.f, 64.f), Vec2(64.f, 64.f), 4, 12);
	gunHitEffect->Animator2D()->Play(L"Fx_ShootGlowSE_04", true);

	dummyParent01->AddChild(gunHitEffect);

	//������ ���� ����
	auto startTime = std::chrono::high_resolution_clock::now();
	int restTime = 10;

	//0.4f�� ����ȵ� �ð�
	float endTime = 0.20f;
	float exeCount = endTime * 1000.f / restTime; //����Ǵ� Ƚ��

	while (true) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);

		//�ð��� ������ ����
		if (elapsedTime.count() >= endTime * 1000.f)
			break;

		std::this_thread::sleep_for(std::chrono::milliseconds(restTime));
	}

	DestroyObject(dummyParent01);
}

void ER_EffectSystem::SpawnYukiHitEffect(Vec3 _pos)
{
	//���� �θ� ����Ʈ01 ����
	CGameObject* dummyParent01 = onew(CGameObject);
	AddComponents(dummyParent01, _TRANSFORM);

	dummyParent01->Transform()->SetRelativeRot(Vec3(Deg2Rad(90.f), 0.f, 0.f));

	SpawnGameObject(dummyParent01, _pos, L"Effect");

	//��Ʈ ����Ʈ1(��) ����
	CGameObject* gunHitEffect = onew(CGameObject);
	gunHitEffect->SetName(L"YukiHitEffect02");
	AddComponents(gunHitEffect, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	gunHitEffect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	gunHitEffect->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);
	gunHitEffect->MeshRender()->GetDynamicMaterial(0);

	Ptr<CTexture> animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"qburst01.png");
	gunHitEffect->Animator2D()->CreateAnimation(L"qburst01", animAtlas, Vec2(0.f, 0.f), Vec2(256.f, 256.f), Vec2(256.f, 256.f), 4, 12);
	gunHitEffect->Animator2D()->Play(L"qburst01", true);

	dummyParent01->AddChild(gunHitEffect);

	//������ ���� ����
	auto startTime = std::chrono::high_resolution_clock::now();
	int restTime = 10;

	//0.4f�� ����ȵ� �ð�
	float endTime = 0.20f;
	float exeCount = endTime * 1000.f / restTime; //����Ǵ� Ƚ��

	while (true) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);

		//�ð��� ������ ����
		if (elapsedTime.count() >= endTime * 1000.f)
			break;

		std::this_thread::sleep_for(std::chrono::milliseconds(restTime));
	}

	DestroyObject(dummyParent01);
}

void ER_EffectSystem::SpawnHyunwooHitEffect(Vec3 _pos)
{
	//���� �θ� ����Ʈ01 ����
	CGameObject* dummyParent01 = onew(CGameObject);
	AddComponents(dummyParent01, _TRANSFORM);

	dummyParent01->Transform()->SetRelativeRot(Vec3(Deg2Rad(90.f), 0.f, 0.f));

	SpawnGameObject(dummyParent01, _pos, L"Effect");

	//��Ʈ ����Ʈ1(��) ����
	CGameObject* gunHitEffect = onew(CGameObject);
	gunHitEffect->SetName(L"HyunwooHitEffect");
	AddComponents(gunHitEffect, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	gunHitEffect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	gunHitEffect->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);
	gunHitEffect->MeshRender()->GetDynamicMaterial(0);

	Ptr<CTexture> animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"FX_BI_Hit_01.png");
	gunHitEffect->Animator2D()->CreateAnimation(L"FX_BI_Hit_01", animAtlas, Vec2(511.8f, 0.f), Vec2(170.6666f, 171.f), Vec2(170.6666f, 171.f), 6, 12);
	gunHitEffect->Animator2D()->Play(L"FX_BI_Hit_01", false);

	dummyParent01->AddChild(gunHitEffect);

	//������ ���� ����
	auto startTime = std::chrono::high_resolution_clock::now();
	int restTime = 10;

	//0.4f�� ����ȵ� �ð�
	float endTime = 0.20f;
	float exeCount = endTime * 1000.f / restTime; //����Ǵ� Ƚ��

	while (true) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);

		//�ð��� ������ ����
		if (elapsedTime.count() >= endTime * 1000.f)
			break;

		std::this_thread::sleep_for(std::chrono::milliseconds(restTime));
	}

	DestroyObject(dummyParent01);
}

void ER_EffectSystem::SpawnRioWHitEffect(Vec3 _pos, Vec3 _dir, Vec3 _effectMoveDir, CGameObject* _hitter)
{
	//���� �θ� ����Ʈ01 ����
	CGameObject* dummyParent01 = onew(CGameObject);
	AddComponents(dummyParent01, _TRANSFORM);

	dummyParent01->Transform()->SetRelativeRot(Vec3(Deg2Rad(90.f), 0.f, 0.f));

	SpawnGameObject(dummyParent01, L"Effect");

	//maskWind ����
	CGameObject* maskWind = onew(CGameObject);
	maskWind->SetName(L"RioHitEffect");
	AddComponents(maskWind, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	maskWind->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	maskWind->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);
	maskWind->MeshRender()->GetDynamicMaterial(0);

	maskWind->Transform()->SetRelativeScale(maskWind->Transform()->GetRelativeScale() * 4.f);

	Ptr<CTexture> animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"wMaskWind.png");
	maskWind->Animator2D()->CreateAnimation(L"wMaskWind", animAtlas, Vec2(0.f, 0.f), Vec2(256.f, 256.f), Vec2(256.f, 256.f), 1, 15);
	maskWind->Animator2D()->Play(L"wMaskWind", true);

	dummyParent01->AddChild(maskWind);

	//������ �ð����� ����
	auto startTime = std::chrono::high_resolution_clock::now();
	int restTime = 10;

	//0.4f�� ����ȵ� �ð�
	float endTime = 1.f;
	float exeCount = endTime * 1000.f / restTime; //����Ǵ� Ƚ��

	while (true) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);

		//�ð��� ������ ����
		if (elapsedTime.count() >= endTime * 1000.f)
			break;

		//��ġ ����
		Vec3 hitterPos = _hitter->Transform()->GetRelativePos();
		dummyParent01->Transform()->SetRelativePos(hitterPos);

		//ȸ�� ����
		Vec3 rot = maskWind->Transform()->GetRelativeRot();
		rot.z += exeCount / 2000.f;
		maskWind->Transform()->SetRelativeRot(rot);

		std::this_thread::sleep_for(std::chrono::milliseconds(restTime));
	}

	DestroyObject(dummyParent01);
}

void ER_EffectSystem::SpawnRioRHitEffect(Vec3 _pos, Vec3 _dir, Vec3 _effectMoveDir)
{//���� �θ� ����Ʈ01 ����
	CGameObject* dummyParent01 = onew(CGameObject);
	AddComponents(dummyParent01, _TRANSFORM);

	dummyParent01->Transform()->SetRelativeRot(Vec3(0.f, _dir.y, 0.f));

	SpawnGameObject(dummyParent01, _pos, L"Effect");

	//���� �θ� ����Ʈ01 ����
	CGameObject* dummyParent02 = onew(CGameObject);
	AddComponents(dummyParent02, _TRANSFORM);

	dummyParent02->Transform()->SetRelativeRot(Vec3(0.f, _dir.y, 0.f));

	SpawnGameObject(dummyParent02, _pos, L"Effect");

	//��Ʈ ����Ʈ1(�ٶ�) ����
	CGameObject* windHitEffect = onew(CGameObject);
	windHitEffect->SetName(L"RioRHitEffect");
	AddComponents(windHitEffect, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	windHitEffect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	windHitEffect->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);
	windHitEffect->MeshRender()->GetDynamicMaterial(0);

	Ptr<CTexture> animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"wWind.png");
	windHitEffect->Animator2D()->CreateAnimation(L"wWind", animAtlas, Vec2(0.f, 0.f), Vec2(100.f, 100.f), Vec2(100.f, 100.f), 1, 15);
	windHitEffect->Animator2D()->Play(L"wWind", true);

	dummyParent01->AddChild(windHitEffect);

	//��Ʈ �̺�Ʈ2(Ÿ��) ����
	CGameObject* attackHitEffect = onew(CGameObject);
	attackHitEffect->SetName(L"SpawnRioRHitEffect02");
	AddComponents(attackHitEffect, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	attackHitEffect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	attackHitEffect->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);
	attackHitEffect->MeshRender()->GetDynamicMaterial(0);
	animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"rioHit2_Orange.png");
	attackHitEffect->Animator2D()->CreateAnimation(L"rioHit2_Orange", animAtlas, Vec2(0.f, 0.f), Vec2(1024.f, 1024.f), Vec2(1024.f, 1024.f), 1, 15);
	attackHitEffect->Animator2D()->Play(L"rioHit2_Orange", true);

	dummyParent02->AddChild(attackHitEffect);

	auto startTime = std::chrono::high_resolution_clock::now();
	int restTime = 10;

	//0.4f�� ����ȵ� �ð�
	float endTime = 0.25f;
	float exeCount = endTime * 1000.f / restTime; //����Ǵ� Ƚ��

	//1.0f�� ���� ũ��, 3.0f�� ������ ũ��
	float startScale = 1.0f;
	float endScale = 5.0f;
	float scaleIncreasing = (endScale - startScale) / exeCount;

	//startMovePos�� ���� ��ġ, endMovePos�� ������ ��ġ
	float startMovePos = 0.f;
	float endMovePos = 1.0f;
	float moveIncreasing = (endMovePos - startMovePos) / exeCount;

	while (true) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);

		//�ð��� ������ ����
		if (elapsedTime.count() >= endTime * 1000.f)
			break;

		//ũ�� ����
		if (endScale >= startScale) {
			startScale += scaleIncreasing;
		}
		windHitEffect->Transform()->SetRelativeScale(Vec3(startScale / 1.f, startScale / 1.f, startScale / 1.f));
		attackHitEffect->Transform()->SetRelativeScale(Vec3(startScale / 2.f, startScale / 2.f, startScale / 2.f));

		//ȸ�� ����
		Vec3 rot = windHitEffect->Transform()->GetRelativeRot();
		rot.z += exeCount / 500.f;
		windHitEffect->Transform()->SetRelativeRot(rot);

		//�����̱�
		Vec3 pos = dummyParent01->Transform()->GetRelativePos();
		_effectMoveDir.y = 0.f;
		_effectMoveDir.Normalize();
		Vec3 resultPos = pos + (_effectMoveDir * moveIncreasing);
		dummyParent01->Transform()->SetRelativePos(resultPos);

		std::this_thread::sleep_for(std::chrono::milliseconds(restTime));
	}

	DestroyObject(dummyParent01);
	DestroyObject(dummyParent02);
}

void ER_EffectSystem::SpawnYukiQHitEffect(Vec3 _pos)
{
	//��Ʈ ����Ʈ1(��) ����
	CGameObject* swordHitEffect = onew(CGameObject);
	swordHitEffect->SetName(L"YukiQHitEffect01");
	AddComponents(swordHitEffect, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	swordHitEffect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	swordHitEffect->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);
	swordHitEffect->MeshRender()->GetDynamicMaterial(0);

	swordHitEffect->Transform()->SetRelativeRot(Vec3(0.f, Deg2Rad(-15.f), 0.f));
	swordHitEffect->Transform()->SetRelativeScale(Vec3(1.0f, 12.0f, 1.0f));

	Ptr<CTexture> animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"qburst01.png");
	swordHitEffect->Animator2D()->CreateAnimation(L"qburst01", animAtlas, Vec2(0.f, 0.f), Vec2(256.f, 256.f), Vec2(256.f, 256.f), 4, 20);
	swordHitEffect->Animator2D()->Play(L"qburst01", true);

	SpawnGameObject(swordHitEffect, _pos, L"Effect");

	//������ ���� ����
	auto startTime = std::chrono::high_resolution_clock::now();
	int restTime = 10;

	//0.4f�� ����ȵ� �ð�
	float endTime = 0.20f;
	float exeCount = endTime * 1000.f / restTime; //����Ǵ� Ƚ��

	while (true) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);

		//�ð��� ������ ����
		if (elapsedTime.count() >= endTime * 1000.f)
			break;

		std::this_thread::sleep_for(std::chrono::milliseconds(restTime));
	}

	DestroyObject(swordHitEffect);
}

void ER_EffectSystem::SpawnYukiR1HitEffect(Vec3 _pos, CGameObject* _hitter)
{
	//��Ʈ ����Ʈ1(��) ����
	CGameObject* swordHitEffect = onew(CGameObject);
	swordHitEffect->SetName(L"YukiRHitEffect01");
	AddComponents(swordHitEffect, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	swordHitEffect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	swordHitEffect->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);
	swordHitEffect->MeshRender()->GetDynamicMaterial(0);

	swordHitEffect->Transform()->SetRelativeRot(Vec3(Deg2Rad(60.f), Deg2Rad(-45.f), 0.f));
	swordHitEffect->Transform()->SetRelativeScale(Vec3(1.5f, 1.5f, 1.5f));

	Ptr<CTexture> animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"FX_BI_Yuki_01SE_01_Violet.png");
	swordHitEffect->Animator2D()->CreateAnimation(L"FX_BI_Yuki_01SE_01_Violet", animAtlas, Vec2(0.f, 0.f), Vec2(85.3f, 85.f), Vec2(85.3f, 85.f), 36, 36);
	swordHitEffect->Animator2D()->Play(L"FX_BI_Yuki_01SE_01_Violet", false);

	SpawnGameObject(swordHitEffect, _pos, L"Effect");

	//������ ���� ����
	auto startTime = std::chrono::high_resolution_clock::now();
	int restTime = 10;

	//0.4f�� ����ȵ� �ð�
	float endTime = 0.8f;
	float exeCount = endTime * 1000.f / restTime; //����Ǵ� Ƚ��

	while (true) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);

		//�ð��� ������ ����
		if (elapsedTime.count() >= endTime * 1000.f)
			break;

		Vec3 pos = _hitter->Transform()->GetRelativePos();
		pos.y += 2.5f;
		swordHitEffect->Transform()->SetRelativePos(pos);

		std::this_thread::sleep_for(std::chrono::milliseconds(restTime));
	}

	DestroyObject(swordHitEffect);
}

void ER_EffectSystem::SpawnYukiR2HitEffect(Vec3 _pos)
{
	//���� �θ� ����Ʈ01 ����
	CGameObject* dummyParent01 = onew(CGameObject);
	AddComponents(dummyParent01, _TRANSFORM);

	dummyParent01->Transform()->SetRelativeRot(Vec3(Deg2Rad(90.f), 0.f, 0.f));

	SpawnGameObject(dummyParent01, _pos, L"Effect");

	//��Ʈ ����Ʈ1(��) ����
	CGameObject* gunHitEffect = onew(CGameObject);
	gunHitEffect->SetName(L"YukiR2HitEffect01");
	AddComponents(gunHitEffect, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	gunHitEffect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	gunHitEffect->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);
	gunHitEffect->MeshRender()->GetDynamicMaterial(0);
	Ptr<CTexture> animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"qburst_Violet01.png");
	gunHitEffect->Animator2D()->CreateAnimation(L"qburst_Violet01", animAtlas, Vec2(0.f, 0.f), Vec2(256.f, 256.f), Vec2(256.f, 256.f), 4, 12);
	gunHitEffect->Animator2D()->Play(L"qburst_Violet01", true);

	dummyParent01->AddChild(gunHitEffect);

	//������ ���� ����
	auto startTime = std::chrono::high_resolution_clock::now();
	int restTime = 10;

	//0.4f�� ����ȵ� �ð�
	float endTime = 0.4f;
	float exeCount = endTime * 1000.f / restTime; //����Ǵ� Ƚ��

	//endScale�� ���� Ŀ���� ũ��
	float endScale = 1.f;
	float scaleIncreasing = endScale / restTime;

	while (true) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);

		//�ð��� ������ ����
		if (elapsedTime.count() >= endTime * 1000.f)
			break;

		Vec3 scale = dummyParent01->Transform()->GetRelativeScale();
		scale.x += scaleIncreasing;
		scale.y += scaleIncreasing;
		scale.z += scaleIncreasing;
		dummyParent01->Transform()->SetRelativeScale(scale);

		std::this_thread::sleep_for(std::chrono::milliseconds(restTime));
	}

	DestroyObject(dummyParent01);
}

void ER_EffectSystem::SpawnHyunwooQHitEffect(Vec3 _pos, CGameObject* _hitter)
{
	CGameObject* dummyParent = onew(CGameObject);
	AddComponents(dummyParent, _TRANSFORM);

	dummyParent->Transform()->SetRelativeRot(Vec3(Deg2Rad(90.f), 0.f, 0.f));

	SpawnGameObject(dummyParent, _pos, L"Effect");

	//��Ʈ ����Ʈ1 ����
	CGameObject* qHitEffect01 = onew(CGameObject);
	qHitEffect01->SetName(L"HyunwooQHitEffect");
	AddComponents(qHitEffect01, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	qHitEffect01->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	qHitEffect01->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);
	qHitEffect01->MeshRender()->GetDynamicMaterial(0);

	qHitEffect01->Transform()->SetRelativeRot(Vec3(Deg2Rad(5.f), 0.f, 0.f));
	qHitEffect01->Transform()->SetRelativeScale(Vec3(2.0f, 2.0f, 2.0f));

	Ptr<CTexture> animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"slow.png");
	qHitEffect01->Animator2D()->CreateAnimation(L"slow", animAtlas, Vec2(0.f, 0.f), Vec2(225.f, 225.f), Vec2(225.f, 225.f), 1, 36);
	qHitEffect01->Animator2D()->Play(L"slow", false);

	dummyParent->AddChild(qHitEffect01);

	//��Ʈ ����Ʈ1 ����
	CGameObject* qHitEffect02 = onew(CGameObject);
	qHitEffect02->SetName(L"HyunwooQHitEffect02");
	AddComponents(qHitEffect02, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	qHitEffect02->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	qHitEffect02->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);
	qHitEffect02->MeshRender()->GetDynamicMaterial(0);
	qHitEffect02->Transform()->SetRelativeRot(Vec3(-Deg2Rad(5.f), 0.f, 0.f));
	qHitEffect02->Transform()->SetRelativeScale(Vec3(2.0f, 2.0f, 2.0f));

	animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"slow.png");
	qHitEffect02->Animator2D()->CreateAnimation(L"slow", animAtlas, Vec2(0.f, 0.f), Vec2(225.f, 225.f), Vec2(225.f, 225.f), 1, 36);
	qHitEffect02->Animator2D()->Play(L"slow", false);

	dummyParent->AddChild(qHitEffect02);

	//������ ���� ����
	auto startTime = std::chrono::high_resolution_clock::now();
	int restTime = 10;

	//endTime�� ����ȵ� �ð�
	float endTime = 2.0f;
	float exeCount = endTime * 1000.f / restTime; //����Ǵ� Ƚ��

	while (true) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);

		//�ð��� ������ ����
		if (elapsedTime.count() >= endTime * 1000.f)
			break;

		//��ġ �̵�
		Vec3 pos = _hitter->Transform()->GetRelativePos();
		pos.y += 0.4f;
		dummyParent->Transform()->SetRelativePos(pos);

		//ȸ��
		Vec3 rot = qHitEffect01->Transform()->GetRelativeRot();
		rot.z += Deg2Rad(180.f) / exeCount;
		qHitEffect01->Transform()->SetRelativeRot(rot);
		rot = qHitEffect02->Transform()->GetRelativeRot();
		rot.z += Deg2Rad(360.f) / exeCount;
		qHitEffect02->Transform()->SetRelativeRot(rot);

		std::this_thread::sleep_for(std::chrono::milliseconds(restTime));
	}

	DestroyObject(dummyParent);
}

void ER_EffectSystem::SpawnHyunwooEHitEffect(Vec3 _pos, Vec3 _dir, CGameObject* _hitter)
{
	//���� �θ� ����Ʈ01 ����
	CGameObject* dummyParent01 = onew(CGameObject);
	AddComponents(dummyParent01, _TRANSFORM);

	dummyParent01->Transform()->SetRelativeRot(_dir);
	dummyParent01->Transform()->SetRelativeScale(Vec3(1.5f, 1.5f, 1.5f));

	SpawnGameObject(dummyParent01, _pos, L"Effect");

	//��Ʈ ����Ʈ1(��) ����
	CGameObject* gunHitEffect = onew(CGameObject);
	gunHitEffect->SetName(L"HyunwooEHitEffect");
	AddComponents(gunHitEffect, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	gunHitEffect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	gunHitEffect->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);
	gunHitEffect->MeshRender()->GetDynamicMaterial(0);
	gunHitEffect->Transform()->SetRelativeRot(Vec3(Deg2Rad(90.f), Deg2Rad(90.f), 0.f));

	Ptr<CTexture> animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"FX_BI_William_Skill04_ExpLine01.png");
	gunHitEffect->Animator2D()->CreateAnimation(L"FX_BI_William_Skill04_ExpLine01", animAtlas, Vec2(0.f, 0.f), Vec2(128.f, 85.f), Vec2(128.f, 85.f), 4, 12);
	gunHitEffect->Animator2D()->Play(L"FX_BI_William_Skill04_ExpLine01", true);

	dummyParent01->AddChild(gunHitEffect);

	//������ ���� ����
	auto startTime = std::chrono::high_resolution_clock::now();
	int restTime = 10;

	//0.4f�� ����ȵ� �ð�
	float endTime = 0.2f;
	float exeCount = endTime * 1000.f / restTime; //����Ǵ� Ƚ��

	while (true) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);

		//�ð��� ������ ����
		if (elapsedTime.count() >= endTime * 1000.f)
			break;

		Vec3 pos = _hitter->Transform()->GetRelativePos();
		pos.y += 1.8f;
		pos.z -= 0.2f;
		pos.x += 0.2f;
		dummyParent01->Transform()->SetRelativePos(pos);

		std::this_thread::sleep_for(std::chrono::milliseconds(restTime));
	}

	DestroyObject(dummyParent01);
}

void ER_EffectSystem::AddDeleteParticles(CGameObject* _obj, CGameObject* _parentObj)
{
	lock_guard<mutex> lockGuard(m_aFuncMutex);

	if (_obj->ParticleSystem() == nullptr)
		assert(false);

	CParticleSystem* Particle = _obj->ParticleSystem();
	tParticleModule particle_data = Particle->GetParticleInfo();
	m_vParticleLifeTime.push_back(particle_data.MaxLifeTime + 0.1f);
	particle_data.MinLifeTime = 0.f;
	particle_data.MaxLifeTime = 0.f;
	particle_data.SpawnRate = 0;
	Particle->SetParticleInfo(particle_data);

	if(_parentObj != nullptr)
		m_vDeleteParticles.push_back(_parentObj);
	else 
		m_vDeleteParticles.push_back(_obj);
}

void ER_EffectSystem::SpawnLevelUpEffect(CGameObject* _Owner)
{
	//���� �θ� ����Ʈ01 ����
	CGameObject* dummyParent01 = onew(CGameObject);
	AddComponents(dummyParent01, _TRANSFORM);

	dummyParent01->Transform()->SetRelativeScale(Vec3(1.5f, 1.5f, 1.5f));

	SpawnGameObject(dummyParent01, L"Effect");

	//������ ����Ʈ ����
	CGameObject* gunHitEffect = onew(CGameObject);
	gunHitEffect->SetName(L"LevelUpEffectAnim");
	AddComponents(gunHitEffect, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	gunHitEffect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	gunHitEffect->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);
	gunHitEffect->MeshRender()->GetDynamicMaterial(0);
	gunHitEffect->Transform()->SetRelativeRot(Vec3(Deg2Rad(30.f), -Deg2Rad(45.f), 0.f));
	gunHitEffect->Transform()->SetRelativePos(Vec3(0.f, 3.0f, 0.f));

	Ptr<CTexture> animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"FX_BI_Levelup_01.png");
	gunHitEffect->Animator2D()->CreateAnimation(L"FX_BI_Levelup_01", animAtlas, Vec2(0.f, 0.f), Vec2(256.f, 256.f), Vec2(256.f, 256.f), 1, 12);
	gunHitEffect->Animator2D()->Play(L"FX_BI_Levelup_01", true);

	dummyParent01->AddChild(gunHitEffect);


	auto startTime = std::chrono::high_resolution_clock::now();
	int restTime = 10;

	float endTime = 0.5f;
	float exeCount = endTime * 1000.f / restTime; //����Ǵ� Ƚ��

	//�ִϸ��̼��� ���İ��� ������� �ð�
	float alphaTime = 0.1f;
	if (endTime < 0.2f)
		alphaTime = 0.05f;

	bool alphaTrigger = true;
	while (true) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);

		//�ð��� ������ ����
		if (elapsedTime.count() >= endTime * 1000.f)
			break;

		if ((endTime * 1000.f) - elapsedTime.count() <= alphaTime * 1000.f && alphaTrigger) {
			gunHitEffect->Animator2D()->SetAlphaEraseTime(alphaTime);
			alphaTrigger = false;
		}

		dummyParent01->Transform()->SetRelativePos(_Owner->Transform()->GetRelativePos());

		std::this_thread::sleep_for(std::chrono::milliseconds(restTime));
	}

	if (dummyParent01 != nullptr)
		DestroyObject(dummyParent01);
}
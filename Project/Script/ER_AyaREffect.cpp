#include "pch.h"
#include "ER_AyaREffect.h"

void ER_AyaREffect::begin()
{
}

void ER_AyaREffect::tick()
{
}

void ER_AyaREffect::SpawnEffect(Vec3 _pos, Vec3 _dir, float _scale)
{
}

void ER_AyaREffect::RangeSpawn(Vec3 _pos)
{
	CGameObject* tdExample01 = onew(CGameObject);
	AddComponents(tdExample01, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	tdExample01->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	tdExample01->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);

	tdExample01->Transform()->SetRelativePos(_pos);
	tdExample01->Transform()->SetRelativeRot(Vec3(Deg2Rad(90.f), 0.f, 0.f));
	tdExample01->Transform()->SetRelativeScale(Vec3(16.f, 16.f, 16.f));

	Ptr<CTexture> animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"rRange3.png");
	tdExample01->Animator2D()->CreateAnimation(L"rRange3", animAtlas, Vec2(0.f, 0.f), Vec2(256.f, 256.f), Vec2(256.f, 256.f), 1, 15);
	tdExample01->Animator2D()->Play(L"rRange3", true);

	SpawnGameObject(tdExample01, L"Effect");

	std::thread t(&ER_AyaREffect::RangeSpawnToMultiThread, this, tdExample01);
	t.detach();
}

void ER_AyaREffect::RangeSpawnToMultiThread(CGameObject* _obj01)
{
	auto startTime = std::chrono::high_resolution_clock::now();
	int restTime = 10;

	float endTime = 2.0f;
	float exeCount = endTime * 1000.f / restTime; //실행되는 횟수

	//애니메이션의 알파값이 사라지는 시간
	float alphaTime = 0.1f;
	if (endTime < 0.2f)
		alphaTime = 0.05f;

	//타겟 오브젝트가 애니메이터 2D를 가지고 있을 때만 계산
	bool alphaTrigger;
	if (_obj01->Animator2D())
		alphaTrigger = true;
	else
		alphaTrigger = false;

	while (true) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);

		//시간이 지나면 종료
		if (elapsedTime.count() >= endTime * 1000.f)
			break;

		if ((endTime * 1000.f) - elapsedTime.count() <= alphaTime * 1000.f && alphaTrigger) {
			_obj01->Animator2D()->SetAlphaEraseTime(alphaTime);
			alphaTrigger = false;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(restTime));
	}

	DestroyObject(_obj01);
}

void ER_AyaREffect::AttackSpawn(Vec3 _pos)
{
	float scaleArr[6] = { 1.0f, 2.0f, 1.8f, 1.6f, 1.3f, 1.0f };
	_pos.y += 1.0f;

	CGameObject* dummyParent = onew(CGameObject);
	AddComponents(dummyParent, _TRANSFORM);
	dummyParent->Transform()->SetRelativeRot(Vec3(Deg2Rad(90.f), 0.f, 0.f));
	SpawnGameObject(dummyParent, _pos, L"Effect");

	CGameObject* tdExample01 = nullptr;
	Ptr<CTexture> animAtlas = nullptr;
	float offsetY = 0.f;
	for (int i = 0; i < 6; ++i) {
		tdExample01 = onew(CGameObject);
		AddComponents(tdExample01, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
		tdExample01->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		tdExample01->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);
		
		tdExample01->Transform()->SetRelativePos(Vec3(0.f,0.f,-offsetY));
		offsetY += 0.4f;
		tdExample01->Transform()->SetRelativeScale(Vec3(scaleArr[i], scaleArr[i], scaleArr[i]));

		animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"rShot.png");
		tdExample01->Animator2D()->CreateAnimation(L"rShot", animAtlas, Vec2(0.f, 0.f), Vec2(256.f, 256.f), Vec2(256.f, 256.f), 1, 15);
		tdExample01->Animator2D()->Play(L"rShot", true);
		
		dummyParent->AddChild(tdExample01);
	}

	CGameObject* noise = new CGameObject();
	AddComponents(noise, _TRANSFORM | _MESHRENDER);
	noise->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CircleMesh"));
	noise->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DistortionMtrl"), 0);

	noise->Transform()->SetRelativeScale(Vec3(16.f, 16.f, 16.f));
	noise->Transform()->SetRelativeRot(Vec3(Deg2Rad(45.f), Deg2Rad(-45.f), 0.f));
	noise->Transform()->SetRelativeRot(Vec3(Deg2Rad(90.f), 0.f, 0.f));

	_pos.y -= 1.0f;
	SpawnGameObject(noise, _pos, L"Effect");

	std::thread t(&ER_AyaREffect::AttackSpawnToMultiThread, this, dummyParent, noise);
	t.detach();
}

void ER_AyaREffect::AttackSpawnToMultiThread(CGameObject* _obj, CGameObject* _noise)
{
	auto startTime = std::chrono::high_resolution_clock::now();
	int restTime = 10;

	float endTime = 0.5f;
	float exeCount = endTime * 1000.f / restTime; //실행되는 횟수

	Vec3 startScale = Vec3(1.f, 1.f, 1.f);
	Vec3 endScale = Vec3(2.0f, 2.0f, 2.0f);
	float scaleIncreasing = (endScale.x - startScale.x) / exeCount;

	//애니메이션의 알파값이 사라지는 시간
	float alphaTime = 0.1f;
	if (endTime < 0.2f)
		alphaTime = 0.05f;

	bool alphaTrigger = true;
	while (true) {
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);

		//시간이 지나면 종료
		if (elapsedTime.count() >= endTime * 1000.f)
			break;

		//크기 변경
		if (endScale.x >= startScale.x) {
			startScale.x += scaleIncreasing;
			startScale.y += scaleIncreasing;
			startScale.z += scaleIncreasing;
		}
		_obj->Transform()->SetRelativeScale(startScale);

		//회전 변경
		Vec3 rot = _obj->Transform()->GetRelativeRot();
		rot.y += exeCount / 1000.f;
		_obj->Transform()->SetRelativeRot(rot);

		if ((endTime * 1000.f) - elapsedTime.count() <= alphaTime * 1000.f && alphaTrigger) {
			//타겟 오브젝트가 애니메이터 2D를 가지고 있을 때만 계산
			for (int i = 0; i < _obj->GetChild().size(); ++i) {
				_obj->GetChild()[i]->Animator2D()->SetAlphaEraseTime(alphaTime);
			}
			alphaTrigger = false;
		}
		

		std::this_thread::sleep_for(std::chrono::milliseconds(restTime));
	}

	DestroyObject(_obj);
	DestroyObject(_noise);
}

void ER_AyaREffect::SaveToLevelFile(FILE* _File)
{
}

void ER_AyaREffect::LoadFromLevelFile(FILE* _FILE)
{
}

ER_AyaREffect::ER_AyaREffect()
{
}

ER_AyaREffect::~ER_AyaREffect()
{
}

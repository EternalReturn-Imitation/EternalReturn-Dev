#include "pch.h"
#include "HyunwooEEffect.h"

void HyunwooEEffect::begin()
{
}

void HyunwooEEffect::tick()
{
}

void HyunwooEEffect::SpawnEffect(Vec3 _pos, Vec3 _dir, float _scale)
{
	CGameObject* dummyParent01 = new CGameObject();
	AddComponents(dummyParent01, _TRANSFORM);

	dummyParent01->Transform()->SetRelativeRot(_dir);
	
	_pos.y += 1.5f;
	SpawnGameObject(dummyParent01, _pos, L"Effect");

	CGameObject* tdExample01 = new CGameObject();
	tdExample01->SetName(L"HyunwooEEffect");
	AddComponents(tdExample01, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	tdExample01->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	tdExample01->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);
	tdExample01->MeshRender()->GetDynamicMaterial(0);

	tdExample01->Transform()->SetRelativeRot(Vec3(Deg2Rad(90.f), 0.f, 0.f));
	tdExample01->Transform()->SetRelativeScale(Vec3(0.5f, 3.0f, 2.0f));
	Vec3 pos = tdExample01->Transform()->GetRelativePos();
	tdExample01->Transform()->SetRelativePos(pos);

	Ptr<CTexture> animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"FX_Wind01.png");
	tdExample01->Animator2D()->CreateAnimation(L"FX_Wind01", animAtlas, Vec2(0.f, 0.f), Vec2(51.2f, 256.f), Vec2(51.2f, 256.f), 5, 15);
	tdExample01->Animator2D()->Play(L"FX_Wind01", true);

	dummyParent01->AddChild(tdExample01);

	std::thread t1(&HyunwooEEffect::SpawnEEffect, this, tdExample01, 0.2f, dummyParent01);
	t1.detach();
}

void HyunwooEEffect::SpawnEEffect(CGameObject* _targetObj, float _endTime, CGameObject* _dummyParent)
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

		Vec3 pos = Transform()->GetRelativePos();
		pos.y += 1.5f;
		_dummyParent->Transform()->SetRelativePos(pos);

		std::this_thread::sleep_for(std::chrono::milliseconds(restTime));
	}

	if (_dummyParent != nullptr)
		DestroyObject(_dummyParent);
	else {
		DestroyObject(_targetObj);
	}
}

void HyunwooEEffect::SaveToLevelFile(FILE* _File)
{
}

void HyunwooEEffect::LoadFromLevelFile(FILE* _FILE)
{
}

HyunwooEEffect::HyunwooEEffect()
{
}

HyunwooEEffect::~HyunwooEEffect()
{
}

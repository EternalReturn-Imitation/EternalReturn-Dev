#include "pch.h"
#include "ER_HyunwooWEffect.h"

ER_HyunwooWEffect::ER_HyunwooWEffect()
{
}

ER_HyunwooWEffect::~ER_HyunwooWEffect()
{
}

void ER_HyunwooWEffect::begin()
{
}

void ER_HyunwooWEffect::tick()
{
}

void ER_HyunwooWEffect::SpawnEffect(Vec3 _pos, Vec3 _dir, float _scale)
{
	CGameObject* dummyParent01 = new CGameObject();
	AddComponents(dummyParent01, _TRANSFORM);

	_pos.y += 1.2f;
	_pos.z -= 0.2f;
	_pos.x += 0.2f;

	SpawnGameObject(dummyParent01, _pos, L"Effect");

	CGameObject* tdExample01 = new CGameObject();
	AddComponents(tdExample01, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	tdExample01->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	tdExample01->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);

	tdExample01->Transform()->SetRelativeRot(Vec3(Deg2Rad(60.f), -Deg2Rad(45.f), 0.f));
	tdExample01->Transform()->SetRelativeScale(Vec3(4.0f, 4.0f, 4.0f));
	Vec3 pos = tdExample01->Transform()->GetRelativePos();
	tdExample01->Transform()->SetRelativePos(pos);

	Ptr<CTexture> animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"FX_BI_Hit_02.png");
	tdExample01->Animator2D()->CreateAnimation(L"FX_BI_Hit_02", animAtlas, Vec2(0.f, 0.f), Vec2(170.6666f, 171.f), Vec2(170.6666f, 171.f), 9, 9);
	tdExample01->Animator2D()->Play(L"FX_BI_Hit_02", true);

	dummyParent01->AddChild(tdExample01);

	CGameObject* tdExample02 = new CGameObject();
	AddComponents(tdExample02, _TRANSFORM | _MESHRENDER | _ANIMATOR2D);
	tdExample02->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	tdExample02->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimMtrl"), 0);

	tdExample02->Transform()->SetRelativeRot(Vec3(Deg2Rad(60.f), -Deg2Rad(45.f), 0.f));
	tdExample02->Transform()->SetRelativeScale(Vec3(1.2f, 1.2f, 1.2f));
	pos = tdExample02->Transform()->GetRelativePos();
	pos.y += 3.3f;
	tdExample02->Transform()->SetRelativePos(pos);

	animAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"FX_BI_Shield_02.png");
	tdExample02->Animator2D()->CreateAnimation(L"FX_BI_Shield_02", animAtlas, Vec2(0.f, 0.f), Vec2(128.f, 128.f), Vec2(128.f, 128.f), 9, 17);
	tdExample02->Animator2D()->Play(L"FX_BI_Shield_02", true);

	dummyParent01->AddChild(tdExample02);

	std::thread t1(&ER_HyunwooWEffect::SpawnWEffect, this, tdExample01, 2.0f, dummyParent01);
	t1.detach();
}

void ER_HyunwooWEffect::SpawnWEffect(CGameObject* _targetObj, float _endTime, CGameObject* _dummyParent)
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
		pos.y += 1.4f;
		pos.z -= 0.5f;
		pos.x += 0.5f;
		_dummyParent->Transform()->SetRelativePos(pos);

		std::this_thread::sleep_for(std::chrono::milliseconds(restTime));
	}

	if (_dummyParent != nullptr)
		DestroyObject(_dummyParent);
	else {
		DestroyObject(_targetObj);
	}
}

void ER_HyunwooWEffect::SaveToLevelFile(FILE* _File)
{
}

void ER_HyunwooWEffect::LoadFromLevelFile(FILE* _FILE)
{
}
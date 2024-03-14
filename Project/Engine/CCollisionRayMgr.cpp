#include "pch.h"
#include "CCollisionMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CCollider2D.h"
#include "CCollider3D.h"
#include "CTransform.h"
#include "CPathFindMgr.h"
#include "CRenderMgr.h"
#include "CCamera.h"

#include "CKeyMgr.h"

void CCollisionMgr::RayCollideCheck()
{
	CLevel* CurLevel = CLevelMgr::GetInst()->GetCurLevel();

	//Map Collider가 없으면 리턴
	CGameObject* Map = CPathFindMgr::GetInst()->GetMapCollider();
	if (!Map)
		return;

	CCamera* mainCam = CRenderMgr::GetInst()->GetMainCam();
	tRay ray = mainCam->GetRay();
	IntersectResult result = IsCollidingBtwRayRect(ray, Map);
	//MapCol범위 벗어나면 계산 안함
	if (!result.bResult)
		return;

	vector<CGameObject*> rayColResult = CheckRayRangeObject(result);

	CollisionID id = {};
	for (UINT i = 0; i < rayColResult.size(); ++i) {
		id.id = rayColResult[i]->GetID();
		map<UINT_PTR, bool>::iterator iter = m_mRayColID.find(id.id);
		if (iter == m_mRayColID.end())
		{
			m_mRayColID.insert(make_pair(id.id, false));
			iter = m_mRayColID.find(id.id);
		}

		m_mPrevRayColID.erase(rayColResult[i]);

		// 계속 충돌중이라면
		if (iter->second)
		{
			if (rayColResult[i]->Collider2D())
				rayColResult[i]->Collider2D()->OnRayOverlap();
			if (rayColResult[i]->Collider3D())
				rayColResult[i]->Collider3D()->OnRayOverlap();
		}
		//처음 충돌이라면
		else {
			if (rayColResult[i]->Collider2D())
				rayColResult[i]->Collider2D()->BeginRayOverlap();
			if (rayColResult[i]->Collider3D())
				rayColResult[i]->Collider3D()->BeginRayOverlap();

			iter->second = true;
		}
	}

	for (auto it : m_mPrevRayColID) {
		if (it->Collider2D())
			it->Collider2D()->EndRayOverlap();
		if (it->Collider3D())
			it->Collider3D()->EndRayOverlap();

		map<UINT_PTR, bool>::iterator iter = m_mRayColID.find(it->GetID());
		iter->second = false;
	}

	m_mPrevRayColID.clear();
	for (UINT i = 0; i < rayColResult.size(); ++i) {
		m_mPrevRayColID.insert(rayColResult[i]);
	}
}

vector<CGameObject*> CCollisionMgr::CheckRayRangeObject(IntersectResult _intersectResult)
{
	vector<CGameObject*> semiResult;
	vector<CGameObject*> result;

	CLevel* CurLevel = CLevelMgr::GetInst()->GetCurLevel();
	for (UINT i = 0; i < MAX_LAYER; ++i) {
		vector<CGameObject*> Objects = CurLevel->GetLayer(i)->GetObjects();

		//Rayer체크 해제 시 안씀.
		if (m_bRayCol[i] == 0)
			continue;

		for (UINT j = 0; j < Objects.size(); ++j) {
			Vec3 objTransform = Objects[j]->Transform()->GetRelativePos();
			Vec3 objScale = Objects[j]->Transform()->GetRelativeScale();

			//Collider 컴포넌트가 없으면 안씀
			if (Objects[j]->Collider2D()) {
				objTransform += Objects[j]->Collider2D()->GetOffsetPos();
				Vec3 colScale = Objects[j]->Collider2D()->GetOffsetScale();
				objScale.x += colScale.x;
				objScale.y += colScale.z;
				objScale.z += colScale.y;
			}
			else if (Objects[j]->Collider3D()) {
				objTransform += Objects[j]->Collider3D()->GetOffsetPos();
				objScale += Objects[j]->Collider3D()->GetOffsetScale();
			}
			else {
				continue;
			}
			objScale *= 1.5;

			//탑뷰겜이라 y값은 계산 안함.
			objTransform.y = 0.f;
			_intersectResult.vCrossPoint.y = 0.f;

			Vec3 objRange[2];
			objRange[0] = Vec3(objTransform.x - (objScale.x / 2), objTransform.y - (objScale.y / 2), objTransform.z - (objScale.z / 2));
			objRange[1] = Vec3(objTransform.x + (objScale.x / 2), objTransform.y + (objScale.y / 2), objTransform.z + (objScale.z / 2));

			if (objRange[0].x< _intersectResult.vCrossPoint.x && objRange[1].x > _intersectResult.vCrossPoint.x
				&& objRange[0].y< _intersectResult.vCrossPoint.y && objRange[1].y > _intersectResult.vCrossPoint.y
				&& objRange[0].z< _intersectResult.vCrossPoint.z && objRange[1].z > _intersectResult.vCrossPoint.z) {
				semiResult.push_back(Objects[j]);
			}
		}
	}


	CCamera* mainCam = CRenderMgr::GetInst()->GetMainCam();
	tRay ray = mainCam->GetRay();
	IntersectResult rayResult;

	for (UINT i = 0; i < semiResult.size(); ++i) {
		if (semiResult[i]->Collider2D()) {
			rayResult = CCollisionMgr::GetInst()->IsCollidingBtwRayRect(ray, semiResult[i]);
		}
		else if (semiResult[i]->Collider3D()) {
			switch (semiResult[i]->Collider3D()->GetColliderShape())
			{
			case COLLIDER3D_TYPE::CUBE:
				rayResult = CCollisionMgr::GetInst()->IsCollidingBtwRayCube(ray, semiResult[i]);
				break;
			case COLLIDER3D_TYPE::SPHERE:
				rayResult = CCollisionMgr::GetInst()->IsCollidingBtwRaySphere(ray, semiResult[i]);
				break;
			}
		}

		if (rayResult.bResult) {
			result.push_back(semiResult[i]);
		}
	}

	return result;
}

IntersectResult CCollisionMgr::IsCollidingBtwRayRect(tRay& _ray, CGameObject* _Object)
{
	// 만약에 Collider2D가 없거나 Rect모양이 아닌 경우 return
	if (_Object->Collider2D() == nullptr)
		return IntersectResult{ Vec3(0.f, 0.f, 0.f), 0.f, false };
	if (_Object->Collider2D()->GetCollider2DType() != COLLIDER2D_TYPE::RECT)
		return IntersectResult{ Vec3(0.f, 0.f, 0.f), 0.f, false };

	Matrix ColliderWorldMat = _Object->Collider2D()->GetColliderWorldMat();

	// Local Rect의 3개의 Pos에 대해서 World Pos 계산을 해준다.
	// 점 4개까지 필요없고 3개만 있으면 됨
	// 아래의 정점들의 순서는 의미가 있음
	// IntersectsLay함수에서 특정 정점의 순서에서만 올바르게 계산하도록 함
	Vec3 arrLocal[3] =
	{
		Vec3(-0.5f, -0.5f, 0.f),
		Vec3(0.5f, -0.5f, 0.f),
		Vec3(-0.5f, 0.5f, 0.f),
	};

	for (int i = 0; i < 3; ++i)
		arrLocal[i] = Vector4::Transform(Vec4(arrLocal[i], 1.f), ColliderWorldMat);


	return IntersectsLay(arrLocal, _ray);
}

IntersectResult CCollisionMgr::IsCollidingBtwRayCube(tRay& _ray, CGameObject* _Object)
{
	// 만약에 Collider3D가 없거나 Cube모양이 아닌 경우 return
	if (_Object->Collider3D() == nullptr || _Object->Collider3D()->GetCollider3DType() != COLLIDER3D_TYPE::CUBE)
		return IntersectResult{ Vec3(0.f, 0.f, 0.f), 0.f, false };

	Matrix ColliderWorldMat = _Object->Collider3D()->GetColliderWorldMat();

	// 위의 IsCollidingBtwRayRect함수와 같은 알고리즘으로 계산
	// 단 Cube는 면이 6개이기 때문에 6번 계산해 줘야 하는 것임
	Vec3 arrLocal[6][3] =
	{
		{Vec3(-0.5f, 0.5f, -0.5f),  Vec3(0.5f, 0.5f, -0.5f),  Vec3(-0.5f, 0.5f, 0.5f)},	 // 윗면
		{Vec3(-0.5f, -0.5f, -0.5f), Vec3(0.5f, -0.5f, -0.5f), Vec3(-0.5f, -0.5f, 0.5f)}, // 밑면
		{Vec3(-0.5f, -0.5f, -0.5f), Vec3(0.5f, -0.5f, -0.5f), Vec3(-0.5f, 0.5f, -0.5f)}, // 앞면
		{Vec3(-0.5f, -0.5f, 0.5f),  Vec3(0.5f, -0.5f, 0.5f),  Vec3(-0.5f, 0.5f, 0.5f)},  // 뒷면
		{Vec3(-0.5f, 0.5f, -0.5f),  Vec3(-0.5f, -0.5f, -0.5f),Vec3(-0.5f, 0.5f, 0.5f)},  // 왼쪽면
		{Vec3(0.5f, 0.5f, -0.5f),   Vec3(0.5f, -0.5f, -0.5f), Vec3(0.5f, 0.5f, 0.5f)},   // 오른쪽면
	};

	for (int i = 0; i < 6; ++i)
		for (int j = 0; j < 3; ++j)
			arrLocal[i][j] = Vec4::Transform(Vec4(arrLocal[i][j], 1.f), ColliderWorldMat);

	IntersectResult Final = IntersectResult{ Vec3(0.f, 0.f, 0.f), 0.f, false };
	IntersectResult Temp;

	for (int i = 0; i < 6; ++i)
	{
		Temp = IntersectsLay(arrLocal[i], _ray);

		if (Temp.bResult == true)
		{
			if (Final.bResult == false)
			{
				Final = Temp;
			}
			else
			{
				if (Temp.fResult < Final.fResult)
					Final = Temp;
			}
		}
	}

	return Final;
}

IntersectResult CCollisionMgr::IsCollidingBtwRaySphere(tRay& _ray, CGameObject* _Object)
{
	IntersectResult result;

	// 만약에 Collider3D가 없거나 Sphere모양이 아닌 경우 return
	if (_Object->Collider3D() == nullptr || _Object->Collider3D()->GetCollider3DType() != COLLIDER3D_TYPE::SPHERE)
		return IntersectResult{ Vec3(0.f, 0.f, 0.f), 0.f, false };

	// 일단 있다가 작성..
	Vec3 SpherePos = _Object->Transform()->GetRelativePos();
	Vec3 SphereRadiusAll = _Object->Transform()->GetRelativeScale();
	float SphereRadius = SphereRadiusAll.x / 2.f;

	Vec3 m = _ray.vStart - SpherePos;

	// 레이의 방향과 m 벡터의 내적을 계산합니다.
	float b = m.x * _ray.vDir.x + m.y * _ray.vDir.y + m.z * _ray.vDir.z;

	// m 벡터의 제곱과 구의 반지름의 제곱의 차를 계산합니다.
	float c = (m.x * m.x + m.y * m.y + m.z * m.z) - SphereRadius * SphereRadius;

	// 레이의 원점이 구의 외부에 있고 (c > 0), 레이가 구를 향하고 있지 않으면 (b > 0) false를 반환합니다.
	if (c > 0.0f && b > 0.0f)
	{
		result.vCrossPoint = Vec3(0.f, 0.f, 0.f);
		result.fResult = 0.f;
		result.bResult = false;
		return result;
	}

	// 판별식을 계산합니다. 판별식이 음수이면 레이가 구를 놓치는 것을 의미합니다.
	float discr = b * b - c;

	// 판별식이 음수이면 레이가 구를 놓친 것이므로 false를 반환합니다.
	if (discr < 0.0f)
	{
		result.vCrossPoint = Vec3(0.f, 0.f, 0.f);
		result.fResult = 0.f;
		result.bResult = false;
		return result;
	}

	// 이제 레이가 구와 교차하는 것이 확인되었으므로, 교차점의 가장 작은 t 값을 계산합니다.
	float t = -b - sqrt(discr);

	// 만약 t가 음수이면 레이가 구 내부에서 시작된 것이므로 t를 0으로 설정합니다.
	if (t < 0.0f) t = 0.0f;

	// 레이와 교차한 깊이값을 계산합니다.
	float depth = XMVectorGetX(XMVector3Length(_ray.vDir) * t);

	//m_LayMinDistance = depth;  // 현재 디스탠스 값으로 업데이트
	result.vCrossPoint = _ray.vStart + _ray.vDir * t;
	result.fResult = depth;
	result.bResult = true;

	return result;
}

IntersectResult CCollisionMgr::IntersectsLay(Vec3* _vertices, tRay _ray)
{
	IntersectResult result;
	result.vCrossPoint = Vec3(0.f, 0.f, 0.f);
	result.bResult = false;

	Vec3 edge[2] = { Vec3(), Vec3() };
	edge[0] = _vertices[1] - _vertices[0];
	edge[1] = _vertices[2] - _vertices[0];

	Vec3 normal = (edge[0].Cross(edge[1])).Normalize();
	float b = normal.Dot(_ray.vDir);


	Vec3 w0 = _ray.vStart - _vertices[0];
	float a = -(normal.Dot(w0));
	float t = a / b;

	result.fResult = t;

	Vec3 p = _ray.vStart + t * _ray.vDir;

	result.vCrossPoint = p;

	float uu, uv, vv, wu, wv, inverseD;
	uu = edge[0].Dot(edge[0]);
	uv = edge[0].Dot(edge[1]);
	vv = edge[1].Dot(edge[1]);

	Vec3 w = p - _vertices[0];
	wu = w.Dot(edge[0]);
	wv = w.Dot(edge[1]);

	inverseD = uv * uv - uu * vv;
	inverseD = 1.0f / inverseD;

	float u = (uv * wv - vv * wu) * inverseD;
	if (u < 0.0f || u > 1.0f)
	{
		result.vCrossPoint = Vec3();
		result.fResult = 0.0f;
		result.bResult = false;
		return result;
	}

	float v = (uv * wu - uu * wv) * inverseD;
	if (v < 0.0f || v > 1.0f)
	{
		result.vCrossPoint = Vec3();
		result.fResult = 0.0f;
		result.bResult = false;
		return result;
	}

	result.bResult = true;
	return result;
}
#include "pch.h"
#include "CCollisionMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CCollider3D.h"
#include "CTransform.h"


void CCollisionMgr::CollisionBtw3DObject(CGameObject* _LeftObject, CGameObject* _RightObject)
{
	// 충돌체를 보유하지 않은 경우
	if (!(_LeftObject->Collider3D() && _RightObject->Collider3D()))
		return;

	// 충돌체 ID 생성
	CollisionID id = {};
	if (_LeftObject->Collider3D()->GetID() < _RightObject->Collider3D()->GetID()) {
		id.LeftID = _LeftObject->Collider3D()->GetID();
		id.RightID = _RightObject->Collider3D()->GetID();
	}
	else {
		id.LeftID = _RightObject->Collider3D()->GetID();
		id.RightID = _LeftObject->Collider3D()->GetID();
	}

	// ID 검색
	map<UINT_PTR, bool>::iterator iter = m_mapColID.find(id.id);
	if (iter == m_mapColID.end())
	{
		m_mapColID.insert(make_pair(id.id, false));
		iter = m_mapColID.find(id.id);
	}

	// 둘 중 하나라도 삭제 예정 상태라면(Dead 상태)
	bool bDead = false;
	if (_LeftObject->IsDead() || _RightObject->IsDead())
	{
		bDead = true;
	}

	// 두 충돌체가 지금 충돌 중인지 확인
	if (CollisionBtw3DCollider(_LeftObject->Collider3D(), _RightObject->Collider3D()))
	{
		// 이전에 충돌한 적이 있고, 둘중 하나 이상이 삭제 예정이라면
		if (bDead && iter->second)
		{
			_LeftObject->Collider3D()->EndOverlap(_RightObject->Collider3D());
			_RightObject->Collider3D()->EndOverlap(_LeftObject->Collider3D());
		}
		else if (iter->second)
		{
			// 이전에도 충돌한적이 있었고, 이번 프레임에서도 충돌중이다.
			_LeftObject->Collider3D()->OnOverlap(_RightObject->Collider3D());
			_RightObject->Collider3D()->OnOverlap(_LeftObject->Collider3D());
		}
		else
		{
			// 이번 프레임에 처음 충돌했다.
			if (!bDead) // 둘중 하나라도 Dead 상태면 충돌을 무시한다.
			{
				_LeftObject->Collider3D()->BeginOverlap(_RightObject->Collider3D());
				_RightObject->Collider3D()->BeginOverlap(_LeftObject->Collider3D());
				iter->second = true;
			}
		}
	}

	else
	{
		// 저번 프레임에선 충돌했었는데 이번 프레임에선 충돌을 안했다.
		if (iter->second)
		{
			_LeftObject->Collider3D()->EndOverlap(_RightObject->Collider3D());
			_RightObject->Collider3D()->EndOverlap(_LeftObject->Collider3D());
			iter->second = false;
		}
	}
}

bool CCollisionMgr::CollisionBtw3DCollider(CCollider3D* _pLeft, CCollider3D* _pRight)
{
	if (_pLeft->GetCollider3DType() == COLLIDER3D_TYPE::CUBE && _pRight->GetCollider3DType() == COLLIDER3D_TYPE::CUBE)
	{
		// 사각형의 꼭지점(local space에서의 좌표)을 나타내는 배열
		Vec3 arrLocal[8] =
		{
			Vec3(-0.5f, 0.5f, 0.5f),
			Vec3(0.5f, 0.5f, 0.5f),
			Vec3(0.5f, -0.5f, 0.5f),
			Vec3(-0.5f, -0.5f, 0.5f),
			Vec3(-0.5f, 0.5f, -0.5f),
			Vec3(0.5f, 0.5f, -0.5f),
			Vec3(0.5f, -0.5f, -0.5f),
			Vec3(-0.5f, -0.5f, -0.5f)
		};

		// 두 충돌체의 각 표면 벡터 3개씩 구함
		// 투영축 6개를 구한다.
		Vec3 arrProj[6] = {};

		// 왼쪽 충돌체 4->0로 가는 벡터
		arrProj[0] = XMVector3TransformCoord(arrLocal[0], _pLeft->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[4], _pLeft->GetColliderWorldMat());
		// 왼쪽 충돌체 4->5
		arrProj[1] = XMVector3TransformCoord(arrLocal[5], _pLeft->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[4], _pLeft->GetColliderWorldMat());
		// 왼쪽 충돌체 4->7
		arrProj[2] = XMVector3TransformCoord(arrLocal[7], _pLeft->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[4], _pLeft->GetColliderWorldMat());
		// 오른쪽 충돌체 4->0로 가는 벡터
		arrProj[3] = XMVector3TransformCoord(arrLocal[0], _pRight->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[4], _pRight->GetColliderWorldMat());
		// 오른쪽 충돌체 4->5
		arrProj[4] = XMVector3TransformCoord(arrLocal[5], _pRight->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[4], _pRight->GetColliderWorldMat());
		// 오른쪽 충돌체 4->7
		arrProj[5] = XMVector3TransformCoord(arrLocal[7], _pRight->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[4], _pRight->GetColliderWorldMat());

		// 두 충돌체의 중심점을 구함
		Vec3 vCenter = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _pRight->GetColliderWorldMat()) - XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _pLeft->GetColliderWorldMat());

		// 분리축 테스트
		for (int i = 0; i < 6; ++i)
		{
			Vec3 vProj = arrProj[i];
			vProj.Normalize(); //단위벡터로 만든다.


			// 6개의 표면백터를 지정된 투영축으로 투영시킨 거리의 총합 / 2
			float fProjDist = 0.f;
			for (int j = 0; j < 6; ++j)
			{
				//Dot : 내적
				//Cos그래프는 -1~1 사이이므로 절대값 취해야함.
				fProjDist += fabsf(arrProj[j].Dot(vProj));
			}
			fProjDist /= 2.f;

			float fCenter = fabsf(vCenter.Dot(vProj));

			if (fProjDist < fCenter)
				return false;
		}

		return true;
	}

	//구, 구
	else if (_pLeft->GetCollider3DType() == COLLIDER3D_TYPE::SPHERE && _pRight->GetCollider3DType() == COLLIDER3D_TYPE::SPHERE) {
		Vec3 RightCenter = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _pRight->GetColliderWorldMat());
		Vec3 LeftCenter = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _pLeft->GetColliderWorldMat());

		float CenterLength = pow(pow((RightCenter.x - LeftCenter.x), 2) + pow((RightCenter.y - LeftCenter.y), 2) + pow((RightCenter.z - LeftCenter.z), 2), 1.0 / 2.0);

		Vec3 LeftCircleRadius = Vec3(1.f, 1.f, 1.f);
		LeftCircleRadius = XMVector3TransformCoord(LeftCircleRadius, _pLeft->GetColliderScaleMat());
		float LeftRadius = LeftCircleRadius.x / 2.f;

		Vec3 RightCircleRadius = Vec3(1.f, 1.f, 1.f);
		RightCircleRadius = XMVector3TransformCoord(RightCircleRadius, _pRight->GetColliderScaleMat());
		float RightRadius = RightCircleRadius.x / 2.f;

		if (LeftRadius + RightRadius >= CenterLength) return true;
	}

	//구, 큐브 
	else if ((_pLeft->GetCollider3DType() == COLLIDER3D_TYPE::SPHERE && _pRight->GetCollider3DType() == COLLIDER3D_TYPE::CUBE) ||
		(_pLeft->GetCollider3DType() == COLLIDER3D_TYPE::CUBE && _pRight->GetCollider3DType() == COLLIDER3D_TYPE::SPHERE)) {
		CCollider3D* CubeCollider;
		CCollider3D* SphereCollider;

		if (_pLeft->GetCollider3DType() == COLLIDER3D_TYPE::CUBE) {
			CubeCollider = _pLeft;
			SphereCollider = _pRight;
		}
		else {
			CubeCollider = _pRight;
			SphereCollider = _pLeft;
		}


		// 사각형의 꼭지점(local space에서의 좌표)을 나타내는 배열
		Vec3 arrLocal[8] =
		{
			Vec3(-0.5f, 0.5f, 0.5f),
			Vec3(0.5f, 0.5f, 0.5f),
			Vec3(0.5f, -0.5f, 0.5f),
			Vec3(-0.5f, -0.5f, 0.5f),
			Vec3(-0.5f, 0.5f, -0.5f),
			Vec3(0.5f, 0.5f, -0.5f),
			Vec3(0.5f, -0.5f, -0.5f),
			Vec3(-0.5f, -0.5f, -0.5f)
		};
		Vec3 sphereCenter = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), SphereCollider->GetColliderWorldMat());
		Vec3 sphereRadius = Vec3(1.f, 1.f, 1.f);
		sphereRadius = XMVector3TransformCoord(sphereRadius, SphereCollider->GetColliderScaleMat());
		float radius = sphereRadius.x / 2.f;

		// Cube의 AABB 구하기
		Vec3 cubeMin = XMVector3TransformCoord(arrLocal[7], CubeCollider->GetColliderWorldMat()); // 기본적으로 (0.5, 0.5, 0.5)를 최소로 가정
		Vec3 cubeMax = cubeMin;

		for (int i = 0; i < 8; ++i) {
			Vec3 cubeCorner = XMVector3TransformCoord(arrLocal[i], CubeCollider->GetColliderWorldMat());
			cubeMin = Vec3::Min(cubeMin, cubeCorner);
			cubeMax = Vec3::Max(cubeMax, cubeCorner);
		}

		// Sphere의 중심이 AABB 내에 있는지 확인
		if (sphereCenter.x >= cubeMin.x && sphereCenter.y >= cubeMin.y && sphereCenter.z >= cubeMin.z &&
			sphereCenter.x <= cubeMax.x && sphereCenter.y <= cubeMax.y && sphereCenter.z <= cubeMax.z) {
			return true;
		}

		// 그렇지 않다면, AABB의 가장 가까운 지점까지의 거리와 Sphere의 반지름 비교
		float x = max(cubeMin.x - sphereCenter.x, max(0.f, sphereCenter.x - cubeMax.x));
		float y = max(cubeMin.y - sphereCenter.y, max(0.f, sphereCenter.y - cubeMax.y));
		float z = max(cubeMin.z - sphereCenter.z, max(0.f, sphereCenter.z - cubeMax.z));

		return (x * x + y * y + z * z) <= (radius * radius);
	}


	return false;
}
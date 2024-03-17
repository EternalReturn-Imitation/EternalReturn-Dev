#include "pch.h"
#include "CCollisionMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CCollider2D.h"
#include "CTransform.h"

CCollisionMgr::CCollisionMgr()
    : m_matrix{}
{
	m_bRayCol.reset();
}


CCollisionMgr::~CCollisionMgr()
{

}

void CCollisionMgr::tick()
{
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurLevel();

	// 31번째 UI Layer는 제외하고 아래 별도로 진행해준다.

	for (UINT iRow = 0; iRow < MAX_LAYER - 1; ++iRow)
	{
		for (UINT iCol = iRow; iCol < MAX_LAYER - 1; ++iCol)
		{
			if (!(m_matrix[iRow] & (1 << iCol)))
				continue;

			// iRow 레이어와 iCol 레이어는 서로 충돌검사를 진행한다.
			CollisionBtwLayer(pLevel->GetLayer(iRow), pLevel->GetLayer(iCol));
		}
	}

	RayCollideCheck();
}

void CCollisionMgr::CollisionBtwLayer(CLayer* _Left, CLayer* _Right)
{
	const vector<CGameObject*>& vecLeft = _Left->GetObjects();
	const vector<CGameObject*>& vecRight = _Right->GetObjects();

	if (_Left == _Right)
	{
		for (size_t i = 0; i < vecLeft.size(); ++i)
		{
			for (size_t j = i + 1; j < vecRight.size(); ++j)
			{
				CollisionBtw2DObject(vecLeft[i], vecRight[j]);
				CollisionBtw3DObject(vecLeft[i], vecRight[j]);
			}
		}
	}
	else
	{
		for (size_t i = 0; i < vecLeft.size(); ++i)
		{
			for (size_t j = 0; j < vecRight.size(); ++j)
			{
				CollisionBtw2DObject(vecLeft[i], vecRight[j]);
				CollisionBtw3DObject(vecLeft[i], vecRight[j]);
			}
		}
	}
}

void CCollisionMgr::CollisionBtw2DObject(CGameObject* _LeftObject, CGameObject* _RightObject)
{
	if (!(_LeftObject->Collider2D() && _RightObject->Collider2D()))
		return;

	//비활성화된경우 제외
	if (_LeftObject->IsEnable() == false || _LeftObject->IsEnable() == false)
		return;

	// 충돌체 ID 생성
	CollisionID id = {};
	id.LeftID = _LeftObject->Collider2D()->GetID();
	id.RightID = _RightObject->Collider2D()->GetID();
	
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
	if (CollisionBtw2DCollider(_LeftObject->Collider2D(), _RightObject->Collider2D()))
	{
		// 이전에 충돌한 적이 있고, 둘중 하나 이상이 삭제 예정이라면
		if (bDead && iter->second)
		{
			_LeftObject->Collider2D()->EndOverlap(_RightObject->Collider2D());
			_RightObject->Collider2D()->EndOverlap(_LeftObject->Collider2D());
		}
		else if (iter->second)
		{
			// 계속 충돌 중
			_LeftObject->Collider2D()->OnOverlap(_RightObject->Collider2D());
			_RightObject->Collider2D()->OnOverlap(_LeftObject->Collider2D());
		}
		else
		{
			// 이번 프레임에 충돌
			if (!bDead) // 둘중 하나라도 Dead 상태면 충돌을 무시한다.
			{
				_LeftObject->Collider2D()->BeginOverlap(_RightObject->Collider2D());
				_RightObject->Collider2D()->BeginOverlap(_LeftObject->Collider2D());
				iter->second = true;
			}			
		}
	}

	else
	{
		// 충돌 해제
		if (iter->second)
		{
			_LeftObject->Collider2D()->EndOverlap(_RightObject->Collider2D());
			_RightObject->Collider2D()->EndOverlap(_LeftObject->Collider2D());
			iter->second = false;
		}
	}


}

// 두 충돌체의 충돌 검사 진행
bool CCollisionMgr::CollisionBtw2DCollider(CCollider2D* _pLeft, CCollider2D* _pRight)
{
	//XZ 평면끼리 검사
	if (
		(_pLeft->GetOffsetRot() == Vec3(XMConvertToRadians(90.f), 0.f, 0.f) &&
			_pRight->GetOffsetRot() == Vec3(XMConvertToRadians(90.f), 0.f, 0.f))
		||
		(_pLeft->GetOffsetRot() == Vec3(XM_PI / 2.f, 0.f, 0.f) &&
			_pRight->GetOffsetRot() == Vec3(XM_PI / 2.f, 0.f, 0.f))
		) {
		//y값이 다르면 무조건 충돌 안함.
		if (_pLeft->GetOwner()->Transform()->GetWorldPos().y != _pRight->GetOwner()->Transform()->GetWorldPos().y)
			return false;;
		//사각형, 사각형
		if (_pLeft->GetCollider2DType() == COLLIDER2D_TYPE::RECT && _pRight->GetCollider2DType() == COLLIDER2D_TYPE::RECT)
		{

			// 0 -- 1
			// |    |
			// 3 -- 2
			// 방향 벡터는 길이가 1이다.
			Vec3 arrLocal[4] =
			{
				Vec3(-0.5f, 0.5f, 0.f),
				Vec3(0.5f, 0.5f, 0.f),
				Vec3(0.5f, -0.5f, 0.f),
				Vec3(-0.5f, -0.5f, 0.f),
			};

			// 두 충돌체의 각 표면 벡터 2개씩 구함
			// 투영축 4개를 구한다.
			Vec3 arrProj[4] = {};

			// 왼쪽 충돌체 0->1로 가는 벡터
			arrProj[0] = XMVector3TransformCoord(arrLocal[1], _pLeft->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[0], _pLeft->GetColliderWorldMat());
			// 왼쪽 충돌체 0->3
			arrProj[1] = XMVector3TransformCoord(arrLocal[3], _pLeft->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[0], _pLeft->GetColliderWorldMat());
			// 오른쪽 충돌체 0->1
			arrProj[2] = XMVector3TransformCoord(arrLocal[1], _pRight->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[0], _pRight->GetColliderWorldMat());
			// 오른쪽 충돌체 0->3
			arrProj[3] = XMVector3TransformCoord(arrLocal[3], _pRight->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[0], _pRight->GetColliderWorldMat());

			// 두 충돌체의 중심점을 구함
			Vec3 vCenter = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _pRight->GetColliderWorldMat()) - XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _pLeft->GetColliderWorldMat());


			// 분리축 테스트
			for (int i = 0; i < 4; ++i)
			{
				Vec3 vProj = arrProj[i];
				vProj.Normalize(); //단위벡터로 만든다.


				// 4개의 표면백터를 지정된 투영축으로 투영시킨 거리의 총합 / 2
				float fProjDist = 0.f;
				for (int j = 0; j < 4; ++j)
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
		//원, 원
		else if (_pLeft->GetCollider2DType() == COLLIDER2D_TYPE::CIRCLE && _pRight->GetCollider2DType() == COLLIDER2D_TYPE::CIRCLE) {
			Vec3 RightCenter = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _pRight->GetColliderWorldMat());
			Vec3 LeftCenter = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _pLeft->GetColliderWorldMat());

			float CenterLength = (float)pow(pow((RightCenter.x - LeftCenter.x), 2) + pow((RightCenter.z - LeftCenter.z), 2), 1.0 / 2.0);

			Vec3 LeftCircleRadius = Vec3(1.f, 0.f, 1.f);
			LeftCircleRadius = XMVector3TransformCoord(LeftCircleRadius, _pLeft->GetColliderScaleMat());
			float LeftRadius = LeftCircleRadius.x / 2.f;

			Vec3 RightCircleRadius = Vec3(1.f, 0.f, 1.f);
			RightCircleRadius = XMVector3TransformCoord(RightCircleRadius, _pRight->GetColliderScaleMat());
			float RightRadius = RightCircleRadius.x / 2.f;

			if (LeftRadius + RightRadius >= CenterLength) return true;

			return false;
		}

		//원, 사각형
		else if ((_pLeft->GetCollider2DType() == COLLIDER2D_TYPE::RECT && _pRight->GetCollider2DType() == COLLIDER2D_TYPE::CIRCLE) ||
			(_pLeft->GetCollider2DType() == COLLIDER2D_TYPE::CIRCLE && _pRight->GetCollider2DType() == COLLIDER2D_TYPE::RECT)) {
			CCollider2D* SquareCollider;
			CCollider2D* CircleCollider;

			if (_pLeft->GetCollider2DType() == COLLIDER2D_TYPE::RECT) {
				SquareCollider = _pLeft;
				CircleCollider = _pRight;
			}
			else {
				SquareCollider = _pRight;
				CircleCollider = _pLeft;
			}

			//0---1
			//|   |
			//3---2

			Vec3 BoxCorner[4] =
			{
				Vec3(-0.5f, 0.5f, 0.f),
				Vec3(0.5f, 0.5f, 0.5f),
				Vec3(0.5f, -0.5f, 0.f),
				Vec3(-0.5f, -0.5f, 0.f),
			};
			Vec3 OuterBoxCorner[4] =
			{
				Vec3(-0.5f, 0.5f, 0.f),
				Vec3(0.5f, 0.5f, 0.f),
				Vec3(0.5f, -0.5f, 0.f),
				Vec3(-0.5f, -0.5f, 0.f)
			};
			Matrix SquareWorldMatRotated = XMMatrixIdentity();
			SquareWorldMatRotated = SquareCollider->GetColliderScaleMat();
			Vec3 CircleRadius = Vec3(1.f, 1.f, 0.f);
			CircleRadius = XMVector3TransformCoord(CircleRadius, CircleCollider->GetColliderScaleMat());
			float Radius = CircleRadius.x / 2.f;

			for (int i = 0; i < 4; ++i) {
				BoxCorner[i] = XMVector3TransformCoord(BoxCorner[i], SquareWorldMatRotated);
				OuterBoxCorner[i] = XMVector3TransformCoord(OuterBoxCorner[i], SquareWorldMatRotated);
			}
			//OuterBoxCorner 확장
			OuterBoxCorner[0].x -= Radius;
			OuterBoxCorner[0].y += Radius;
			OuterBoxCorner[1].x += Radius;
			OuterBoxCorner[1].y += Radius;
			OuterBoxCorner[2].x += Radius;
			OuterBoxCorner[2].y -= Radius;
			OuterBoxCorner[3].x -= Radius;
			OuterBoxCorner[3].y -= Radius;

			Vec3 CircleCenter = Vec3(0.f, 0.f, 0.f);
			Matrix CircleColliderRotated = XMMatrixIdentity();

			Matrix SquareColliderScale = SquareCollider->GetColliderScaleMat();
			Matrix SquareColliderScaleInv = XMMatrixInverse(nullptr, SquareColliderScale);
			Matrix temp = SquareColliderScaleInv * SquareCollider->GetColliderWorldMat();
			Matrix SquareColliderRotated = XMMatrixInverse(nullptr, temp);



			CircleColliderRotated = CircleCollider->GetColliderWorldMat();


			CircleCenter = XMVector3TransformCoord(CircleCenter, CircleColliderRotated);
			CircleCenter = XMVector3TransformCoord(CircleCenter, SquareColliderRotated);


			if (OuterBoxCorner[0].x < CircleCenter.x && CircleCenter.x < OuterBoxCorner[1].x
				&& OuterBoxCorner[3].y < CircleCenter.y && CircleCenter.y < OuterBoxCorner[0].y)
			{
				if (CircleCenter.x < BoxCorner[0].x && BoxCorner[0].y < CircleCenter.y)
				{
					Vec2 BoxVertex2Circle(CircleCenter.x - BoxCorner[0].x, CircleCenter.y - BoxCorner[0].y);
					float Length = BoxVertex2Circle.Length();
					if (Length <= Radius) return true;
					else return false;
				}
				else if (CircleCenter.x > BoxCorner[1].x && BoxCorner[1].y < CircleCenter.y)
				{
					Vec2 BoxVertex2Circle(CircleCenter.x - BoxCorner[1].x, CircleCenter.y - BoxCorner[1].y);
					float Length = BoxVertex2Circle.Length();
					if (Length <= Radius) return true;
					else return false;
				}
				else if (CircleCenter.x > BoxCorner[2].x && BoxCorner[2].y > CircleCenter.y)
				{
					Vec2 BoxVertex2Circle(CircleCenter.x - BoxCorner[2].x, CircleCenter.y - BoxCorner[2].y);
					float Length = BoxVertex2Circle.Length();
					if (Length <= Radius) return true;
					else return false;
				}
				else if (CircleCenter.x < BoxCorner[3].x && BoxCorner[3].y > CircleCenter.y)
				{
					Vec2 BoxVertex2Circle(CircleCenter.x - BoxCorner[3].x, CircleCenter.y - BoxCorner[3].y);
					float Length = BoxVertex2Circle.Length();
					if (Length <= Radius) return true;
					else return false;
				}

				return true;
			}
			else {
				return false;
			}

		}
	}
	//XY 평면끼리 검사
	else if (_pLeft->GetOffsetRot() == Vec3(0.f, 0.f, 0.f) &&
		_pRight->GetOffsetRot() == Vec3(0.f, 0.f, 0.f)) {
		//z값이 다르면 무조건 충돌 안함.
		if (_pLeft->GetOwner()->Transform()->GetWorldPos().z != _pRight->GetOwner()->Transform()->GetWorldPos().z)
			return false;
		//사각형, 사각형
		if (_pLeft->GetCollider2DType() == COLLIDER2D_TYPE::RECT && _pRight->GetCollider2DType() == COLLIDER2D_TYPE::RECT)
		{

			// 0 -- 1
			// |    |
			// 3 -- 2
			// 방향 벡터는 길이가 1이다.
			Vec3 arrLocal[4] =
			{
				Vec3(-0.5f, 0.5f, 0.f),
				Vec3(0.5f, 0.5f, 0.f),
				Vec3(0.5f, -0.5f, 0.f),
				Vec3(-0.5f, -0.5f, 0.f),
			};

			// 두 충돌체의 각 표면 벡터 2개씩 구함
			// 투영축 4개를 구한다.
			Vec3 arrProj[4] = {};

			// 왼쪽 충돌체 0->1로 가는 벡터
			arrProj[0] = XMVector3TransformCoord(arrLocal[1], _pLeft->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[0], _pLeft->GetColliderWorldMat());
			// 왼쪽 충돌체 0->3
			arrProj[1] = XMVector3TransformCoord(arrLocal[3], _pLeft->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[0], _pLeft->GetColliderWorldMat());
			// 오른쪽 충돌체 0->1
			arrProj[2] = XMVector3TransformCoord(arrLocal[1], _pRight->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[0], _pRight->GetColliderWorldMat());
			// 오른쪽 충돌체 0->3
			arrProj[3] = XMVector3TransformCoord(arrLocal[3], _pRight->GetColliderWorldMat()) - XMVector3TransformCoord(arrLocal[0], _pRight->GetColliderWorldMat());

			// 두 충돌체의 중심점을 구함
			Vec3 vCenter = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _pRight->GetColliderWorldMat()) - XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _pLeft->GetColliderWorldMat());


			// 분리축 테스트
			for (int i = 0; i < 4; ++i)
			{
				Vec3 vProj = arrProj[i];
				vProj.Normalize(); //단위벡터로 만든다.


				// 4개의 표면백터를 지정된 투영축으로 투영시킨 거리의 총합 / 2
				float fProjDist = 0.f;
				for (int j = 0; j < 4; ++j)
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
		//원, 원
		else if (_pLeft->GetCollider2DType() == COLLIDER2D_TYPE::CIRCLE && _pRight->GetCollider2DType() == COLLIDER2D_TYPE::CIRCLE) {
			Vec3 RightCenter = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _pRight->GetColliderWorldMat());
			Vec3 LeftCenter = XMVector3TransformCoord(Vec3(0.f, 0.f, 0.f), _pLeft->GetColliderWorldMat());

			float CenterLength = (float)pow(pow((RightCenter.x - LeftCenter.x), 2) + pow((RightCenter.y - LeftCenter.y), 2), 1.0 / 2.0);

			Vec3 LeftCircleRadius = Vec3(1.f, 1.f, 0.f);
			LeftCircleRadius = XMVector3TransformCoord(LeftCircleRadius, _pLeft->GetColliderScaleMat());
			float LeftRadius = LeftCircleRadius.x / 2.f;

			Vec3 RightCircleRadius = Vec3(1.f, 1.f, 0.f);
			RightCircleRadius = XMVector3TransformCoord(RightCircleRadius, _pRight->GetColliderScaleMat());
			float RightRadius = RightCircleRadius.x / 2.f;

			if (LeftRadius + RightRadius >= CenterLength) return true;

			return false;
		}

		//원, 사각형
		else if ((_pLeft->GetCollider2DType() == COLLIDER2D_TYPE::RECT && _pRight->GetCollider2DType() == COLLIDER2D_TYPE::CIRCLE) ||
			(_pLeft->GetCollider2DType() == COLLIDER2D_TYPE::CIRCLE && _pRight->GetCollider2DType() == COLLIDER2D_TYPE::RECT)) {
			CCollider2D* SquareCollider;
			CCollider2D* CircleCollider;

			if (_pLeft->GetCollider2DType() == COLLIDER2D_TYPE::RECT) {
				SquareCollider = _pLeft;
				CircleCollider = _pRight;
			}
			else {
				SquareCollider = _pRight;
				CircleCollider = _pLeft;
			}

			//0---1
			//|   |
			//3---2

			Vec3 BoxCorner[4] =
			{
				Vec3(-0.5f, 0.5f, 0.f),
				Vec3(0.5f, 0.5f, 0.f),
				Vec3(0.5f, -0.5f, 0.f),
				Vec3(-0.5f, -0.5f, 0.f),
			};
			Vec3 OuterBoxCorner[4] =
			{
				Vec3(-0.5f, 0.5f, 0.f),
				Vec3(0.5f, 0.5f, 0.f),
				Vec3(0.5f, -0.5f, 0.f),
				Vec3(-0.5f, -0.5f, 0.f)
			};
			Matrix SquareWorldMatRotated = XMMatrixIdentity();
			SquareWorldMatRotated = SquareCollider->GetColliderScaleMat();
			Vec3 CircleRadius = Vec3(1.f, 1.f, 0.f);
			CircleRadius = XMVector3TransformCoord(CircleRadius, CircleCollider->GetColliderScaleMat());
			float Radius = CircleRadius.x / 2.f;

			for (int i = 0; i < 4; ++i) {
				BoxCorner[i] = XMVector3TransformCoord(BoxCorner[i], SquareWorldMatRotated);
				OuterBoxCorner[i] = XMVector3TransformCoord(OuterBoxCorner[i], SquareWorldMatRotated);
			}
			//OuterBoxCorner 확장
			OuterBoxCorner[0].x -= Radius;
			OuterBoxCorner[0].y += Radius;
			OuterBoxCorner[1].x += Radius;
			OuterBoxCorner[1].y += Radius;
			OuterBoxCorner[2].x += Radius;
			OuterBoxCorner[2].y -= Radius;
			OuterBoxCorner[3].x -= Radius;
			OuterBoxCorner[3].y -= Radius;

			Vec3 CircleCenter = Vec3(0.f, 0.f, 0.f);
			Matrix CircleColliderRotated = XMMatrixIdentity();

			Matrix SquareColliderScale = SquareCollider->GetColliderScaleMat();
			Matrix SquareColliderScaleInv = XMMatrixInverse(nullptr, SquareColliderScale);
			Matrix temp = SquareColliderScaleInv * SquareCollider->GetColliderWorldMat();
			Matrix SquareColliderRotated = XMMatrixInverse(nullptr, temp);



			CircleColliderRotated = CircleCollider->GetColliderWorldMat();


			CircleCenter = XMVector3TransformCoord(CircleCenter, CircleColliderRotated);
			CircleCenter = XMVector3TransformCoord(CircleCenter, SquareColliderRotated);


			if (OuterBoxCorner[0].x < CircleCenter.x && CircleCenter.x < OuterBoxCorner[1].x
				&& OuterBoxCorner[3].y < CircleCenter.y && CircleCenter.y < OuterBoxCorner[0].y)
			{
				if (CircleCenter.x < BoxCorner[0].x && BoxCorner[0].y < CircleCenter.y)
				{
					Vec2 BoxVertex2Circle(CircleCenter.x - BoxCorner[0].x, CircleCenter.y - BoxCorner[0].y);
					float Length = BoxVertex2Circle.Length();
					if (Length <= Radius) return true;
					else return false;
				}
				else if (CircleCenter.x > BoxCorner[1].x && BoxCorner[1].y < CircleCenter.y)
				{
					Vec2 BoxVertex2Circle(CircleCenter.x - BoxCorner[1].x, CircleCenter.y - BoxCorner[1].y);
					float Length = BoxVertex2Circle.Length();
					if (Length <= Radius) return true;
					else return false;
				}
				else if (CircleCenter.x > BoxCorner[2].x && BoxCorner[2].y > CircleCenter.y)
				{
					Vec2 BoxVertex2Circle(CircleCenter.x - BoxCorner[2].x, CircleCenter.y - BoxCorner[2].y);
					float Length = BoxVertex2Circle.Length();
					if (Length <= Radius) return true;
					else return false;
				}
				else if (CircleCenter.x < BoxCorner[3].x && BoxCorner[3].y > CircleCenter.y)
				{
					Vec2 BoxVertex2Circle(CircleCenter.x - BoxCorner[3].x, CircleCenter.y - BoxCorner[3].y);
					float Length = BoxVertex2Circle.Length();
					if (Length <= Radius) return true;
					else return false;
				}

				return true;
			}
			else {
				return false;
			}

		}
	}

	return false;
}

void CCollisionMgr::LayerCheck(UINT _left, UINT _right)
{
	UINT iRow = (UINT)_left;
	UINT iCol = (UINT)_right;

	if (iRow > iCol)
	{
		UINT iTemp = iCol;
		iCol = iRow;
		iRow = iTemp;
	}

	m_matrix[iRow] |= (1 << iCol);
}

void CCollisionMgr::LayerCheck(const wstring& _strLeftLayer, const wstring& _strRightLayer)
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();

	CLayer* pLeft = pCurLevel->FindLayerByName(_strLeftLayer);
	CLayer* pRight = pCurLevel->FindLayerByName(_strRightLayer);

	LayerCheck(pLeft->GetLayerIndex(), pRight->GetLayerIndex());
}

void CCollisionMgr::RayLayerCheck(UINT _num)
{
	m_bRayCol.set(_num);
}

void CCollisionMgr::RayLayerCheck(const wstring& _strLayer)
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();

	CLayer* pLayer = pCurLevel->FindLayerByName(_strLayer);

	m_bRayCol.set(pLayer->GetLayerIndex());
}
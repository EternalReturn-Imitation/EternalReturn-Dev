#pragma once
#include "CSingleton.h"

class CLayer;
class CCollider3D;
class CCollider2D;

union CollisionID
{
	struct
	{
		UINT LeftID;
		UINT RightID;
	};

	UINT_PTR id;
};

union RayCollisionID {
	UINT_PTR id;
};


class CCollisionMgr :
    public CSingleton<CCollisionMgr>
{
	SINGLE(CCollisionMgr);
private:
	UINT					m_matrix[MAX_LAYER];
	map<UINT_PTR, bool>		m_mapColID;
	std::bitset<MAX_LAYER>	m_bRayCol;
	map<UINT_PTR, bool>		m_mRayColID;
	std::set<CGameObject*>	m_mPrevRayColID;

public:
	void LayerCheck(UINT _left, UINT _right);
	void LayerCheck(const wstring& _strLeftLayer, const wstring& _strRightLayer);
	void RayLayerCheck(UINT _num);
	void RayLayerCheck(const wstring& _strLayer);
	void RayCollideCheck();
	vector<CGameObject*> CheckRayRangeObject(IntersectResult _intersectResult);

	void Clear()
	{
		memset(m_matrix, 0, sizeof(UINT) * MAX_LAYER);
	}

public:
	void tick();

private:
	void CollisionBtwLayer(CLayer* _LeftLayer, CLayer* _RightLayer);

	//2D 오브젝트 충돌
	void CollisionBtw2DObject(CGameObject* _LeftObject, CGameObject* _RightObject);
	void CollisionBtw3DObject(CGameObject* _LeftObject, CGameObject* _RightObject);

	//3D 오브젝트 충돌
	bool CollisionBtw2DCollider(CCollider2D* _pLeft, CCollider2D* _pRight);
	bool CollisionBtw3DCollider(CCollider3D* _pLeft, CCollider3D* _pRight);

public:
	//오브젝트 RAY 충돌 체크
	IntersectResult IntersectsLay(Vec3* _vertices, tRay _ray);
	IntersectResult IsCollidingBtwRayRect(tRay& _ray, CGameObject* _Object);
	IntersectResult IsCollidingBtwRayCube(tRay& _ray, CGameObject* _Object);
	IntersectResult IsCollidingBtwRaySphere(tRay& _ray, CGameObject* _Object);

};


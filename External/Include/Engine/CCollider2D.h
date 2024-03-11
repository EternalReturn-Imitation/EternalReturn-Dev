#pragma once
#include "CComponent.h"

class CCollider2D :
    public CComponent
{
private:
    Vec3            m_vOffsetPos;
    Vec3            m_vOffsetScale;
    Vec3            m_vOffsetRot;

    bool            m_bAbsolute;
    COLLIDER2D_TYPE m_Shape;

    Matrix          m_matColliderScale;
    Matrix          m_matColliderPos;
    Matrix          m_matColliderRot;

    Matrix          m_matCollider2D;    // Collider 의 월드행렬
    int             m_iCollisionCount;  // 충돌 횟수


    bool            m_bIsCollidedFromRay;
    bool            m_bDrawCollision; //콜리전을 보이게 할지 여부

    // 하은님이 요청하신 변수 추가
    bool            m_bFixed;

public:
    virtual void finaltick() override;

public:
    void SetOffsetPos(Vec3 _vOffsetPos) { m_vOffsetPos = Vec3(_vOffsetPos.x, _vOffsetPos.y, _vOffsetPos.z); }
    void SetOffsetScale(Vec2 _vOffsetScale) { m_vOffsetScale = Vec3(_vOffsetScale.x, _vOffsetScale.y, 1.f); }
    void SetOffsetRot(Vec3 _vOffsetRot) { m_vOffsetRot = _vOffsetRot; }

    void SetAbsolute(bool _bSet) { m_bAbsolute = _bSet; }
    void SetCollider2DType(COLLIDER2D_TYPE _Type) { m_Shape = _Type; }

    Vec3 GetOffsetPos() { return m_vOffsetPos; }
    Vec3 GetOffsetScale() { return m_vOffsetScale; }
    Vec3 GetOffsetRot() { return m_vOffsetRot; }

    bool GetColliderAbsolute() { return m_bAbsolute; }
    COLLIDER2D_TYPE GetColliderShape() { return m_Shape; }

    bool IsFixed() { return m_bFixed; }
    void SetFixed(bool _b) { m_bFixed = _b; }

    const Matrix& GetColliderPosMat() { return m_matColliderPos; }
    const Matrix& GetColliderScaleMat() { return m_matColliderScale; }
    const Matrix& GetColliderWorldMat() { return m_matCollider2D; }
    const COLLIDER2D_TYPE& GetCollider2DType() { return m_Shape; }

    const int& GetCollisionCount() { return m_iCollisionCount; }

    bool  IsCollidedFromRay() { return m_bIsCollidedFromRay; }
    void  SetCollidedFromRay(bool _state) { m_bIsCollidedFromRay = _state; }

    bool GetDrawCollision() { return m_bDrawCollision; }
    void SetDrawCollision(bool _IsDraw) { m_bDrawCollision = _IsDraw; }

public:
    void BeginOverlap(CCollider2D* _Other);
    void OnOverlap(CCollider2D* _Other);
    void EndOverlap(CCollider2D* _Other);


    void BeginRayOverlap();
    void OnRayOverlap();
    void EndRayOverlap();


    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    virtual void SaveToDB(int _gameObjectID, COMPONENT_TYPE _componentType);
    virtual void LoadFromDB(int _gameObjectID);

    CLONE(CCollider2D);
public:
    CCollider2D();
    CCollider2D(const CCollider2D& _other);
    ~CCollider2D();
};



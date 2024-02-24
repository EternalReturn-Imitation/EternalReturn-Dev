#pragma once
#include "CComponent.h"

class CTransform :
    public CComponent
{
private:
    Vec3    m_vRelativePos;
    Vec3    m_vRelativeScale;
    Vec3    m_vRelativeRot;

    Vec3    m_vOffsetRelativePos;//오프셋을 입력한 상대 포즈

    bool    m_bAbsolute;    // 상대 이동, 크기를 절대값으로 지정    

    Vec3    m_vRelativeDir[3];
    Vec3    m_vWorldDir[3];

    Matrix  m_matWorldScale;    // 월드 크기 행렬
    Matrix  m_matWorld; // 크기, 회전, 이동 정보를 합쳐놓음
    Matrix  m_matWorldInv;

    Matrix  m_matOffsetWorld; //오프셋이 반영된 매트릭스
    Matrix  m_matOffsetWorldInv; //오프셋이 반영된 매트릭스의 역행렬

    Matrix  m_matWorldForGizmo; // 기즈모용 월드매트릭스

    float   m_fBoundingRadius;       // 최대 반경 반지름
    Matrix  m_matWorldBoundingScale;
    Matrix  m_matWorldBoundingBox;    // 최대 반경 행렬

    bool    b_GizmoOnSet;

    //오프셋 트리거
    bool    OffsetTrigger;

public:
    void SetRelativePos(Vec3 _vPos) { m_vRelativePos = _vPos; }
    void SetRelativeScale(Vec3 _vScale) { 
        m_vRelativeScale = _vScale;/*
        m_vOffsetRelativePos.x *= m_vRelativeScale.x;
        m_vOffsetRelativePos.y *= m_vRelativeScale.y;
        m_vOffsetRelativePos.z *= m_vRelativeScale.z;*/
    }
    void SetRelativeRot(Vec3 _vRot) { m_vRelativeRot = _vRot; }

    void SetRelativePos(float _x, float _y, float _z) { m_vRelativePos = Vec3(_x, _y, _z); }
    void SetRelativeScale(float _x, float _y, float _z) { 
        m_vRelativeScale = Vec3(_x, _y, _z);/*
        m_vOffsetRelativePos.x *= m_vRelativeScale.x;
        m_vOffsetRelativePos.y *= m_vRelativeScale.y;
        m_vOffsetRelativePos.z *= m_vRelativeScale.z;*/
    }
    void SetRelativeRot(float _x, float _y, float _z) { m_vRelativeRot = Vec3(_x, _y, _z);  }

    // 상대 이동, 크기를 절대값으로 지정  
    void SetAbsolute(bool _Set) { m_bAbsolute = _Set; }    

    // 오프셋 위치를 지정
    void SetOffsetRelativePos(Vec3 _vPos) { 
        m_vOffsetRelativePos = _vPos;
        m_vOffsetRelativePos.x *= m_vRelativeScale.x;
        if(m_vOffsetRelativePos.y == 0.f)
            m_vOffsetRelativePos.y *= 1.f;
        else
            m_vOffsetRelativePos.y *= m_vRelativeScale.y;
        m_vOffsetRelativePos.z *= m_vRelativeScale.z;
    }
    void SetOffsetTrigger(bool _trigger) { OffsetTrigger = _trigger; }

    Vec3 GetRelativePos() const { return m_vRelativePos; }
    Vec3 GetRelativeScale() const { return m_vRelativeScale; }
    Vec3 GetRelativeRot() const { return m_vRelativeRot; }

    Vec3 GetRelativeDir(DIR_TYPE _type) const { return m_vRelativeDir[(UINT)_type]; }
    Vec3 GetWorldDir(DIR_TYPE _type) const { { return m_vWorldDir[(UINT)_type]; } }
    Vec3 GetWorldPos() { return m_matWorld.Translation(); }

    const Matrix& GetWorldScaleMat() { return m_matWorldScale; }
    const Matrix& GetWorldMat() const { return m_matWorld; }
    const Matrix& GetWorldInvMat() const { return m_matWorldInv; }

    const Matrix& GetWorldMatForGizmo() const { return m_matWorldForGizmo; }

    void SetWorldMat(const Matrix& _mat) { m_matWorld = _mat; }

    const float& GetBoundingRadius() { return m_fBoundingRadius; }
    const Matrix& GetWorldBoundingMat() { return m_matWorldBoundingBox; }

    Vec3 GetOffsetRelativePos() { return m_vOffsetRelativePos; }

    //기즈모가 세팅되어 있는 오브젝트인지
    void SetGizmoOnSet(bool _IsNoGizmoObj) { b_GizmoOnSet = _IsNoGizmoObj; }
    bool GetGizmoOnSet() { return b_GizmoOnSet; }

public:
    virtual void finaltick() override;    
    void UpdateData();

    void operator = (const CTransform& _OtherTransform)
    {
        m_vRelativePos = _OtherTransform.m_vRelativePos;
        m_vRelativeScale = _OtherTransform.m_vRelativeScale;
        m_vRelativeRot = _OtherTransform.m_vRelativeRot;
        m_bAbsolute = _OtherTransform.m_bAbsolute;
        m_matWorldScale = _OtherTransform.m_matWorldScale;
        m_matWorld = _OtherTransform.m_matWorld;
        m_matWorldInv = _OtherTransform.m_matWorldInv;


        for (int i = 0; i < 3; ++i)
        {
            m_vRelativeDir[i] = _OtherTransform.m_vRelativeDir[i];
            m_vWorldDir[i] = _OtherTransform.m_vWorldDir[i];
        }
    }

public:
    void SetOriginAlignment();

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    virtual void SaveToDB(int _gameObjectID, COMPONENT_TYPE _componentType) override;
    virtual void LoadFromDB(int _gameObjectID) override;

    CLONE(CTransform);
public:
    CTransform();
    ~CTransform();
};


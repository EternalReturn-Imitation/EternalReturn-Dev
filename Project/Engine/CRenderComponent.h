#pragma once
#include "CComponent.h"

#include "CMesh.h"
#include "CMaterial.h"
#include "ptr.h"


struct tMtrlSet
{
    Ptr<CMaterial>  pSharedMtrl;    // 공유 메테리얼
    Ptr<CMaterial>  pDynamicMtrl;   // 공유 메테리얼의 복사본    
    Ptr<CMaterial>  pCurMtrl;       // 현재 사용 할 메테리얼
};

class CRenderComponent :
    public CComponent
{
private:
    Ptr<CMesh>              m_pMesh;
    vector<tMtrlSet>        m_vecMtrls;     // 재질    

    float                   m_fBounding;        // FrustumCheck 용도 경계범위
    bool                    m_bDynamicShadow;

    Ptr<CMaterial>          m_pSharedMtrl;  // 원본 메테리얼
    Ptr<CMaterial>          m_pDynamicMtrl; // SharedMaterial 복사본
    Ptr<CMaterial>          m_pCurrentMtrl;  // 현재 사용 중인 재질

    bool                    m_bFrustumCheck; // 절두체 컬링 체크 유무

    float                   m_fBoundingBoxScale;
    float                   m_fBoundingBoxOffsetScale;

public:
    virtual void render() = 0;
    virtual void render_shadowmap();

public:
    void SetMesh(Ptr<CMesh> _Mesh);
    void SetMaterial(Ptr<CMaterial> _Mtrl, UINT _idx);
    void SetFrustumCheck(bool _Check) { m_bFrustumCheck = _Check; }
    void SetBoundingBoxScale(float _Radius) { m_fBoundingBoxOffsetScale = _Radius; }
    void SetBoundingBoxOffsetScale(float _Offset) { m_fBoundingBoxOffsetScale = _Offset; }

    Ptr<CMesh> GetMesh() { return m_pMesh; }

    Ptr<CMaterial> GetMaterial(UINT _idx);
    Ptr<CMaterial> GetSharedMaterial(UINT _idx);
    Ptr<CMaterial> GetDynamicMaterial(UINT _idx);

    UINT GetMtrlCount() { return (UINT)m_vecMtrls.size(); }

    bool IsUseFrustumCheck() { return m_bFrustumCheck; }    
    void SetBounding(float _fBounding) { m_fBounding = _fBounding; }
    float GetBounding() { return m_fBounding; }
    void SetDynamicShadow(bool _bSet) { m_bDynamicShadow = _bSet; }
    bool IsDynamicShadow() { return m_bDynamicShadow; }
    float GetBoundingBoxScale() { return m_fBoundingBoxScale + m_fBoundingBoxOffsetScale; }

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    virtual void SaveToDB(int _gameObjectID, COMPONENT_TYPE _componentType) override;
    virtual void LoadFromDB(int _gameObjectID) override;

    bool IsFrustumCheck() { return m_bFrustumCheck; }

public:
    CRenderComponent(COMPONENT_TYPE _type);
    ~CRenderComponent();
};


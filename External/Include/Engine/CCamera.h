#pragma once
#include "CComponent.h"

#include "CFrustum.h"

class CCamera :
    public CComponent
{
private:
    CFrustum    m_Frustum;

    float       m_fAspectRatio;
    float       m_fScale;           // Orthograpic 에서 사용하는 카메라 배율
    
    float       m_Near;
    float       m_Far;
    
    float       m_FOV;

    bool        m_bMainCamera;      // 메인카메라 여부

    float   m_OrthoWidth;   // OrthoGraphic 에서의 가로 투영 범위
    float   m_OrthoHeight;  // OrthoGraphic 에서의 세로 투영 범위

    PROJ_TYPE   m_ProjType;

    Matrix      m_matView;
    Matrix      m_matViewInv;

    Matrix      m_matProj;
    Matrix      m_matProjInv;

    UINT        m_iLayerMask;       // 촬영 레이어
    UINT        m_iLayerFrustum;    // 절두체렌더 레이어

    int         m_iCamIdx;          // 카메라 우선순위

    tRay                    m_ray;      // 마우스 방향을 향하는 직선

    bool        m_bDebugView;
    bool        m_bDebugFrustumView;


    map<ULONG64, vector<tInstObj>>		m_mapInstGroup_D;	    // Deferred
    map<ULONG64, vector<tInstObj>>		m_mapInstGroup_F;	    // Foward ( Opaque, Mask )	
    map<INT_PTR, vector<tInstObj>>		m_mapSingleObj;		    // Single Object

    //vector<CGameObject*>    m_vecDeferred;
    //vector<CGameObject*>    m_vecDeferredDecal;

    vector<CGameObject*>    m_vecOpaque;
    vector<CGameObject*>    m_vecMask;
    vector<CGameObject*>    m_vecDecal;
    vector<CGameObject*>    m_vecTransparent;    
    vector<CGameObject*>    m_vecUI;
    vector<CGameObject*>    m_vecPost;

    vector<CGameObject*>    m_vecShadow;

public:
    void SetProjType(PROJ_TYPE _Type) { m_ProjType = _Type; }
    PROJ_TYPE GetProjType() { return m_ProjType; }

    void SetMainCamera() { m_bMainCamera = true; }
    bool IsMainCamera() { return m_bMainCamera; }

    void SetScale(float _fScale) { m_fScale = _fScale; }
    float GetScale() { return m_fScale; }

    void SetNear(float _Near) { m_Near = _Near; }
    float GetNear() { return m_Near; }

    void SetFar(float _Far) { m_Far = _Far; }
    float GetFar() { return m_Far; }

    void SetLayerMask(UINT _iLayerMask) { m_iLayerMask = _iLayerMask; }
    void SetLayerMask(int _iLayer, bool _Visible);
    void SetLayerMaskAll(bool _Visible);

    UINT GetLayerMask() { return m_iLayerMask; }

    void SetLayerFrustum(UINT _iLayerFrustum) { m_iLayerFrustum = _iLayerFrustum; }
    void SetLayerFrustum(int _iLayer, bool _Visible);
    void SetLayerFrustumAll(bool _Visible);

    UINT GetLayerFrustum() { return m_iLayerFrustum; }

    void SetFOV(float _Radian) { m_FOV = _Radian; }
    float GetFOV() { return m_FOV; }

    void SetOrthoWidth(float _width) { m_OrthoWidth = _width; }
    void SetOrthoHeight(float _height) { m_OrthoHeight = _height; }

    float GetOrthoWidth() { return m_OrthoWidth; }
    float GetOrthoHeight() { return m_OrthoHeight; }

    void SetCameraIndex(int _idx);

    const tRay& GetRay() { return m_ray; }

    const Matrix& GetViewMat() { return m_matView; }
    const Matrix& GetProjMat() { return m_matProj; }

    const Matrix& GetViewInvMat() { return m_matViewInv; }
    const Matrix& GetProjInvMat() { return m_matProjInv; }

    void ViewDebugCube(bool _view) { m_bDebugView = _view; }
    bool IsDebugView() { return m_bDebugView; }

    void ViewDebugFrustum(bool _view) { m_bDebugFrustumView = _view; }
    bool IsDebugFrustumView();

    int GetCamIdx() { return m_iCamIdx; }

    void MatrixUpdate();

public:
    tRaycastOutV3 Raycasting(Vec3* _vertices, tRay _ray);
    IntersectResult IntersectsLay(Vec3* _vertices, tRay _ray);
    IntersectResult IsCollidingBtwRayRect(tRay& _ray, CGameObject* _Object);
    IntersectResult IsCollidingBtwRayCube(tRay& _ray, CGameObject* _Object);
    IntersectResult IsCollidingBtwRaySphere(tRay& _ray, CGameObject* _Object);

public:
    void SortObject();
    void SortObject(CCamera* _MainCamera);
    void SortObject_Shadow();

    void render();
    void render_shadowmap();

public:
    virtual void begin() override;
    virtual void finaltick() override;

public:
    tRay CalRay();  // 마우스 방향으로 광선 연산

private:
    void clear();
    void clear_shadow();

    void render_deferred();
    void render_forward();

    void render_decal();
    void render_transparent();
    void render_postprocess();
    void render_ui();


    void CalcViewMat();
    void CalcProjMat();
    void UpdateMatrix();


    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    virtual void SaveToDB(int _gameObjectID, COMPONENT_TYPE _componentType) override;
    virtual void LoadFromDB(int _gameObjectID) override;

    CLONE(CCamera);
public:    
    CCamera();
    CCamera(const CCamera& _Other);
    ~CCamera();
};


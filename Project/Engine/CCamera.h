#pragma once
#include "CComponent.h"

class CCamera :
    public CComponent
{
private:
    float       m_fAspectRatio;
    float       m_fScale;           // Orthograpic 에서 사용하는 카메라 배율

    PROJ_TYPE   m_ProjType;

    Matrix      m_matView;
    Matrix      m_matProj;

    UINT        m_iLayerMask;

    int         m_iCamIdx;          // 카메라 우선순위


    vector<CGameObject*>    m_vecOpaque;
    vector<CGameObject*>    m_vecMask;
    vector<CGameObject*>    m_vecTransparent;    
    vector<CGameObject*>    m_vecUI;
    vector<CGameObject*>    m_vecPost;



public:
    void SetProjType(PROJ_TYPE _Type) { m_ProjType = _Type; }
    PROJ_TYPE GetProjType() { return m_ProjType; }

    void SetScale(float _fScale) { m_fScale = _fScale; }
    float GetScale() { return m_fScale; }

    void SetLayerMask(int _iLayer, bool _Visible);
    void SetLayerMaskAll(bool _Visible);

    void SetCameraIndex(int _idx);

    const Matrix& GetViewMat() { return m_matView; }
    const Matrix& GetProjMat() { return m_matProj; }

public:
    void SortObject();
    void render();

public:
    virtual void begin() override;
    virtual void finaltick() override;


private:
    void clear();
    void render_opaque();
    void render_mask();
    void render_transparent();
    void render_postprocess();
    void render_ui();


    void CalcViewMat();
    void CalcProjMat();


    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CCamera);
public:    
    CCamera();
    CCamera(const CCamera& _Other);
    ~CCamera();
};


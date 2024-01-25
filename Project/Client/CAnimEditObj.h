#pragma once

#include <Engine/ptr.h>

class CGameObject;
class CMeshData;
class CGraphicsShader;
class CAnimator3D;

class CAnimEditObj
{
private:
    CGameObject* m_pRenderObj;
    CGameObject* m_pControlObj;
    CGameObject* m_pCamObj;
    Ptr<CGraphicsShader> m_pOriginShader;

    Matrix m_matOriginView;
    Matrix m_matOriginProj;
    Matrix m_matOriginViewInv;
    Matrix m_matOriginProjInv;

public:
    void tick();
    void setobject(Ptr<CMeshData> _MeshData);
    void update();
    void render();

    void Clear();

    // Camera
    void CamMatrixUpdate();
    void ChangeEditShader(bool _SetType);

    CAnimator3D* GetAnimator3D() { return m_pRenderObj->Animator3D(); }

public:
    CAnimEditObj();
    ~CAnimEditObj();
};


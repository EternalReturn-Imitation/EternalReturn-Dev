#pragma once
#include "CEntity.h"

enum FACE_TYPE
{
    FT_NEAR,
    FT_FAR,
    FT_UP,
    FT_DOWN,
    FT_LEFT,
    FT_RIGHT,

    FT_END,
};


class CCamera;

class CFrustum :
    public CEntity
{
private:
    CCamera* m_pCam;

    // 투영 공간에서의 8개의 점
    Vec3        m_arrProj[8];


    // 절두체의 6면
    Vec4        m_arrFace[FT_END];

public:
    void finaltick();


public:
    bool FrustumCheck(Vec3 _vPos);
    bool FrustumCheckBound(Vec3 _vPos, float _fRadis);

public:
    CLONE(CFrustum);



public:
    CFrustum() {}
    CFrustum(CCamera* _Camera);
    ~CFrustum();
};


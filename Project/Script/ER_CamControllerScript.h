#pragma once
#include <Engine\CScript.h>

class ER_CamControllerScript :
    public CScript
{
private:
    CGameObject* m_pTargetObj;
    CGameObject* m_pShadowCam;

    Vec2    m_vResolution;
    RECT    m_Window;
    
    float   m_fCamSpeed;
    bool    m_bFollowToPlayer; // Player
    
    float   m_fCurFOV;

public:
    virtual void begin() override;
    virtual void tick() override;
    void SetTarget(CGameObject* _Target);
    void CameraFixToggle() {
        if (m_bFollowToPlayer) m_bFollowToPlayer = false;
        else m_bFollowToPlayer = true;
    }

    void FollowPlayerCamera();
    float AccelLerp(float _CurFOV, float Acceleration);

private:
    void CameraMove();
    void ZoomInOut();

private:
    Vec3 CalculateCamPos_ForTarget(Vec3 _CamPos, Vec3 _vTargetPos, Vec3 _CamFrontDir);

    CLONE(ER_CamControllerScript);
public:
    ER_CamControllerScript();
    ~ER_CamControllerScript();
};


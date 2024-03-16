#pragma once
#include <Engine\CScript.h>

class ER_CamControllerScript :
    public CScript
{
private:
    CGameObject* m_pTargetObj;

    float   m_fCamSpeed;
    bool    m_bFollowToPlayer; // Player

public:
    virtual void tick() override;
    void SetTarget(CGameObject* _Target);
    void CameraFixToggle() {
        if (m_bFollowToPlayer) m_bFollowToPlayer = false;
        else m_bFollowToPlayer = true;
    }

private:
    void CameraMove();
    void FollowPlayerCamera();

private:
    Vec3 CalculateCamPos_ForTarget(Vec3 _CamPos, Vec3 _vTargetPos, Vec3 _CamFrontDir);

    CLONE(ER_CamControllerScript);
public:
    ER_CamControllerScript();
    ~ER_CamControllerScript();
};

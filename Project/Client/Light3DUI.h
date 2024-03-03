#pragma once
#include "ComponentUI.h"

class Light3DUI :
    public ComponentUI
{
private:
    Vec3 vMemLightAmbient;
    float fMemRadius;
    float fMemAngle;
    bool  bDebug;

public:
    virtual int render_update() override;
    void render_LightRenderLayer(bool* _bLightRenderLayerCheckWin, CCamera* _LightRenderCam);
    

public:
    Light3DUI();
    ~Light3DUI();
};

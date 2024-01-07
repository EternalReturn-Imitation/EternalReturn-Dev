#pragma once
#include "ComponentUI.h"

class CameraUI :
    public ComponentUI
{
private:
    bool m_bLayerCheckWindow;
    bool m_bFrustumCheckWindow;

public:
    virtual int render_update() override;
    void render_LayerCheck();
    void render_FrustumCheck();

public:
    CameraUI();
    ~CameraUI();
};


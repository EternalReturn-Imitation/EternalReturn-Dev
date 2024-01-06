#pragma once
#include "ComponentUI.h"

class CameraUI :
    public ComponentUI
{
private:
    bool m_bLayerCheckWindow;

public:
    virtual int render_update() override;
    void render_LayerCheck();

public:
    CameraUI();
    ~CameraUI();
};


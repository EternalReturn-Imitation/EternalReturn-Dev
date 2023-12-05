#pragma once
#include "ComponentUI.h"
class CameraUI :
    public ComponentUI
{

public:
    virtual int render_update() override;

public:
    CameraUI();
    ~CameraUI();
};


#pragma once
#include "ComponentUI.h"
class Light2DUI :
    public ComponentUI
{

public:
    virtual int render_update() override;

public:
    Light2DUI();
    ~Light2DUI();
};


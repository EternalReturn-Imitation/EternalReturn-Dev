#pragma once
#include "ComponentUI.h"

class Animator3DUI :
    public ComponentUI
{
public:
    virtual int render_update() override;

public:
    Animator3DUI();
    ~Animator3DUI();
};


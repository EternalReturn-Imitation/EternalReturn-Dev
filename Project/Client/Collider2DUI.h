#pragma once
#include "ComponentUI.h"
class Collider2DUI :
    public ComponentUI
{
public:
    virtual int render_update() override;

public:
    Collider2DUI();
    ~Collider2DUI();
};


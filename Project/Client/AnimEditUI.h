#pragma once
#include "UI.h"

class AnimEditUI :
    public UI
{
private:

public:
    virtual void tick() override;
    virtual void finaltick() override;
    virtual int render_update() override;

public:
    AnimEditUI();
    virtual ~AnimEditUI();
};


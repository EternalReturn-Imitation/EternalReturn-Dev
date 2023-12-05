#pragma once
#include "ResUI.h"
class SoundUI :
    public ResUI
{

public:
    virtual int render_update() override;

public:
    SoundUI();
    ~SoundUI();
};


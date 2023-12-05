#pragma once
#include "ResUI.h"
class ComputeShaderUI :
    public ResUI
{

public:
    virtual int render_update() override;

public:
    ComputeShaderUI();
    ~ComputeShaderUI();
};


#pragma once
#include "ResUI.h"
class GraphicsShaderUI :
    public ResUI
{

public:
    virtual int render_update() override;

public:
    GraphicsShaderUI();
    ~GraphicsShaderUI();
};


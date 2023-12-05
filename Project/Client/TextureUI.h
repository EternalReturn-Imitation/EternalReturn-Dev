#pragma once
#include "ResUI.h"
class TextureUI :
    public ResUI
{

public:
    virtual int render_update() override;

public:
    TextureUI();
    ~TextureUI();
};


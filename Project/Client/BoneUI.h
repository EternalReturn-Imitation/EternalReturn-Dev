#pragma once
#include "ResUI.h"
class BoneUI :
    public ResUI
{

public:
    virtual int render_update() override;

public:
    BoneUI();
    ~BoneUI();
};


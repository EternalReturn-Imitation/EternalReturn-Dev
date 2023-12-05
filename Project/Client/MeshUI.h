#pragma once
#include "ResUI.h"
class MeshUI :
    public ResUI
{

public:
    virtual int render_update() override;

public:
    MeshUI();
    ~MeshUI();
};


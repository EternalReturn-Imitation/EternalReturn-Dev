#pragma once
#include "ResUI.h"
class PrefabUI :
    public ResUI
{

public:
    virtual int render_update() override;

public:
    PrefabUI();
    ~PrefabUI();
};


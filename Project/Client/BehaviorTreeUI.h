#pragma once
#include "ComponentUI.h"

class BTNode;

class BehaviorTreeUI :
    public ComponentUI
{
public:
    virtual int render_update() override;

public:
    BehaviorTreeUI();
    ~BehaviorTreeUI();
};


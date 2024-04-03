#pragma once
#include "ComponentUI.h"

class CText;

class TextUI :
    public ComponentUI
{
public:
    virtual int render_update() override;

public:
    TextUI();
    ~TextUI();
};


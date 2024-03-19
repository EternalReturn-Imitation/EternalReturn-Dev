#pragma once
#include "ComponentUI.h"

class CText;

class TextUI :
    public ComponentUI
{
private:

public:
    virtual int render_update() override;

public:
    TextUI();
    ~TextUI();
};


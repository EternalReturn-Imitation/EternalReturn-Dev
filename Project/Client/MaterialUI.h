#pragma once
#include "ResUI.h"
class MaterialUI :
    public ResUI
{
private:
    TEX_PARAM       m_eSelected;

public:
    virtual int render_update() override;


public:
    void SelectTexture(DWORD_PTR _Key);

public:
    MaterialUI();
    ~MaterialUI();
};


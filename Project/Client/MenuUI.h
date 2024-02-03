#pragma once
#include "UI.h"


class MenuUI :
    public UI
{
private:


public:
    virtual void finaltick() override;
    virtual int render_update() override;


    // Menu
private:
    void CreateEmptyObject();
    void CreateEmptyMaterial();

    void AddComponent(COMPONENT_TYPE _type);
    void AddScript(const wstring& _strScriptName);

    void LoadFBX();
    void LoadFBX_Bone();

public:
    MenuUI();
    ~MenuUI();
};


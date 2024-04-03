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

    void CreateObjectFromMeshData(DWORD_PTR _pMeshData);

    void AddComponent(COMPONENT_TYPE _type);
    void AddScript(const wstring& _strScriptName);

    void LoadFBX(int _MeshCnt = 0);
    void LoadFBX_Bone();

public:
    MenuUI();
    ~MenuUI();
};


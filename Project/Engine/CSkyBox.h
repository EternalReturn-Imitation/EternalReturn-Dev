#pragma once

#include "CRenderComponent.h"

enum class SKYBOX_TYPE
{
    SPHERE,
    CUBE,
};

class CSkyBox :
    public CRenderComponent
{
private:
    SKYBOX_TYPE     m_Type;
    Ptr<CTexture>   m_SkyBoxTex;


public:
    void SetSkyBoxType(SKYBOX_TYPE _Type);
    void SetSkyBoxTexture(Ptr<CTexture> _Tex);

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

public:
    virtual void finaltick() override;
    virtual void render() override;
    virtual void render(UINT _iSubset) override;

    virtual void SaveToDB(int _gameObjectID, COMPONENT_TYPE _componentType) override;
    virtual void LoadFromDB(int _gameObjectID) override;

    CLONE(CSkyBox);
public:
    CSkyBox();
    ~CSkyBox();
};


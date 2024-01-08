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


public:
    virtual void finaltick() override;
    virtual void render() override;

    virtual void SaveToDB(int _gameObjectID, COMPONENT_TYPE _componentType) override;
    virtual void LoadFromDB(int _gameObjectID) override;

    CLONE(CSkyBox);
public:
    CSkyBox();
    ~CSkyBox();
};


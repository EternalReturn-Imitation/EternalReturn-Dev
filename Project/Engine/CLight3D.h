#pragma once
#include "CComponent.h"

#include "CMesh.h"
#include "CMaterial.h"

class CLight3D :
    public CComponent
{
private:
    tLightInfo      m_LightInfo;

    Ptr<CMesh>      m_Mesh;
    Ptr<CMaterial>  m_Mtrl;

    UINT            m_LightIdx;

public:
    virtual void finaltick() override;
    void render();


    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

public:
    void SetLightColor(Vec3 _Color) { m_LightInfo.Color.vDiffuse = _Color; }
    void SetLightAmbient(Vec3 _Amb) { m_LightInfo.Color.vAmbient = _Amb; }
    void SetLightType(LIGHT_TYPE _type);
    void SetRadius(float _fRadius) { m_LightInfo.Radius = _fRadius; };
    void SetAngle(float _Angle) { m_LightInfo.Angle = _Angle; }
    
    Vec3 GetLightColor() { return m_LightInfo.Color.vDiffuse; }
    Vec3 GetLightAmbient() { return m_LightInfo.Color.vAmbient; }
    LIGHT_TYPE GetLightType() { return (LIGHT_TYPE)m_LightInfo.LightType; }
    float GetRadius() { return m_LightInfo.Radius; }
    float GetAngle() { return m_LightInfo.Angle; }

    CLONE(CLight3D);
public:
    CLight3D();
    ~CLight3D();
};


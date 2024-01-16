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
    int             m_LightIdx;
    bool            m_bShowRange;

    CGameObject*    m_pCamObj;

    bool            m_bDebug;

public:
    virtual void finaltick() override;
    
    void render(); // 라이팅 렌더
    void render_shadowmap(); // 광원 시점에서 물체들의 깊이(ShadowMap)를 그림

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    virtual void SaveToDB(int _gameObjectID, COMPONENT_TYPE _componentType) override;
    virtual void LoadFromDB(int _gameObjectID) override;

public:
    void SetLightColor(Vec3 _Color) { m_LightInfo.Color.vDiffuse = _Color; }
    void SetLightAmbient(Vec3 _Amb) { m_LightInfo.Color.vAmbient = _Amb; }
    void SetLightType(LIGHT_TYPE _type);
    void SetRadius(float _fRadius);
    void SetAngle(float _Angle) { m_LightInfo.Angle = _Angle; }
    void SetDebug(bool _Debug) { m_bDebug = _Debug; }
    
    Vec3 GetLightColor() { return m_LightInfo.Color.vDiffuse; }
    Vec3 GetLightAmbient() { return m_LightInfo.Color.vAmbient; }
    LIGHT_TYPE GetLightType() { return (LIGHT_TYPE)m_LightInfo.LightType; }
    float GetRadius() { return m_LightInfo.Radius; }
    float GetAngle() { return m_LightInfo.Angle; }
    bool IsDebug() { return m_bDebug; }

    CCamera* GetLightRenderCam();

    void ShowRange(bool _bSet) { m_bShowRange = _bSet; }

    CLONE(CLight3D);
public:
    CLight3D();
    CLight3D(const CLight3D& _Origin);
    ~CLight3D();
};


#pragma once
#include "CComponent.h"

#include "ptr.h"
#include "CRaycastShader.h"
#include "CTexture.h"

class CNaviMap :
    public CComponent
{
private:
    Ptr<CRaycastShader> m_pCSRaycast;//레이케스팅 쉐이더
    CStructuredBuffer*  m_pCrossBuffer;//레이케스팅 결과 버퍼

    Ptr<CTexture>       m_NaviMap; //네비 맵 텍스쳐

    bool                m_bTrigger;

public:
    CNaviMap();
    ~CNaviMap();

public:
    virtual void begin();
    virtual void tick();
    virtual void finaltick();

    void Raycasting();

public:
    virtual void SaveToLevelFile(FILE* _File);
    virtual void LoadFromLevelFile(FILE* _FILE);
    
    virtual void SaveToDB(int _gameObjectID, COMPONENT_TYPE _componentType);
    virtual void LoadFromDB(int _gameObjectID);

    CLONE(CNaviMap);
};


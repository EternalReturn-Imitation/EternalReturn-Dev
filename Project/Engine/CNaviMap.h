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

    tNaviResult         m_sResultPos;

    float               m_fMinMaxArr[6];//차례대로 xMin, xMax, yMin, yMax, zMin, zMax값이 들어가있음.

    //네비 맵 텍스트
    Ptr<CTexture>       m_pNaviTex;

public:
    CNaviMap();
    ~CNaviMap();

public:
    virtual void begin();
    virtual void tick();
    virtual void finaltick();

    void Raycasting();
    
public:
    tNaviResult GetRayResultPos() { return m_sResultPos; }

public:
    virtual void SaveToLevelFile(FILE* _File);
    virtual void LoadFromLevelFile(FILE* _FILE);
    
    virtual void SaveToDB(int _gameObjectID, COMPONENT_TYPE _componentType);
    virtual void LoadFromDB(int _gameObjectID);

    CLONE(CNaviMap);
};

